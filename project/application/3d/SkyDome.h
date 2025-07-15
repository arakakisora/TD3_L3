#pragma once
#include "engine/3d/Object3D.h"
#include "engine/3d/Object3DCommon.h"
#include <memory>

class SkyDome
{
public:
	// コンストラクタ
	SkyDome();
	// デストラクタ
	~SkyDome();
	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw();
	// 終了処理
	void Finalize();

private:
	std::unique_ptr<Object3D> skyDomeObject; // 天球の3Dオブジェクト	
	Vector3 skyDomePosition;

};

