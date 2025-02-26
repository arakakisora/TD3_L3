#include "Player.h"
#include "imgui.h"

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
void Player::Update() {
	//移動(マップチップ対応前)
	if (Input::GetInstans()->PushKey(DIK_W))
	{
		transform.translate.y += speed;
	}
	else if (Input::GetInstans()->PushKey(DIK_S))
	{
		transform.translate.y -= speed;
	}
	else if (Input::GetInstans()->PushKey(DIK_D))
	{
		transform.translate.x += speed;
	}
	else if (Input::GetInstans()->PushKey(DIK_A))
	{
		transform.translate.x -= speed;
	}

	object->SetTranslate(transform.translate);
	
	object->Update();

#ifdef _DEBUG

#endif
}

//描画
void Player::Draw() {
	
	object->Draw();
}