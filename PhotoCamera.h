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

public:	// Setter / Getter
	// 変更したマップデータをmapにセット
	//void SetMap(Map* map) { this->map = map; }

	// カメラの位置を取得
	Vector2 GetPosition() { return position; }
	// カメラのサイズを取得
	Vector2 GetRangeSize() { return rangeSize; }
	// カメラのオブジェクトを取得
	Object3D* GetObject3D() { return object3D.get(); }
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
};

