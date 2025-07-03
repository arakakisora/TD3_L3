#include "Block.h"
#include "Object3D.h"
#include "Object3DCommon.h"
#include "ModelManager.h"
#include "Map.h"
#include <imgui.h>


Block::Block()
{
}

Block::~Block()
{
	Finalize();
}

void Block::Initialize(MapChipType type, const Vector3& position, Map* map) {
	this->type = type;
	this->map = map;
	object3D = new Object3D();
	object3D->Initialize(Object3DCommon::GetInstance());
	object3D->SetTranslate(position);
	velocity = 0.0f;

	switch ((type))
	{
		//case MapChipType::ここにマップチップタイプ:		// コピー不可能 No.2
		//	// モデル指定
		//	object3D->SetModel("使用したいモデル.obj");
			//　もし、ブロックようにクラスを作ったのであればここに初期化
			// 
		//	break;
	case MapChipType::kBlank:
		// モデル設定
		object3D->SetModel("nullBlock.obj");
		break;
	case MapChipType::kNCopyBlock:				// No.2 コピー不可能 
		// モデル指定
		object3D->SetModel("ncopyblock.obj");
		break;

	case MapChipType::kCopyBlock:				// No.3 コピー可能 
		// モデル指定
		object3D->SetModel("block.obj");
		break;

	case MapChipType::kGoalUp:					// No.4 ゴール上 
		// モデル指定
		object3D->SetModel("goalup.obj");
		object3D->SetScale(Vector3{ 0.6f, 0.6f, 0.6f });
		pos = object3D->GetTranslate();
		pos.y -=1.0f;
		object3D->SetTranslate(pos);
		break;

	case MapChipType::kGoalDown:				// No.5 ゴール下 
		// モデル指定
		basePosition_ = position;
		object3D->SetModel("gall.obj");
		object3D->SetScale(Vector3{ 0.6f, 0.6f, 0.6f });
		break;

	case MapChipType::kFallBlock:				// No.6 落下ブロック
		//モデル指定
		object3D->SetModel("fallblock.obj");
		break;

	case MapChipType::kFixedTimeBlock:			// No.7 一定時間経過したら消えるブロック 
		// モデル指定
		object3D->SetModel("Timer.obj");
		break;

	case MapChipType::kPutFixedTimeBlock:		// No.8 貼り付け後一定時間 
		// モデル指定
		object3D->SetModel("putTimer.obj");
		break;

	case MapChipType::kjumpBlock:               // No.9 プレイヤーの上昇
		// モデル指定
		object3D->SetModel("jump.obj");
		break;
	}

	object3D->SetLighting(true);
	object3D->SetDirectionalLightEnable(true);
	object3D->SetDirectionalLightDirection({ 0.88f, -1.90f, 4.0f });
}


void Block::Update(const bool cameraMode) {

	cameraMode_ = cameraMode;
	/*else if (MapChipType::マップチップタイプ == type) {
	* モデルの更新なのでこれは絶対に必要
		object3D->Update();
		作ったブロックの更新処理
		ブロッククラス内で作ればそれを呼ぶだけでいい
		追加する場合はkNCopyBlockの部分を参考に
	}*/
	if (MapChipType::kCopyBlock == type) {
		object3D->Update();
	} else if (MapChipType::kGoalUp == type) {

		
		object3D->Update();
	} else if (MapChipType::kGoalDown == type) {
		frameCount_-=0.05f;
		//// 浮遊の動きを作る
		//float time = static_cast<float>(frameCount_) * 0.05f; // frameCount_ は毎フレーム +1 されると仮定
		//float amplitude = 0.2f;  // 浮遊の高さ（-0.7 ～ +0.7）
		//float frequency = 0.5f;  // 動く速さ（大きいほど速くなる）
		//float cycle = 2.0f * 3.14159265f / frequency;
		//if (time >= cycle) {
		//	frameCount_ = 0;
		//	time = 0.0f;
		//}
		//float floatY = std::sin(time * frequency) * amplitude;
		//offset = { 0.0f,0.5f,0.0f };
		//offset.y += floatY;
		//Vector3 newpos;
		//newpos = { basePosition_.x + offset.x, basePosition_.y + offset.y, basePosition_.z + offset.z };

		object3D->SetRotate(Vector3{ 0.0f, 0.0f, (float)frameCount_ });
		object3D->Update();
	} else if (MapChipType::kNCopyBlock == type) {
		object3D->Update();
	} else if (MapChipType::kFixedTimeBlock == type) {
		object3D->Update();
		FixedTimeBlock();
	} else if (MapChipType::kPutFixedTimeBlock == type) {
		object3D->Update();
		if (!cameraMode_) {
			PutFixedTimeBlock();
		}
	} else if (MapChipType::kFallBlock == type) {
		Vector3 position = object3D->GetTranslate();
		IndexSet index = map->GetMapChipIndexSetByPosition(position);
		uint32_t belowIndex = index.yIndex + 1;
		//下にブロックがあるか
		if (belowIndex < map->GetNumBlockVirtical() &&
			map->GetMapChipTypeByIndex(index.xIndex, belowIndex) == MapChipType::kBlank) {
			if (!isFalling) {
				map->SetMapData(index.xIndex, index.yIndex, MapChipType::kBlank);
			}
			isFalling = true;
		}
		//落下
		if (isFalling) {
			velocity += gravity;
			position.y -= velocity;

			IndexSet newIndex = map->GetMapChipIndexSetByPosition(position);

			//下にブロックがあるか
			if (belowIndex < map->GetNumBlockVirtical() &&
				map->GetMapChipTypeByIndex(index.xIndex, belowIndex) != MapChipType::kBlank) {
				isFalling = false;
				velocity = 0.0f;
				position.y = map->GetMapChipPostionByIndex(index.xIndex, belowIndex - 1).y;
				map->RemoveObjectAt(index.xIndex, index.yIndex);
				map->GenerateObjectAt(newIndex.xIndex, newIndex.yIndex, MapChipType::kFallBlock);
			}

			object3D->SetTranslate(position);
		}
		object3D->Update();
	}
	else if (MapChipType::kjumpBlock == type) {
		object3D->Update();
	}

	else if (cameraMode_ && MapChipType::kBlank == type) {
		object3D->Update();
	}

#ifdef _DEBUG

	/*if (ImGui::CollapsingHeader("Block", ImGuiTreeNodeFlags_DefaultOpen))
	{

		DirectionalLight directionalLight = object3D->GetDirectionalLight();
		if (ImGui::DragFloat3("Block Directional Light Direction", &directionalLight.direction.x, 0.01f)) {
			object3D->SetDirectionalLightDirection(directionalLight.direction);
		}

	}*/
#endif // DEBUG_

};


void Block::Draw() {
	// Drawはelse ifを追加してDrawかくだけ
	if (MapChipType::kCopyBlock == type) {
		object3D->Draw();
	} else if (MapChipType::kGoalUp == type) {
		object3D->Draw();
	} else if (MapChipType::kGoalDown == type) {
		object3D->Draw();
	} else if (MapChipType::kNCopyBlock == type) {
		object3D->Draw();
	} else if (MapChipType::kFixedTimeBlock == type) {
		object3D->Draw();
	} else if (MapChipType::kPutFixedTimeBlock == type) {
		object3D->Draw();
	} else if (MapChipType::kFallBlock == type) {
		object3D->Draw();
	} else if (MapChipType::kjumpBlock == type) {
		object3D->Draw();
	}
	else if (cameraMode_ && MapChipType::kBlank == type) {
		object3D->Draw();
	}
}

void Block::Finalize() {
	// 別のクラスを作った場合はそのクラスの終了処理を呼ぶ
	// モデルの解放はこの変数しか使用しないようになっているので追加でかく必要は無し
	if (object3D) {
		delete object3D;
		object3D = nullptr;
	}
}

Block* Block::CreateBlock(MapChipType type, const Vector3& position)
{
	return nullptr;
}


void Block::SetFalling(bool falling) {
	isFalling = falling;
}

Block* Block::CreateBlock(MapChipType type, const Vector3& position, Map* map) {
	Block* block = new Block();
	block->Initialize(type, position, map);
	return block;
}

void Block::SetObject3DPosiition(const Vector3& position)
{
	object3D->SetTranslate(position);
}

void Block::FixedTimeBlock()
{


}

void Block::PutFixedTimeBlock()
{
	if (!isFixedTimeBlockPut) {
		isFixedTimeBlockPut = true;
		fixedTimeCounter = 0;
	}

	// カメラモードがOFF（プレイヤーが操作可能な時間）だけカウントを進める
	if (!cameraMode_) {
		fixedTimeCounter++;
	}
	// 点滅処理：4秒経過〜5秒までの間
	const int blinkStart = 60 * 4;       // 4秒後
	const int blinkEnd = kFixedTime;     // 5秒後（たとえば300など）

	if (fixedTimeCounter >= blinkStart && fixedTimeCounter <= blinkEnd) {
		int blinkFrame = fixedTimeCounter - blinkStart;

		// 点滅速度を上げる：初期は20フレームごと、最終的に4フレームごとに変化するように
		// 速くするため、フレーム数を直線的に減らす
		int blinkInterval = (std::max)(4, 20 - (blinkFrame / 3)); // 0〜60で 20→4 に変化

		bool isVisible = (blinkFrame / blinkInterval) % 2 == 0;

		if (isVisible) {
			// 明るい赤（表示）
			object3D->SetColor({ 1.0f, 0.3f, 0.3f, 1.0f });
		} else {
			// 半透明（非表示気味）
			object3D->SetColor({ 1.0f, 0.0f, 0.0f, 0.1f });
		}
	}

	if (fixedTimeCounter > kFixedTime) {
		IndexSet index = map->GetMapChipIndexSetByPosition(object3D->GetTranslate());
		map->RemoveObjectAt(index.xIndex, index.yIndex);
		isAlive = false;
	}
}