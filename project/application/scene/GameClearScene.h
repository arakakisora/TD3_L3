#pragma once
#include "BaseScene.h"
#include "Camera.h"	
#include "Model.h"
#include"Sprite.h"
#include "Object3D.h"
#include "Audio.h"
#include<GameClearResourceID.h>

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

	void ControllerUpdate();

	// 2dリソースのIDを読み込む
	void LoadTextures(const std::vector<GameClearTextureID>& textureIDs);
	// 3dリソースのIDを読み込む
	void LoadModels(const std::vector<GameClearModelID>& modelIDs);

private:
	// テキストオブジェクト
	std::vector<std::unique_ptr<Object3D>> Cleartext_;
	// 最大テキスト数
	static const uint32_t MaxtextIndex_ = 3;
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

	float elapsedTime_;
	float elapsedFrames_ = 0;
	float jumpEndTime_[3] = { 0.0f };

	uint32_t MaxStageIndex_ = 13;

	uint32_t Selectindex = 2;

	bool nextsneneonthit = false;


	// 背景
	std::unique_ptr<Object3D> skydome_;


	// メンバ変数で以下を持つ（全オブジェクト用）
	uint32_t nextStage = 0;
	float acceleration = -9.8f;
	float jumpHeight = 2.5f;
	float jumpInterval = 0.2f;
	float cooldownTime = 1.0f; // 個々のクールタイム（秒）

	bool initialized = false;

	std::vector<float> jumpStartTimes;
	std::vector<float> velocities;
	std::vector<bool> isJumping;
	std::vector<float> originalYPositions;
	std::vector<float> cooldownTimers;       // 個々のクールタイム管理
	std::vector<float> totalElapsedTimes;    // 個々の経過時間（ジャンプのタイミング調整）

	// 外に定義：補間進捗
	float easeT = 0.0f; // 0.0 ～ 1.0 に進める（別で Update）
	float easeSpeed = 0.05f; // 進むスピード（毎フレーム加算）

	// セレクトサウンド
	SoundData selectSound;
	// 決定サウンド
	SoundData ButtonSound;
};