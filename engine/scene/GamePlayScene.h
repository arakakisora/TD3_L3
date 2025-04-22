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


	Map* map=nullptr;
	
	// @ 消すAlso delete the ones in the CPP file.
	// ゲームカメラ
	//ObjectCamera* gameCamera_;

	PhotoCamera* photoCamera;

	// 天球モデル
	unique_ptr<Object3D> skydome_ = nullptr;
	float skydomerotate;

	// ビットマップフォント
	unique_ptr<BitmapFont>bitmapFont = nullptr;
};

