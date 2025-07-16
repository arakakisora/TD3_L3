#include "StageSelectScene.h"
#include "Object3DCommon.h"
#include "SpriteCommon.h"
#include "Input.h"
#include "SceneManager.h"
#include "ImGuiManager.h"
#include <ModelManager.h>
#include <CameraManager.h>
#include <MyMath.h>
#include <TextureManager.h>
#include <numbers>
#include<Audio.h>
#include <Easing.h>
#ifdef _DEBUG
#endif // _DEBUG
#include <imgui.h>

using namespace Easing;

void StageSelectScene::Initialize(){
	//カメラの生成
	camera_ = std::make_unique<Camera>();
	camera_->SetTranslate({ 0,0,-50, });//カメラの位置
	CameraManager::GetInstans()->AddCamera("maincam", camera_.get());

	// モデル名
	const std::vector<std::string> modelNames = {
	"StageSelect/playercharacter",
	"StageSelect/SelectSceneBackPlane",
	};
	// ステージモデル名
	const std::vector<std::string> stageNames = {
	"StageSelect/Stage01","StageSelect/Stage02","StageSelect/Stage03","StageSelect/Stage04","StageSelect/Stage05",
	"StageSelect/Stage06","StageSelect/Stage07","StageSelect/Stage08","StageSelect/Stage09","StageSelect/Stage10",
	"StageSelect/Stage11","StageSelect/Stage12","StageSelect/Stage13",
	};
	// モデルの読み込み
	ModelManager::GetInstans()->LoadAllModels(modelNames);
	ModelManager::GetInstans()->LoadAllModels(stageNames);
	// サウンドの読み込み
	selectSound = Audio::GetInstance()->SoundLoadWave("Resources/Audio/Select.wav");    // セレクト用サウンド
	ButtonSound = Audio::GetInstance()->SoundLoadWave("Resources/Audio/Button.wav");	// 決定用サウンド
	
	// フェードインの初期化
	fadeManager_.Initialize("Resources/StageSelect/white.png");
	fadeManager_.StartFadeIn(0.5);

	// ステージのインデックスを取得
	int stageIndex = SceneManager::GetInstance()->GetStageIndex();
	currentIndex_ = stageIndex;	// 現在のステージを設定	 (ステージ1)

	// ステージオブジェクトの生成
	for (size_t i = 0; i < std::min(stages_.size(), stageNames.size()); ++i) {
		// 共通の処理
		stages_[i] = std::make_unique<Object3D>();
		stages_[i]->Initialize(Object3DCommon::GetInstance());
		stages_[i]->SetModel(stageNames[i] + ".obj");
		stages_[i]->SetLighting(false);
		stages_[i]->SetScale(Vector3(2.0f, 1.5f, 1.5f));
		stages_[i]->SetTranslate(Vector3(9.0f * i, 0.0f, 0.0f)); // X座標を変更して配置
	}

	Player_ = std::make_unique<Object3D>();
	Player_->Initialize(Object3DCommon::GetInstance());
	Player_->SetModel("StageSelect/playercharacter.obj");
	Vector3 initialPos = Vector3(9.0f * currentIndex_, -2.5f, 0.0f);
	Player_->SetTranslate(initialPos);
	Player_->SetLighting(true);
	Player_->SetDirectionalLightEnable(true);
	Player_->SetDirectionalLightDirection({ -1.3f,-1.82f,-4.77f });
	Player_->SetRotate(Vector3(0.0f, 180.0f * (DirectX::XM_PI / 180.0f), 0.0f));
	FollowTargetposition = { 0.0f,1.0f,-20.0f };		



	// プレイヤーのカメラをセット
	CameraManager::GetInstans()->GetCamera("maincam")->SetFollowTarget(Player_.get(), FollowTargetposition);
	CameraManager::GetInstans()->GetCamera("maincam")->SetFollowMode(true);
	CameraManager::GetInstans()->SetActiveCamera("maincam");

	// イージングに必要な変数
	easingmoveFlag_ = false;  // イージングフラグ
	easingProgress_ = 0.0f; // イージングの進行具合
	startPos_ = stages_[StageType::stage_01]->GetTranslate();  // 初期のステージの位置に設定
	endPos_ = stages_[StageType::stage_02]->GetTranslate();    // 次のステージの位置に設定
	easingDuration_ = 2.0f;  // イージングの期間（秒）


	// コントローラ操作のUI
	// 作成してでリストに追加
	for (uint32_t i = 0; i < 4; ++i) {
		std::unique_ptr<Sprite> newSprite = std::make_unique<Sprite>();
		if (i == 0) {
			newSprite->Initialize(SpriteCommon::GetInstance(), "Resources/xbox_stick_l.png");
			newSprite->SetPosition(Vector2(15.0f, 650.0f));
			newSprite->SetSize(Vector2(70, 60));
		} else if (i == 1) {
			newSprite->Initialize(SpriteCommon::GetInstance(), "Resources/idou.png");
			newSprite->SetPosition(Vector2(100.0f, 660.0f));
			newSprite->SetSize(Vector2(60, 50));
		} else if (i == 2) {
			newSprite->Initialize(SpriteCommon::GetInstance(), "Resources/xbox_button_color_a.png");
			newSprite->SetPosition(Vector2(180.0f, 648.0f));
			newSprite->SetSize(Vector2(70, 70));
		} else if (i == 3) {
			newSprite->Initialize(SpriteCommon::GetInstance(), "Resources/kettei.png");
			newSprite->SetPosition(Vector2(270.0f, 660.0f));
			newSprite->SetSize(Vector2(60, 50));
		}
		newSprite->setColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		xboxui.push_back(std::move(newSprite));
	}

	for (uint32_t i = 0; i < 2; ++i) {
		std::unique_ptr<Sprite> newSprite = std::make_unique<Sprite>();
		if (i == 0) {
			newSprite->Initialize(SpriteCommon::GetInstance(), "Resources/PauseMenu/Pause.png");
			newSprite->SetPosition(Vector2(15.0f, 15.0f));
			newSprite->SetSize(Vector2(150, 50));
		} else if (i == 1) {
			newSprite->Initialize(SpriteCommon::GetInstance(), "Resources/xbox_button_menu.png");
			newSprite->SetPosition(Vector2(170.0f, 5.0f));
			newSprite->SetSize(Vector2(70, 70));
		}
		newSprite->setColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		pauseui.push_back(std::move(newSprite));
	}

	//ポーズメニュー
	pauseMenu = std::make_unique<PauseMenu>();
	pauseMenu->Initialize(Object3DCommon::GetInstance(), PauseType::StageSelectScene);
	pauseMenu->SetCamera(CameraManager::GetInstans()->GetCamera("maincam"));

	// 背景
	skydome_ = std::make_unique<Object3D>();
	skydome_->Initialize(Object3DCommon::GetInstance());
	skydome_->SetModel("StageSelect/SelectSceneBackPlane.obj");
	Vector3 planePos = { -8.0f,0.0f,135.0f };
	planePos.x =  -8.0f + initialPos.x;
	Vector3 planeScale = { 1.0f,0.55f,1.0f };
	skydome_->SetTranslate(planePos);
	skydome_->SetScale(planeScale);

	// プレイヤーに追従するパーティクルの設定
	ParticleMnager::GetInstance()->CreateParticleGroup("Player", "Resources/StageSelect/block.png", "StageSelect/block.obj");
	playeremitter_ = std::make_unique<ParticleEmitter>(
		Vector3{ 0.0f,0.0f,0.0f },
		5.0f,
		0.0f,
		1,
		"Player"
	);
}

void StageSelectScene::Finalize(){
	CameraManager::GetInstans()->RemoveCamera("maincam");
	CameraManager::GetInstans()->RemoveCamera("subcam");
	CameraManager::GetInstans()->Finalize();
	CameraManager::GetInstans()->Finalize();
}

void StageSelectScene::Update(){
	// フェード更新
	fadeManager_.Update();
	// オーディオの更新
	UpdateAudio(); 
	// ポーズ中かどうかで変わる処理
	if (!pauseMenu->IsPaused()) {
		// ポーズ中の処理
        UpdateDuringPlay();
    } else {
		// 非ポーズ中の処理
        UpdateDuringPause();
    }
	// キー入力しない限り更新
	if (!easingsceneFlag_ && !easingmoveFlag_) {
		// ポーズ
		pauseMenu->Update();
	}
	
	skydome_->Update();	

	for (size_t i = 0; i < stages_.size(); ++i) {
		stages_[i]->Update();		// ステージの更新
	}		
	// UI
	for (std::unique_ptr<Sprite>& Uitext : xboxui) {
		Uitext->Update();
	}
	for (std::unique_ptr<Sprite>& Uitext : pauseui) {
		Uitext->Update();
	}

	DebugimgGui();	// デバッグ用のImGui描画
}

void StageSelectScene::DebugimgGui() {
#ifdef _DEBUG
	if (ImGui::CollapsingHeader("Skydome SRT", ImGuiTreeNodeFlags_DefaultOpen)) {
		Transform transform = skydome_->GetTransform();

		if (ImGui::DragFloat3("Skydome Translate", &transform.translate.x, 0.1f)) {
			skydome_->SetTranslate(transform.translate);
		}
		if (ImGui::DragFloat3("Skydome Rotate", &transform.rotate.x, 0.01f)) {
			skydome_->SetRotate(transform.rotate);
		}
		if (ImGui::DragFloat3("Skydome Scale", &transform.scale.x, 0.01f, 0.01f, 10.0f)) {
			skydome_->SetScale(transform.scale);
		}
	}

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
#endif // _DEBUG
}

void StageSelectScene::Draw() {
#pragma region 3Dオブジェクト描画
	//3dオブジェクトの描画準備。3Dオブジェクトの描画に共通のグラフィックスコマンドを積む
	Object3DCommon::GetInstance()->CommonDraw();
	skydome_->Draw();


	Player_->Draw();

	for (size_t i = 0; i < stages_.size(); ++i) {
		stages_[i]->Draw();		// ステージの描画
	}		

	//ポーズメニュー
	pauseMenu->Draw();

	ParticleMnager::GetInstance()->Draw();

#pragma endregion

#pragma region スプライト描画
	//Spriteの描画準備。spriteの描画に共通のグラフィックスコマンドを積む
	SpriteCommon::GetInstance()->CommonDraw();

	// UI
	for (std::unique_ptr<Sprite>& Uitext : xboxui) {
		Uitext->Draw();
	}

	for (std::unique_ptr<Sprite>& Uitext : pauseui) {
		Uitext->Draw();
	}
	// フェード描画
	fadeManager_.Draw();

#pragma endregion
}

void StageSelectScene::move() {
	// 長押し対応用の遅延時間
	static float holdDelay_ = 0.1f; 	                              // 長押しと認識するための値 0.1秒（調整可能）
	static float holdTimer_ = 0.0f;	                                  // 入力が続いた時間を測る判定用のタイマー
	bool continueMove = (holdTimer_ > holdDelay_);                    // 一定時間（0.1秒）スティックが倒されたままなら長押しと判定。
	float rightStickX = Input::GetInstance()->GetGamePadStickX();     // 右スティックのX方向の値（-1.0〜1.0）を取得。
	const float stickThreshold = 0.5f;	                              // スティックのしきい値（デッドゾーンを超えたときのみ反応）

	Vector3 Rotate = Player_->GetRotate();
#ifdef _DEBUG
	// キー入力による代替
	if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
		rightStickX = 1.0f;
	} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
		rightStickX = -1.0f;
	}
#endif // _DEBUG

	if ((rightStickX > stickThreshold || (continueMove && rightStickX > stickThreshold))             // 右かつ長押しも考慮した移動
		&& currentIndex_ < MaxSelectIndex_ - 1 && !easingmoveFlag_ && !easingsceneFlag_) {
		// プレイヤーのイージング移動準備
		currentIndex_++;
		easingmoveFlag_ = true;
		// ステージの位置を取得してイージング開始
		startPos_ = stages_ [currentIndex_ - 1]->GetTranslate();  // 直前の位置
		endPos_ = stages_ [currentIndex_]->GetTranslate();        // 新しい位置
		easingProgress_ = 0.0f;  // イージング開始
		holdTimer_ = 0.0f; // 長押しリセット
		// プレイヤーを右に90度回転
		Rotate.y -= 90.0f * (DirectX::XM_PI / 180.0f);
		Player_->SetRotate(Rotate);
		// パーティクルのフラグ設定（右移動）
		playermoveright = true;
		playermoveleft = false;
		Audio::GetInstance()->SoundPlayWave(selectSound);		// セレクト音声を流す
	} else if ((rightStickX < -stickThreshold || (continueMove && rightStickX < -stickThreshold))     // 左かつ長押しも考慮した移動
		&& currentIndex_ > 0 && !easingmoveFlag_ && !easingsceneFlag_) {
		// プレイヤーのイージング移動準備
		currentIndex_--;
		easingmoveFlag_ = true;
		// ステージの位置を取得してイージング開始
		startPos_ = stages_[currentIndex_ + 1]->GetTranslate();  // 直前の位置
		endPos_ = stages_[currentIndex_]->GetTranslate();        // 新しい位置
		easingProgress_ = 0.0f;  // イージング開始
		holdTimer_ = 0.0f; // 長押しリセット
		// プレイヤーを左に-90度回転
		Rotate.y += 90.0f * (DirectX::XM_PI / 180.0f);
		Player_->SetRotate(Rotate);
		// パーティクルのフラグ設定（左移動）
		playermoveright = false;
		playermoveleft = true;
		Audio::GetInstance()->SoundPlayWave(selectSound);		// セレクト音声を流す
	}

	if (easingmoveFlag_) {	// プレイヤーのイージング移動処理
		// フレーム毎秒
		easingProgress_ += (1.0f / (easingDuration_ * 20.0f));
		if (easingProgress_ > 1.0f) {
			easingProgress_ = 1.0f;
		}

		// 線形補間		
		float easedValue = EaseInOutQuad(easingProgress_);
		Vector3 newPos = SmoothLerp(startPos_, endPos_, easedValue);
		// プレイヤーのY座標を固定
		newPos.y = -2.5f;
		// メインオブジェクトを移動
		Player_->SetTranslate(newPos);
		// カメラが "main" のときだけカメラの位置更新
		Camera* activeCam = CameraManager::GetInstans()->GetActiveCamera();
		if (activeCam == CameraManager::GetInstans()->GetCamera("maincam")) {
			FollowTargetposition.y = 1.0f;
			FollowTargetposition.z = -20.0f;
			activeCam->SetFollowTarget(Player_.get(), FollowTargetposition);
		}

		Vector3 planePos = skydome_->GetTranslate();
		planePos = { Player_->GetTransform().translate.x + -8.0f, planePos.y,planePos.z };
		skydome_->SetTranslate(planePos);
		skydome_->Update();

		// イージング完了
		if (easingProgress_ >= 1.0f) {
			easingmoveFlag_ = false;

			// プレイヤーの回転をリセット
			Player_->SetRotate(Vector3(0.0f, 180.0f * (DirectX::XM_PI / 180.0f), 0.0f));

			// スティックがニュートラルならフォロー位置もリセット
			if (fabs(rightStickX) < stickThreshold) {
				FollowTargetposition.y = 1.0f;
				FollowTargetposition.z = -20.0f;
			}
			// フォローターゲットを再設定（Z位置が変わったときのみでもOK）
			activeCam->SetFollowTarget(Player_.get(), FollowTargetposition);

			// パーティクルのフラグリセット
			playermoveright = false;
			playermoveleft = false;
		}
	} else {
		// イージングが終わっている間は長押しタイマーをカウント
		if (fabs(rightStickX) > stickThreshold) {
			holdTimer_ += (1.0f / 60.0f); // 60FPS想定
		} else {
			holdTimer_ = 0.0f; // スティックが戻ったらリセット
		}
	}
}

void StageSelectScene::moveChangeScene() {
	if (!easingsceneFlag_ && !easingmoveFlag_) {
#ifdef _DEBUG
		if (Input::GetInstance()->PushKey(DIK_SPACE)) {
			easingsceneFlag_ = true;
			easingmoveFlag_ = true;

			// stageObjects_ の現在位置にカメラを移動
			Vector3 selectObjectPos = stages_ [currentIndex_]->GetTranslate();
			// mainObject の位置を stageObjects_ の位置に設定
			Player_->SetTranslate(selectObjectPos);
			// カメラのターゲットを現在選択されているオブジェクトに設定
			CameraManager::GetInstans()->GetCamera("maincam")->SetFollowTarget(stages_ [currentIndex_].get(), { 0, 0, -15 });
			CameraManager::GetInstans()->GetCamera("maincam")->SetFollowMode(true);

			// 開始位置
			startPos_ = Vector3(Player_->GetTranslate().x, Player_->GetTranslate().y, -15.0f);
			// 終了位置
			endPos_ = Vector3(Player_->GetTranslate().x, Player_->GetTranslate().y, 15.0f);
			easingProgress_ = 0.0f;  // イージング開始
		}
#endif // _DEBUG

		// Aボタンが押されたときに開始
		if (Input::GetInstance()->TriggerGamePadButton(XINPUT_GAMEPAD_A)) {
			easingsceneFlag_ = true;
			easingmoveFlag_ = true;

			// stageObjects_ の現在位置にカメラを移動
			Vector3 selectObjectPos = stages_ [currentIndex_]->GetTranslate();
			// mainObject の位置を stageObjects_ の位置に設定
			Player_->SetTranslate(selectObjectPos);
			// カメラのターゲットを現在選択されているオブジェクトに設定
			CameraManager::GetInstans()->GetCamera("maincam")->SetFollowTarget(stages_ [currentIndex_].get(), { 0, 0, -15 });
			CameraManager::GetInstans()->GetCamera("maincam")->SetFollowMode(true);

			// 開始位置
			startPos_ = Vector3(Player_->GetTranslate().x, Player_->GetTranslate().y, -15.0f);
			// 終了位置
			endPos_ = Vector3(Player_->GetTranslate().x, Player_->GetTranslate().y, 15.0f);
			easingProgress_ = 0.0f;  // イージング開始
			
			// 決定の音声を流す
			Audio::GetInstance()->SoundPlayWave(ButtonSound);
		}
	}

	if (easingsceneFlag_) {

		Vector3 newPos = Player_->GetTranslate();
		newPos.y = -2.5f;
		newPos.z = 0.0f;
		Player_->SetTranslate(newPos);

		// カメラの位置をイージングで移動
		Camera* activeCam = CameraManager::GetInstans()->GetActiveCamera();
		if (activeCam == CameraManager::GetInstans()->GetCamera("maincam")) {
			activeCam->SetFollowTarget(stages_ [currentIndex_].get(), Vector3(0.0f, 0.0f, -15.0f));

			// イージングの進行状況を更新
			easingProgress_ += (1.0f / (easingDuration_ * 120.0f));

			// イージングの進行が1を超えないように制限
			if (easingProgress_ > 1.0f) {
				easingProgress_ = 1.0f;
			}

			// 線形補間（カメラの移動）
			float easedValue = EaseInOutBack(easingProgress_);

			// カメラの移動をイージングで計算
			Vector3 newCamPos = SmoothLerp(startPos_, endPos_, easedValue);

			// カメラの位置をイージングで移動
			activeCam->SetTranslate(newCamPos);
		}

		// イージング完了
		if (easingProgress_ >= 0.8f) {
			easingsceneFlag_ = false;  // イージング完了後、フラグをリセット


			SceneManager::GetInstance()->SetStageIndex(currentIndex_);
			// シーン変更（必要に応じてシーン変更を実行）
			SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
		}
	}
}

void StageSelectScene::UpdateAudio() {	
	// 音量設定
	Audio::GetInstance()->SetVolume(&selectSound, 2.0f);
	Audio::GetInstance()->SetVolume(&ButtonSound, 3.0f);
}

void StageSelectScene::UpdateDuringPlay() {
	const int MaxframeCounter = 10;
	//カメラの更新
	CameraManager::GetInstans()->GetActiveCamera()->Update();
	// 移動処理			
	move();
	// 10フレーム経過するまでシーン遷移禁止
	if (frameCounter_ < MaxframeCounter) {
		frameCounter_++;
	} else {
		frameCounter_ = MaxframeCounter;
		// シーン遷移
		moveChangeScene();
	}
	Player_->Update();
	// パーティクルの更新
	UpdatePlayerParticle();
}

void StageSelectScene::UpdateDuringPause() {
	// ▼ カメラを引く処理をここにも追加 ▼
	FollowTargetposition.y = 1.0f;
	FollowTargetposition.z = -20.0f;
	Camera* activeCam = CameraManager::GetInstans()->GetActiveCamera();
	if (activeCam == CameraManager::GetInstans()->GetCamera("maincam")) {
		activeCam->SetFollowTarget(Player_.get(), FollowTargetposition);
	}

	Player_->Update();

	// パーティクルの更新
	UpdatePlayerParticle();

	// ポーズ中でもカメラだけ更新
	CameraManager::GetInstans()->GetActiveCamera()->Update();
}

void StageSelectScene::UpdatePlayerParticle() {	
	// プレイヤー用のパーティクルの位置を常に更新
	Vector3 pos = Player_->GetTranslate();

	if (playermoveright) {         // 右に移動中
		// 左方向に設定
		playeremitter_->SetisRight(false);
		Vector3 offset = { -0.3f,0.0f,0.0f };
		playeremitter_->SetPosition(pos + offset);
		playeremitter_->PlayerEmit();
	}else if (playermoveleft) {	   // 左に移動中
		// 右方向に設定
		playeremitter_->SetisRight(true);
		Vector3 offset = { 0.3f,0.0f,0.0f };
		playeremitter_->SetPosition(pos + offset);
		playeremitter_->PlayerEmit();
	}
}