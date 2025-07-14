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


class Enemy;
class Map;
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



	void CalculateCollisionBounds(CollisionMapInfo& info);

	// 当たり判定
	void CollisionMapInfoTop(CollisionMapInfo& info);// 天井衝突
	void CollisionMapInfoBootm(CollisionMapInfo& info);// 床衝突
	void CollisionMapInfoRight(CollisionMapInfo& info);// 右壁衝突
	void CollisionMapInfoLeft(CollisionMapInfo& info);// 左壁衝突

	void PlayerCollisionMove(const CollisionMapInfo& inffo);// プレイヤー衝突移動
	void CeilingCollisionMove(const CollisionMapInfo& info);// 天井衝突移動
	void OnGroundSwitching(CollisionMapInfo& info);// 着地判定
	void HitWallCollisionMove(const CollisionMapInfo& info);// 壁衝突移動
	
	// Blockクラスへのカメラモード 
	bool SetCameraMode(bool cameraMode) { return cameraMode = cameraMode_; }

	Vector3 CornerPosition(const Vector3& centor, Corner corner);// 4つの角の位置を計算yo
	// map衝突判定
	void MapCollision(CollisionMapInfo& info);// マップ衝突判定


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


	Object3D* GetObject3D() { return object3D_.get(); } // Object3D取得

private:

	//objec3D
	std::unique_ptr<Object3D> object3D_ = nullptr; // Object3D


	Vector3 velocity_ = {};                          // 速度
	static inline const float kAccleration = 0.05f;  // 定数加速度
	static inline const float kAttenuation = 0.2f;   // 速度減衰率
	static inline const float kLimitRunSpeed = 0.2f; // 最大速度制限
	float deltaTime_ = 1.0f / 60.0f;
	// ジャンプ
	bool onGround_ = true;                                 // 接点状態フラグ
	static inline const float kGravityAccleration = 0.05f; // 重力加速度
	static inline const float kLimitFallSpeed = 1.0f;      // 最大落下速度
	static inline const float kJampAcceleration = 0.5f;    // ジャンプ初速
	static inline const float kJampBlockAcceleration = 0.8f;//ジャンプブロックのジャンプ初速
	bool isAccumulateJump_ = false;                         //ジャンプ準備
	float AccumulateJumpTimer_ = 0.0f;
	static inline const float kAccumulateJumpTime_ = 0.2f;   //溜め時間
	// 当たり判定
	Map* mapChipFild_ = nullptr;
	static inline const float kWidth = 0.8f;//当たり判定の幅
	static inline const float kHeight = 0.8f;//当たり判定の高さ
	static inline const float kBlank = 2.0;//当たり判定の余裕
	static inline const float kAttenuationLanding = 0.1f;//着地時の減衰率
	static inline const float kCollisionsmallnumber = 0.1f;//当たり判定の余裕
	static inline const float kAttenuationWall = 1.0f;//壁に当たった時の減衰率

	// 振り向き
	LRDirecion lrDirection_ = LRDirecion::kright;
	float turnFirstRotationY_ = 0.0f;           // 現在の向き
	float turnTimer_ = 0.0f;                    // 振り向き時間
	static inline const float KtimeTurn = 1.0f; // 角度補間タイム

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
