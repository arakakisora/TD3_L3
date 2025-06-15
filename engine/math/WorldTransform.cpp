#include "WorldTransform.h"
#include "MyMath.h"

void WorldTransform::Initialize()
{
	worldMatrix_ = MyMath::MakeIdentity4x4();
}

void WorldTransform::UpdateMatrix()
{
	//スケール、回転、平行移動を合成して行列を計算する
	worldMatrix_ = MyMath::MakeAffineMatrix(scale_, rotation_, translation_);

	//親があれば親のワールド行列を掛ける
	if (parent_) {
		worldMatrix_ = MyMath::Multiply(worldMatrix_, parent_->worldMatrix_);
	}
}
