#include "Operate.h"

//初期化
void Operate::Initialize() {

	const std::array<UiSpriteData, 14>spriteData = { {
		{ "Resources/xbox_stick_l.png",  {43, 655}, {70, 70} },
		{ "Resources/xbox_button_color_b.png",  {416, 655}, {70, 70} },
		{"Resources/xbox_button_color_a.png",  {228, 655}, {70, 70} },
		{"Resources/xbox_button_color_x.png",  {628, 655}, {70, 70} },
		{"Resources/xbox_button_color_y.png",  {792, 655}, {70, 70} },
		{"Resources/xbox_lb.png",  {964, 655}, {70, 60} },
		{"Resources/xbox_rb.png",  {1090, 655}, {70, 60} },
		{ "Resources/idou.png",  {135, 655}, {60, 60} },
		{ "Resources/kirikae.png",  {507, 655}, {100, 60} },
		{ "Resources/toru.png",  {712, 655}, {60, 60} },
		{ "Resources/haiti.png",  {882, 655}, {60, 60} },
		{"Resources/zyanpu.png",  {326, 655}, {60, 60} },
		{ "Resources/reset.png",  {1167, 655}, {70, 60} },
		{ "Resources/plus.png",  {1031, 655}, {60, 60} },
} };

	for (size_t i = 0; i < operationSprites.size(); ++i) {
		operationSprites[i] = std::make_unique<Sprite>();
		operationSprites[i]->Initialize(SpriteCommon::GetInstance(), spriteData[i].texturePath);
		operationSprites[i]->SetPosition(spriteData[i].position);
		operationSprites[i]->SetSize(spriteData[i].size);
	}
}

//更新
void Operate::Update() {

	//カメラモード非起動時
	if (!photoCamera->GetCameraMode()) {
		operationSprites[2]->Update();
		operationSprites[11]->Update();
	}
	//カメラモード起動時
	if (photoCamera->GetCameraMode()) {
		operationSprites[3]->Update();
		operationSprites[4]->Update();
		operationSprites[9]->Update();
		operationSprites[10]->Update();
	}
	//常時

	operationSprites[0]->Update();
	operationSprites[1]->Update();
	operationSprites[5]->Update();
	operationSprites[6]->Update();
	operationSprites[7]->Update();
	operationSprites[8]->Update();
	operationSprites[12]->Update();
	operationSprites[13]->Update();


	for (const auto& sprite : operationSprites) {
		sprite->Update();
	}
}

//描画
void Operate::Draw() {
	//カメラモード非起動時
	if (!photoCamera->GetCameraMode()) {
		operationSprites[2]->Draw();
		operationSprites[11]->Draw();
	}
	//カメラモード起動時
	if (photoCamera->GetCameraMode()) {
		operationSprites[3]->Draw();
		operationSprites[4]->Draw();
		operationSprites[9]->Draw();
		operationSprites[10]->Draw();
	}
	//常時
	operationSprites[0]->Draw();
	operationSprites[1]->Draw();
	operationSprites[5]->Draw();
	operationSprites[6]->Draw();
	operationSprites[7]->Draw();
	operationSprites[8]->Draw();
	operationSprites[12]->Draw();
	operationSprites[13]->Draw();

}

void Operate::SetPhotoCamera(PhotoCamera* camera) {
	photoCamera = camera;
}