#pragma once
#include "Camera.h"	
#include "Model.h"
#include"Sprite.h"
#include "Object3D.h"
#include "Audio.h"
#include "BaseScene.h"
#include <cmath>
#include "ParticleMnager.h"
#include "PauseMenu.h"
#include "FadeManager.h"

class StageSelectScene :public BaseScene
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

	// 移動
	void move();

	// シーン遷移
	void moveChangeScene();

	// イージング関数（EaseInOutQuad）
	float EaseInOutQuad(float t) {
		return t < 0.5f ? 2.0f * t * t : 1.0f - static_cast<float>(pow(-2.0f * t + 2.0f, 2)) / 2.0f;
	}

	float EaseInOutBack(float x) {
		const float c1 = 1.70158f;
		const float c2 = c1 * 1.525f;

		if (x < 0.5f) {
			return (powf(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2;
		} else {
			return (powf(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2;
		}
	}

	float EaseInQuad(float t, float b, float c, float d) {
		t /= d;  // t を 0 から 1 の範囲に正規化
		return c * t * t + b;  // 二次イージング（遅く始まり、加速する）
	}

	Vector3 SmoothLerp(const Vector3& a, const Vector3& b, float t) {
		Vector3 ans;
		ans.x = a.x + t * (b.x - a.x);
		ans.y = a.y + t * (b.y - a.y);
		ans.z = a.z + t * (b.z - a.z);
		return ans;
	}

public:
	std::unique_ptr<Camera> camera_;
	// メインオブジェクト
	Object3D* Player_;
	// ステージ選択オブジェクトのリスト
	std::vector<std::unique_ptr<Object3D>> stageObjects_;
	// テキストオブジェクトのリスト
	std::vector<std::unique_ptr<Object3D>> textoObjects_;
	// 最大ステージ数
	uint32_t MaxSelectIndex_ = 3;
	// 現在のステージ
	uint32_t currentIndex_ = 0;

	// イージング
	bool easingmoveFlag_;
	bool easingsceneFlag_;
	float easingProgress_;
	Vector3 startPos_;
	Vector3 endPos_;
	float easingDuration_;

	Vector3 FollowTargetposition;
	int frameCounter_ = 0;  // フレームカウンター

	std::vector<std::unique_ptr<Sprite>> xboxui;
	std::vector < std::unique_ptr<Sprite>> pauseui;
	bool titlefige_ =false;

	//ポーズメニュー
	std::unique_ptr<PauseMenu>pauseMenu;

	// 背景
	std::unique_ptr<Object3D>skydome_;

	FadeManager fadeManager_;

};