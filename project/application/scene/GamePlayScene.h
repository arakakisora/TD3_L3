#pragma once
#pragma once

#include "Camera.h"	
#include "Model.h"
#include"Sprite.h"
#include "Object3D.h"
#include "Object3DCommon.h"
#include "Audio.h"
#include "BaseScene.h"

#include "SceneManager.h"
#include "ParticleEmitter.h"
#include "ParticleMnager.h"
#include "Player.h"

#include "Map.h"

#include "PhotoCamera.h"
#include "PauseMenu.h"

#include <array>
#include"FadeManager.h"
#include "Tutorial.h"
#include "Operate.h"
#include "Reset.h"

struct OperationText {
	std::string texturePath;
	Vector2 position;
	Vector2 size;
};


#include "BitmapFont.h"

class GamePlayScene :public BaseScene
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

	// Imgui描画
	void DrawImgui();

public:	// Getter
	bool GetCameraMode();

private:
	std::unique_ptr<Camera> camera1;
	std::unique_ptr<Camera> camera2;
	//プレイヤー
	std::unique_ptr<Player>player;
	Vector3 playeroffset{};
	//チュートリアルテキスト
	std::unique_ptr<Tutorial>tutorial;
	//操作説明テキスト
	std::unique_ptr<Operate>operate;
	//リセット
	std::unique_ptr<Reset>reset;
	std::vector < std::unique_ptr<Sprite>> pauseui;
	Map* map = nullptr;

	std::unique_ptr<PhotoCamera>photoCamera;

	// 天球モデル
	unique_ptr<Object3D> skydome_ = nullptr;
	float skydomerotate;

	//ポーズメニュー
	std::unique_ptr<PauseMenu>pauseMenu;

	// フェードアウト
	FadeManager fadeManager_;
	// 切り替えフラグ:
	bool isfadesense_ = false;

	const float deltaTime = 1.0f / 60.0f;

	ParticleEmitter* emitter_;
	ParticleEmitter* playeremitter_;

};

