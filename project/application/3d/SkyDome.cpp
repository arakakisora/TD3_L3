#include "SkyDome.h"

SkyDome::SkyDome()
{
}

SkyDome::~SkyDome()
{
	Finalize();
}


void SkyDome::Initialize()
{
	// 3Dオブジェクトの初期化
	skyDomeObject = std::make_unique<Object3D>();
	skyDomeObject->Initialize(Object3DCommon::GetInstance());
	skyDomeObject->SetModel("GamePlay/PlaySceneBackPlane.obj");
	skyDomeObject->SetTranslate(Vector3{ 17.6f, 15.28f, 62.72f });
	skyDomeObject->SetRotate(Vector3{ 0.0f, 0.0f, -1.57f });
	skyDomeObject->SetScale(Vector3{ 0.22f, 0.4f, 2.23f });
}


void SkyDome::Update()
{
	skyDomeObject->Update();
}



void SkyDome::Draw()
{
	// 天球の描画
	skyDomeObject->Draw();
}

void SkyDome::Finalize()
{
	// 3Dオブジェクトの解放
	if (skyDomeObject) {
		skyDomeObject.reset();
	}
}