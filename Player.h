#pragma once
#include "Object3D.h"
#include "Object3DCommon.h"
#include "Input.h"
#include "Camera.h"
#include "Map.h"

//角
enum Corner {
	kRightBottom,//右下
	kLeftBottom,//左下
	kRightTop,//右上
	kLeftTop,//左上

	kNumCorner//要素数
};
//マップ殿当たり判定情報
struct CollisionMapInfo {
	//天井衝突フラグ
	bool ceilCollision = false;
	//着地フラグ
	bool onGround = false;
	//壁接触フラグ
	bool wallhit = false;
	//移動量
	Vector3 moveAmount;
};
class Player
{
public:
	//初期化
	void Initialize(Object3DCommon* object3dcommon);
	//更新
	void Update(const Map& map);
	//描画
	void Draw();
	//移動
	void Move();
	//マップ衝突判定
	void MapCollision(CollisionMapInfo& info);
	void MapTopCollision(CollisionMapInfo& info);
	void MapBottomCollision(CollisionMapInfo& info);
	void MapRightCollision(CollisionMapInfo& info);
	void MapLeftCollision(CollisionMapInfo& info);
	//判定結果を反映して移動
	void ResultMove(const CollisionMapInfo& info);

	//ゴール判定bool CheckGoal(const Map& map);
    bool CheckGoal(const Map&map);
public:
	const Transform& GetPosition()const { return transform; }
	void SetTransform(const Transform& newTransform) { transform = newTransform; }

	Vector3 CornerPosition(const Vector3& center, Corner corner);
private:
	Object3DCommon* object3dcommon;
	Camera* camera;
	Transform transform;
	Input* input;
	std::unique_ptr<Object3D>object;
	Vector3 velocity = {};
	//移動速度
	float speed = 0.05f;

	//加速度
	static inline const float kAcceleration = 0.1f;
	//速度減衰
	static inline const float kAttenuation = 0.5f;
	//速度制限
	static inline const float kLimitRunSpeed = 1.0f;
	//重力加速度
	static inline const float kGravityAcceleration = 0.1f;
	//最大落下速度
	static inline const float kLimitFallSpeed = 1.0f;
	//ジャンプ初速
	static inline const float kJumpAcceleration = 1.0f;
	//減衰
	static inline const float kAttenuationLanding = 0.5f;
	//プレイヤーの当たり判定サイズ
	static inline const float kWidth = 1.0f;
	static inline const float kHeight = 1.0f;
	//接地状態フラグ
	bool onGround = true;
	//着地フラグ
	bool landing = false;
	//カメラモードフラグ
	bool CamerMode = false;
};

