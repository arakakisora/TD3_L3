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

void TitleScene::LoadAllTextures() {
	const std::vector<std::string> textureNames = {
		"WhiteRooms",
	};

	for (const auto& name : textureNames) {			
		TextureManager::GetInstance()->LoadTexture("Resources/" + name + ".png");		// テクスチャ読み込み
	}
}

void TitleScene::LoadAllModels() {
	const std::vector<std::string> modelNames = {
		 "Text_Title",
		 "UI_Title_Stsrt",
		 "UI_Title_A",
		 "plane",
		 "playercharacter",
		 "backPlane",
		 "WhiteRooms",
		 "shutterEffect"
	};

    for (const auto& name : modelNames) {
        ModelManager::GetInstans()->LoadModel(name + ".obj");		// モデル読み込み
    }
}

void TitleScene::Initialize()
{
	// カメラの初期化
	CameraManager::GetInstans()->Initialize();

	// テクスチャの読み込み
	LoadAllTextures();
	// モデルの読み込み
	LoadAllModels();

	// 背景の生成
	skydome_ = std::make_unique<Object3D>();
	skydome_->Initialize(Object3DCommon::GetInstance());
	skydome_->SetTranslate(Vector3{ 0.0f,0.0f,10.0f });
	skydome_->SetRotate(Vector3{ 0.0f,0.0f,0.0f });
	skydome_->SetScale(Vector3{ 1.5f, 1.0f, 1.0f });
	skydome_->SetModel("WhiteRooms.obj");

	// オブジェクトの生成
	for (size_t i = 0; i < titleObjects_.size(); ++i) {
		// 共通の処理
		titleObjects_[i] = std::make_unique<Object3D>();
		titleObjects_[i]->Initialize(Object3DCommon::GetInstance());
		titleObjects_[i]->SetLighting(false);
		if (i == ObjectType::Title){                             	// タイトルの生成
			titleObjects_[i]->SetTranslate(Vector3(0.0f, 0.5f, 0.0f));
			titleObjects_[i]->SetRotate(Vector3(0.0f, 3.3f, 0.0f));
			titleObjects_[i]->SetModel("Text_Title.obj");
		} else if (i == ObjectType::Start) {                        // startの生成
			titleObjects_[i]->SetTranslate(Vector3(-0.53f, -0.5f, 0.0f));
			titleObjects_[i]->SetScale(Vector3(0.3f, 0.3f, 0.3f));	
			titleObjects_[i]->SetModel("UI_Title_Stsrt.obj");
		} else if (i == ObjectType::Player) {                       // プレイヤーの生成
			titleObjects_[i]->SetModel("playercharacter.obj");
			titleObjects_[i]->SetLighting(true);
			titleObjects_[i]->SetDirectionalLightEnable(true);
			titleObjects_[i]->SetDirectionalLightDirection(Vector3{ -1.8f, -2.0f, -2.0f });
			titleObjects_[i]->SetRotate(Vector3{ 0.0f,180.0f * (DirectX::XM_PI / 180.0f),0.0f });
			titleObjects_[i]->SetScale(Vector3{ 1.0f, 1.0f, 1.0f });
			titleObjects_[i]->SetTranslate(Vector3{ 0.0f,0.0f,7.0f });
		}
	}
	
	// フェードインの初期化
	fadeManager_.Initialize("Resources/white.png");
	fadeManager_.StartFadeIn();
	
	// シャッターオブジェクトの生成
	shutterObjects_.resize(2);
	for (int i = 0; i < 2; ++i) {
		shutterObjects_[i] = std::make_unique<Object3D>();
		shutterObjects_[i]->Initialize(Object3DCommon::GetInstance());
		shutterObjects_[i]->SetModel("shutterEffect.obj");
		if (i == 0) {                                                        // [0]=上
			shutterObjects_[i]->SetScale(Vector3{ 2.0f,2.0f,1.0f });
			shutterObjects_[i]->SetTranslate(Vector3(0.0f, 13.0f, -1.0f));
			shutterObjects_[i]->SetRotate(Vector3{ 0,0,0 });
		} else if (i == 1) {                                                 // [1]=下
			shutterObjects_[i]->SetScale(Vector3{ 2.0f,2.0f,1.0f });
			shutterObjects_[i]->SetTranslate(Vector3(0.0f, -10.0f, -1.0f));
			shutterObjects_[i]->SetRotate(Vector3{ 0,0,0 });
		}
	}

	// ステージを0からに初期化
	SceneManager::GetInstance()->SetStageIndex(0);

	// 決定用サウンド
	ButtonSound = Audio::GetInstance()->SoundLoadWave("Resources/Audio/Button.wav");
	// コピー用サウンド
	copeSound = Audio::GetInstance()->SoundLoadWave("Resources/Audio/Camera_copy.wav");

	// 初期化前に音声を削除(重複予防)
	Audio::GetInstance()->StopAudio();
	
	
	InitStepTable();
    currentStep = -1;

	// サウンドの初期化
	ButtonSound = Audio::GetInstance()->SoundLoadWave("Resources/Audio/Button.wav");	   // 決定用サウンド
	copeSound = Audio::GetInstance()->SoundLoadWave("Resources/Audio/Camera_copy.wav");	   // コピー用サウンド
	Bgm = Audio::GetInstance()->SoundLoadWave("Resources/Audio/bgm.wav");                  // メインサウンド
}

void TitleScene::Finalize(){}

void TitleScene::Update() {
	// オーディオの更新
	UpdateAudio();
	// 
	UpdateFadeAndInput();
	// ステップ処理だけを切り出す
	//UpdateStep();

	UpdateStep();

	// シャッター演出の更新
	shutterEffectUpdate();
	// 背景の更新
	skydome_->Update();
	for (const std::unique_ptr<Object3D>& object : titleObjects_) {
		if (object) {
			// タイトル用オブジェクトの更新
			object->Update();
		}
	}
	//sahtter演出用のオブジェクト
	for (std::unique_ptr<Object3D>& shutter : shutterObjects_) {
		if (shutter) {
			shutter->Update();
		}
	}

	// imguiの更新
	DebugGui();
}

void TitleScene::DebugGui(){
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

	if (ImGui::CollapsingHeader("Model", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Text("titleScene %d");
		if (ImGui::Button("gamePlayScene"))
		{
			SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
		}
	}
#endif // _DEBUG
}

void TitleScene::Draw()
{
	//3dオブジェクトの描画準備。3Dオブジェクトの描画に共通のグラフィックスコマンドを積む
	Object3DCommon::GetInstance()->CommonDraw();
	// 背景の描画
	skydome_->Draw();

	// 演出後に描画
	if (currentStep >= 1) {
		// プレイヤーの描画
		auto& player = titleObjects_[ObjectType::Player];
		if (player) {
			player->Draw();
		}
		if (currentStep >= 3) {
			for (size_t i = 0; i < titleObjects_.size(); ++i) {
				if (i == ObjectType::Player) continue; // プレイヤーはもう描画済み
				if (titleObjects_[i]) {
					titleObjects_[i]->Draw();
				}
			}
		}
	}

	// シャッター演出用
	for (std::unique_ptr<Object3D>& shutter : shutterObjects_) {
		if (shutter) {
			shutter->Draw();
		}
	}
	//Spriteの描画準備。spriteの描画に共通のグラフィックスコマンドを積む
	SpriteCommon::GetInstance()->CommonDraw();
	// フェード描画
	fadeManager_.Draw();
}

void TitleScene::shatterEffect() {
	if (isShutterEffectPlaying) return;
	isShutterEffectPlaying = true;
	shutterAnimTime = 0.0f;
	// 初期位置にリセット
	for (int i = 0; i < 2; ++i) {
		if (i == 0) {                                                         // [0]=上
			shutterObjects_[i]->SetTranslate(Vector3(0.0f, 13.0f, -1.0f));
		} else if (i == 1) {                                                  // [1]=下
			shutterObjects_[i]->SetTranslate(Vector3(0.0f, -10.0f, -1.0f));
		}
	}
}

void TitleScene::shutterEffectUpdate() {
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
		// ステップを移行
		if (currentStep == 2) {
			currentStep = 3;
		}
	}
	// シャッターオブジェクトの位置の更新
	for (int i = 0; i < 2; ++i) {
		if (i == 0) {                                                         // [0]=上
			shutterObjects_[i]->SetTranslate(Vector3(0.0f, topY, -1.0f));
		} else if (i == 1) {                                                  // [1]=下
			shutterObjects_[i]->SetTranslate(Vector3(0.0f, bottomY, -1.0f));
		}
	}
	if (t >= 1.0f) {
		// 挙動の終了
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
		std::unique_ptr<Object3D>& player = titleObjects_[ObjectType::Player];
		if (player) {
			Vector3 currentPos = player->GetTranslate();
			currentPos.y = easedY;
			player->SetTranslate(currentPos);
		}

		if (t >= 1.0f) {
			isEasing = false; // 完了したら止める

			// ステップ2へ進める
			currentStep = 2;
		}
	}
}

void TitleScene::UpdateAudio() {
	// 音量設定
	Audio::GetInstance()->SetVolume(&ButtonSound, 3.5f);
	Audio::GetInstance()->SetVolume(&copeSound, 2.0f);
	Audio::GetInstance()->SetVolume(&Bgm, 0.2f);
	// メインサウンド開始
	if (!bgmstart) {
		bgmstart = true;
		Audio::GetInstance()->SoundPlayloop(Bgm);
	}
}

void TitleScene::UpdateFadeAndInput() {
	// フェード更新
	fadeManager_.Update();
	CameraManager::GetInstans()->GetActiveCamera()->Update();
    if (++time > 20) {
        timehige = true;
    }

    if (timehige && currentStep >= 3) {	
		// Aボタンが押されたときに開始
        if (!fadeManager_.IsFading() && Input::GetInstance()->TriggerGamePadButton(XINPUT_GAMEPAD_A)) {	
			// 決定の音声を流す
            Audio::GetInstance()->SoundPlayWave(ButtonSound);
            isnextStep = true;
            fadeManager_.StartFadeOut();
        }

        if (fadeManager_.IsFadeOutFinished()) {	
			// シーン切り替え	
            SceneManager::GetInstance()->ChangeScene("STAGESELECTSCENE");
        }
    }
}

void TitleScene::InitStepTable() {
    stepTable_[-1] = [this]() { StepMinus1(); };
    stepTable_[0] = [this]() { Step0(); };
    stepTable_[1] = [this]() { Step1(); };
    stepTable_[2] = [this]() { Step2(); };
    stepTable_[3] = [this]() { Step3(); };
}

void TitleScene::UpdateStep() {
    auto it = stepTable_.find(currentStep);
    if (it != stepTable_.end()) {
        it->second();  // 関数呼び出し
    } else {
        Step3(); // フォールバック
    }
}

void TitleScene::StepMinus1() {
    fadeManager_.StartFadeIn();
    currentStep = 0;
}

void TitleScene::Step0() {
    auto& player = titleObjects_[ObjectType::Player];
    if (player) {
        player->SetRotate(Vector3{ 0.0f, DirectX::XMConvertToRadians(180.0f), 0.0f });
        player->SetTranslate(Vector3{ 0.0f, -10.0f, 7.0f });
    }
    currentStep = 1;
}

void TitleScene::Step1() {
    UpdatePlayerPositionByStep(0.01f);
}

void TitleScene::Step2() {
    if (!isShutterEffectPlaying) {
        shatterEffect();
        Audio::GetInstance()->SoundPlayWave(copeSound);
    }
    // currentStep = 3; として進めても良い
}

void TitleScene::Step3() {
    frameCount_++;

    float time = static_cast<float>(frameCount_) * 0.05f;
    float amplitude = 0.2f;
    float frequency = 0.5f;
    float cycle = 2.0f * 3.14159265f / frequency;
    if (time >= cycle) {
        frameCount_ = 0;
        time = 0.0f;
    }

    float floatY = std::sin(time * frequency) * amplitude;
    offset = { 0.0f, -0.7f + floatY, 0.0f };
    Vector3 newpos = {
        basePosition_.x + offset.x,
        basePosition_.y + offset.y,
        basePosition_.z + offset.z
    };

    auto& player = titleObjects_[ObjectType::Player];
    if (player) {
        player->SetTranslate(Vector3(-2.5f, newpos.y, 3.596f));
        player->SetRotate(Vector3(0.0f, DirectX::XMConvertToRadians(134.0f), DirectX::XMConvertToRadians(18.75f)));
    }

    for (size_t i = 0; i < titleObjects_.size(); ++i) {
        if (i == ObjectType::Title) {
            float yoffset = std::sinf(timer * 0.05f) * 0.1f;
            Transform trans = titleObjects_[i]->GetTransform();
            trans.translate = Vector3(0.0f, 0.5f + yoffset, 0.0f);
            trans.rotate.y += 0.01f;
            titleObjects_[i]->SetTransform(trans);
        }
    }

    float scale = 0.3f + std::sinf(timer * 0.07f) * 0.03f;
    timer++;

    if (!isnextStep) {
        nextcurrentSteptime++;
        if (nextcurrentSteptime >= MaxnextcurrentSteptime) {
            currentStep = -1;
            nextcurrentSteptime = 0;
        }
    }
}