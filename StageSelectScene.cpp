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
	fadeManager_.Initialize("Resources/white.png");
	fadeManager_.StartFadeIn(0.5f);



	//カメラの生成
	camera_ = std::make_unique<Camera>();
	camera_->SetTranslate({ 0,0,-50, });//カメラの位置
	CameraManager::GetInstans()->AddCamera("maincam", camera_.get());

	TextureManager::GetInstance()->LoadTexture("Resources/StageSelect/controllerUI.png");
	TextureManager::GetInstance()->LoadTexture("Resources/TextUI_Title.png");
	TextureManager::GetInstance()->LoadTexture("Resources/TextUI_X.png");

	//モデルの読み込み				
	ModelManager::GetInstans()->LoadModel("axis.obj");
	ModelManager::GetInstans()->LoadModel("plane.obj");
	ModelManager::GetInstans()->LoadModel("sphere.obj");
	ModelManager::GetInstans()->LoadModel("terrain.obj");

	ModelManager::GetInstans()->LoadModel("Player.obj");
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


	Player_ = new Object3D();
	Player_->Initialize(Object3DCommon::GetInstance());
	Player_->SetModel("Player.obj");
	Player_->SetTranslate(Vector3(0.0f, -2.0f, 0.0f));	
	Player_->SetRotate({ 0,  -90.0f * (DirectX::XM_PI / 180.0f) , 180.0f * (DirectX::XM_PI / 180.0f) });
	Player_->SetLighting(false);

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
		} else {
			newObject->SetModel("StageSelect/Stage01.obj");
		}
		newObject->SetTranslate(Vector3(7.0f * i, 0.0f, 0.0f)); // X座標を変更して配置
		newObject->SetLighting(false);
		stageObjects_.push_back(std::move(newObject));
	}
	// 作成してでリストに追加
	for (uint32_t i = 0; i < MaxSelectIndex_; ++i) {
		std::unique_ptr<Object3D> newObject = std::make_unique<Object3D>();
		newObject->Initialize(Object3DCommon::GetInstance());
		if (i == 0) {
			newObject->SetModel("StageSelect/Text_1-1.obj");
		} else if (i == 1) {
			newObject->SetModel("StageSelect/Text_1-2.obj");
		} else if (i == 2) {
			newObject->SetModel("StageSelect/Text_1-3.obj");
		} else {
			newObject->SetModel("StageSelect/Text_1-1.obj");
		}
		newObject->SetTranslate(Vector3(7.0f * i, 2.5f, 0.0f)); // X座標を変更して配置
		newObject->SetLighting(false);
		textoObjects_.push_back(std::move(newObject));
	}


	FollowTargetposition = { 0.0f,0.0f,-15.0f };

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
	uIController_ = std::make_unique <Sprite>();
	uIController_->Initialize(SpriteCommon::GetInstance(), "Resources/StageSelect/controllerUI.png");
	uIController_->SetPosition(Vector2( 15.0f,610.0f ));
	uIController_->SetSize({ 340.0f, 100.0f });
	uIController_->SetRotation(0.0f);
	uIController_->setColor({ 1.0f, 1.0f, 1.0f, 1.0f });

	uITitle_ = std::make_unique <Sprite>();
	uITitle_->Initialize(SpriteCommon::GetInstance(), "Resources/TextUI_Title.png");
	uITitle_->SetPosition(Vector2(-50.0f, -10.0f));
	uITitle_->SetSize({ 336.0f, 70.0f });
	uITitle_->SetRotation(0.0f);
	uITitle_->setColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	
	uIX_ = std::make_unique <Sprite>();
	uIX_->Initialize(SpriteCommon::GetInstance(), "Resources/TextUI_X.png");
	uIX_->SetPosition(Vector2(230.0f, 15.0f));
	uIX_->SetSize({ 30.0f, 30.0f });
	uIX_->SetRotation(0.0f);
	uIX_->setColor({ 1.0f, 1.0f, 1.0f, 1.0f });

	//ポーズメニュー
	pauseMenu = std::make_unique<PauseMenu>();
	pauseMenu->Initialize(Object3DCommon::GetInstance(), false);
	pauseMenu->SetCamera(CameraManager::GetInstans()->GetCamera("maincam"));
}

void StageSelectScene::Finalize()
{
	CameraManager::GetInstans()->RemoveCamera("maincam");
	CameraManager::GetInstans()->RemoveCamera("subcam");
	CameraManager::GetInstans()->Finalize();

	CameraManager::GetInstans()->Finalize();

	delete Player_;
}


void StageSelectScene::Update()
{
	fadeManager_.Update();

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

	} else {
		// ▼ カメラを引く処理をここにも追加 ▼
		FollowTargetposition.z = -30.0f;
		Camera* activeCam = CameraManager::GetInstans()->GetActiveCamera();
		if (activeCam == CameraManager::GetInstans()->GetCamera("maincam")) {
			activeCam->SetFollowTarget(Player_, FollowTargetposition);
		}

		Player_->Update();

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

	for (std::unique_ptr<Object3D>& text : textoObjects_) {
		text->Update();
	}


	// UI
	uIController_->Update();
	uITitle_->Update();
	uIX_->Update();


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

	// ステージを決定していないなら
	if (!easingsceneFlag_ && !easingmoveFlag_) {
		// タイトルへシーン遷移
		if (Input::GetInstance()->TriggerGamePadButton(XINPUT_GAMEPAD_X)) {
			titlefige_ = true;
		}

		if (titlefige_) {
			SceneManager::GetInstance()->ChangeScene("TITELE");
		}
	}
}

void StageSelectScene::Draw() {
#pragma region 3Dオブジェクト描画
	//3dオブジェクトの描画準備。3Dオブジェクトの描画に共通のグラフィックスコマンドを積む
	Object3DCommon::GetInstance()->CommonDraw();


	Player_->Draw();
	
	for (std::unique_ptr<Object3D>& stage : stageObjects_) {
		stage->Draw();
	}

	for (std::unique_ptr<Object3D>& text : textoObjects_) {
		text->Draw();
	}


	//ポーズメニュー
	pauseMenu->Draw();

#pragma endregion


#pragma region スプライト描画
	//Spriteの描画準備。spriteの描画に共通のグラフィックスコマンドを積む
	SpriteCommon::GetInstance()->CommonDraw();

	// UI
	uIController_->Draw();
	uITitle_->Draw();
	uIX_->Draw();
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

	// (右に移動)
	if ((rightStickX > stickThreshold || (continueMove && rightStickX > stickThreshold))
		&& currentIndex_ < MaxSelectIndex_ - 1 && !easingmoveFlag_ && !easingsceneFlag_|| Input::GetInstance()->TriggerKey(DIK_D)) {

		currentIndex_++;
		easingmoveFlag_ = true;
		startPos_ = stageObjects_[currentIndex_ - 1]->GetTranslate();  // 直前の位置
		endPos_ = stageObjects_[currentIndex_]->GetTranslate();        // 新しい位置
		easingProgress_ = 0.0f;  // イージング開始
		holdTimer_ = 0.0f; // 長押しリセット

		// プレイヤーを右に90度回転
		Rotate.y += 90.0f * (DirectX::XM_PI / 180.0f);
		Player_->SetRotate(Rotate);
	}

	// (左に移動)
	if ((rightStickX < -stickThreshold || (continueMove && rightStickX < -stickThreshold))
		&& currentIndex_ > 0 && !easingmoveFlag_ && !easingsceneFlag_ || Input::GetInstance()->TriggerKey(DIK_A)) {

		currentIndex_--;
		easingmoveFlag_ = true;
		startPos_ = stageObjects_[currentIndex_ + 1]->GetTranslate();  // 直前の位置
		endPos_ = stageObjects_[currentIndex_]->GetTranslate();        // 新しい位置
		easingProgress_ = 0.0f;  // イージング開始
		holdTimer_ = 0.0f; // 長押しリセット

		// プレイヤーを左に-90度回転
		Rotate.y -= 90.0f * (DirectX::XM_PI / 180.0f);
		Player_->SetRotate(Rotate);
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
		newPos.y = -2.0f;

		// メインオブジェクトを移動
		Player_->SetTranslate(newPos);

		// カメラが "main" のときだけカメラの位置更新
		Camera* activeCam = CameraManager::GetInstans()->GetActiveCamera();
		if (activeCam == CameraManager::GetInstans()->GetCamera("maincam")) {
			FollowTargetposition.z = -30.0f;
			activeCam->SetFollowTarget(Player_, FollowTargetposition);
		}

		// イージング完了
		if (easingProgress_ >= 1.0f) {
			easingmoveFlag_ = false;

			// プレイヤーの回転を元に戻す
			Player_->SetRotate(Vector3(0.0f, -90.0f * (DirectX::XM_PI / 180.0f), 180.0f * (DirectX::XM_PI / 180.0f)));

			// スティックがニュートラルならフォロー位置をリセット
			if (fabs(rightStickX) < stickThreshold) {
				FollowTargetposition.z = -15.0f;
			}
			activeCam->SetFollowTarget(Player_, FollowTargetposition);
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
		// Aボタンが押されたときに開始
		if (Input::GetInstance()->TriggerGamePadButton(XINPUT_GAMEPAD_A) || Input::GetInstance()->TriggerKey(DIK_SPACE)) {
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

			/* 以前のコード

			

			// ステージによってシーン遷移
			if (currentIndex_ == 0) {                                  // Stage_01
				// シーン変更（必要に応じてシーン変更を実行）
				SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
			} else if (currentIndex_ == 1) {                           // Stage_02
				// シーン変更（必要に応じてシーン変更を実行）
				SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
			}
			*/
      
		}
	}
}