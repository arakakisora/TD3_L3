#define NOMINMAX
#include "Player.h"

#include <cassert>
#include "Input.h"
#include "Logger.h"
#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG
#include "Object3DCommon.h"
#include <SceneManager.h>
#include "Easing.h"

using namespace Easing;

void Player::Initialize(const Vector3& position) {

	// モデルの初期化
	object3D_ = std::make_unique<Object3D>();
	object3D_->Initialize(Object3DCommon::GetInstance());
	object3D_->SetModel("playercharacter.obj");
	object3D_->SetScale(Vector3{ 1.0f,1.0f,1.0f });
	object3D_->SetLighting(true);
	object3D_->SetDirectionalLightEnable(true);
	object3D_->SetDirectionalLightDirection({ -1.3f,-1.82f,-4.77f });
	object3D_->SetTranslate(position);	// プレイヤーの初期位置
	object3D_->SetRotate({ 0, std::numbers::pi_v<float> / 2.0f , 0 });
	SetDeathHeight(0.0f);

	// ジャンプ用サウンド
	jumpSound = Audio::GetInstance()->SoundLoadWave("Resources/Audio/Jump.wav");
	// 決定用サウンド
	ButtonSound = Audio::GetInstance()->SoundLoadWave("Resources/Audio/Button.wav");
}

Player::~Player()
{

}

void Player::Update() {


	PlayerDebug();


	if (Input::GetInstance()->TriggerGamePadButton(XINPUT_GAMEPAD_B) && onGround_) {
		cameraMode_ = !cameraMode_;
		// 決定の音声を流す
		Audio::GetInstance()->SoundPlayWave(ButtonSound);
	}

	//カメらモードがoffならプレイヤーモード
	if (!cameraMode_) {

		PlayerMode();

	}


	object3D_->Update();
}

void Player::Draw() {
	object3D_->Draw();

}

void Player::PrayerMove() {
	// コントローラー操作（左右移動）
	if (
#ifdef _DEBUG
		Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT) ||
#endif // _DEBUG
		Input::GetInstance()->GetGamePadStickX() > 0 || Input::GetInstance()->GetGamePadStickX() < 0) {


		Vector3 accceleration = {};
		if (
#ifdef _DEBUG
			Input::GetInstance()->PushKey(DIK_RIGHT) ||
#endif // _DEBUG
			Input::GetInstance()->GetGamePadStickX() > 0) {
			if (velocity_.x < 0.0f) {
				velocity_.x *= (1.0f - kAttenuation);
			}
			if (lrDirection_ != LRDirecion::kright) {
				lrDirection_ = LRDirecion::kright;
				turnFirstRotationY_ = object3D_->GetTransform().rotate.y;
				turnTimer_ = KtimeTurn;
			}
			accceleration.x += kAccleration;

			// パーティクルのフラグを設定（右移動）
			playermoveright = true;
			playermoveleft = false;
		} else if (
#ifdef _DEBUG
			Input::GetInstance()->PushKey(DIK_LEFT) ||
#endif // _DEBUG
			Input::GetInstance()->GetGamePadStickX() < 0) {
			if (velocity_.x > 0.0f) {
				velocity_.x *= (1.0f - kAttenuation);
			}
			if (lrDirection_ != LRDirecion::kLeft) {
				lrDirection_ = LRDirecion::kLeft;
				turnFirstRotationY_ = object3D_->GetTransform().rotate.y;
				turnTimer_ = KtimeTurn;
			}
			accceleration.x -= kAccleration;

			// パーティクルのフラグを設定（左移動）
			playermoveleft = true;
			playermoveright = false;
		}

		velocity_.x += accceleration.x;
		velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
	} else {
		velocity_.x *= (1.0f - kAttenuation);

		// スティックが真ん中なら両方falseにする
		playermoveright = false;
		playermoveleft = false;
	}

	// ジャンプ処理
	if (onGround_) {
		if (
#ifdef _DEBUG
			Input::GetInstance()->PushKey(DIK_UP) ||
#endif // _DEBUG
			Input::GetInstance()->TriggerGamePadButton(XINPUT_GAMEPAD_A))
		{
			// ジャンプサウンド開始
			Audio::GetInstance()->SoundPlayWave(jumpSound);

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

		float t = std::clamp(1.0f - turnTimer_ / 1.0f, 0.0f, 1.0f);
		float easedT = EaseOutQuad(t);

		float destinationRotationYTable[] = {
			std::numbers::pi_v<float> / 2.0f,
			std::numbers::pi_v<float> *3.0f / 2.0f,
		};
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];

		float newY = Lerp(turnFirstRotationY_, destinationRotationY, easedT);
		object3D_->SetRotate({ 0, newY, 0 });
	}

}

void Player::PlayerDebug()
{
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
	//Cキーを押してカメラモードへ
	if (Input::GetInstance()->TriggerKey(DIK_C)) {
		cameraMode_ = !cameraMode_;
	}
#endif // DEBUG_


}

void Player::PlayerMode()
{
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

bool Player::IsHittableBlock(MapChipType type)
{
	switch (type) {
	case MapChipType::kCopyBlock:
	case MapChipType::kFallBlock:
	case MapChipType::kNCopyBlock:
	case MapChipType::kFixedTimeBlock:
	case MapChipType::kPutFixedTimeBlock:
		return true;
	default:
		return false;
	}
}

bool Player::CheckCollisionPoints(const std::array<Vector3, 2>& posList, CollisionType type, CollisionMapInfo& info)
{
	bool hit = false;

	for (const auto& pos : posList) {
		IndexSet index = mapChipFild_->GetMapChipIndexSetByPosition(pos);
		MapChipType chip = mapChipFild_->GetMapChipTypeByIndex(index.xIndex, index.yIndex);

		if (chip == MapChipType::kjumpBlock) {
			if (type == CollisionType::Bottom) {
				velocity_.y = kJampBlockAcceleration;
				onGround_ = false;
				return false; // 地面に着地して跳ねたら他の処理不要
			} else {
				hit = true; // 天井や壁からは跳ね返すだけ
			}
		}
		else if (IsHittableBlock(chip)) {
			hit = true;
		}
		else if (chip == MapChipType::kGoalUp || chip == MapChipType::kGoalDown) {
			CheckGoal = true;
		}
	}

	if (hit) {
		Vector3 position = object3D_->GetTransform().translate;
		IndexSet index = mapChipFild_->GetMapChipIndexSetByPosition(position);

		Rect rect = mapChipFild_->GetRectByIndex(index.xIndex, index.yIndex);

		switch (type) {
		case CollisionType::Top:
			info.move.y = std::max(0.0f, rect.bottom - position.y - (kHeight / 2.0f + kBlank));
			info.ceiling = true;
			break;
		case CollisionType::Bottom:
			info.move.y = std::min(0.0f, rect.top - position.y + (kHeight / 2.0f + kBlank));
			info.landing = true;
			break;
		case CollisionType::Right:
			info.move.x = std::max(0.0f, rect.left - position.x - (kWidth / 2.0f + kBlank));
			info.hitWall = true;
			break;
		case CollisionType::Left:
			info.move.x = std::min(0.0f, rect.right - position.x + (kWidth / 2.0f + kBlank));
			info.hitWall = true;
			break;
		}
	}

	return hit;
}

void Player::CollisionMapInfoDirection(CollisionMapInfo& info, CollisionType dir, const std::array<Corner, 2>& checkCorners, const Vector3& offset, std::function<bool(const CollisionMapInfo&)> moveCondition)
{
	if (!moveCondition(info)) return;// 移動量が0なら何もしない
	Vector3 position = object3D_->GetTransform().translate + info.move;// 現在の位置に移動量を加算

	// 2つのコーナー位置を計算
	std::array<Vector3, 2> points = {
		CornerPosition(position, checkCorners[0]) + offset,
		CornerPosition(position, checkCorners[1]) + offset
	};
	// 衝突判定を行う
	if (CheckCollisionPoints(points, static_cast<CollisionType>(dir), info)) {
#ifdef _DEBUG
		switch (dir) {
		case CollisionType::Top: Logger::Log("hit ceiling\n"); break;
		case CollisionType::Bottom: Logger::Log("hit landing\n"); break;
		case CollisionType::Left:
		case CollisionType::Right: Logger::Log("hit hitwall\n"); break;
		}
#endif
	}
}


void Player::MapCollision(CollisionMapInfo& info) {

	CollisionMapInfoDirection(
		info,
		CollisionType::Right,
		{ kRightTop, kRightBottom },
		Vector3(kCollisionsmallnumber, 0, 0),
		[](const CollisionMapInfo& i) { return i.move.x > 0; }
	);

	CollisionMapInfoDirection(
		info,
		CollisionType::Left,
		{ kLeftTop, kLeftBottom },
		Vector3(-kCollisionsmallnumber, 0, 0),
		[](const CollisionMapInfo& i) { return i.move.x < 0; }
	);

	CollisionMapInfoDirection(
		info,
		CollisionType::Top,
		{ kLeftTop, kRightTop },
		Vector3(0, 0, 0),
		[](const CollisionMapInfo& i) { return i.move.y > 0; }
	);

	CollisionMapInfoDirection(
		info,
		CollisionType::Bottom,
		{ kLeftBottom, kRightBottom },
		Vector3(0, 0, 0),
		[](const CollisionMapInfo& i) { return i.move.y < 0; }
	);
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
			} else if (mapChipType == MapChipType::kjumpBlock) {

				velocity_.y = kJampBlockAcceleration;
				onGround_ = false;
				return;
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
			} else if (mapChipType == MapChipType::kjumpBlock) {
				velocity_.y = kJampBlockAcceleration;
				onGround_ = false;
				return;
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


// 壁に当たった？
void Player::HitWallCollisionMove(const CollisionMapInfo& info) {

	if (info.hitWall) {

		velocity_.x *= (1.0f - kAttenuationWall);
	}
}

