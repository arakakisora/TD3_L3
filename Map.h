#pragma once
#include <iostream>
#include <vector>

#include "Vector3.h"
#include <assert.h>
#include <stdint.h>
#include "MyMath.h"
#include "Object3D.h"

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

	void Initialize();

	void Finalize();
	
	void Update();

	void Draw();

	/// ブロックの生成
	void GenerateObject3D();

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

	std::vector<std::vector<Object3D*>> blockobject3D;
	
public:
	

};

