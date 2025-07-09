#pragma once
#include "Vector3.h"
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "MapChipType.h"

class Map;
struct MapChipData;


class Object3D;
class Block
{
public:
	Block();
	~Block();

	// 初期化
	void Initialize(MapChipType type, const Vector3& position, Map* map);
	// 更新
	void Update(const bool cameramode = false);
	// 描画
	void Draw();

	// 終了処理
	void Finalize();

	static Block* CreateBlock(MapChipType type, const Vector3& position);
	// 一定時間経過後に消えるブロックの貼り付け前（FixedTime)
	void FixedTimeBlock();
	// 一定時間経過後に消えるブロックの貼り付け後（PutFixedTime）
	void PutFixedTimeBlock();

	//落下フラグセット
	void SetFalling(bool falling);


	static Block* CreateBlock(MapChipType type, const Vector3& position, Map* map);

	
public:
		// ブロックの位置を設定
        void SetPosition(const Vector3& position) { this->position = position; }
		// ブロックの座標を取得し直接モデルの座標にする
		void SetObject3DPosiition(const Vector3& position);
		
		Vector3 GetPosition() { return position; }

private:
		int mapID;
		MapChipType type;
		// 3Dオブジェクト
		std::unique_ptr<Object3D> object3D; 
		// マップへの参照
		Map* map;

		Vector3 position;

private:	// 各ブロック用の変数
	// 一定時間経過後に消えるブロックの貼り付け前（FixedTime）No.7
	int fixedTimeCounter = 0; // フレームカウント
	bool isAlive = true;      // ブロックが生存しているか

	// 一定時間経過後に消えるブロックの貼り付け後（PutFixedTime）No.8
	bool isFixedTimeBlockPut = false;
	const int kFixedTime = 60 * 5; // 5秒

	// FallBlock用の変数
	float velocity = 0.0f;
	bool isFalling = false;
	static constexpr float gravity = 0.02f;

	// nullBlock用のCameraMode
	bool cameraMode_;
	Vector3 basePosition_{};
	Vector3 offset{};
	float frameCount_ = 0;


	Vector3 pos;
};

