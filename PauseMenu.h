#pragma once
#include "Object3D.h"
#include "Object3DCommon.h"
#include "Input.h"
#include "Camera.h"

class PauseMenu
{
public:
	//初期化
	void Initialize(Object3DCommon* object3dcommon);
	//更新
	void Update();
	//描画
	void Draw();
public:
	bool IsPaused()const;
private:
	Object3DCommon* object3dcommon_;
	Camera* camera;
	Transform transform;
	Input* input;
	//ポーズの文字のオブジェクト
	std::unique_ptr<Object3D>object;
	//ポーズ画面のオブジェクト
	std::unique_ptr<Object3D>object2;
	//操作説明のオブジェクト
	std::unique_ptr<Object3D>object3;
	//ポーズ開く
	bool isPaused_ = false;
	//操作説明表示
	bool isOperation_ = false;
	//イージングタイマー
	float easeTimer_ = 0.0f;
	float easeTimer2_ = 0.0f;
};

