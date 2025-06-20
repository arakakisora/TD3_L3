#pragma once
#include "Object3D.h"
#include "Object3DCommon.h"
#include "Sprite.h"
#include <array>
#include <memory>
#include <string>

//チュートリアルテキストデータ
struct TutorialTextData {
	std::string modelPath;
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

//ブロックスプライトデータ
struct SpriteData {
	std::string texturePath;
	Vector2 position;
	Vector2 size;
};

class Tutorial
{
public:
	//初期化
	void Initialize();
    //更新
	void Update();
	//テキスト描画
	void TextDraw();
	//スプライト描画
	void SpriteDraw();
private:
	//チュートリアルテキスト
	std::array<std::unique_ptr<Object3D>, 13>tutorialTexts;
	//ブロックスプライト
	std::array<std::unique_ptr<Sprite>, 3> blockSprites;
	//描画フラグ
	bool tutorial1_2 = false;
	bool tutorial3_4 = false;
	bool tutorial5 = false;
	bool tutorial6_7 = false;
	bool tutorial8 = false;
	bool tutorial9 = false;
	bool tutorial10 = false;
	bool tutorial11 = false;
	bool tutorial12 = false;
	bool tutorial13 = false;
	
	//経過時間
	float elapsedTime = 0.0f;
	//30秒後に表示
	const float afterseconds = 30.0f;
	//30秒経過フラグ
	bool secondspassed = false;

	const float deltaTime = 1.0f / 60.0f;
};

