#pragma once
#pragma once

#include "Camera.h"	
#include "Model.h"
#include"Sprite.h"
#include "Object3D.h"
#include "Object3DCommon.h"
#include "Audio.h"
#include "BaseScene.h"
#include"ObjectCamera.h"

#include "SceneManager.h"
#include "ParticleEmitter.h"
#include "ParticleMnager.h"
#include "Player.h"

#include "Map.h"

#include "PhotoCamera.h"

#include <array>


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

private:
	std::unique_ptr<Camera> camera1;
	std::unique_ptr<Camera> camera2;
	//プレイヤー
	std::unique_ptr<Player>player;
	Object3D* object3DPlayer=nullptr;

	//チュートリアルテキスト
	//std::array<std::unique_ptr<Object3D>, 8>tutorialTexts;
	std::unique_ptr<Object3D> Tutorialtext1;
	std::unique_ptr<Object3D>Tutorialtext2;
	std::unique_ptr<Object3D>Tutorialtext3;
	std::unique_ptr<Object3D>Tutorialtext4;
	std::unique_ptr<Object3D>Tutorialtext5;
	std::unique_ptr<Object3D>Tutorialtext6;
	std::unique_ptr<Object3D>Tutorialtext7;
	std::unique_ptr<Object3D>Tutorialtext8;
	std::unique_ptr<Object3D>Tutorialtext9;//空白コピペ

	//操作説明テキスト
	//std::vector<std::unique_ptr<Sprite>>operationTexts;
	std::unique_ptr<Sprite>OperationtextStickL;
	std::unique_ptr<Sprite>OperationtextButtonB;
	std::unique_ptr<Sprite>OperationtextButtonA;
	std::unique_ptr<Sprite>OperationtextLB;
	std::unique_ptr<Sprite>OperationtextRB;
	std::unique_ptr<Sprite>OperationtextIdou;
	std::unique_ptr<Sprite>OperationtextKrikae;
	std::unique_ptr<Sprite>OperationtextToru;
	std::unique_ptr<Sprite>OperationtextHaiti;
	std::unique_ptr<Sprite>OperationtextZyanpu;

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

	bool cameraStarted = false;
	bool cameraMoved = false;
	bool cameraCopied = false;
	bool cameraPaseted = false;

	// 天球モデル
	unique_ptr<Object3D> skydome_ = nullptr;
	float skydomerotate;

};

