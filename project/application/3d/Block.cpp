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
	object3D = std::make_unique<Object3D>();
	object3D->Initialize(Object3DCommon::GetInstance());
	object3D->SetTranslate(position);
	velocity = 0.0f;

	// モデルの設定（共通処理）
	auto it = modelTable.find(type);
	if (it != modelTable.end()) {
		object3D->SetModel(it->second);
	}

	// 特別な処理が必要なタイプだけ個別に記述
	switch (type) {
	case MapChipType::kGoalUp: {
		object3D->SetScale(Vector3{ 0.6f, 0.6f, 0.6f });
		Vector3 pos = object3D->GetTranslate();
		pos.y -= 1.0f;
		object3D->SetTranslate(pos);
		break;
	}
	case MapChipType::kGoalDown: {
		basePosition_ = position;
		object3D->SetScale(Vector3{ 0.6f, 0.6f, 0.6f });
		break;
	}
							   
	default:
		break;
	}

	object3D->SetLighting(true);
	object3D->SetDirectionalLightEnable(true);
	object3D->SetDirectionalLightDirection({ 0.88f, -1.90f, 4.0f });
}


void Block::Update(const bool cameraMode) {

	cameraMode_ = cameraMode;
	if (simpleUpdateTypes.count(type)) {
		// 追加処理がないタイプ
		object3D->Update();
	}

	// 特別な処理だけ個別に
	switch (type) {
	case MapChipType::kGoalDown:
		frameCount_ -= 0.05f;
		object3D->SetRotate(Vector3{ 0.0f, 0.0f, (float)frameCount_ });
		object3D->Update();
		break;

	case MapChipType::kFixedTimeBlock:
		object3D->Update();
		FixedTimeBlock();
		break;

	case MapChipType::kPutFixedTimeBlock:
		object3D->Update();
		if (!cameraMode_) {
			PutFixedTimeBlock();
		}
		break;

	case MapChipType::kFallBlock: {
		Vector3 position = object3D->GetTranslate();
		IndexSet index = map->GetMapChipIndexSetByPosition(position);
		uint32_t belowIndex = index.yIndex + 1;

		if (belowIndex < map->GetNumBlockVirtical() &&
			map->GetMapChipTypeByIndex(index.xIndex, belowIndex) == MapChipType::kBlank) {
			if (!isFalling) {
				map->SetMapData(index.xIndex, index.yIndex, MapChipType::kBlank);
			}
			isFalling = true;
		}

		if (isFalling) {
			velocity += gravity;
			position.y -= velocity;

			IndexSet newIndex = map->GetMapChipIndexSetByPosition(position);

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
		break;
	}

	default:
		break;
	}

	// カメラモード中にBlankでもUpdateする処理
	if (cameraMode_ && type == MapChipType::kBlank) {
		object3D->Update();
	}

#ifdef _DEBUG
	if (ImGui::CollapsingHeader("Block", ImGuiTreeNodeFlags_DefaultOpen)) {
		DirectionalLight directionalLight = object3D->GetDirectionalLight();
		if (ImGui::DragFloat3("Block Directional Light Direction", &directionalLight.direction.x, 0.01f)) {
			object3D->SetDirectionalLightDirection(directionalLight.direction);
		}
	}
#endif

};


void Block::Draw() {
	if (drawTypes.count(type) || (cameraMode_ && type == MapChipType::kBlank)) {
		object3D->Draw();
	}
}

void Block::Finalize() {
	// 別のクラスを作った場合はそのクラスの終了処理を呼ぶ
	// モデルの解放はこの変数しか使用しないようになっているので追加でかく必要は無し
	if (object3D) {
		object3D.reset();
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