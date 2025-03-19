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
    void Initialize();
    void Finalize();
    void Update();
    void Draw();
    void GenerateObject3D();
    void ResetMapChipData();
    void LoadMapChipCsv(const std::string& filePath);
    MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);
    Vector3 GetMapChipPostionByIndex(uint32_t xIndex, uint32_t yIndex);
    uint32_t GetNumBlockVirtical() { return kNumBlockVirtical; }
    uint32_t GetNumBlockHorizontal() { return kNumBlockHorizontal; }
    IndexSet GetMapChipIndexSetByPosition(const Vector3& posotopn);
    Rect GetRectByIndex(uint32_t xindex, uint32_t yIndex);

private:
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
};
