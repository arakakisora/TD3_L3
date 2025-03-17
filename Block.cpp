#include "Block.h"
#include "Object3D.h"
#include "Object3DCommon.h"
#include "ModelManager.h"
Block::Block()
{
}

Block::~Block()
{
}

void Block::Initialize(MapChipType type, const Vector3& position) {
    this->type = type;
    this->position = position;
    if (type == MapChipType::kBlock) {
        object3D = new Object3D();
        object3D->Initialize(Object3DCommon::GetInstance());
        object3D->SetModel("cube.obj");
        object3D->SetTranslate(position);
    }
}

void Block::Update() {
    if (object3D) {
        object3D->Update();
    }
}

void Block::Draw() {
    if (object3D) {
        object3D->Draw();
    }
}

void Block::Finalize() {
    if (object3D) {
        delete object3D;
        object3D = nullptr;
    }
}