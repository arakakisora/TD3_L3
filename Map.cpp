#include "Map.h"
#include "Block.h"
#include <fstream>
#include <string>
#include <iostream>
#include <map>
#include <sstream>
#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG
#include "Object3DCommon.h"
#include <algorithm>

void Map::Initialize() {
	GenerateStageBlock();
	// マップチップデータ変更検出用
	mapChipDataNext_ = mapChipData_;
}

void Map::Finalize() {
	// マップの更新
	for (std::vector<Block*>& blockLine : blockobject3D) {
		for (Block* block : blockLine) {
			delete block;
		}
	}
	blockobject3D.clear();
}

void Map::Update(const bool cameraMode) {
	this->cameraMode_ = cameraMode;
	GenerateChangeStageBlock(mapChipDataNext_);
	// 3Dオブジェクトの更新
	for (std::vector<Block*>& blockLine : blockobject3D) {
		for (Block* block : blockLine) {
			if (!block)
				continue;
			block->Update(cameraMode_);
		}
	}


#ifdef _DEBUG
	// ImGuiウィンドウの開始
	ImGui::Begin("Map Chip Data");

	// マップチップデータの表示
	for (uint32_t y = 0; y < mapChipData_.data.size(); ++y) {
		for (uint32_t x = 0; x < mapChipData_.data[y].size(); ++x) {
			// マップチップタイプを取得
			MapChipType mapChipType = mapChipData_.data[y][x];

			// マップチップタイプを番号として表示
			ImGui::Text("%d", static_cast<int>(mapChipType));

			// 同じ行に表示するためのスペースを追加
			if (x < mapChipData_.data[y].size() - 1) {
				ImGui::SameLine();
			}
		}
	}

	// ImGuiウィンドウの終了
	ImGui::End();
#endif // _DEBUG
}

void Map::Draw() {

	for (std::vector<Block*>& blockLine : blockobject3D) {
		for (Block* block : blockLine) {
			if (!block) { // ブロックが存在しない場合はスキップ
				continue;
			}
			block->Draw();
		}
	}
}

void Map::GenerateStageBlock() {
	// 要素数
	uint32_t numBlokVirtical = this->GetNumBlockVirtical();     //縦
	uint32_t numBlokHorizontal = this->GetNumBlockHorizontal(); //横

	blockobject3D.resize(numBlokVirtical);

	for (uint32_t i = 0; i < numBlokVirtical; ++i) {
		blockobject3D[i].resize(numBlokHorizontal);
	}

	// キューブ生成
	for (uint32_t i = 0; i < numBlokVirtical; ++i) {
		for (uint32_t j = 0; j < numBlokHorizontal; ++j) {
			MapChipType type = this->GetMapChipTypeByIndex(j, i);
			if (type != MapChipType::kBlank) {
				blockobject3D[i][j] = Block::CreateBlock(type, this->GetMapChipPostionByIndex(j, i), this);
			}
		}
	}
}

void Map::GenerateChangeStageBlock(const MapChipData& mapChipData)
{
	// マップチップデータの変更を検出
	for (uint32_t y = 0; y < kNumBlockVirtical; ++y) {
		for (uint32_t x = 0; x < kNumBlockHorizontal; ++x) {
			if (mapChipData.data[y][x] != mapChipData_.data[y][x]) {
				// マップチップデータが変更された場合、3Dオブジェクトを再生成
				if (blockobject3D[y][x] != nullptr) {
					delete blockobject3D[y][x]; // メモリ解放
					blockobject3D[y][x] = nullptr; // ポインタを nullptr に設定
				}
				// 新しいマップチップデータに基づいてオブジェクトを生成
				blockobject3D[y][x] = Block::CreateBlock(mapChipData.data[y][x], GetMapChipPostionByIndex(x, y), this);
			}
		}
	}
	// 変更されたマップチップデータをおおもとにする
	mapChipData_ = mapChipData;
}


void Map::ResetMapChipData() {
	mapChipData_.data.clear();
	mapChipData_.data.resize(kNumBlockVirtical);
	for (std::vector<MapChipType>& mapChipDataLine : mapChipData_.data) {
		mapChipDataLine.resize(kNumBlockHorizontal);
	}
}

void Map::LoadMapChipCsv(const std::string& filePath) {
	// マップチップデータをリセット
	ResetMapChipData();

	// ファイルを開く
	std::ifstream file(filePath);
	assert(file.is_open());

	// メタ情報の初期化（念のため）
	photoCameraCount = 0;
	kameraSizeX = 2; 
	kameraSizeY = 2;

	std::string line;
	uint32_t currentMapY = 0;

	while (std::getline(file, line)) {
		// コメント行や空行をスキップ
		if (line.empty() || line.find("//") == 0) {
			continue;
		}

		std::istringstream lineStream(line);
		std::string firstWord;
		getline(lineStream, firstWord, ',');

		// メタ情報の処理
		if (firstWord == "SHUTTER") {
			std::string value;
			getline(lineStream, value, ',');
			photoCameraCount = std::stoi(value);

		} else if (firstWord == "FRAMESIZE") {
			std::string valueX, valueY;
			getline(lineStream, valueX, ',');
			getline(lineStream, valueY, ',');

			// 安全性のためバリデーション（オプション）
			kameraSizeX = (std::max)(1, std::stoi(valueX));
			kameraSizeY = (std::max)(1, std::stoi(valueY));

		} else {
			// マップデータとして処理
			if (currentMapY >= kNumBlockVirtical) continue;

			std::istringstream mapLineStream(line);
			for (uint32_t x = 0; x < kNumBlockHorizontal; ++x) {
				std::string word;
				if (!std::getline(mapLineStream, word, ',')) break;

				if (mapChipTable.contains(word)) {
					mapChipData_.data[currentMapY][x] = mapChipTable[word];
				}
			}
			++currentMapY;
		}
	}

	file.close();
	// プレイヤーの初期位置を設定
	MapDataToPlayerInitPosition();
}

void Map::MapDataToPlayerInitPosition()
{
	// プレイヤーの初期位置をマップデータから取得
	for (uint32_t y = 0; y < kNumBlockVirtical; ++y) {
		for (uint32_t x = 0; x < kNumBlockHorizontal; ++x) {
			if (mapChipData_.data[y][x] == MapChipType::kPlayer) {
				playerStartX = x;
				playerStartY = y;
				return;
			}
		}
	}
}

MapChipType Map::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex) {
	// マップチップの範囲外の場合は空白を返す
	if (xIndex < 0 || kNumBlockHorizontal - 1 < xIndex) {
		return MapChipType::kBlank;
	}
	// マップチップの範囲外の場合は空白を返す
	if (yIndex < 0 || kNumBlockVirtical - 1 < yIndex) {
		return MapChipType::kBlank;
	}
	// マップチップの種類を返す
	return mapChipData_.data[yIndex][xIndex];
}

Vector3 Map::GetMapChipPostionByIndex(uint32_t xIndex, uint32_t yIndex) {
	return Vector3(kBlockWidth * xIndex, kBlockHeight * (kNumBlockVirtical - 1 - yIndex), 0);
}

IndexSet Map::GetMapChipIndexSetByPosition(const Vector3& posotopn) {
	// 指定座標がマップチップの何番にあるかを取得する関数
	IndexSet indexSet = {};
	indexSet.xIndex = static_cast<uint32_t>((posotopn.x + kBlockWidth / 2) / kBlockWidth);
	indexSet.yIndex = kNumBlockVirtical - 1 - static_cast<uint32_t>((posotopn.y + kBlockHeight / 2) / kBlockHeight);
	return indexSet;
}

Rect Map::GetRectByIndex(uint32_t xindex, uint32_t yIndex) {
	Vector3 center = GetMapChipPostionByIndex(xindex, yIndex);
	Rect rect;
	rect.left = center.x - kBlockWidth / 2.0f;
	rect.right = center.x + kBlockWidth / 2.0f;
	rect.bottom = center.y - kBlockHeight / 2.0f;
	rect.top = center.y + kBlockHeight / 2.0f;
	return rect;
}

void Map::SetMapData(uint32_t xIndex, uint32_t yIndex, MapChipType mapChipType) {
	if (xIndex < kNumBlockHorizontal && yIndex < kNumBlockVirtical) {
		mapChipData_.data[yIndex][xIndex] = mapChipType;
	}
}

void Map::GenerateObjectAt(uint32_t x, uint32_t y, MapChipType mapChipType) {

	// 範囲チェック
	if (x >= this->GetNumBlockHorizontal() || y >= this->GetNumBlockVirtical()) {
		return; // 範囲外なら処理をしない
	}

	// 現在のマップチップのタイプを取得
	MapChipType currentMapChipType = this->GetMapChipTypeByIndex(x, y);

	// 現在のマップチップタイプと引数で渡された mapChipType が同じなら処理をスルー
	if (currentMapChipType == mapChipType) {
		return; // タイプが変わらない場合、処理をスルー
	}

	// オブジェクトがすでに存在している場合は削除
	if (blockobject3D[y][x] != nullptr) {
		delete blockobject3D[y][x];  // オブジェクトを削除
		blockobject3D[y][x] = nullptr; // ポインタを nullptr に設定
	}

	// オブジェクト生成
	Vector3 position = this->GetMapChipPostionByIndex(x, y); // 座標を取得
	blockobject3D[y][x] = Block::CreateBlock(mapChipType, position, this);

	// マップデータを更新（オブジェクトのタイプに基づいてマップデータも更新）
	SetMapData(x, y, mapChipType);
}

void Map::RemoveObjectAt(uint32_t x, uint32_t y) {
	// 範囲チェック
	if (x >= this->GetNumBlockHorizontal() || y >= this->GetNumBlockVirtical()) {
		return; // 範囲外なら処理をしない
	}

	// その位置にある Block を削除
	if (blockobject3D[y][x]) {
		delete blockobject3D[y][x]; // メモリ解放
		blockobject3D[y][x] = nullptr;
	}

	// マップデータを更新(削除を反映）
	SetMapData(x, y, MapChipType::kBlank);
}

Vector3 Map::FindMapChipPosition(MapChipType mapChipType) {
	for (uint32_t y = 0; y < kNumBlockVirtical; ++y) {
		for (uint32_t x = 0; x < kNumBlockHorizontal; ++x) {
			if (mapChipData_.data[y][x] == mapChipType) {
				return GetMapChipPostionByIndex(x, y);
			}
		}
	}
	// 見つからなかった場合は (0, 0, 0) を返す（用途に応じて変更）
	return Vector3{ 0.0f, 0.0f, 0.0f };
}