#pragma once
#include "Camera.h"	
#include "Model.h"
#include"Sprite.h"
#include "Object3D.h"
#include "Audio.h"
#include "BaseScene.h"

#include "SceneManager.h"
#include "ParticleEmitter.h"
#include "ParticleMnager.h"

#include "Map.h"

struct ModelAndTexture {
	std::string model;      // モデルファイルのパス
	std::string texture;    // テクスチャファイルのパス
};
	
// マップチップタイプごとの対応するモデルを管理するマップ
static std::map<MapChipType, std::string> mapChipTypemodelMap = {
	{MapChipType::kBlank, "axis.obj"},
	{MapChipType::kCopyBlock, "block.obj"},
	{MapChipType::kFallBlock,"fallblock.obj"},
	// 他のマップチップタイプに対しても設定可能
};


// ゲームカメラ
class ObjectCamera {
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

	// 移動
	void move();

	// オブジェクトをコピー
	void GameCamertakeaphoto();

	// オブジェクトを貼り付け
	void GameCameraphoto();

	// MapChipType に対応する処理を関数ポインタとして管理
	void HandleMapChip(uint32_t currentXIndex, uint32_t currentYIndex, MapChipType mapChipType);

private:
	Map* map_;
	// ゲームカメラ
	std::vector<std::vector<std::unique_ptr<Object3D>>> gamecameras_;
	// スタート開始番号
	uint32_t xIndex = 0;
	uint32_t yIndex = 0;
	// 座標
	Vector3 position;
	Vector3 size;
};
