#include "StageSelectScene.h"
#include "Object3DCommon.h"
#include "SpriteCommon.h"
#include "Input.h"
#include "SceneManager.h"
#include "ImGuiManager.h"
#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG
#include <ModelManager.h>
#include <CameraManager.h>
#include <MyMath.h>
#include <TextureManager.h>
#include <numbers>

void StageSelectScene::Initialize()
{
	//カメラの生成
	camera_ = std::make_unique<Camera>();
	camera_->SetTranslate({ 0,0,-50, });//カメラの位置
	CameraManager::GetInstans()->AddCamera("maincam", camera_.get());

	//モデルの読み込み				
	ModelManager::GetInstans()->LoadModel("axis.obj");
	ModelManager::GetInstans()->LoadModel("plane.obj");
	ModelManager::GetInstans()->LoadModel("sphere.obj");
	ModelManager::GetInstans()->LoadModel("terrain.obj");

	ModelManager::GetInstans()->LoadModel("playercharacter.obj");
	ModelManager::GetInstans()->LoadModel("StageSelect/Text_1-1.obj");
	ModelManager::GetInstans()->LoadModel("StageSelect/Text_1-2.obj");
	ModelManager::GetInstans()->LoadModel("StageSelect/Text_1-3.obj");

	ModelManager::GetInstans()->LoadModel("StageSelect/Stage01.obj");
	ModelManager::GetInstans()->LoadModel("StageSelect/Stage02.obj");
	ModelManager::GetInstans()->LoadModel("StageSelect/Stage03.obj");

	ModelManager::GetInstans()->LoadModel("Pause.obj");

	ModelManager::GetInstans()->LoadModel("StageSelect/title.obj");
	ModelManager::GetInstans()->LoadModel("StageSelect/explanation.obj");
	ModelManager::GetInstans()->LoadModel("StageSelect/return.obj");


	// 背景
	ModelManager::GetInstans()->LoadModel("SelectSceneBackPlane.obj");




	int stageIndex = SceneManager::GetInstance()->GetStageIndex();

	currentIndex_ = stageIndex;

	Player_ = new Object3D();
	Player_->Initialize(Object3DCommon::GetInstance());
	Player_->SetModel("playercharacter.obj");
	Vector3 initialPos = Vector3(9.0f * currentIndex_, -2.5f, 0.0f);
	Player_->SetTranslate(initialPos);
	Player_->SetLighting(true);
	Player_->SetDirectionalLightEnable(true);
	Player_->SetDirectionalLightDirection({ -1.3f,-1.82f,-4.77f });
	Player_->SetRotate(Vector3(0.0f, 180.0f * (DirectX::XM_PI / 180.0f), 0.0f));

	// 作成してでリストに追加
	for (uint32_t i = 0; i < MaxSelectIndex_; ++i) {
		std::unique_ptr<Object3D> newObject = std::make_unique<Object3D>();
		newObject->Initialize(Object3DCommon::GetInstance());
		if (i == 0) {
			newObject->SetModel("StageSelect/Stage01.obj");
		} else if (i == 1) {
			newObject->SetModel("StageSelect/Stage02.obj");
		} else if (i == 2) {
			newObject->SetModel("StageSelect/Stage03.obj");
		} else if (i == 3) {
			newObject->SetModel("StageSelect/Stage01.obj");
		} else if (i == 4) {
			newObject->SetModel("StageSelect/Stage02.obj");
		} else if (i == 5) {
			newObject->SetModel("StageSelect/Stage03.obj");
		} else if (i == 6) {
			newObject->SetModel("StageSelect/Stage01.obj");
		} else if (i == 7) {
			newObject->SetModel("StageSelect/Stage02.obj");
		} else if (i == 8) {
			newObject->SetModel("StageSelect/Stage03.obj");
		} else if (i == 9) {
			newObject->SetModel("StageSelect/Stage01.obj");
		} else {
			newObject->SetModel("StageSelect/Stage01.obj");
		}
		newObject->SetTranslate(Vector3(9.0f * i, 0.0f, 0.0f)); // X座標を変更して配置
		newObject->SetLighting(false);
		newObject->SetScale(Vector3(1.5f, 1.5f, 1.5f));
		stageObjects_.push_back(std::move(newObject));
	}

	FollowTargetposition = { 0.0f,1.0f,-20.0f };

	CameraManager::GetInstans()->GetCamera("maincam")->SetFollowTarget(Player_, FollowTargetposition);
	CameraManager::GetInstans()->GetCamera("maincam")->SetFollowMode(true);
	CameraManager::GetInstans()->SetActiveCamera("maincam");


	// イージングに必要な変数
	easingmoveFlag_ = false;  // イージングフラグ
	easingProgress_ = 0.0f; // イージングの進行具合
	startPos_ = stageObjects_.front()->GetTranslate();  // 移動開始位置
	endPos_ = stageObjects_.at(1)->GetTranslate();  // 移動終了位置（例えば2番目のオブジェクトへ）
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
			newSprite->Initialize(SpriteCommon::GetInstance(), "Resources/Pause.png");
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
	skydome_->SetModel("SelectSceneBackPlane.obj");
	Vector3 planePos = { -8.0f,0.0f,135.0f };
	Vector3 planeScale = { 1.0f,0.55f,1.0f };
	skydome_->SetTranslate(planePos);
	skydome_->SetScale(planeScale);



	fadeManager_.Initialize("Resources/white.png");
	fadeManager_.StartFadeIn(0.5);


	ParticleMnager::GetInstance()->CreateParticleGroup("Player", "Resources/block.png", "block.obj");

	playeremitter_ = new ParticleEmitter(
		{ 0.0f,0.0f,0.0f },
		5.0f,
		0.0f,
		1,
		"Player"
	);
}

void StageSelectScene::Finalize()
{
	CameraManager::GetInstans()->RemoveCamera("maincam");
	CameraManager::GetInstans()->RemoveCamera("subcam");
	CameraManager::GetInstans()->Finalize();

	CameraManager::GetInstans()->Finalize();

	delete Player_;

	delete playeremitter_;
}


void StageSelectScene::Update()
{
	// フェード更新
	fadeManager_.Update();
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

	skydome_->Update();





	//ポーズ画面が出ている間は停止
	if (!pauseMenu->IsPaused()) {
		//カメラの更新
		CameraManager::GetInstans()->GetActiveCamera()->Update();

		// 移動処理			
		move();

		// 10フレーム経過するまでシーン遷移禁止
		if (frameCounter_ < 10) {
			frameCounter_++;
		} else
		{
			frameCounter_ = 10;
			// シーン遷移
			moveChangeScene();
		}

		Player_->Update();
		// プレイヤー用のパーティクルの位置を常に更新
		Vector3 pos = Player_->GetTranslate();

		// 右に移動中
		if (playermoveright) {
			// 左方向に設定
			playeremitter_->SetisRight(false);
			Vector3 offset = { -0.3f,0.0f,0.0f };
			playeremitter_->SetPosition(pos + offset);
			playeremitter_->PlayerEmit();
		}
		// 左に移動中
		if (playermoveleft) {
			// 右方向に設定
			playeremitter_->SetisRight(true);
			Vector3 offset = { 0.3f,0.0f,0.0f };
			playeremitter_->SetPosition(pos + offset);
			playeremitter_->PlayerEmit();
		}

	} else {
		// ▼ カメラを引く処理をここにも追加 ▼
		FollowTargetposition.y = 1.0f;
		FollowTargetposition.z = -20.0f;
		Camera* activeCam = CameraManager::GetInstans()->GetActiveCamera();
		if (activeCam == CameraManager::GetInstans()->GetCamera("maincam")) {
			activeCam->SetFollowTarget(Player_, FollowTargetposition);
		}

		Player_->Update();
		// プレイヤー用のパーティクルの位置を常に更新
		Vector3 pos = Player_->GetTranslate();

		// ←ポーズ中でもカメラだけ更新
		CameraManager::GetInstans()->GetActiveCamera()->Update();
	}

	if (!easingsceneFlag_ && !easingmoveFlag_) {
		// ポーズ
		pauseMenu->Update();
	}

	for (std::unique_ptr<Object3D>& stage : stageObjects_) {
		stage->Update();
	}

	// UI
	for (std::unique_ptr<Sprite>& Uitext : xboxui) {
		Uitext->Update();
	}
	for (std::unique_ptr<Sprite>& Uitext : pauseui) {
		Uitext->Update();
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

#endif // _DEBUG

	//// ステージを決定していないなら
	//if (!easingsceneFlag_ && !easingmoveFlag_) {
	//	// タイトルへシーン遷移
	//	if (Input::GetInstance()->TriggerGamePadButton(XINPUT_GAMEPAD_X)) {
	//		titlefige_ = true;
	//	}

	//	if (titlefige_) {
	//		SceneManager::GetInstance()->ChangeScene("TITELE");
	//	}
	//}
}

void StageSelectScene::Draw() {
#pragma region 3Dオブジェクト描画
	//3dオブジェクトの描画準備。3Dオブジェクトの描画に共通のグラフィックスコマンドを積む
	Object3DCommon::GetInstance()->CommonDraw();
	skydome_->Draw();


	Player_->Draw();

	for (std::unique_ptr<Object3D>& stage : stageObjects_) {
		stage->Draw();
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
	static float holdDelay_ = 0.1f; // 0.1秒（調整可能）
	static float holdTimer_ = 0.0f; // 長押し判定用のタイマー

	// 長押し時の処理継続
	bool continueMove = (holdTimer_ > holdDelay_);

	// 右スティックのX軸入力を取得
	float rightStickX = Input::GetInstance()->GetGamePadStickX(); // 右スティックのX軸入力

	// スティックのしきい値（デッドゾーンを超えたときのみ反応）
	const float stickThreshold = 0.5f;

	Vector3 Rotate = Player_->GetRotate();
#ifdef _DEBUG
	// キー入力による代替
	if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
		rightStickX = 1.0f;
	} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
		rightStickX = -1.0f;
	}
#endif // _DEBUG

	// (右に移動)
	if ((rightStickX > stickThreshold || (continueMove && rightStickX > stickThreshold))
		&& currentIndex_ < MaxSelectIndex_ - 1 && !easingmoveFlag_ && !easingsceneFlag_) {

		currentIndex_++;
		easingmoveFlag_ = true;
		startPos_ = stageObjects_[currentIndex_ - 1]->GetTranslate();  // 直前の位置
		endPos_ = stageObjects_[currentIndex_]->GetTranslate();        // 新しい位置
		easingProgress_ = 0.0f;  // イージング開始
		holdTimer_ = 0.0f; // 長押しリセット

		// プレイヤーを右に90度回転
		Rotate.y -= 90.0f * (DirectX::XM_PI / 180.0f);
		Player_->SetRotate(Rotate);

		// パーティクルのフラグ設定（右移動）
		playermoveright = true;
		playermoveleft = false;
	}

	// (左に移動)
	if ((rightStickX < -stickThreshold || (continueMove && rightStickX < -stickThreshold))
		&& currentIndex_ > 0 && !easingmoveFlag_ && !easingsceneFlag_) {

		currentIndex_--;
		easingmoveFlag_ = true;
		startPos_ = stageObjects_[currentIndex_ + 1]->GetTranslate();  // 直前の位置
		endPos_ = stageObjects_[currentIndex_]->GetTranslate();        // 新しい位置
		easingProgress_ = 0.0f;  // イージング開始
		holdTimer_ = 0.0f; // 長押しリセット

		// プレイヤーを左に-90度回転
		Rotate.y += 90.0f * (DirectX::XM_PI / 180.0f);
		Player_->SetRotate(Rotate);

		// パーティクルのフラグ設定（左移動）
		playermoveright = false;
		playermoveleft = true;
	}

	// イージング処理
	if (easingmoveFlag_) {

		// フレーム毎秒
		easingProgress_ += (1.0f / (easingDuration_ * 20.0f));

		if (easingProgress_ > 1.0f) {
			easingProgress_ = 1.0f;
		}

		// 線形補間		
		float easedValue = EaseInOutQuad(easingProgress_);

		Vector3 newPos = SmoothLerp(startPos_, endPos_, easedValue);

		// mainObject の y 座標を常に -2.0f に設定
		newPos.y = -2.5f;

		// メインオブジェクトを移動
		Player_->SetTranslate(newPos);

		// カメラが "main" のときだけカメラの位置更新
		Camera* activeCam = CameraManager::GetInstans()->GetActiveCamera();
		if (activeCam == CameraManager::GetInstans()->GetCamera("maincam")) {
			FollowTargetposition.y = 1.0f;
			FollowTargetposition.z = -20.0f;
			activeCam->SetFollowTarget(Player_, FollowTargetposition);

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
			activeCam->SetFollowTarget(Player_, FollowTargetposition);

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
			Vector3 selectObjectPos = stageObjects_[currentIndex_]->GetTranslate();
			// mainObject の位置を stageObjects_ の位置に設定
			Player_->SetTranslate(selectObjectPos);
			// カメラのターゲットを現在選択されているオブジェクトに設定
			CameraManager::GetInstans()->GetCamera("maincam")->SetFollowTarget(stageObjects_[currentIndex_].get(), { 0, 0, -15 });
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
			Vector3 selectObjectPos = stageObjects_[currentIndex_]->GetTranslate();
			// mainObject の位置を stageObjects_ の位置に設定
			Player_->SetTranslate(selectObjectPos);
			// カメラのターゲットを現在選択されているオブジェクトに設定
			CameraManager::GetInstans()->GetCamera("maincam")->SetFollowTarget(stageObjects_[currentIndex_].get(), { 0, 0, -15 });
			CameraManager::GetInstans()->GetCamera("maincam")->SetFollowMode(true);

			// 開始位置
			startPos_ = Vector3(Player_->GetTranslate().x, Player_->GetTranslate().y, -15.0f);
			// 終了位置
			endPos_ = Vector3(Player_->GetTranslate().x, Player_->GetTranslate().y, 15.0f);
			easingProgress_ = 0.0f;  // イージング開始
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
			activeCam->SetFollowTarget(stageObjects_[currentIndex_].get(), Vector3(0.0f, 0.0f, -15.0f));

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