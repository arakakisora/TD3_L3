#include "BitmapFont.h"
#include "TextureManager.h"
void BitmapFont::Initialize(uint32_t drawNum)
{
	drawNumber = drawNum;
	// 数字の画像データを全て読み込む


	sprite = std::make_unique<Sprite>();
	


	for (int i = 0; i <= 9; i++) {
		if (drawNumber == 0) {
			TextureManager::GetInstance()->LoadTexture("Resources/BitmapFont/0.png");
			sprite->Initialize(SpriteCommon::GetInstance(), "Resources/BitmapFont/0.png");
			break;
		} else if (drawNumber == 1) {
			TextureManager::GetInstance()->LoadTexture("Resources/BitmapFont/1.png");
			sprite->Initialize(SpriteCommon::GetInstance(), "Resources/BitmapFont/1.png");
			break;
		} else if (drawNumber == 2) {
			TextureManager::GetInstance()->LoadTexture("Resources/BitmapFont/2.png");
			sprite->Initialize(SpriteCommon::GetInstance(), "Resources/BitmapFont/2.png");
			break;
		} else if (drawNumber == 3) {
			TextureManager::GetInstance()->LoadTexture("Resources/BitmapFont/3.png");
			sprite->Initialize(SpriteCommon::GetInstance(), "Resources/BitmapFont/3.png");
			break;
		} else if (drawNumber == 4) {
			TextureManager::GetInstance()->LoadTexture("Resources/BitmapFont/4.png");
			sprite->Initialize(SpriteCommon::GetInstance(), "Resources/BitmapFont/4.png");
			break;
		} else if (drawNumber == 5) {
			TextureManager::GetInstance()->LoadTexture("Resources/BitmapFont/5.png");
			sprite->Initialize(SpriteCommon::GetInstance(), "Resources/BitmapFont/5.png");
			break;
		} else if (drawNumber == 6) {
			TextureManager::GetInstance()->LoadTexture("Resources/BitmapFont/6.png");
			sprite->Initialize(SpriteCommon::GetInstance(), "Resources/BitmapFont/6.png");
			break;
		} else if (drawNumber == 7) {
			TextureManager::GetInstance()->LoadTexture("Resources/BitmapFont/7.png");
			sprite->Initialize(SpriteCommon::GetInstance(), "Resources/BitmapFont/7.png");
			break;
		} else if (drawNumber == 8) {
			TextureManager::GetInstance()->LoadTexture("Resources/BitmapFont/8.png");
			sprite->Initialize(SpriteCommon::GetInstance(), "Resources/BitmapFont/8.png");
			break;
		} else if (drawNumber == 9) {
			TextureManager::GetInstance()->LoadTexture("Resources/BitmapFont/9.png");
			sprite->Initialize(SpriteCommon::GetInstance(), "Resources/BitmapFont/9.png");
			break;
		}
		// 下記のコードなら簡略できる
		//sprite->Initialize(SpriteCommon::GetInstance(), 
		//"Resources/BitmapFont/" + std::to_string(drawNumber) + ".png");

	}

	sprite->SetSize(Vector2(200.0f, 200.0f));
	sprite->SetRotation(0.0f);
	sprite->setColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	sprite->SetPosition(Vector2(10.0f, 10.0f));

}

void BitmapFont::Update()
{
	sprite->Update();
}

void BitmapFont::Draw()
{
	sprite->Draw();
}
