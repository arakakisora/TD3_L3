#include "Player.h"
#include "MyMath.h"
#include <algorithm>

//初期化
void Player::Initialize(Object3DCommon* object3dcommon) {
	object3dcommon = object3dcommon;
	object = std::make_unique<Object3D>();
	object->Initialize(object3dcommon);
	object->SetModel("plane.obj");
	object->SetScale({ 0.5f,0.5f,0.5f });
	transform.translate = { 0.0f,0.0f,0.0f };
}
//更新
void Player::Update(const Map& map) {

	//カメラモードに入っていない間移動可能
	if (!CamerMode) {
		//仮移動処理 ただ上下左右に動くだけ。
		if (Input::GetInstans()->PushKey(DIK_W))
		{
			transform.translate.y += speed;
		} else if (Input::GetInstans()->PushKey(DIK_S))
		{
			transform.translate.y -= speed;
		} else if (Input::GetInstans()->PushKey(DIK_D))
		{
			transform.translate.x += speed;
		} else if (Input::GetInstans()->PushKey(DIK_A))
		{
			transform.translate.x -= speed;
		}

		transform.translate = transform.translate + velocity;
	}

	//本実装移動処理
	//Move(); 

	//Cキーを押してカメラモードへ
	if (Input::GetInstans()->TriggerKey(DIK_C) && !CamerMode) {
		CamerMode = true;
	}
	else if (Input::GetInstans()->TriggerKey(DIK_C) && CamerMode)
	{
		CamerMode = false;
	}

	//クリア判定
	if (CheckGoal(map)) {
		speed = 2;
	}

	object->SetTranslate(transform.translate);

	object->Update();
}
//描画
void Player::Draw() {

	object->Draw();
}
//移動
void Player::Move() {
	//接地状態
	if (onGround) {
		//左右移動操作
		if (Input::GetInstans()->PushKey(DIK_A) || Input::GetInstans()->PushKey(DIK_D)) {
			//左右加速
			Vector3 acceleration = {};
			if (Input::GetInstans()->PushKey(DIK_D)) {
				//左移動中の右入力
				if (velocity.x < 0.0f) {
					//速度と逆方向に入力中は急ブレーキ
					velocity.x *= (1.0f - kAttenuation);
				}
				acceleration.x += kAcceleration;
			} else if (Input::GetInstans()->PushKey(DIK_A)) {
				//右移動中の左入力
				if (velocity.x > 0.0f) {
					//速度と逆方向に入力中は急ブレーキ
					velocity.x *= (1.0f - kAttenuation);
				}
				acceleration.x -= kAcceleration;
			}
			//加速・減速
			velocity = MyMath::Add(velocity, acceleration);
			//最大速度制限
			velocity.x = std::clamp(velocity.x, -kLimitRunSpeed, kLimitRunSpeed);
		} else
		{
			//非入力時は移動減衰をかける
			velocity.x *= (1.0f - kAttenuation);
		}
		if (Input::GetInstans()->PushKey(DIK_W)) {
			//ジャンプ初速
			velocity = MyMath::Add(velocity, Vector3(0, kJumpAcceleration, 0));
		}
	} else
	{
		/*
		//落下速度
		velocity = MyMath::Add(velocity, Vector3(0, -kGravityAcceleration, 0));
		//落下速度制限
		velocity.y = max(velocity.y, -kLimitFallSpeed);
	}
	//地面との当たり判定
	if (velocity.y < 0) {
		//Y座標が地面以下になったら着地
		if (transform.translate.y <= 1.0f) {
			landing = true;
		}
	}
	//着地判定
	if (onGround) {
		//ジャンプ開始
		if (velocity.y > 0.0f) {
			//空中状態へ
			onGround = false;
		}
	}
	else
	{
		//着地
		if (landing)
		{
			//めり込み排斥
			transform.translate.y = 3.0f;
			//摩擦で横方向速度が減衰
			velocity.x *= (1.0f - kAttenuationLanding);
			//下方向速度をリセット
			velocity.y = 0.0f;
			//接地状態へ
			onGround = true;
		}
		*/
	}
}
//マップ衝突判定
void Player::MapCollision(CollisionMapInfo& info) {
	MapTopCollision(info);
	MapBottomCollision(info);
	MapRightCollision(info);
	MapLeftCollision(info);
}
//上
void Player::MapTopCollision(CollisionMapInfo& info) {
	//上昇あり？
	if (info.moveAmount.y <= 0)
	{
		return;
	}
	//移動後の4つの角の座標
	std::array<Vector3, kNumCorner>positionNew;
	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(transform.translate + info.moveAmount, static_cast<Corner>(i));
	}
	//真上の当たり判定(Mapchip実装後)

}
//下
void Player::MapBottomCollision(CollisionMapInfo& info) {
	//下降あり
	if (info.moveAmount.y >= 0) {
		return;
	}
	std::array<Vector3, kNumCorner>positionNew;

	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(transform.translate + info.moveAmount, static_cast<Corner>(i));
	}
	//真下の当たり判定(Mapchip実装後)

}
//右
void Player::MapRightCollision(CollisionMapInfo& info) {
	//右移動？
	if (info.moveAmount.x <= 0) {
		return;
	}
	std::array<Vector3, kNumCorner>positionNew;
	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(transform.translate + info.moveAmount, static_cast<Corner>(i));
	}

	//Mapchip実装後
}
//左
void Player::MapLeftCollision(CollisionMapInfo& info) {
	//左移動？
	if (info.moveAmount.x >= 0) {
		return;
	}
	std::array<Vector3, kNumCorner>positionNew;
	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(transform.translate + info.moveAmount, static_cast<Corner>(i));
	}

	//Mapchip実装後
}

//判定結果を反映して移動
void Player::ResultMove(const CollisionMapInfo& info) {
	//Mapchip実装後
}

//ゴール判定
bool Player::CheckGoal(const Map& map) {
	Vector3 goalPos = map.GetGoalPosition();
	float distance = MyMath::Length(transform.translate - goalPos);
	if (distance < 0.8f) {//ゴールに十分に近づいたら判定内
		return true;
	}
	return false;
}

Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {
	Vector3 offsetTable[kNumCorner] = {
		{kWidth / 2.0f, -kHeight / 2.0f, 0},//kRightBottom
		{-kWidth / 2.0f,-kHeight / 2.0f ,0},//kLeftBottom
		{kWidth / 2.0f,kHeight / 2.0f,0},//kRightTop
		{-kWidth / 2.0f,kHeight / 2.0f,0}//kLeftTop
	};
	return center + offsetTable[static_cast<uint32_t>(corner)];
}