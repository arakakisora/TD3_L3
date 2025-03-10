#pragma once
#include "Camera.h"	
#include "Model.h"
#include"Sprite.h"
#include "Object3D.h"
#include "Object3DCommon.h"
#include "Audio.h"
#include "BaseScene.h"

class TitleScene :public BaseScene
{

public:
	/// <summary>
	/// シーンの初期化
	/// </summary>
	void Initialize()override;
	/// <summary>
	/// シーンの終了処理
	/// </summary>
	void Finalize()override;
	/// <summary>
	/// シーンの更新
	/// </summary>
	void Update()override;
	/// <summary>
	/// シーンの描画
	/// </summary>
	void Draw()override;

public:
	std::unique_ptr<Camera> camera1;
	std::unique_ptr<Camera> camera2;
	Object3DCommon* object3dcommon;
	Transform transform;
	//シーン演出時のオブジェクト
	std::unique_ptr<Object3D>object1;
	//シーン遷移フラグ
	bool isSceneChange_ = false;
	//イージングタイマー
	float easingTimer_ = 0.0f;
};

