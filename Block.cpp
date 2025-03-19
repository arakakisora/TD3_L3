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
		object3D->SetModel("cube.obj");

		break;

	default:
		break;
	}
}

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
	}
}


void Block::Draw() {
	// Drawはelse ifを追加してDrawかくだけ
	if (MapChipType::kCopyBlock == type) {
		object3D->Draw();
	} else if (MapChipType::kNCopyBlock == type) {
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

Block* Block::CreateBlock(MapChipType type, const Vector3& position) {
	Block* block = new Block();
	block->Initialize(type, position);
	return block;
}