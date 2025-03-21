#include "Map.h"
#include "Block.h"
#include <fstream>
#include <string>
#include <iostream>
#include <map>
#include <sstream>

#include "Object3DCommon.h"

void Map::Initialize() {
    GenerateObject3D();
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

void Map::Update() {
    // 3Dオブジェクトの更新
    for (std::vector<Block*>& blockLine : blockobject3D) {
        for (Block* block : blockLine) {
            if (!block)
                continue;
            block->Update();
        }
    }
}

void Map::Draw() {
    for (std::vector<Block*>& blockLine : blockobject3D) {
        for (Block* block : blockLine) {
            if (!block) {
                continue;
            }
            block->Draw();
        }
    }
}

void Map::GenerateObject3D() {
    // 要素数
    uint32_t numBlokVirtical = this->GetNumBlockVirtical();     // 縦
    uint32_t numBlokHorizontal = this->GetNumBlockHorizontal(); // 横

    blockobject3D.resize(numBlokVirtical);

    for (uint32_t i = 0; i < numBlokVirtical; ++i) {
        blockobject3D[i].resize(numBlokHorizontal);
    }

    // キューブ生成
    for (uint32_t i = 0; i < numBlokVirtical; ++i) {
        for (uint32_t j = 0; j < numBlokHorizontal; ++j) {
            MapChipType type = this->GetMapChipTypeByIndex(j, i);
            if (type != MapChipType::kBlank) {
                blockobject3D[i][j] = Block::CreateBlock(type, this->GetMapChipPostionByIndex(j, i),this);
            }
        }
    }
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
    std::ifstream file;
    file.open(filePath);
    assert(file.is_open());

    // マップチップCSV
    std::stringstream mapChipCsv;
    // ファイルの内容を文字列ストリームにコピー
    mapChipCsv << file.rdbuf();
    // ファイルを閉じる
    file.close();

    // csvからマップチップデータを読み込む
    for (uint32_t y = 0; y < kNumBlockVirtical; ++y) {
        std::string line;
        getline(mapChipCsv, line);

        // 1桁分の文字列をストリームに変換して解析しやすくする
        std::istringstream line_stream(line);

        for (uint32_t x = 0; x < kNumBlockHorizontal; ++x) {
            std::string word;
            getline(line_stream, word, ',');

            if (mapChipTable.contains(word)) {
                mapChipData_.data[y][x] = mapChipTable[word];
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
    blockobject3D[y][x] = Block::CreateBlock(mapChipType, position,this);

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