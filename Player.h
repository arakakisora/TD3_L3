#pragma once
#include "Object3D.h"
#include "Object3DCommon.h"
#include "Input.h"
#include "Camera.h"

class Player
{
public:
	//初期化
	void Initialize(Object3DCommon*object3dcommon);
	//更新
	void Update();
	//描画
	void Draw();
private:
	Object3DCommon* object3dcommon;
	Camera* camera;
	Transform transform;
	Input* input;
	std::unique_ptr<Object3D>object;
	
	float speed = 0.1f;

};

