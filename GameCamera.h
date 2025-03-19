#pragma once
#include "Camera.h"	
#include "Model.h"
#include"Sprite.h"
#include "Object3D.h"
#include "Audio.h"
#include "BaseScene.h"
#include"GameCamera.h"

#include "SceneManager.h"
#include "ParticleEmitter.h"
#include "ParticleMnager.h"

#include "Map.h"


// ゲームカメラ
class GameCamera {
public:
	// 描画範囲 (例: 2×2)
	static inline const uint32_t kRenderWidth = 2;
	static inline const uint32_t kRenderHeight = 2;
	/// <summary>
	/// シーンの初期化
	/// </summary>
	void Initialize(Map* map);
	/// <summary>
	/// シーンの終了処理
	/// </summary>
	void Finalize();
	/// <summary>
	/// シーンの更新
	/// </summary>
	void Update();
	/// <summary>
	/// シーンの描画
	/// </summary>
	void Draw();


	void move();

	void GameCamertakeaphoto();
	void GameCameraphoto(vector<std::vector<Block*>>& blockobject3D);
private:
	Map* map_;
	// ゲームカメラ
	std::vector<std::unique_ptr<Object3D>> gamecameras_;
	// スタート開始番号
	uint32_t xIndex = 0;
	uint32_t yIndex = 0;
	// 座標
	Vector3 position;
	Vector3 size;
};
