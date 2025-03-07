#pragma once
#include <iostream>
#include <vector>

#include "Vector3.h"
#include <assert.h>
#include <stdint.h>
#include "MyMath.h"

enum class MapChipType {

	kBlank, // 空白
	kBlock, // ブロック
	

};


struct MapChipData {

	std::vector<std::vector<MapChipType>> data;
};


struct IndexSet {
	uint32_t xIndex;
	uint32_t yIndex;
};

struct Rect {

	float left;
	float right;
	float bottom;
	float top;
};

using namespace std;
class Block;
class Map
{
public:
public:

	void ResetMapChipData();
	void LoadMapChipCsv(const std::string& filePath);
	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);//縦横のインデックスからマップチップの種類を取得する関数
	Vector3 GetMapChipPostionByIndex(uint32_t xIndex, uint32_t yIndex);
	uint32_t GetNumBlockVirtical() { return kNumBlockVirtical; }
	uint32_t GetNumBlockHorizontal() { return kNumBlockHorizontal; }
	IndexSet GetMapChipIndexSetByPosition(const Vector3& posotopn);//指定座標がマップチップの何番にあるかを取得する関数
	Rect GetRectByIndex(uint32_t xindex, uint32_t yIndex);
	


private:
	// 1ブロックのサイズ
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;
	// ブロック個数
	static inline const uint32_t kNumBlockVirtical = 25;
	static inline const uint32_t kNumBlockHorizontal = 100;
	MapChipData mapChipData_;

	
	
public:
	//// getter
	//Vector3 GetBlockPosition(int x, int y);
	//const vector<vector<int>>& GetMapData() const { return mapData; }
	//const vector<vector<Block>>& GetMapBlock() const { return mapBlock; }
	//size_t GetMapWidth() const { return mapWidth; }
	//size_t GetMapHeight() const { return mapHeight; }
	//Block* GetBlock(int x, int y) {
	//	if (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight) {
	//		return &mapBlock[mapHeight - 1 - y][x];
	//	}
	//	return nullptr;
	//}
	//// setter
	//void SetMapData(int x, int y, int value)
	//{
	//	// 範囲チェック
	//	if (y >= 0 && y < mapHeight && x >= 0 && x < mapWidth) { // ここは変更なし
	//		// 既存ブロックのリソース解放
	//		mapBlock[y][x].Finalize();
	//		// mapDataの更新
	//		mapData[y][x] = value;  // マップデータを更新
	//		// mapBlockの見た目を更新
	//		Vector3 blockPosition = GetBlockPosition(x, y);  // x と y の順番を修正
	//		mapBlock[y][x].Initialize(value, blockPosition);  // ブロックを再初期化
	//	}
	//}

};

