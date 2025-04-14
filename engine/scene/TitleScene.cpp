#include "TitleScene.h"
#include "Object3DCommon.h"
#include "SpriteCommon.h"
#include "Input.h"
#include "SceneManager.h"
#include "ImGuiManager.h"
#include <ModelManager.h>
#include <CameraManager.h>
#include<TextureManager.h>
#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

void TitleScene::Initialize()
{
	CameraManager::GetInstans()->Initialize();


	ModelManager::GetInstans()->LoadModel("Text_Title.obj");
	ModelManager::GetInstans()->LoadModel("UI_Title_Stsrt.obj");
	ModelManager::GetInstans()->LoadModel("UI_Title_A.obj");

	// タイトル生成
	titileobject_ = std::make_unique<Object3D>();
	titileobject_->Initialize(Object3DCommon::GetInstance());
	titileobject_->SetTranslate(Vector3(0.0f, 0.5f, 0.0f));
	titileobject_->SetModel("Text_Title.obj");
	titileobject_->SetLighting(false);

	// startの生成
	uIbject_start_ = std::make_unique<Object3D>();
	uIbject_start_->Initialize(Object3DCommon::GetInstance());
	uIbject_start_->SetTranslate(Vector3(-0.1f, -0.5f, 0.0f));
	uIbject_start_->SetScale(Vector3(0.3f, 0.3f, 0.3f));
	uIbject_start_->SetModel("UI_Title_Stsrt.obj");
	uIbject_start_->SetLighting(false);

	// Aの生成
	uIbject_A_ = std::make_unique<Object3D>();
	uIbject_A_->Initialize(Object3DCommon::GetInstance());
	uIbject_A_->SetTranslate(Vector3(-0.1f, -0.5f, 0.0f));
	uIbject_A_->SetScale(Vector3(0.3f, 0.3f, 0.3f));
	uIbject_A_->SetModel("UI_Title_A.obj");
	uIbject_A_->SetLighting(false);
}

void TitleScene::Finalize()
{

}

void TitleScene::Update()
{
	CameraManager::GetInstans()->GetActiveCamera()->Update();

	// Aボタンが押されたときに開始
	if (Input::GetInstance()->TriggerGamePadButton(XINPUT_GAMEPAD_A) || Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		// シーン切り替え
		SceneManager::GetInstance()->ChangeScene("STAGESELECTSCENE");
	}


#ifdef _DEBUG
	if (ImGui::CollapsingHeader("Model", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("titleScene %d");
		if (ImGui::Button("gamePlayScene"))
		{
			SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
		}



	}
#endif // _DEBUG

	// タイトル更新処理
	titileobject_->Update();

	// UI更新処理
	uIbject_start_->Update();
	uIbject_A_->Update();
}

void TitleScene::Draw()
{
	//3dオブジェクトの描画準備。3Dオブジェクトの描画に共通のグラフィックスコマンドを積む
	Object3DCommon::GetInstance()->CommonDraw();

	// タイトル描画処理
	titileobject_->Draw();

	// UI描画処理
	uIbject_start_->Draw();
	uIbject_A_->Draw();

	//Spriteの描画準備。spriteの描画に共通のグラフィックスコマンドを積む
	SpriteCommon::GetInstance()->CommonDraw();
}
