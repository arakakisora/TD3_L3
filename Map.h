#pragma once
#include <iostream>
#include <vector>

#include "Block.h"
#include "Vector3.h"
#include <assert.h>
#include <stdint.h>
#include "MyMath.h"
#include "Object3D.h"

using namespace std;
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

class Map {
public:
	// 初期化
    void Initialize();
	// 終了処理
    void Finalize();
	// 更新
    void Update();
	// 描画
    void Draw();
	// 3Dオブジェクト生成
    void GenerateObject3D();
	// マップチップデータのリセット
    void ResetMapChipData();
	// マップチップデータの読み込み
    void LoadMapChipCsv(const std::string& filePath);
    
public: // Getter
	// マップチップの種類を取得
    MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);
	// マップチップの位置を取得
    Vector3 GetMapChipPostionByIndex(uint32_t xIndex, uint32_t yIndex);
      
    uint32_t GetNumBlockVirtical() { return kNumBlockVirtical; }
    uint32_t GetNumBlockHorizontal() { return kNumBlockHorizontal; }
    IndexSet GetMapChipIndexSetByPosition(const Vector3& posotopn);
    Rect GetRectByIndex(uint32_t xindex, uint32_t yIndex);

private:    // ブロック関連
    static inline const float kBlockWidth = 1.0f;
    static inline const float kBlockHeight = 1.0f;
    static inline const uint32_t kNumBlockVirtical = 25;
    static inline const uint32_t kNumBlockHorizontal = 100;
    MapChipData mapChipData_;
    std::vector<std::vector<Block*>> blockobject3D;

public:
    uint32_t GetMapWidth() const { return kNumBlockHorizontal; }
    uint32_t GetMapHeight() const { return kNumBlockVirtical; }
    std::vector<std::vector<Block*>>& GetBlockObject3D() { return blockobject3D; }
    void SetMapData(uint32_t xIndex, uint32_t yIndex, MapChipType mapChipType);
    void GenerateObjectAt(uint32_t x, uint32_t y, MapChipType mapChipType);
    void RemoveObjectAt(uint32_t x, uint32_t y);

public: // 新設 Setter / Getter
    std::vector<std::vector<MapChipType>> GetMap() const { return mapChipData_.data; }
	void SetMap(std::vector<std::vector<MapChipType>> map) { mapChipData_.data = map; }
	void SetMap(const MapChipData& map) { mapChipData_ = map; }
};
