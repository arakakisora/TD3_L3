#pragma once
#include "Object3D.h"
#include "Map.h"
#include "Block.h"
#include "Sprite.h"
#include "BitmapFont.h"
#include <memory>
#include <Audio.h>

using namespace std;
class PhotoCamera
{
public:
	// コンストラクタ
	PhotoCamera();
	// デストラクタ
	~PhotoCamera();

	// 初期化
	void Initialize(Map* map);
	// 更新
	void Update(Map* map,const bool cameraMode);
	// 描画 / 3DObject
	void Draw3DObject();
	// 描画 / Sprite
	void DrawSprite();
	// 終了処理
	void Finalize();
	// カメラの移動
	void Move();

	//コントローラー操作
	void stickMove();
	
	// マップデータのコピー
	void Copy();

	// コピーしたマップデータの貼り付け 
	void Paste();

	// imguiの描画
	void DrawImGui();

	void shatterEffect();
	void shutterEffectUpdate();

public:	// Setter / Getter

	// カメラの位置を取得
	Vector2 GetPosition() { return position; }
	// カメラのサイズを取得
	Vector2 GetRangeSize() { return rangeSize; }
	// カメラのオブジェクトを取得
	Object3D* GetObject3D() { return object3D.get(); }

	//カメラが起動したか
	bool HasStarted() const { return CamerMode; }
	//カメラが動いたか
	bool HasMoved() const { return position.x != initialPos.x || position.y != initialPos.y; }
	//コピーしたか
	bool HasCopied() const { return shutterCount > 0; }
	//初回コピーフラグ取得
	bool GetFirstCopied()const { return isFirstCopied; }
	//初回ペーストフラグ取得
	bool GetFirstPaseted()const { return isFirstPasted; }
	//カメラモード取得
	bool GetCameraMode()const { return CamerMode; }
	void SetcameraMode(bool mode) { CamerMode = mode; } // カメラモードを設定
public:
	//初回コピーしたか
	bool isFirstCopied = false;
	//初回ペーストしたか
	bool isFirstPasted = false;
private:
	static PhotoCamera* instance_;
	Map* map;
	MapChipData mapData;
	// カメラの位置
	Vector2 position;
	Vector2 initialPos;
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

	// cameraMode用
	bool cameraMode_;


	//イージング用
	Vector2 currentPos; // 実際に描画される位置（イージング用）
	Vector2 targetPos;  // 入力で更新されるターゲット位置
	float moveTimer = 1.0f;     // イージング用時間 [0〜1]
	float moveSpeed = 0.1f;     // 1フレームごとの t 増加量
	bool isMoving = false;      // 現在移動中かどうか

	//シャッター演出用
	// シャッター演出制御用
	bool isShutterEffectPlaying = false;
	float shutterAnimTime = 0.0f;
	const float shutterAnimDuration = 0.15f;
	Object3D* shuttertopObject; // シャッター演出用のオブジェクト
	Object3D* shutterbottomObject; // シャッター演出用のオブジェクト


	// 移動サウンド
	SoundData moveSound;
	// コピーサウンド
	SoundData copeSound;
	// ペーストサウンド
	SoundData pasteSound;
};

