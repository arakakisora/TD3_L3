#pragma once
#include "Input.h"
#include "Model.h"

#include "assert.h"
#include <algorithm>
#include <numbers>
#include "MyMath.h"

#include "RenderingData.h"

#include "Object3D.h"
#include<Audio.h>
#include "Map.h"



enum class LRDirecion {
	kright,
	kLeft,
};

struct CollisionMapInfo {

	bool ceiling = false; // 天井衝突
	bool landing = false; // 着地
	bool hitWall = false; // 壁接触
	Vector3 move;         // 移動量
};

enum Corner {
	kRightBottom,
	kLeftBottom,
	kRightTop,
	kLeftTop,
	kNumCorner // 要素数
};

enum class CollisionType {
	Top,
	Bottom,
	Left,
	Right
};

struct PlayerParameter {
	//プレイヤーパラメータ
//speedパラメータ
	float kAccleration = 0.05f;  // 定数加速度
	float kAttenuation = 0.2f;   // 速度減衰率
	float kLimitRunSpeed = 0.2f; // 最大速度制限
	//jannpパラメータ
	float kGravityAccleration = 0.05f; // 重力加速度
	float kLimitFallSpeed = 1.0f;      // 最大落下速度
	float kJampAcceleration = 0.5f;    // ジャンプ初速
	float kJampBlockAcceleration = 0.8f;//ジャンプブロックのジャンプ初速
	float kAccumulateJumpTime_ = 0.2f;   //溜め時間
	//当たり判定パラメータ
	float kWidth = 0.8f;//当たり判定の幅
	float kHeight = 0.8f;//当たり判定の高さ
	float kBlank = 2.0;//当たり判定の余裕
	float kCollisionsmallnumber = 0.1f;//当たり判定の余裕
	//減衰パラメータ
	float kAttenuationLanding = 0.1f;//着地時の減衰率
	float kAttenuationWall = 1.0f;//壁に当たった時の減衰率
	// 振り向きパラメータ
	float KtimeTurn = 1.0f; // 角度補間タイム
};

class Enemy;
//class Map;
class Player {

public:

	


	// 初期化
	void Initialize(const Vector3& position);

	~Player();

	// 更新
	void Update();

	// 描画
	void Draw();

	void PrayerMove(); // 自機の動き

	void PlayerTurn(); // 自機の向き

	void PlayerDebug(); // 自機の死亡処理

	void PlayerMode(); // 自機のモード

	bool IsHittableBlock(MapChipType type);

	bool CheckCollisionPoints(
		const std::array<Vector3, 2>& posList,
		CollisionType type,
		CollisionMapInfo& info
	);

	void CollisionMapInfoDirection(
		CollisionMapInfo& info,
		CollisionType dir,
		const std::array<Corner, 2>& checkCorners,
		const Vector3& offset,
		std::function<bool(const CollisionMapInfo&)> moveCondition
	);

	void MapCollision(CollisionMapInfo& info); //ポインタ関数std::functionを使って、当たり判定の方向を指定する



	void PlayerCollisionMove(const CollisionMapInfo& inffo);// プレイヤー衝突移動
	void CeilingCollisionMove(const CollisionMapInfo& info);// 天井衝突移動
	void OnGroundSwitching(CollisionMapInfo& info);// 着地判定
	void HitWallCollisionMove(const CollisionMapInfo& info);// 壁衝突移動

	// Blockクラスへのカメラモード 
	bool SetCameraMode(bool cameraMode) { return cameraMode = cameraMode_; }

	Vector3 CornerPosition(const Vector3& centor, Corner corner);// 4つの角の位置を計算yo


	//アクセッサ
	//死ぬ系
	bool GetIsDead_() const { return isDead_; }// 死フラグ
	void SetIsDead_(bool isDead) { isDead_ = isDead; }// 死フラグを立てる
	void SetDeathHeight(float height) { deathHeight_ = height; }// 落下死の高さを設定
	//トランスフォーム
	const Transform& GetTransform() { return object3D_->GetTransform(); }// トランスフォーム取得
	void SetTransform(const Transform& transform) { object3D_->SetTransform(transform); }// トランスフォーム設定
	const Vector3& GetVelocity() const { return velocity_; }// 速度取得
	void SetMapChipField(Map* mapChipFild) { mapChipFild_ = mapChipFild; }// マップチップフィールド設定

	bool GetCameraMode() { return cameraMode_; }// カメラモード取得

	bool GetCheckGoal() { return CheckGoal; }

	Vector3 GetWorldPosition() {

		Vector3 worldPos;

		worldPos.x = object3D_->GetWorldMatrix().m[3][0];;
		worldPos.y = object3D_->GetWorldMatrix().m[3][1];;
		worldPos.z = object3D_->GetWorldMatrix().m[3][2];;
		return worldPos;
	};

	Vector3 GetTranslate() { return object3D_->GetTranslate(); }
	bool GetPrayerMoveRight() { return playermoveright; }
	bool GetPrayerMoveLeft() { return playermoveleft; }
	void SetPrayerMoveRight(bool right) { playermoveright = right; }
	void SetPrayerMoveLeft(bool left) { playermoveleft = left; }
	// プレイヤーパラメータの読み込み
	PlayerParameter LoadPlayerParameters(const std::string& filePath);

	Object3D* GetObject3D() { return object3D_.get(); } // Object3D取得

private:

	//objec3D
	std::unique_ptr<Object3D> object3D_ = nullptr; // Object3D

	float deltaTime_ = 1.0f / 60.0f;


	PlayerParameter playerParameter_; // プレイヤーパラメータ

	Vector3 velocity_ = {}; // 速度
	// ジャンプ
	bool onGround_ = true; // 接点状態フラグ
	bool isAccumulateJump_ = false;                         //ジャンプ準備
	float AccumulateJumpTimer_ = 0.0f;
	// マップ
	Map* mapChipFild_ = nullptr;
	// 振り向き
	LRDirecion lrDirection_ = LRDirecion::kright;
	float turnFirstRotationY_ = 0.0f;           // 現在の向き
	float turnTimer_ = 0.0f;                    // 振り向き時間
	//死んだ
	bool isDead_ = false;
	//落下死高さ
	float deathHeight_; // 落下死の高さ
	//カメラモードフラグ
	bool cameraMode_ = false;
	// クリアシーン移動フラグ
	bool CheckGoal = false;
	// プレイヤー移動フラグ
	bool playermoveright = false;
	bool playermoveleft = false;
	// ジャンプサウンド
	SoundData jumpSound;
	// 決定サウンド
	SoundData ButtonSound;
};
