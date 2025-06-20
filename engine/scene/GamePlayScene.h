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
	void GameReset();

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
	
	enum OperationTextType { // 操作説明の種類
		StickL,
		ButtonB,
		ButtonA,
		ButtonX,
		ButtonY,
		LB,
		RB,
		Idou,
		Kirikae,
		Toru,
		Haiti,
		Zyanpu,
		Reset,
		Plus,
		OperationTextCount // 要素数
	};
	// 操作説明のパラメータ
	struct OperationSpriteParam {
		const char* texturePath;
		Vector2 position;
		Vector2 size;
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
	std::unique_ptr<Tutorial>tutorial;
	//リセットお知らせ
	std::unique_ptr<Object3D>ResetNotice;
	//操作説明テキスト
	std::array<std::unique_ptr<Sprite>, OperationTextCount>operationTexts;	

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

