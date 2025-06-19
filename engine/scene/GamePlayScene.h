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

	enum TutorialTextType { // チュートリアルの種類
		Text1,                            // チュートリアル_01 
		Text2,						      // チュートリアル_02
		Text3,							  // チュートリアル_03
		Text4,						      // チュートリアル_04
		Text5,					    	  // チュートリアル_05
		Text6,				    		  // チュートリアル_06
		Text7,			    			  // チュートリアル_07
		Text8,		     				  // チュートリアル_08
		Text9,                            // 空白コピペ
		Text10,                           // リセット
		Text11,                           // ×ブロック説明
		Text12,                           // タイマー
		Text13,                           // ジャンプ
		Count                             // 要素数
	};
	// チュートリアルテキストのパラメータ
	struct TutorialTextParam {
		const char* modelPath;
		Vector3 scale;
		Vector3 rotate;
		Vector3 translate;
	};

public:	// Getter
	bool GetCameraMode();

private:
	std::unique_ptr<Camera> camera1;
	std::unique_ptr<Camera> camera2;
	//プレイヤー
	std::unique_ptr<Player>player;
	Vector3 playeroffset{};
	//チュートリアルテキスト
	std::array<std::unique_ptr<Object3D>, Count>tutorialTexts;	
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

	//ブロックのスプライト
	std::unique_ptr<Sprite>nCopySprite;
	std::unique_ptr<Sprite>jumpSprite;
	std::unique_ptr<Sprite>timerSprite;

	//リセットメータのスプライト
	std::unique_ptr<Sprite>resetMeter;

	std::vector < std::unique_ptr<Sprite>> pauseui;
	Map* map=nullptr;
	
	// @ 消すAlso delete the ones in the CPP file.
	// ゲームカメラ
	//ObjectCamera* gameCamera_;

	PhotoCamera* photoCamera;

	bool tutorial1_2 = false;
	bool tutorial3_4 = false;
	bool tutorial5 = false;
	bool tutorial6_7 = false;
	bool tutorial8 = false;
	bool tutorial9 = false;
	bool tutorial10 = false;
	bool tutorial11 = false;
	bool tutorial12 = false;
	bool tutorial13 = false;

	bool cameraStarted = false;
	bool cameraMoved = false;
	bool cameraCopied = false;
	bool cameraPaseted = false;

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

