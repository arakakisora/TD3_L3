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

struct GameCameraPosition
{
	uint32_t X, Y;
};

// ゲームカメラ
class GameCamera
{
public:

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
	void Update(Map* map);
	/// <summary>
	/// シーンの描画
	/// </summary>
	void Draw();

	// ゲームカメラの移動処理
	void GameCameraMove();
	// 写真を撮って貼る処理
	void GameCamertakeaphoto(Map* map);
	void UpdateGridSize(uint32_t size, bool vertical);
	void UpdateCameraPositions(Map* map);
private:
	// ゲームカメラ
	std::vector<std::unique_ptr<Object3D>> gamecameras_;
	// ゲームカメラ座標
	Vector3 cameraposition_;
	// 要素数
	static uint32_t camesize_;
	// コピーしたマップチップ番号を保存する変数
	uint32_t copiedTile = -1;
	// ゲームカメラの配置
	uint32_t cols_; // 横
	uint32_t rows_; // 盾
	// 縦長か、横長かの判定チェック
	bool isVertical_;
};