#pragma once
#include "Object3D.h"
#include "Map.h"
#include "Block.h"
using namespace std;
class PhotoCamera
{
public:
	// 初期化
	void Initialize();
	// 更新
	void Update(Map* map);
	// 描画
	void Draw();
	// 終了処理
	void Finalize();
	// カメラの移動
	void Move();
	// マップデータのコピー
	void Copy();

	// コピーしたマップデータの貼り付け 
	void Paste();

	// imguiの描画
	void DrawImGui();

	void stickMove(); // スティック移動

public:	// Setter / Getter
	// 変更したマップデータをmapにセット
	//void SetMap(Map* map) { this->map = map; }

	// カメラの位置を取得
	Vector2 GetPosition() { return position; }
	// カメラのサイズを取得
	Vector2 GetRangeSize() { return rangeSize; }
	// カメラのオブジェクトを取得
	Object3D* GetObject3D() { return object3D.get(); }

	void SetcameraMode(bool mode) { CamerMode = mode; } // カメラモードを設定
private:
	Map* map;
	MapChipData mapData;
	// カメラの位置
	Vector2 position;
	// カメラのサイズ
	Vector2 rangeSize;
	// カメラのオブジェクト
	unique_ptr<Object3D> object3D;
	// コピーしたマップデータ
	vector<vector<MapChipType>> copyData;
	// コピーしたマップデータの描画用Blockクラス
	vector<Block*> blocks;
	// コピーしたマップデータの描画用Blockクラスの位置
	Vector3 blockPosition;

	// Y軸の座標変換用
	int photo_ConvertY = ((int)Map::kNumBlockVirtical - (int)position.y) - 1;
	// MapThipTypeのマップデータ
	MapChipType mapChipType;

	bool CamerMode = false;

	// カメラサイズ
	uint32_t cameraSizeX = 2;
	uint32_t cameraSizeY = 2;

	// シャッター回数上限
	uint32_t shutterLimitCountMax = 0;
	// シャッター回数
	uint32_t shutterCount = 0;


};

