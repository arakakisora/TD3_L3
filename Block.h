#pragma once
#include "Vector3.h"
class Object3D;
class Block
{
public:
	Block();
	~Block();

	// 初期化
	void Initialize(const int mapID, const Vector3& position);
	// 更新
	void Update();
	// 描画
	void Draw();


	// 終了処理
	void Finalize();

	// SetPosition


private:
	int mapID;

	Object3D* mapBlock0;
	Object3D* mapBlock1;

	Vector3 position;

};

