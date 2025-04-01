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

	// imguiの描画
	void DrawImGui();

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
	vector<unique_ptr<Block>> blocks;

};

