#pragma once
#include "Camera.h"	
#include "Model.h"
#include"Sprite.h"
#include "Object3D.h"
#include "Audio.h"
#include "BaseScene.h"
#include "FadeManager.h"

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

private:
	enum ObjectType {
		Title,       // タイトル
		Start,       // スタート
		Player,      // プレイヤー
		Count        // 要素数
	};

	// テクスチャの読み込み
	void LoadAllTextures();
	// モデルの読み込み
	void LoadAllModels();
	// シャッター演出の開始
	void shatterEffect();
	// シャッター演出の更新
	void shutterEffectUpdate();
	// プレイヤーのイージング処理移動
	void UpdatePlayerPositionByStep(float deltaTime);
private:
	// UIオブジェクト
	std::array<std::unique_ptr<Object3D>, ObjectType::Count> titleObjects_;

	uint32_t time = 0;
	bool timehige = false;
	int timer = 0;

	// 背景
	std::unique_ptr<Object3D>skydome_;
	FadeManager fadeManager_;

	// 演出のステップ
	int currentStep = 0;

	//シャッター演出用
	// シャッター演出制御用
	bool isShutterEffectPlaying = false;
	float shutterAnimTime = 0.0f;
	const float shutterAnimDuration = 0.15f;
	std::unique_ptr<Object3D> shuttertopObject; // シャッター演出用のオブジェクト
	std::unique_ptr<Object3D> shutterbottomObject; // シャッター演出用のオブジェクト

	// イージング処理
	float easingTimer = 0.0f;
	bool isEasing = false;
	const float easingDuration = 1.0f; // 秒単位（1秒で完了）

	// プレイヤーを浮かせる処理
	bool cameraMode_;
	Vector3 basePosition_{};
	Vector3 offset{};
	int frameCount_ = 0;


	bool isnextStep = false;
	float nextcurrentSteptime = 0;
	float MaxnextcurrentSteptime = 500;

	// 決定サウンド
	SoundData ButtonSound;
	// カメラコピーサウンド
	SoundData copeSound;

	// メインサウンド
	SoundData Bgm;
	// メインサウンド開始フラグ
	bool bgmstart = false;
};