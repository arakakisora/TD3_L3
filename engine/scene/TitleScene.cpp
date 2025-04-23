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

	// 背景
	ModelManager::GetInstans()->LoadModel("backPlane.obj");
	skydome_ = std::make_unique<Object3D>();
	skydome_->Initialize(Object3DCommon::GetInstance());
	skydome_->SetTranslate(Vector3{ 15.0f, 5.0f, 100.0f });
	skydome_->SetScale(Vector3{ 1.0f,1.0f,1.0f });
	skydome_->SetModel("backPlane.obj");

	// タイトル生成
	titileobject_ = std::make_unique<Object3D>();
	titileobject_->Initialize(Object3DCommon::GetInstance());
	titileobject_->SetTranslate(Vector3(0.0f, 0.5f, 0.0f));
	titileobject_->SetRotate(Vector3(0.0f, 3.3f, 0.0f));
	titileobject_->SetModel("Text_Title.obj");
	titileobject_->SetLighting(false);

	// startの生成
	uIbject_start_ = std::make_unique<Object3D>();
	uIbject_start_->Initialize(Object3DCommon::GetInstance());
	uIbject_start_->SetTranslate(Vector3(-0.53f, -0.5f, 0.0f));
	uIbject_start_->SetScale(Vector3(0.3f, 0.3f, 0.3f));
	uIbject_start_->SetModel("UI_Title_Stsrt.obj");
	uIbject_start_->SetLighting(false);

	// Aの生成
	uIbject_A_ = std::make_unique<Object3D>();
	uIbject_A_->Initialize(Object3DCommon::GetInstance());
	uIbject_A_->SetTranslate(Vector3(-0.43f, -0.5f, 0.0f));
	uIbject_A_->SetScale(Vector3(0.3f, 0.3f, 0.3f));
	uIbject_A_->SetModel("UI_Title_A.obj");
	uIbject_A_->SetLighting(false);

	fadeManager_.Initialize("Resources/white.png");
	fadeManager_.StartFadeIn();

}

void TitleScene::Finalize()
{

}

void TitleScene::Update()
{
	// フェード更新
	fadeManager_.Update();
	CameraManager::GetInstans()->GetActiveCamera()->Update();
	skydome_->Update();
	if (time <= 20) {
		time++;
	} else {
		timehige = true;
	}
	if (timehige) {


		// Aボタンが押されたときに開始
		if (!fadeManager_.IsFading()) {
			if (
#ifdef _DEBUG
				Input::GetInstance()->TriggerKey(DIK_SPACE) ||
#endif // _DEBUG
				Input::GetInstance()->TriggerGamePadButton(XINPUT_GAMEPAD_A))
			{
				fadeManager_.StartFadeOut();

			}
		}

		if (fadeManager_.IsFadeOutFinished()) {
			// シーン切り替え
			SceneManager::GetInstance()->ChangeScene("STAGESELECTSCENE");
		}

	}








#ifdef _DEBUG
	if (ImGui::CollapsingHeader("Model", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("titleScene %d");
		if (ImGui::Button("gamePlayScene"))
		{
			SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
		}

		Transform S = uIbject_start_->GetTransform();
		if (ImGui::DragFloat3("STransform", &S.translate.x, 0.01f)) {
			uIbject_start_->SetTransform(S);
		}

		Transform A = uIbject_A_->GetTransform();
		if (ImGui::DragFloat3("ATransform", &A.translate.x, 0.01f)) {
			uIbject_A_->SetTransform(A);
		}

	}
#endif // _DEBUG

	//タイトルの動き
	float yoffset = std::sinf(timer * 0.05f) * 0.1f;
	Transform trans = titileobject_->GetTransform();
	trans.translate = Vector3(0.0f, 0.5f + yoffset, 0.0f);
	trans.rotate.y += 0.01f;
	titileobject_->SetTransform(trans);

	float scale = 0.3f + std::sinf(timer * 0.07f) * 0.03f; 

	//スタートの動き
	Transform startTrans = uIbject_start_->GetTransform();
	startTrans.scale = Vector3(scale, scale, scale); 
	uIbject_start_->SetTransform(startTrans);

	//Aの動き
	Transform ATrans = uIbject_A_->GetTransform();
	ATrans.scale = Vector3(scale, scale, scale);
	uIbject_A_->SetTransform(ATrans);

	timer++;

	// タイトル更新処理
	titileobject_->Update();

	// UI更新処理
	uIbject_start_->Update();
	
}

void TitleScene::Draw()
{

	//3dオブジェクトの描画準備。3Dオブジェクトの描画に共通のグラフィックスコマンドを積む
	Object3DCommon::GetInstance()->CommonDraw();

	skydome_->Draw();

	// タイトル描画処理
	titileobject_->Draw();

	// UI描画処理
	uIbject_start_->Draw();
	

	//Spriteの描画準備。spriteの描画に共通のグラフィックスコマンドを積む
	SpriteCommon::GetInstance()->CommonDraw();

	// フェード描画
	fadeManager_.Draw();

}
