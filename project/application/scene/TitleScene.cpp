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

// 省略
using namespace Easing;

void TitleScene::Initialize() {	
	using TextureID = TitleID::TextureID;
	using ModelID = TitleID::ModelID;		
	using AudioID = TitleID::AudioID;

	// カメラの初期化
	CameraManager::GetInstans()->Initialize();

	// 使用テクスチャ一覧
    std::vector<TextureID> textureIDs = {
        TextureID::WhiteRooms,		        
		TextureID::white,
    };
	// テクスチャの読み込み
    LoadTextures(textureIDs);

	// 使用モデル一覧
	std::vector<ModelID> modelIDs = {
		ModelID::Text_Title,
		ModelID::BackPlane,
		ModelID::PlayerCharacter,
		ModelID::ShutterEffect,
		ModelID::UI_Title_Stsrt,
		ModelID::WhiteRooms
	};
	// モデルの読み込み
    LoadModels(modelIDs);

	// サウンドの読み込み
	ButtonSound_ = Audio::GetInstance()->SoundLoadWave(TitleResourceID::GetAudioPath(AudioID::Button).c_str());	   // 決定用サウンド
	copeSound = Audio::GetInstance()->SoundLoadWave(TitleResourceID::GetAudioPath(AudioID::Camera_copy).c_str());	   // コピー用サウンド
	BgmSound_ = Audio::GetInstance()->SoundLoadWave(TitleResourceID::GetAudioPath(AudioID::bgm).c_str());            // メインサウンド
	// オーディオのリセット
	Audio::GetInstance()->StopAudio();

	// フェードインの初期化
	fadeManager_.Initialize(TitleResourceID::GetTexturePath(TextureID::white));
	fadeManager_.StartFadeIn();

	// ステージを0からに初期化
	SceneManager::GetInstance()->SetStageIndex(0);

	// 演出フェーズの初期化
	PhaseIndex_ = FadeIn;
	
	// 各オブジェクトのパラメータの初期化
	std::array<ObjectInitData, static_cast<size_t>(ObjectType::Count)> objectInitTable = {
	ObjectInitData{ModelID::Text_Title,       {0.0f, 0.5f, 0.0f},     {0.0f, 3.3f, 0.0f}},                                                                    // Title
	ObjectInitData{ModelID::UI_Title_Stsrt,   {-0.53f, -0.5f, 0.0f},  {},                   {0.3f, 0.3f, 0.3f}},                                              // Start
	ObjectInitData{ModelID::PlayerCharacter,  {0.0f, 0.0f, 7.0f},     {0.0f, DirectX::XM_PI, 0.0f}, {1.0f, 1.0f, 1.0f}, true, true, {-1.8f, -2.0f, -2.0f}},   // Player
	ObjectInitData{ModelID::WhiteRooms,       {0.0f, 0.0f, 10.0f},    {},                   {1.5f, 1.0f, 1.0f}},                                              // Skydome
	ObjectInitData{ModelID::ShutterEffect,    {0.0f, 13.0f, -1.0f},   {},                   {2.0f, 2.0f, 1.0f}},                                              // Shuttertop
	ObjectInitData{ModelID::ShutterEffect,    {0.0f, -10.0f, -1.0f},  {},                   {2.0f, 2.0f, 1.0f}},                                              // Shutterbottom
	};

	// オブジェクトの生成
	for (size_t i = 0; i < static_cast<size_t>(ObjectType::Count); ++i) {
		const auto& init = objectInitTable[i];
		objects_[i] = std::make_unique<Object3D>();
		objects_[i]->Initialize(Object3DCommon::GetInstance());
		objects_[i]->SetModel(TitleResourceID::GetModelPath(init.modelID));
		objects_[i]->SetTranslate(init.translate);
		objects_[i]->SetRotate(init.rotate);
		objects_[i]->SetScale(init.scale);
		objects_[i]->SetLighting(init.lighting);
		if (init.directionalLight) {
			objects_[i]->SetDirectionalLightEnable(true);
			objects_[i]->SetDirectionalLightDirection(init.directionalDir);
		}
	}
}

void TitleScene::Finalize(){}

void TitleScene::Update() {
	// カメラの更新（タイトルカメラ）
	CameraManager::GetInstans()->GetActiveCamera()->Update();
	// オーディオの更新
	UpdateAudio();
	// 次のシーンへの入力関数
	UpdateStartTrigger();
	// 演出フェーズの更新
	UpdatePhase();
	// シャッター演出の更新
	shutterEffectUpdate();	
	// フェード更新
	fadeManager_.Update();
	// 全オブジェクトの更新
	for (const std::unique_ptr<Object3D>& object : objects_) {
		if (object) {
			object->Update();			// オブジェクトの更新
		}
	}
	// imguiの更新
	DebugimgGui();
}

void TitleScene::DebugimgGui(){
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
	objects_[ObjectType::Skydome]->Draw();

	// 演出のフェーズによって描画
	if (PhaseIndex_ >= PlayerEasing) {
		objects_[ObjectType::Player]->Draw();		// プレイヤーの描画	
		if (PhaseIndex_ >= LoopAnimation) {
			for (size_t i = 0; i < objects_.size(); ++i) {
				if (i == ObjectType::Player || i == ObjectType::Skydome) continue; // プレイヤーとは背景はもう描画済み
				if (objects_[i]) {
					objects_[i]->Draw();
				}
			}
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
	objects_[ObjectType::Shuttertop]->SetTranslate(Vector3(0.0f, 13.0f, -1.0f));
	objects_[ObjectType::Shutterbottom]->SetTranslate(Vector3(0.0f, -10.0f, -1.0f));
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
		topY = EaseLerp(13.0f, 1.9f, p, EaseOutQuad);
		bottomY = EaseLerp(-10.0f, -1.9f, p, EaseOutQuad);
	} else {
		// 開くフェーズ（0.5〜1.0）
		float p = (t - 0.5f) / 0.5f;
		topY = EaseLerp(1.9f, 13.0f, p, EaseInQuad);
		bottomY = EaseLerp(-1.9f, -10.0f, p, EaseInQuad);
		// ステップを移行
		if (PhaseIndex_ == Shutter) {
			PhaseIndex_ = LoopAnimation;
		}
	}
	// シャッターオブジェクトの位置の更新
	objects_[ObjectType::Shuttertop]->SetTranslate(Vector3(0.0f, topY, -1.0f));
	objects_[ObjectType::Shutterbottom]->SetTranslate(Vector3(0.0f, bottomY, -1.0f));
	if (t >= 1.0f) {
		isShutterEffectPlaying = false;		// 挙動の終了
	}
}

void TitleScene::UpdatePlayerPositionByStep() {	
	// フェーズが1で、イージングがまだ始まっていない場合
	if (PhaseIndex_ == PlayerEasing && !isPhaseEasing) {
		isPhaseEasing = true;     // イージングを開始
		easingTimer = 0.0f;  // タイマーを初期化
		if (objects_[ObjectType::Player]) {     // プレイヤーを開始位置に移動
			objects_[ObjectType::Player]->SetRotate(Vector3{ 0.0f, DirectX::XMConvertToRadians(180.0f), 0.0f });
			objects_[ObjectType::Player]->SetTranslate(Vector3{ 0.0f, -10.0f, 7.0f });
		}
	}

	if (isPhaseEasing) {
		easingTimer += 0.01f;     // 経過時間を加算
		float t = std::clamp(easingTimer / easingDuration, 0.0f, 1.0f);

		// EaseOutQuad
		float easedY = EaseLerp(-10.0f, 0.0f, t, EaseOutQuad);
		std::unique_ptr<Object3D>& player = objects_[ObjectType::Player];
		if (player) {   // プレイヤーの現在の座標を更新
			Vector3 currentPos = player->GetTranslate();
			currentPos.y = easedY;   // Y座標のみ変更
			player->SetTranslate(currentPos);
		}

		if (t >= 1.0f) {
			isPhaseEasing = false;  // 完了したら止める
			PhaseIndex_ = Shutter;  // ステップ2へ進める
		}
	}
}

void TitleScene::UpdateAudio() {
	// 音量設定
	Audio::GetInstance()->SetVolume(&ButtonSound_, 3.5f);
	Audio::GetInstance()->SetVolume(&copeSound, 2.0f);
	Audio::GetInstance()->SetVolume(&BgmSound_, 0.2f);
	// メインサウンド開始
	if (!bgmstart) {
		bgmstart = true;
		Audio::GetInstance()->SoundPlayloop(BgmSound_);
	}
}

void TitleScene::UpdateStartTrigger() {
    // 入力可能状態かつフェーズが 3 以降なら
    if (PhaseIndex_ >= LoopAnimation) {
        // Aボタンが押されたときに開始
        if (!fadeManager_.IsFading() && Input::GetInstance()->TriggerGamePadButton(XINPUT_GAMEPAD_A)) {
            // 決定の音声を流す
            Audio::GetInstance()->SoundPlayWave(ButtonSound_);
            isnextStep = true;
            fadeManager_.StartFadeOut();
        }

        // フェードアウトが完了したらシーン切り替え
        if (fadeManager_.IsFadeOutFinished()) {
            SceneManager::GetInstance()->ChangeScene("STAGESELECTSCENE");
        }
    }
}

void TitleScene::UpdatePhase() {
	switch (PhaseIndex_) {
	case FadeIn:
		fadeManager_.StartFadeIn(); // フェードイン開始
		PhaseIndex_ = PlayerEasing; // プレイヤーイージング開始
		break;
	case PlayerEasing:
		// プレイヤーのイージング移動
		UpdatePlayerPositionByStep();
		break;
	case Shutter:
		if (!isShutterEffectPlaying) {
			shatterEffect();
			Audio::GetInstance()->SoundPlayWave(copeSound); // シャッターオン開始
		}
		break;
	case LoopAnimation:
		frameCount_++;  // フレームカウントを増加
		timer++;        // タイマー増加

		// プレイヤーの上下浮遊アニメーション
		const float time = static_cast<float>(frameCount_) * 0.05f;
		const float amplitude = 0.2f; // 振幅（上下移動の大きさ）
		const float frequency = 0.5f; // 周期（速度）
		const float fullCycle = 2.0f * 3.14159265f / frequency;
		
		// サイクルを超えたらリセット
		if (time >= fullCycle) {
			frameCount_ = 0;
		}

		// 浮遊Y座標の計算
		float floatY = std::sinf(time * frequency) * amplitude;
		playeroffset_ = { 0.0f, -0.7f + floatY, 0.0f };

		// 新しいプレイヤー位置（X,Zは固定、Yのみ動く）
		if (auto& player = objects_[ObjectType::Player]) {
			Vector3 newPos = {
				-2.5f,
				playeroffset_.y,
				3.596f
			};
			player->SetTranslate(newPos);
			player->SetRotate(Vector3(0.0f, DirectX::XMConvertToRadians(134.0f), DirectX::XMConvertToRadians(18.75f)));
		}

		// タイトルロゴの上下揺れ＆回転
		if (auto& title = objects_[ObjectType::Title]) {
			float yOffset = std::sinf(timer * 0.05f) * 0.1f;
			Transform trans = title->GetTransform();
			trans.translate = Vector3(0.0f, 0.5f + yOffset, 0.0f); // 浮遊
			trans.rotate.y += 0.01f; // ゆっくり回転
			title->SetTransform(trans);
		}

		// タイトルロゴのスケーリング揺れ
		float scale = 0.3f + std::sinf(timer * 0.07f) * 0.03f;
		
		// シーン遷移されなかったら時間経過でフェーズ戻す
		if (!isnextStep) {	
			constexpr int kMaxStepResetThreshold = 500;
			if (++nextcurrentSteptime >= kMaxStepResetThreshold) {
				PhaseIndex_ = FadeIn;    // フェーズをリセット
				nextcurrentSteptime = 0; // 経過時間リセット
			}
		}
		break;
	}
}

void TitleScene::LoadTextures(const std::vector<TitleID::TextureID>& textureIDs) {
	std::vector<std::string> textureNames;
    textureNames.reserve(textureIDs.size());
    for (const auto& id : textureIDs) {
		// IDに対応した.pngファイルパスを登録
        textureNames.push_back(TitleResourceID::GetTexturePath(id));
    }
    TextureManager::GetInstance()->LoadAllTextures(textureNames);
}

void TitleScene::LoadModels(const std::vector<TitleID::ModelID>& modelIDs) {
    std::vector<std::string> modelNames;
    modelNames.reserve(modelIDs.size());
    for (const auto& id : modelIDs) {
       		// IDに対応した.objファイルパスを登録
		modelNames.push_back(TitleResourceID::GetModelPath(id));
    }
    ModelManager::GetInstans()->LoadAllModels(modelNames);
}