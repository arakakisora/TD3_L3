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

	//リセット
	void Reset();

	// Imgui描画
	void DrawImgui();

public:	// Getter
	bool GetCameraMode();

private:
	std::unique_ptr<Camera> camera1;
	std::unique_ptr<Camera> camera2;
	//プレイヤー
	std::unique_ptr<Player>player;
	Object3D* object3DPlayer=nullptr;
	Vector3 playeroffset{};

	//チュートリアルテキスト
	std::unique_ptr<Tutorial>tutorial;

	//リセットお知らせ
	std::unique_ptr<Object3D>ResetNotice;

	//操作説明テキスト
	//std::vector<std::unique_ptr<Sprite>>operationTexts;
	std::unique_ptr<Sprite>OperationtextStickL;
	std::unique_ptr<Sprite>OperationtextButtonB;
	std::unique_ptr<Sprite>OperationtextButtonA;
	std::unique_ptr<Sprite>OperationtextX;
	std::unique_ptr<Sprite>OperationtextY;
	std::unique_ptr<Sprite>OperationtextLB;
	std::unique_ptr<Sprite>OperationtextRB;
	std::unique_ptr<Sprite>OperationtextIdou;
	std::unique_ptr<Sprite>OperationtextKrikae;
	std::unique_ptr<Sprite>OperationtextToru;
	std::unique_ptr<Sprite>OperationtextHaiti;
	std::unique_ptr<Sprite>OperationtextZyanpu;
	std::unique_ptr<Sprite>OperationtextReset;
	std::unique_ptr<Sprite>OperationtextPlus;

	//リセットメータのスプライト
	std::unique_ptr<Sprite>resetMeter;

	std::vector < std::unique_ptr<Sprite>> pauseui;
	Map* map=nullptr;
	
	// @ 消すAlso delete the ones in the CPP file.
	// ゲームカメラ
	//ObjectCamera* gameCamera_;

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

	//リセット用タイマー
    float holdTime = 0.0f;
	const float holdDuration = 1.7f;

	//リセットテキスト用タイマー

	//経過時間
	float elapsedTime = 0.0f;
	//30秒後に表示
	const float afterseconds = 30.0f;
	//30秒経過フラグ
	bool secondspassed = false;

	const float deltaTime = 1.0f / 60.0f;

	ParticleEmitter* emitter_;
	ParticleEmitter* playeremitter_;

};

