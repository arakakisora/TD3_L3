#include "Map.h"
#include "Block.h"
#include <fstream>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>

namespace {

	std::map<std::string, MapChipType> mapChipTable = {
		{"0", MapChipType::kBlank},
		{"1", MapChipType::kBlock},
		
	};

}

void Map::ResetMapChipData()
{

	mapChipData_.data.clear();
	mapChipData_.data.resize(kNumBlockVirtical);
	for (std::vector<MapChipType>& mapChipDataLine : mapChipData_.data) {
		mapChipDataLine.resize(kNumBlockHorizontal);
	}

}

void Map::LoadMapChipCsv(const std::string& filePath)
{

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
		std::istringstream lien_stream(line);

		for (uint32_t x = 0; x < kNumBlockHorizontal; ++x) {

			std::string word;
			getline(lien_stream, word, ',');

			if (mapChipTable.contains(word)) {
				mapChipData_.data[y][x] = mapChipTable[word];
			}
		}
	}
}

MapChipType Map::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex)
{
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

Vector3 Map::GetMapChipPostionByIndex(uint32_t xIndex, uint32_t yIndex)
{

	return Vector3(kBlockWidth * xIndex, kBlockHeight * (kNumBlockVirtical - 1 - yIndex), 0);
}

IndexSet Map::GetMapChipIndexSetByPosition(const Vector3& posotopn)
{
	//指定座標がマップチップの何番にあるかを取得する関数
	IndexSet indexSet = {};
	indexSet.xIndex = static_cast<uint32_t>((posotopn.x + kBlockWidth / 2) / kBlockWidth);
	indexSet.yIndex = kNumBlockVirtical - 1 - static_cast<uint32_t>((posotopn.y + kBlockHeight / 2) / kBlockHeight);
	return indexSet;
}

Rect Map::GetRectByIndex(uint32_t xindex, uint32_t yIndex)
{
	Vector3 center = GetMapChipPostionByIndex(xindex, yIndex);
	Rect rect;
	rect.left = center.x - kBlockWidth / 2.0f;
	rect.right = center.x + kBlockWidth / 2.0f;
	rect.bottom = center.y - kBlockHeight / 2.0f;
	rect.top = center.y + kBlockHeight / 2.0f;
	return rect;
}



// 任意のサイズを指定
void Map::DemoMap(int width, int height) {
    mapWidth = width;
    mapHeight = height;

    // mapBlockを指定されたサイズで初期化
    mapBlock.resize(mapHeight, vector<Block>(mapWidth));

    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
			mapBlock[y][x].Initialize(0, Vector3(float(x), 0.0f, float(y)));
        }
    }
}


Vector3 Map::GetBlockPosition(int x, int y) {
    constexpr float blockScaleX = 0.15f;
    constexpr float blockScaleY = 0.3f;
    return Vector3(float(x) * blockScaleX, float(y) * blockScaleY, 0.0f);

}

