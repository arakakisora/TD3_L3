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
		Title,                            // タイトル
		Start,                            // スタート
		Player,                           // プレイヤー
		Skydome,                          // 背景
		Shuttertop,                       // シャッター 上
		Shutterbottom,                    // シャッター 下
		Count                             // 要素数
	};
	enum SequencePhase {
		FadeIn = 0,           // フェードイン開始
		PlayerEasing = 1,     // プレイヤー移動開始
		Shutter = 2,          // シャッター開始
		LoopAnimation = 3,    // 入力待ち中のアニメーション
	};
	// シャッター演出の開始
	void shatterEffect();
	// シャッター演出の更新
	void shutterEffectUpdate();
	// プレイヤーのイージング処理移動
	void UpdatePlayerPositionByStep();
	// オーディオの更新
	void UpdateAudio();
	// 次のシーンへの入力関数
	void UpdateStartTrigger();
	// アニメーションフェーズの更新処理
	void UpdatePhase();
	// デバッグ用の ImGui 描画
	void DebugGui();
private:
	// ポインタ
	FadeManager fadeManager_;	// 背景
	// オブジェクトのリスト
	std::array<std::unique_ptr<Object3D>, ObjectType::Count> titleObjects_;
	// シャッター演出制御用
	bool isShutterEffectPlaying = false;      // シャッター開始フラグ
	float shutterAnimTime = 0.0f;             // シャッターの経過時間
	const float shutterAnimDuration = 0.15f;  // シャッター時間全体の長さ
	// イージング処理	
	bool isPhaseEasing = false;           // イージング開始フラグ
	float easingTimer = 0.0f;             // イージング経過時
	const float easingDuration = 1.0f;    // 秒単位（1秒で完了）
	// プレイヤーを浮かせる処理
	Vector3 playeroffset_{}; // 座標
	int frameCount_ = 0;     // フレームカウント
	// メインサウンド開始フラグ
	bool bgmstart = false;	
	SoundData BgmSound_;	// メインサウンド
	SoundData ButtonSound_;	// 決定サウンド
	SoundData copeSound;	// シャッターサウンド
	float nextcurrentSteptime = 0; // フェーズリセットの経過時間
	int timer = 0; // タイトルロゴ用タイマー
    bool isnextStep = false; // 次のステップへの有無	
	int PhaseIndex_;  // フェーズインデックス
};