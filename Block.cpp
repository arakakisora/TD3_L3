#include "Block.h"
#include "Object3D.h"
#include "Object3DCommon.h"
#include "ModelManager.h"
#include "Map.h"


Block::Block()
{
}

Block::~Block()
{
	Finalize();
}

void Block::Initialize(MapChipType type, const Vector3& position,Map*map) {
	this->type = type;
	this->map = map;
	object3D = new Object3D();
	object3D->Initialize(Object3DCommon::GetInstance());
	object3D->SetTranslate(position);
	velocity = 0.0f;
	//isFalling = false;

	switch ((type))
	{
		//case MapChipType::ここにマップチップタイプ:		// コピー不可能 No.2
		//	// モデル指定
		//	object3D->SetModel("使用したいモデル.obj");
			//　もし、ブロックようにクラスを作ったのであればここに初期化
			// 
		//	break;
	case MapChipType::kNCopyBlock:		// コピー不可能 No.2
		// モデル指定
		object3D->SetModel("axis.obj");

		break;

	case MapChipType::kCopyBlock:		// コピー可能 No.3
		// モデル指定
		object3D->SetModel("block.obj");
		break;
	case MapChipType::kGoalUp:		// ゴール上 No.4
		// モデル指定
		//object3D->SetModel("Goal.obj");
		break;
	case MapChipType::kGoalDown:		// ゴール下 No.5
		// モデル指定
		//object3D->SetModel("Goal.obj");
		break;

	case MapChipType::kFixedTimeBlock:		// 一定時間経過したら消えるブロック No.7
		// モデル指定
		object3D->SetModel("Timer.obj");
		break;
	case MapChipType::kPutFixedTimeBlock:		// 貼り付け後一定時間 No.8
		// モデル指定
		//object3D->SetModel("Timer.obj");
		break;
	case MapChipType::kFallBlock:       // 落下ブロック
		//モデル指定
		object3D->SetModel("fallblock.obj");

		break;

	
	}
}
//int cos sin 
void Block::Update() {
	/*else if (MapChipType::マップチップタイプ == type) {
	* モデルの更新なのでこれは絶対に必要
		object3D->Update();
		作ったブロックの更新処理
		ブロッククラス内で作ればそれを呼ぶだけでいい
		追加する場合はkNCopyBlockの部分を参考に
	}*/
	if (MapChipType::kCopyBlock == type) {
		object3D->Update();

	} else if (MapChipType::kNCopyBlock == type) {
		object3D->Update();
	} else if (MapChipType::kFixedTimeBlock == type) {
		object3D->Update();
		FixedTimeBlock();
	} else if (MapChipType::kPutFixedTimeBlock == type) {
		object3D->Update();
		PutFixedTimeBlock();
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
};


void Block::Draw() {
	// Drawはelse ifを追加してDrawかくだけ
	if (MapChipType::kCopyBlock == type) {
		object3D->Draw();
	} else if (MapChipType::kNCopyBlock == type) {
		object3D->Draw();
	} else if (MapChipType::kFixedTimeBlock == type) {
		object3D->Draw();
	} else if (MapChipType::kPutFixedTimeBlock == type) {
		object3D->Draw();
	} else if (MapChipType::kFallBlock == type) {
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

void Block::SetFalling(bool falling) {
	isFalling = falling;
}

Block* Block::CreateBlock(MapChipType type, const Vector3& position,Map*map) {
	Block* block = new Block();
	block->Initialize(type, position,map);
	return block;
}

void Block::FixedTimeBlock()
{


}

void Block::PutFixedTimeBlock()
{
	if (isFixedTimeBlockPut) {

	}
}
