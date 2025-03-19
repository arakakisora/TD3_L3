#include "Block.h"
#include "Object3D.h"
#include "Object3DCommon.h"
#include "ModelManager.h"


Block::Block()
{
}

Block::~Block()
{
	Finalize();
}

void Block::Initialize(MapChipType type, const Vector3& position) {
	this->type = type;
	object3D = new Object3D();
	object3D->Initialize(Object3DCommon::GetInstance());
	object3D->SetTranslate(position);

	switch ((type))
	{
	case MapChipType::kNCopyBlock:		// コピー不可能 No.2
		// モデル指定
		object3D->SetModel("axis.obj");
		
		break;

	case MapChipType::kCopyBlock:		// コピー可能 No.3
		// モデル指定
		object3D->SetModel("cube.obj");

		break;

	default:
		break;
	}
}

void Block::Update() {
	if (MapChipType::kCopyBlock == type) {
		object3D->Update();
	} else if (MapChipType::kNCopyBlock == type) {
		object3D->Update();
	}
}


void Block::Draw() {
	if (MapChipType::kCopyBlock == type) {
		object3D->Draw();
	} else if (MapChipType::kNCopyBlock == type) {
		object3D->Draw();
	}
}

void Block::Finalize() {
	if (object3D) {
		delete object3D;
		object3D = nullptr;
	}
}

Block* Block::CreateBlock(MapChipType type, const Vector3& position) {
	Block* block = new Block();
	block->Initialize(type, position);
	return block;
}