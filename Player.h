#pragma once
#include "Input.h"
#include "Model.h"

#include "assert.h"
#include <algorithm>
#include <numbers>
#include "MyMath.h"

#include "RenderingData.h"

#include "Object3D.h"

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
	void Initialize(Object3D* object3D, const Vector3& position);

	~Player();

	// 更新
	void Update();

	// 描画
	void Draw();

	void PrayerMove(); // 自機の動き
	


	void CalculateCollisionBounds(CollisionMapInfo& info);
	
	// 当たり判定
	void CollisionMapInfoTop(CollisionMapInfo& info);// 天井衝突
	void CollisionMapInfoBootm(CollisionMapInfo& info);// 床衝突
	void CollisionMapInfoRight(CollisionMapInfo& info);// 右壁衝突
	void CollisionMapInfoLeft(CollisionMapInfo& info);// 左壁衝突

	void PlayerCollisionMove(const CollisionMapInfo& inffo);// プレイヤー衝突移動
	void CeilingCollisionMove(const CollisionMapInfo& info);// 天井衝突移動
	void OnGroundSwitching( CollisionMapInfo& info);// 着地判定
	void HitWallCollisionMove(const CollisionMapInfo& info);// 壁衝突移動


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

	bool GetcamerMode() { return CamerMode; }// カメラモード取得
	
	Vector3 GetWorldPosition() {

		Vector3 worldPos;

		worldPos.x = object3D_->GetWorldMatrix().m[3][0];;
		worldPos.y = object3D_->GetWorldMatrix().m[3][1];;
		worldPos.z = object3D_->GetWorldMatrix().m[3][2];;
		return worldPos;
	};

	

	

private:

	//objec3D
	Object3D* object3D_ = nullptr;

	
	Vector3 velocity_ = {};                          // 速度
	static inline const float kAccleration = 0.05f;  // 定数加速度
	static inline const float kAttenuation = 0.2f;   // 速度減衰率
	static inline const float kLimitRunSpeed = 1.0f; // 最大速度制限
	
	// ジャンプ
	bool onGround_ = true;                                 // 接点状態フラグ
	static inline const float kGravityAccleration = 0.05f; // 重力加速度
	static inline const float kLimitFallSpeed = 2.0f;      // 最大落下速度
	static inline const float kJampAcceleration = 0.5f;    // ジャンプ初速
	// 当たり判定
	Map* mapChipFild_ = nullptr;
	static inline const float kWidth = 0.8f;//当たり判定の幅
	static inline const float kHeight = 0.8f;//当たり判定の高さ
	static inline const float kBlank = 2.0;//当たり判定の余裕
	static inline const float kAttenuationLanding = 0.1f;//着地時の減衰率
	static inline const float kCollisionsmallnumber = 0.1f;//当たり判定の余裕
	static inline const float kAttenuationWall = 1.0f;//壁に当たった時の減衰率

	//死んだ
	bool isDead_ = false;
	//落下死高さ
	float deathHeight_; // 落下死の高さ
	//カメラモードフラグ
	bool CamerMode = false;
	
};
