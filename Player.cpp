#define NOMINMAX
#include "Player.h"
#include "Map.h"
#include <cassert>
#include "Input.h"
#include "Logger.h"
#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG
#include "Object3DCommon.h"
#include <SceneManager.h>
#include "Easing.h"



void Player::Initialize(Object3D* object3D, const Vector3& position) {


	// モデルの初期化
	object3D_ = object3D;
	// プレイヤーの初期位置
	object3D_->SetTranslate(position);
	object3D_->SetRotate({ 0, std::numbers::pi_v<float> / 2.0f , 0 });



}

Player::~Player()
{
}



void Player::Update() {

#ifdef _DEBUG

	if (ImGui::CollapsingHeader("Player", ImGuiTreeNodeFlags_DefaultOpen))
	{
		Transform transform = object3D_->GetTransform();

		//ImGui::DragFloat3("*ModelScale", &transform.scale.x, 0.01f);
		ImGui::DragFloat3("*PlayerRotate", &transform.rotate.x, 0.01f);
		ImGui::DragFloat3("*PlayerTransrate", &transform.translate.x, 0.01f);
		object3D_->SetTransform(transform);

		//カメラフラグ
		ImGui::Text("CameraMode %d", cameraMode_);
	}
#endif // DEBUG_

#ifdef _DEBUG

	//Cキーを押してカメラモードへ
	if (Input::GetInstance()->TriggerKey(DIK_C)) {
		cameraMode_ = !cameraMode_;
	}
#endif // _DEBUG


	if (Input::GetInstance()->TriggerGamePadButton(XINPUT_GAMEPAD_B) && onGround_) {
		cameraMode_ = !cameraMode_;
	}

	if (!cameraMode_) {

		PrayerMove();
		PlayerTurn();
		// 衝突判定を初期化
		CollisionMapInfo collisionMapInfo;
		// 移動量に速度の値をコピー
		collisionMapInfo.move = velocity_;
		// マップ衝突チェック
		MapCollision(collisionMapInfo);
		// 移動
		CeilingCollisionMove(collisionMapInfo);// 天井衝突移動
		OnGroundSwitching(collisionMapInfo);// 着地判定
		HitWallCollisionMove(collisionMapInfo);// 壁衝突移動
		PlayerCollisionMove(collisionMapInfo);// プレイヤー衝突移動

	}



	////PrayerTurn();
	object3D_->Update();

	//// ゴールフラグがたったらクリアシーンに移動
	//if (CheckGoal) {
	//	//SceneManager::GetInstance()->ChangeScene("GAMECLEAR");
	//}
}

void Player::Draw() {
	object3D_->Draw();

}

void Player::PrayerMove() {
#ifdef _DEBUG

	// 左右移動操作（キーボード）
	if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
		// 左右加速
		Vector3 accceleration = {};
		if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
			if (velocity_.x < 0.0f) {
				velocity_.x *= (1.0f - kAttenuation);
			}

			if (lrDirection_ != LRDirecion::kright) {
				lrDirection_ = LRDirecion::kright;
				turnFirstRotationY_ = object3D_->GetTransform().rotate.y;
				turnTimer_ = kLimitRunSpeed;
			}

			accceleration.x += kAccleration;
		} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
			if (velocity_.x > 0.0f) {
				velocity_.x *= (1.0f - kAttenuation);
			}

			if (lrDirection_ != LRDirecion::kLeft) {
				lrDirection_ = LRDirecion::kLeft;
				turnFirstRotationY_ = object3D_->GetTransform().rotate.y;
				turnTimer_ = kLimitRunSpeed;
			}

			accceleration.x -= kAccleration;
		}

		velocity_.x += accceleration.x;
		velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
	} else {
	}
#endif // _DEBUG

		// X軸の減速処理（Y軸には影響を与えない）
		velocity_.x *= (1.0f - kAttenuation);

	// コントローラー操作（左右移動）
	if (Input::GetInstance()->GetGamePadStickX() > 0 || Input::GetInstance()->GetGamePadStickX() < 0) {
		Vector3 accceleration = {};
		if (Input::GetInstance()->GetGamePadStickX() > 0) {
			if (velocity_.x < 0.0f) {
				velocity_.x *= (1.0f - kAttenuation);
			}
			if (lrDirection_ != LRDirecion::kright) {
				lrDirection_ = LRDirecion::kright;
				turnFirstRotationY_ = object3D_->GetTransform().rotate.y;
				turnTimer_ = kLimitRunSpeed;
			}
			accceleration.x += kAccleration;
		} else if (Input::GetInstance()->GetGamePadStickX() < 0) {
			if (velocity_.x > 0.0f) {
				velocity_.x *= (1.0f - kAttenuation);
			}

			if (lrDirection_ != LRDirecion::kLeft) {
				lrDirection_ = LRDirecion::kLeft;
				turnFirstRotationY_ = object3D_->GetTransform().rotate.y;
				turnTimer_ = kLimitRunSpeed;
			}
			accceleration.x -= kAccleration;
		}

		velocity_.x += accceleration.x;
		velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
	} else {
		velocity_.x *= (1.0f - kAttenuation);
	}

	// ジャンプ処理
	if (onGround_) {
		if (
#ifdef _DEBUG
			Input::GetInstance()->PushKey(DIK_UP) ||
#endif // _DEBUG
			Input::GetInstance()->TriggerGamePadButton(XINPUT_GAMEPAD_A))
		{
			velocity_.y = kJampAcceleration; // += ではなく = にすることで、ジャンプの初速を一定にする
		}
	} else
	{
		// 重力適用
		velocity_.y += -kGravityAccleration;
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}


}

void Player::PlayerTurn()
{

	if (turnTimer_ > 0.0f) {
		turnTimer_ -= 1.0f / 30.0f;

		float t = std::clamp(1.0f - turnTimer_ / kLimitRunSpeed, 0.0f, 1.0f);
		float easedT = Easing::EaseOutQuad(t);

		float destinationRotationYTable[] = {
			std::numbers::pi_v<float> / 2.0f,
			std::numbers::pi_v<float> *3.0f / 2.0f,
		};
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];

		float newY = Easing::Lerp(turnFirstRotationY_, destinationRotationY, easedT);
		object3D_->SetRotate({ 0, newY, 0 });
	}


}





void Player::MapCollision(CollisionMapInfo& info) {

	CollisionMapInfoRight(info);
	CollisionMapInfoLeft(info);
	CollisionMapInfoTop(info);
	CollisionMapInfoBootm(info);
}

Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {

	Vector3 offseetTable[kNumCorner] = {

		{+kWidth / 2.0f, -kHeight / 2.0f, 0},
		{-kWidth / 2.0f, -kHeight / 2.0f, 0},
		{+kWidth / 2.0f, +kHeight / 2.0f, 0},
		{-kWidth / 2.0f, +kHeight / 2.0f, 0}
	};

	return center + offseetTable[static_cast<uint32_t>(corner)];


}

void Player::PlayerCollisionMove(const CollisionMapInfo& info) {
	// 移動
	//当たり判定後の移動量を取得
	Vector3 position = object3D_->GetTransform().translate;
	position.x += info.move.x;
	position.y += info.move.y;
	position.z += info.move.z;
	object3D_->SetTranslate(position);


}

// 天井当たった？
void Player::CeilingCollisionMove(const CollisionMapInfo& info) {

	if (info.ceiling) {

		Logger::Log("hit ceiling\n");
		velocity_.y = 0.0f;
	}
}

// 地面に当たった？
void Player::OnGroundSwitching(CollisionMapInfo& info) {

	if (onGround_) {
		if (velocity_.y > 0.0f) {

			onGround_ = false;

		} else {
			// 移動後4つの計算
			std::array<Vector3, kNumCorner> positionsNew;
			for (uint32_t i = 0; i < positionsNew.size(); ++i) {

				Vector3 position = object3D_->GetTransform().translate;
				position += info.move;
				positionsNew[i] = CornerPosition(position, static_cast<Corner>(i));
			}
			MapChipType mapChipType;
			// 真下の当たり判定
			bool hit = false;

			// 左点の判定
			IndexSet indexSet;
			indexSet = mapChipFild_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom] + Vector3(0, -kCollisionsmallnumber, 0));
			mapChipType = mapChipFild_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kCopyBlock) {
				hit = true;
			} else if (mapChipType == MapChipType::kFallBlock) {
				hit = true;
			} else if (mapChipType == MapChipType::kNCopyBlock) {
				hit = true;
			} else if (mapChipType == MapChipType::kGoalUp) {
				CheckGoal = true;
			} else if (mapChipType == MapChipType::kGoalDown) {
				CheckGoal = true;
			} else if (mapChipType == MapChipType::kFixedTimeBlock) {
				hit = true;
			} else if (mapChipType == MapChipType::kPutFixedTimeBlock) {
				hit = true;
			}

			// 右点の判定
			indexSet = mapChipFild_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom] + Vector3(0, -kCollisionsmallnumber, 0));
			mapChipType = mapChipFild_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kCopyBlock) {
				hit = true;
			} else if (mapChipType == MapChipType::kFallBlock) {
				hit = true;
			} else if (mapChipType == MapChipType::kNCopyBlock) {
				hit = true;
			} else if (mapChipType == MapChipType::kGoalUp) {
				CheckGoal = true;
			} else if (mapChipType == MapChipType::kGoalDown) {
				CheckGoal = true;
			} else if (mapChipType == MapChipType::kFixedTimeBlock) {
				hit = true;
			} else if (mapChipType == MapChipType::kPutFixedTimeBlock) {
				hit = true;
			}



			if (!hit) {

				onGround_ = false;
			}
		}

	} else {

		if (info.landing) {

			Logger::Log("hit ongruond\n");
			onGround_ = true;
			velocity_.x = 0.0f;
			velocity_.y = 0.0f;
		}
	}
}

void Player::CollisionMapInfoBootm(CollisionMapInfo& info) {
	if (info.move.y >= 0) {
		return;
	}
	// 移動後4つの計算
	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {

		Vector3 position = object3D_->GetTransform().translate;
		position += info.move;
		positionsNew[i] = CornerPosition(position, static_cast<Corner>(i));
		//positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}
	MapChipType mapChipType;
	// 真下の当たり判定
	bool hit = false;

	// 左点の判定
	IndexSet indexSet;
	indexSet = mapChipFild_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipFild_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kCopyBlock) {
		hit = true;
	} else if (mapChipType == MapChipType::kFallBlock) {
		hit = true;
	} else if (mapChipType == MapChipType::kNCopyBlock) {
		hit = true;
	} else if (mapChipType == MapChipType::kGoalUp) {
		CheckGoal = true;
	} else if (mapChipType == MapChipType::kGoalDown) {
		CheckGoal = true;
	} else if (mapChipType == MapChipType::kFixedTimeBlock) {
		hit = true;
	} else if (mapChipType == MapChipType::kPutFixedTimeBlock) {
		hit = true;
	}
	// 右点の判定
	indexSet = mapChipFild_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipFild_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kCopyBlock) {
		hit = true;
	} else if (mapChipType == MapChipType::kFallBlock) {
		hit = true;
	} else if (mapChipType == MapChipType::kNCopyBlock) {
		hit = true;
	} else if (mapChipType == MapChipType::kGoalUp) {
		CheckGoal = true;
	} else if (mapChipType == MapChipType::kGoalDown) {
		CheckGoal = true;
	} else if (mapChipType == MapChipType::kFixedTimeBlock) {
		hit = true;
	} else if (mapChipType == MapChipType::kPutFixedTimeBlock) {
		hit = true;
	}

	// hit
	if (hit) {
		Logger::Log("hit landing\n");
		Vector3 position = object3D_->GetTransform().translate;
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipFild_->GetMapChipIndexSetByPosition(position + Vector3(0, +kHeight / 2.0f, 0));
		// めり込み先ブロックの範囲矩形
		Rect rect = mapChipFild_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.y = std::min(0.0f, rect.top - position.y + (kHeight / 2.0f + kBlank));
		// 地面に当たったらことを記録する
		info.landing = true;

	}
}

// 壁に当たった？
void Player::HitWallCollisionMove(const CollisionMapInfo& info) {

	if (info.hitWall) {

		velocity_.x *= (1.0f - kAttenuationWall);
	}
}



void Player::CollisionMapInfoTop(CollisionMapInfo& info) {

	if (info.move.y <= 0) {
		return;
	}
	// 移動後4つの計算
	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {

		Vector3 position = object3D_->GetTransform().translate;
		position += info.move;
		positionsNew[i] = CornerPosition(position, static_cast<Corner>(i));

		//positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	// 真上の当たり判定
	bool hit = false;
	// 左点の判定
	IndexSet indexSet;
	indexSet = mapChipFild_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipFild_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kCopyBlock) {
		hit = true;
	} else if (mapChipType == MapChipType::kFallBlock) {
		hit = true;
	} else if (mapChipType == MapChipType::kNCopyBlock) {
		hit = true;
	} else if (mapChipType == MapChipType::kGoalUp) {
		CheckGoal = true;
	} else if (mapChipType == MapChipType::kGoalDown) {
		CheckGoal = true;
	} else if (mapChipType == MapChipType::kFixedTimeBlock) {
		hit = true;
	} else if (mapChipType == MapChipType::kPutFixedTimeBlock) {
		hit = true;
	}
	// 右点の判定
	//   左点の判定

	indexSet = mapChipFild_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipFild_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kCopyBlock) {
		hit = true;
	} else if (mapChipType == MapChipType::kFallBlock) {
		hit = true;
	} else if (mapChipType == MapChipType::kNCopyBlock) {
		hit = true;
	} else if (mapChipType == MapChipType::kGoalUp) {
		CheckGoal = true;
	} else if (mapChipType == MapChipType::kGoalDown) {
		CheckGoal = true;
	} else if (mapChipType == MapChipType::kFixedTimeBlock) {
		hit = true;
	} else if (mapChipType == MapChipType::kPutFixedTimeBlock) {
		hit = true;
	}

	// hit
	if (hit) {

		Vector3 position = object3D_->GetTransform().translate;

		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipFild_->GetMapChipIndexSetByPosition(position + Vector3(0, -kHeight / 2.0f, 0));
		// めり込み先ブロックの範囲矩形
		Rect rect = mapChipFild_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.y = std::max(0.0f, rect.bottom - position.y - (kHeight / 2.0f + kBlank));
		// 天井に当たったらことを記録する
		info.ceiling = true;
	}
}



void Player::CollisionMapInfoRight(CollisionMapInfo& info) {

	if (info.move.x <= 0) {
		return;
	}
	// 移動後4つの計算
	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		Vector3 position = object3D_->GetTransform().translate;
		position += info.move;
		positionsNew[i] = CornerPosition(position, static_cast<Corner>(i));

		//positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	// 真上の当たり判定
	bool hit = false;
	// 右上点の判定
	IndexSet indexSet;
	indexSet = mapChipFild_->GetMapChipIndexSetByPosition(positionsNew[kRightTop] + Vector3(+kCollisionsmallnumber, 0, 0));
	mapChipType = mapChipFild_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kCopyBlock) {
		hit = true;
	} else if (mapChipType == MapChipType::kFallBlock) {
		hit = true;
	} else if (mapChipType == MapChipType::kNCopyBlock) {
		hit = true;
	} else if (mapChipType == MapChipType::kGoalUp) {
		CheckGoal = true;
	} else if (mapChipType == MapChipType::kGoalDown) {
		CheckGoal = true;
	} else if (mapChipType == MapChipType::kFixedTimeBlock) {
		hit = true;
	} else if (mapChipType == MapChipType::kPutFixedTimeBlock) {
		hit = true;
	}

	// 右下点の判定

	indexSet = mapChipFild_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom] + Vector3(+kCollisionsmallnumber, 0, 0));
	mapChipType = mapChipFild_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kCopyBlock) {
		hit = true;
	} else if (mapChipType == MapChipType::kFallBlock) {
		hit = true;
	} else if (mapChipType == MapChipType::kNCopyBlock) {
		hit = true;
	} else if (mapChipType == MapChipType::kGoalUp) {
		CheckGoal = true;
	} else if (mapChipType == MapChipType::kGoalDown) {
		CheckGoal = true;
	} else if (mapChipType == MapChipType::kFixedTimeBlock) {
		hit = true;
	} else if (mapChipType == MapChipType::kPutFixedTimeBlock) {
		hit = true;
	}

	// hit
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		//DebugText::GetInstance()->ConsolePrintf("hit hitwall\n");

		Logger::Log("hit hitwall\n");

		Vector3 position = object3D_->GetTransform().translate;
		indexSet = mapChipFild_->GetMapChipIndexSetByPosition(position + Vector3(-kWidth / 2.0f, 0, 0));
		// めり込み先ブロックの範囲矩形
		Rect rect = mapChipFild_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.x = std::max(0.0f, rect.left - position.x - (kWidth / 2.0f + kBlank));
		// 壁に当たったらことを記録する
		info.hitWall = true;
	}
}

void Player::CollisionMapInfoLeft(CollisionMapInfo& info) {
	if (info.move.x >= 0) {
		return;
	}
	// 移動後4つの計算
	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		Vector3 position = object3D_->GetTransform().translate;
		position += info.move;
		positionsNew[i] = CornerPosition(position, static_cast<Corner>(i));
		//positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	// 真上の当たり判定
	bool hit = false;
	// hidari上点の判定
	IndexSet indexSet;
	indexSet = mapChipFild_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop] + Vector3(-kCollisionsmallnumber, 0, 0));
	mapChipType = mapChipFild_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kCopyBlock) {
		hit = true;
	} else if (mapChipType == MapChipType::kFallBlock) {
		hit = true;
	} else if (mapChipType == MapChipType::kNCopyBlock) {
		hit = true;
	} else if (mapChipType == MapChipType::kGoalUp) {
		CheckGoal = true;
	} else if (mapChipType == MapChipType::kGoalDown) {
		CheckGoal = true;
	} else if (mapChipType == MapChipType::kFixedTimeBlock) {
		hit = true;
	} else if (mapChipType == MapChipType::kPutFixedTimeBlock) {
		hit = true;
	}

	// hidari下点の判定

	indexSet = mapChipFild_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom] + Vector3(-kCollisionsmallnumber, 0, 0));
	mapChipType = mapChipFild_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kCopyBlock) {
		hit = true;
	} else if (mapChipType == MapChipType::kFallBlock) {
		hit = true;
	} else if (mapChipType == MapChipType::kNCopyBlock) {
		hit = true;
	} else if (mapChipType == MapChipType::kGoalUp) {
		CheckGoal = true;
	} else if (mapChipType == MapChipType::kGoalDown) {
		CheckGoal = true;
	} else if (mapChipType == MapChipType::kFixedTimeBlock) {
		hit = true;
	} else if (mapChipType == MapChipType::kPutFixedTimeBlock) {
		hit = true;
	}
	// hit
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		//DebugText::GetInstance()->ConsolePrintf("hit hitwall\n");
		Vector3 position = object3D_->GetTransform().translate;
		Logger::Log("hit hitwall\n");
		indexSet = mapChipFild_->GetMapChipIndexSetByPosition(position + Vector3(+kWidth / 2.0f, 0, 0));
		// めり込み先ブロックの範囲矩形
		Rect rect = mapChipFild_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.x = std::min(0.0f, rect.right - position.x + (kWidth / 2.0f + kBlank));
		// 壁に当たったらことを記録する
		info.hitWall = true;
	}
}



