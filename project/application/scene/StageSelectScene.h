#pragma once
#include "Camera.h"	
#include "Model.h"
#include "Sprite.h"
#include "Object3D.h"
#include "Audio.h"
#include "BaseScene.h"
#include <cmath>
#include "ParticleMnager.h"
#include "PauseMenu.h"
#include "FadeManager.h"
#include "ParticleEmitter.h"
#include "StageSelectResourceID.h"

class StageSelectScene :public BaseScene{
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
	enum StageType { // ステージの種類
		stage_01, stage_02, stage_03, stage_04, stage_05,
		stage_06, stage_07, stage_08, stage_09, stage_10,
		stage_11, stage_12, stage_13,
		Count                             // 要素数
	};
	// 移動
	void move();
	// シーン遷移
	void moveChangeScene();	
	// オーディオの更新
	void UpdateAudio();
	// 非ポーズ中の更新
	void UpdateDuringPlay();
	// ポーズ中の更新
	void UpdateDuringPause();
	// パーティクルの更新
	void UpdatePlayerParticle();
	// デバッグ用の ImGui 
	void DebugimgGui();
private:
	// 2dリソースのIDを読み込む
	void LoadTextures(const std::vector<StageSelectID::TextureID>& textureIDs);
	// 3dリソースのIDを読み込む
	void LoadModels(const std::vector<StageSelectID::ModelID>& modelIDs);
private:
	// ポインタ
	std::unique_ptr<Camera> camera_;
	std::unique_ptr<Object3D> Player_;
	// ステージオブジェクトのリスト
	std::array<std::unique_ptr<Object3D>, StageType::Count> stages_;
	uint32_t MaxSelectIndex_ = 13;	// 最大ステージ数
	uint32_t currentIndex_ = 0;	// 現在のステージ
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
	bool titlefige_ = false;
	//ポーズメニュー
	std::unique_ptr<PauseMenu>pauseMenu;
	// 背景
	std::unique_ptr<Object3D>skydome_;
	FadeManager fadeManager_;
	// ステージ選択時のプレイヤーの位置
	Vector3 playerInitialOffset_;
	// プレイヤーのパーティクルエミッター
	std::unique_ptr<ParticleEmitter> playeremitter_;
	// プレイヤー移動フラグ
	bool playermoveright = false; // 右移動フラグ
	bool playermoveleft = false;  // 左移動フラグ
	// 背景モデル
	std::unique_ptr<Object3D> backPlane_;
	SoundData selectSound;	// セレクトサウンド
	SoundData ButtonSound;	// 決定サウンド
};