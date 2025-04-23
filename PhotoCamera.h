#pragma once
#include "Object3D.h"
#include "Map.h"
#include "Block.h"
#include "Sprite.h"
#include "BitmapFont.h"
#include <memory>
using namespace std;
class PhotoCamera
{
public:
	// 初期化
	void Initialize(Map* map);
	// 更新
	void Update(Map* map);
	// 描画 / 3DObject
	void Draw3DObject();
	// 描画 / Sprite
	void DrawSprite();
	// 終了処理
	void Finalize();
	// カメラの移動
	void Move();
	// マップデータのコピー
	void Copy();

	// コピーしたマップデータの貼り付け 
	void Paste();

	// imguiの描画
	void DrawImGui();

public:	// Setter / Getter
	// 変更したマップデータをmapにセット
	//void SetMap(Map* map) { this->map = map; }

	// カメラの位置を取得
	Vector2 GetPosition() { return position; }
	// カメラのサイズを取得
	Vector2 GetRangeSize() { return rangeSize; }
	// カメラのオブジェクトを取得
	Object3D* GetObject3D() { return object3D.get(); }

	void SetcameraMode(bool mode) { CamerMode = mode; } // カメラモードを設定
private:
	Map* map;
	MapChipData mapData;
	// カメラの位置
	Vector2 position;
	// カメラのサイズ
	Vector2 rangeSize;
	// カメラのオブジェクト
	unique_ptr<Object3D> object3D;
	// コピーしたマップデータ
	vector<vector<MapChipType>> copyData;
	// コピーしたマップデータの描画用Blockクラス
	vector<Block*> blocks;
	// コピーしたマップデータの描画用Blockクラスの位置
	Vector3 blockPosition;

	// Y軸の座標変換用
	int photo_ConvertY = ((int)Map::kNumBlockVirtical - (int)position.y) - 1;
	// MapThipTypeのマップデータ
	MapChipType mapChipType;

	bool CamerMode = false;

	// カメラサイズ
	uint32_t cameraSizeX = 2;
	uint32_t cameraSizeY = 2;

	// シャッター回数上限
	uint32_t shutterLimitCountMax = 0;
	// シャッター回数
	uint32_t shutterCount = 0;

	// 残りシャッター枚数のリソースデータ
	vector<unique_ptr<Sprite>>shutterRests_;
	// ビットマップフォント
	unique_ptr<BitmapFont>bitmapFont = nullptr;


	//イージング用
	Vector2 currentPos; // 実際に描画される位置（イージング用）
	Vector2 targetPos;  // 入力で更新されるターゲット位置
	float moveTimer = 1.0f;     // イージング用時間 [0〜1]
	float moveSpeed = 0.1f;     // 1フレームごとの t 増加量
	bool isMoving = false;      // 現在移動中かどうか

};

