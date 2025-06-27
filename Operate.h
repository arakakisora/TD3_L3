#pragma once
#include "Object3D.h"
#include "Object3DCommon.h"
#include "PhotoCamera.h"
#include "Sprite.h"
#include <array>
#include <memory>
#include <string>

//スプライトデータ
struct UiSpriteData {
	std::string texturePath;
	Vector2 position;
	Vector2 size;
};

class Operate
{
public:
	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw();
public:
	void SetPhotoCamera(PhotoCamera* camera);
private:
	//操作説明
	std::array<std::unique_ptr<Sprite>, 14> operationSprites;
	//フォトカメラ
	PhotoCamera* photoCamera = nullptr;
};

