#pragma once
#include "BaseScene.h"
#include "Camera.h"	
#include "Model.h"
#include"Sprite.h"
#include "Object3D.h"
#include "Audio.h"
#define _USE_MATH_DEFINES
#include <math.h>

class GameClearScene :public BaseScene
{

public:
	/// <summary>
	/// シーンの初期化
	/// </summary>
	void Initialize() override;
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


	void EasingMove();

	void StartJump();


	float EaseOutElastic(float t, float b, float c, float d, float amplitudeScale = 0.3f) {
		if (t == 0) return b;
		if ((t /= d) == 1) return b + c;
		float p = d * 0.1f; // 伸縮の周期
		float a = c * amplitudeScale; // 振幅をスケールダウン
		float s = p / 4.0f;
		return a * powf(2.0f, -10.0f * t) * sinf((t * d - s) * (2.0f * static_cast<float>(M_PI)) / p) + c + b;
	}

	float EaseOutBounce(float time, float start, float change, float duration) {
		float t = time / duration;
		if (t < (1 / 2.75f)) {
			return change * (7.5625f * t * t) + start;
		} else if (t < (2 / 2.75f)) {
			t -= (1.5f / 2.75f);
			return change * (7.5625f * t * t + 0.75f) + start;
		} else if (t < (2.5 / 2.75)) {
			t -= (2.25f / 2.75f);
			return change * (7.5625f * t * t + 0.9375f) + start;
		} else {
			t -= (2.625f / 2.75f);
			return change * (7.5625f * t * t + 0.984375f) + start;
		}
	}

	void ControllerUpdate();

private:
	// テキストオブジェクト
	std::vector<std::unique_ptr<Object3D>> Cleartext_;
	// 最大テキスト数
	uint32_t MaxtextIndex_ = 3;
	// 	UI
	std::vector<std::unique_ptr<Sprite>> TextUI_;
	std::unique_ptr<Sprite> ArroTextUI_;

	float elapsedTime = 0.0f; // 経過時間
	const float animationDuration = 1.0f; // アニメーション時間
	bool allObjectsFinished = false; // すべてのオブジェクトがアニメーションを終わらせたかを追跡

	float deltaTime = 0.01f;

	float objecttime_[3];
	float startDelay[3];
	bool Changefige = false;
	// 速度を格納するための配列（各オブジェクトの速度を管理）
	std::vector<Vector3> velocity_;


	uint32_t Selectindex = 2;

};