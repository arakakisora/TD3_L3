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
    static inline const float kBlockWidth = 1.0f;
    static inline const float kBlockHeight = 1.0f;
    static inline const uint32_t kNumBlockVirtical = 25;
    static inline const uint32_t kNumBlockHorizontal = 25;
public:
  
	// 初期化
    void Initialize();
	// 終了処理
    void Finalize();
	// 更新
    void Update(const bool cameraMode= false);
	// 描画
    void Draw();
	// 3Dオブジェクト生成
    void GenerateStageBlock();
	// マップチップデータのリセット
    void ResetMapChipData();
	// マップチップデータの読み込み
    void LoadMapChipCsv(const std::string& filePath);
    
    // 変更されたマップデータのブロック生成
	void GenerateChangeStageBlock(const MapChipData& mapChipData);


    //プレイヤーの初期位置をマップデータからプレイヤー変数へ渡す
    void MapDataToPlayerInitPosition();
public: // Getter
	// マップチップの種類を取得
    MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);
	// マップチップの位置を取得
    Vector3 GetMapChipPostionByIndex(uint32_t xIndex, uint32_t yIndex);
      
    uint32_t GetNumBlockVirtical() { return kNumBlockVirtical; }
    uint32_t GetNumBlockHorizontal() { return kNumBlockHorizontal; }
    IndexSet GetMapChipIndexSetByPosition(const Vector3& posotopn);
    Rect GetRectByIndex(uint32_t xindex, uint32_t yIndex);

public: //カメラに使用する
    // 読み込んだカメラの範囲をフォトカメラに渡すためのもの
	uint32_t GetkameraSizeX() const { return kameraSizeX; }
	uint32_t GetkameraSizeY() const { return kameraSizeY; }

	// フォトカメラのシャッター回数を取得
	uint32_t GetShutterCount() const { return photoCameraCount; }

	// プレイヤーの初期位置を取得
	uint32_t GetPlayerStartX() const { return playerStartX; }
	uint32_t GetPlayerStartY() const { return playerStartY; }


    // playerPositionInit
    Vector3 GetPlayerStartPosition() const { return playerStartPosition; }

private: // ブロック関連

    // マップチップデータ
    MapChipData mapChipData_;

	// 受け取ったマップチップデータ
    MapChipData mapChipDataNext_;

    // ブロックのオブジェクト
    std::vector<std::vector<Block*>> blockobject3D;

private: // マップデータの読み込み用
    // フォトカメラの使用回数
    uint32_t photoCameraCount = 0;
    // フレームの大きさ
	uint32_t kameraSizeX = 0;
	uint32_t kameraSizeY = 0;

    //プレイヤーの初期位置のマップ番号を記録する変数
	uint32_t playerStartX = 0;
	uint32_t playerStartY = 0;

    
    //カメラモードラッパー
    bool cameraMode_;

    Vector3 playerStartPosition; 


public:
    uint32_t GetMapWidth() const { return kNumBlockHorizontal; }
    uint32_t GetMapHeight() const { return kNumBlockVirtical; }
    std::vector<std::vector<Block*>>& GetBlockObject3D() { return blockobject3D; }
    void SetMapData(uint32_t xIndex, uint32_t yIndex, MapChipType mapChipType);
    void GenerateObjectAt(uint32_t x, uint32_t y, MapChipType mapChipType);
    void RemoveObjectAt(uint32_t x, uint32_t y);
public: // 新設 Setter / Getter
    std::vector<std::vector<MapChipType>> GetMap() const { return mapChipData_.data; }
	//void SetMap(std::vector<std::vector<MapChipType>> map) { mapChipData_.data = map; }
	void SetMap(const MapChipData& map) { mapChipDataNext_ = map; }
    Vector3 FindMapChipPosition(MapChipType mapChipType);
};
