#pragma once
#include "Object3D.h"
#include "Object3DCommon.h"
#include "Input.h"
#include "Camera.h"

struct MenuItem {
	Vector2 position;//左上の座標
	Vector2 size;//幅と高さ
};

class PauseMenu
{
public:
	//初期化
	void Initialize(Object3DCommon* object3dcommon);
	//更新
	void Update();
	//描画
	void Draw();

private:
	Object3DCommon* object3dcommon_;
	Camera* camera;
	Transform transform;
	Input* input;
	std::unique_ptr<Object3D>object;
	std::array<MenuItem, 4>menuItems;
};

