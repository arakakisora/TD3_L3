#include "BitmapFont.h"
#include "TextureManager.h"
void BitmapFont::Initialize()
{
#pragma region 残り枚数画像初期化
	
	// 数字の画像データを全て読み込む
	TextureManager::GetInstance()->LoadTexture("Resources/BitmapFont/0.png");
	TextureManager::GetInstance()->LoadTexture("Resources/BitmapFont/1.png");
	TextureManager::GetInstance()->LoadTexture("Resources/BitmapFont/2.png");
	TextureManager::GetInstance()->LoadTexture("Resources/BitmapFont/3.png");
	TextureManager::GetInstance()->LoadTexture("Resources/BitmapFont/4.png");
	TextureManager::GetInstance()->LoadTexture("Resources/BitmapFont/5.png");
	TextureManager::GetInstance()->LoadTexture("Resources/BitmapFont/6.png");
	TextureManager::GetInstance()->LoadTexture("Resources/BitmapFont/7.png");
	TextureManager::GetInstance()->LoadTexture("Resources/BitmapFont/8.png");
	TextureManager::GetInstance()->LoadTexture("Resources/BitmapFont/9.png");

	for (int i = 0; i <= 9; ++i) {
		auto sprite = make_unique<Sprite>();
		sprite->Initialize(SpriteCommon::GetInstance(),
			"Resources/BitmapFont/" + std::to_string((uint32_t)i) + ".png");
		sprite->SetSize(Vector2(75.0f, 75.0f));
		sprite->SetRotation(0.0f);
		sprite->setColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		sprite->SetPosition(Vector2(130.0f, 10.0f));
		sprites.push_back(move(sprite));
	}
	

	// 下記のコードなら簡略できる
	



	
#pragma endregion 

#pragma region ｘ表示スプライト初期化

	TextureManager::GetInstance()->LoadTexture("Resources/BitmapFont/x.png");

	// 残り枚数表示のｘ
	spriteMulti = std::make_unique<Sprite>();
	spriteMulti->Initialize(SpriteCommon::GetInstance(), "Resources/BitmapFont/x.png");
	spriteMulti->SetRotation(0.0f);
	spriteMulti->setColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	spriteMulti->SetPosition(Vector2(85.0f, 35.0f));
	spriteMulti->SetSize(Vector2(40.0f, 40.0f));

#pragma endregion
}


void BitmapFont::Update(uint32_t drawNum)
{
	drawNumber = drawNum;
	// スプライトの更新処理
	for (auto& sprite : sprites) {
		sprite->Update();
	}
	spriteMulti->Update();
}


void BitmapFont::Draw() {
	// drawNumber に対応する数字スプライトを描画
	if (drawNumber >= 0 && drawNumber <static_cast<int>(sprites.size())) {
		sprites[drawNumber]->Draw();
	}

	// 「x」スプライトは常に描画
	spriteMulti->Draw();
}

