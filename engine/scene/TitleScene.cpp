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
#include <Easing.h>

void TitleScene::Initialize()
{
	CameraManager::GetInstans()->Initialize();


	ModelManager::GetInstans()->LoadModel("Text_Title.obj");
	ModelManager::GetInstans()->LoadModel("UI_Title_Stsrt.obj");
	ModelManager::GetInstans()->LoadModel("UI_Title_A.obj");
	ModelManager::GetInstans()->LoadModel("plane.obj");
	ModelManager::GetInstans()->LoadModel("playercharacter.obj");

	// 背景
	ModelManager::GetInstans()->LoadModel("backPlane.obj");
	skydome_ = std::make_unique<Object3D>();
	skydome_->Initialize(Object3DCommon::GetInstance());
	skydome_->SetTranslate(Vector3{ 0.0f,0.0f,62.72f });
	skydome_->SetRotate(Vector3{ 0.0f,0.0f,-1.57f });
	skydome_->SetScale(Vector3{ 0.2f, 0.4f, 2.23f });
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

	//sahtter演出用のオブジェクト
	shuttertopObject = std::make_unique<Object3D>();
	shuttertopObject->Initialize(Object3DCommon::GetInstance());
	shuttertopObject->SetModel("plane.obj");
	//サイズは画面いっぱいにする
	shuttertopObject->SetScale(Vector3{ 2.0f,2.0f,1.0f });
	shuttertopObject->SetTranslate(Vector3(0.0f, 13.0f, -1.0f));
	shuttertopObject->SetRotate(Vector3{ 0,0,0 });
	//bottm
	shutterbottomObject = std::make_unique<Object3D>();
	shutterbottomObject->Initialize(Object3DCommon::GetInstance());
	shutterbottomObject->SetModel("plane.obj");
	//サイズは画面いっぱいにする
	shutterbottomObject->SetScale(Vector3{ 2.0f,2.0f,1.0f });
	shutterbottomObject->SetTranslate(Vector3(0.0f, -10.0f, -1.0f));
	shutterbottomObject->SetRotate(Vector3{ 0,0,0 });

	// プレイヤーの生成
	player_ = std::make_unique<Object3D>();
	player_->Initialize(Object3DCommon::GetInstance());
	player_->SetModel("playercharacter.obj");
	player_->SetLighting(true);
	player_->SetDirectionalLightEnable(true);
	player_->SetRotate(Vector3{ 0.0f,180.0f * (DirectX::XM_PI / 180.0f),0.0f });
	player_->SetScale(Vector3{ 1.0f, 1.0f, 1.0f });
	player_->SetTranslate(Vector3{ 0.0f,0.0f,7.0f });

	// ステージを0からに初期化
	SceneManager::GetInstance()->SetStageIndex(0);
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

		if (currentStep >= 3) {
			// Aボタンが押されたときに開始
			if (!fadeManager_.IsFading()) {
				if (
#ifdef _DEBUG
					Input::GetInstance()->TriggerKey(DIK_SPACE) ||
#endif // _DEBUG
					Input::GetInstance()->TriggerGamePadButton(XINPUT_GAMEPAD_A))
				{
					if (currentStep >= 3) {
						isnextStep = true;
						fadeManager_.StartFadeOut();
					}
				}
			}

			if (fadeManager_.IsFadeOutFinished()) {
				// シーン切り替え	
				SceneManager::GetInstance()->ChangeScene("STAGESELECTSCENE");

			}

		}
	}
#ifdef _DEBUG


	if (ImGui::CollapsingHeader("Camera Control", ImGuiTreeNodeFlags_DefaultOpen)) {
		if (ImGui::Button("Switch to Main Camera")) {
			CameraManager::GetInstans()->SetActiveCamera("maincam");
		}
		if (ImGui::Button("Switch to Sub Camera")) {
			CameraManager::GetInstans()->SetActiveCamera("subcam");
		}

		//カメラの位置
		Transform cameraTransform = CameraManager::GetInstans()->GetActiveCamera()->GetTransform();
		if (ImGui::DragFloat3("Camera Position", &cameraTransform.translate.x, 0.01f)) {
			CameraManager::GetInstans()->GetActiveCamera()->SetTranslate(cameraTransform.translate);
		}
		//カメラの向き
		if (ImGui::DragFloat3("Camera Rotation", &cameraTransform.rotate.x, 0.01f)) {
			CameraManager::GetInstans()->GetActiveCamera()->SetRotate(cameraTransform.rotate);
		}


	}

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

	if (currentStep == -1) {
		fadeManager_.StartFadeIn();
		currentStep = 0;
	}

	// ステップ1
	if (currentStep == 0) {
		player_->SetRotate(Vector3{ 0.0f,180.0f * (DirectX::XM_PI / 180.0f),0.0f });
		player_->SetTranslate(Vector3{ 0.0f,-10.0f,7.0f });
		currentStep = 1;
	}

	// ステップ2
	if (currentStep == 1) {
		UpdatePlayerPositionByStep(0.01f);
	}

	// ステップ3
	if (currentStep == 2 && isShutterEffectPlaying == false) {
		shatterEffect();
	}


	// ステップ4以降のオブジェクトの位置
	if (currentStep >= 3) {
		// プレイヤーの位置を変更
		frameCount_++;
		// 浮遊の動きを作る
		float time = static_cast<float>(frameCount_) * 0.05f; // frameCount_ は毎フレーム +1 されると仮定
		float amplitude = 0.2f;  // 浮遊の高さ（-0.7 ～ +0.7）
		float frequency = 0.5f;  // 動く速さ（大きいほど速くなる）
		float cycle = 2.0f * 3.14159265f / frequency;
		if (time >= cycle) {
			frameCount_ = 0;
			time = 0.0f;
		}
		float floatY = std::sin(time * frequency) * amplitude;
		offset = { 0.0f,-0.7f,0.0f };
		offset.y += floatY;
		Vector3 newpos;
		newpos = { basePosition_.x + offset.x, basePosition_.y + offset.y, basePosition_.z + offset.z };
		player_->SetTranslate(Vector3(-2.5f, newpos.y, 3.596f));
		player_->SetRotate(Vector3(0.0f, DirectX::XMConvertToRadians(134.0f), DirectX::XMConvertToRadians(18.75f)));


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



		if (!isnextStep) {
			nextcurrentSteptime++;
			if (nextcurrentSteptime >= 1000) {
				currentStep = -1;
				nextcurrentSteptime = 0;
			}
		}
	}


	// タイトル更新処理
	titileobject_->Update();
	// UI更新処理
	uIbject_start_->Update();

	// シャッター演出の更新
	shutterEffectUpdate();

	//sahtter演出用のオブジェクト
	shuttertopObject->Update();
	shutterbottomObject->Update();

	// プレイヤーの更新処理
	player_->Update();
}

void TitleScene::Draw()
{

	//3dオブジェクトの描画準備。3Dオブジェクトの描画に共通のグラフィックスコマンドを積む
	Object3DCommon::GetInstance()->CommonDraw();

	skydome_->Draw();

	// 演出後に描画
	if (currentStep >= 1) {

		// プレイヤー
		player_->Draw();

		if (currentStep >= 3) {
			// タイトル描画処理
			titileobject_->Draw();

			// UI描画処理
			uIbject_start_->Draw();
		}
	}
	// シャッター演出用
	shuttertopObject->Draw();
	shutterbottomObject->Draw();

	//Spriteの描画準備。spriteの描画に共通のグラフィックスコマンドを積む
	SpriteCommon::GetInstance()->CommonDraw();

	// フェード描画
	fadeManager_.Draw();

}

void TitleScene::shatterEffect()
{
	if (isShutterEffectPlaying) return;

	isShutterEffectPlaying = true;
	shutterAnimTime = 0.0f;

	// 初期位置にリセット
	shuttertopObject->SetTranslate(Vector3(0.0f, 13.0f, -1.0f));
	shutterbottomObject->SetTranslate(Vector3(0.0f, -10.0f, -1.0f));
}

void TitleScene::shutterEffectUpdate()
{
	if (!isShutterEffectPlaying) return;

	shutterAnimTime += 0.005f; // ← deltaTime に置き換えOK
	float t = shutterAnimTime / shutterAnimDuration;
	t = std::min(t, 1.0f);

	// 閉じる：Top 7 → 3, Bottom 5 → -17
	// 開く  ：Top 3 → 7, Bottom -17 → 5
	float topY, bottomY;

	if (t < 0.5f) {
		// 閉じるフェーズ（0.0〜0.5）
		float p = t / 0.5f;
		topY = Easing::EaseLerp(13.0f, 1.9f, p, Easing::EaseOutQuad);
		bottomY = Easing::EaseLerp(-10.0f, -1.9f, p, Easing::EaseOutQuad);
	} else {
		// 開くフェーズ（0.5〜1.0）
		float p = (t - 0.5f) / 0.5f;
		topY = Easing::EaseLerp(1.9f, 13.0f, p, Easing::EaseInQuad);
		bottomY = Easing::EaseLerp(-1.9f, -10.0f, p, Easing::EaseInQuad);
		if (currentStep == 2) {
			currentStep = 3;
		}
	}

	shuttertopObject->SetTranslate(Vector3(0.0f, topY, -1.0f));
	shutterbottomObject->SetTranslate(Vector3(0.0f, bottomY, -1.0f));

	if (t >= 1.0f) {
		isShutterEffectPlaying = false;
	}
}

void TitleScene::UpdatePlayerPositionByStep(float deltaTime) {
	if (currentStep == 1 && !isEasing) {
		isEasing = true;
		easingTimer = 0.0f;
	}

	if (isEasing) {
		easingTimer += deltaTime;
		float t = std::clamp(easingTimer / easingDuration, 0.0f, 1.0f);

		// EaseOutQuad（または他の好みのEasingに変更可）
		float easedY = Easing::EaseLerp(-10.0f, 0.0f, t, Easing::EaseOutQuad);

		Vector3 currentPos = player_->GetTranslate();
		currentPos.y = easedY;
		player_->SetTranslate(currentPos);

		if (t >= 1.0f) {
			isEasing = false; // 完了したら止める

			// ステップ2へ進める
			currentStep = 2;
		}
	}
}
