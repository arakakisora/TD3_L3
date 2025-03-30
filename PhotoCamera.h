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


private:
	Map* map;
	MapChipData mapData;
	// カメラの位置
	Vector2 postion;
	// カメラのサイズ
	Vector2 rangeSize;
	// カメラのオブジェクト
	unique_ptr<Object3D> object3D;



};

