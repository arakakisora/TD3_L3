#pragma once
#include "Object3D.h"
#include "Object3DCommon.h"
#include "Input.h"
#include "Camera.h"

class Camera; // 前方宣言

class PauseMenu
{
public:
	//初期化
	void Initialize(Object3DCommon* object3dcommon, bool isPlayScene);
	//更新
	void Update();
	//描画
	void Draw();
public:
	bool IsPaused()const;
	
	void SetCamera(Camera* camera) {
		camera_ = camera;
	}


	void ControllerUpdate();

	void PausedStart();

private:
	Object3DCommon* object3dcommon_;
	Camera* camera_ = nullptr;
	Transform transform;
	Input* input;
	//ポーズ画面のオブジェクト
	std::unique_ptr<Object3D>object;

	// テキストオブジェクトのリスト
	std::vector<std::unique_ptr<Object3D>> TextObjects;
	Transform texttransform[3]{};

	uint32_t textindex = 0;

	//ポーズ開く
	bool isPaused_ = false;
	//操作説明表示
	bool isOperation_ = false;
	//イージングタイマー
	float easeTimer_ = 0.0f;
	float easeTimer2_ = 0.0f;

	bool easingsceneFlag_ = false; 
	bool easingmoveFlag_ = false;


	bool scenefige = false;
	bool isClosing_ = false;
};

