#include "GamePlayScene.h"
#include <ModelManager.h>
#include <TextureManager.h>
#include "Object3DCommon.h"
#include "SpriteCommon.h"
#include "ImGuiManager.h"
#ifdef _DEBUG
#include "imgui.h"
#endif // _DEBUG
#include "Input.h"
#include "TitleScene.h"
#include "CameraManager.h"
#include "ParticleMnager.h"
#include <Logger.h>

void GamePlayScene::Initialize()
{
	//カメラの生成
	camera1 = std::make_unique<Camera>();
	camera1->SetTranslate({ 12,17,-31 });//カメラの位置
	CameraManager::GetInstans()->AddCamera("maincam", camera1.get());

	//カメラの生成
	camera2 = std::make_unique<Camera>();
	camera2->SetTranslate({ 0,16.0f,-20, });//カメラの位置
	camera2->SetRotate({ 0.3f,0,0 });//カメラの向き
	CameraManager::GetInstans()->AddCamera("subcam", camera2.get());

	// デフォルトカメラを設定
	CameraManager::GetInstans()->SetActiveCamera("maincam");

	// モデル名
	const std::vector<std::string> modelNames = {
	 "axis","plane", "sphere", "cube", "jump", "playercharacter",
	"block", "Timer", "fallblock", "ncopyblock", "GoalBase", "GoreFag",
	"gall", "goalup", "nullBlock", "putTimer", "PlaySceneBackPlane",
	"Frame", "Pause", "resetnotice",
	"StageSelect/return", "StageSelect/title", "StageSelect/explanation","StageSelect/StageSelect",
	"tutorial/tutorial1",	"tutorial/tutorial2",	"tutorial/tutorial3",	"tutorial/tutorial4",	"tutorial/tutorial5",
	"tutorial/tutorial6",	"tutorial/tutorial7",	"tutorial/tutorial8",	"tutorial/tutorial9",	"tutorial/tutorial10",
	"tutorial/tutorial11",	"tutorial/tutorial12",	"tutorial/tutorial13",
	};
	// モデルの読み込み
	ModelManager::GetInstans()->LoadAllModels(modelNames);

	int stageIndex = SceneManager::GetInstance()->GetStageIndex();

	std::string stagePath;
	if (stageIndex >= 0 && stageIndex <= 12) {
		stagePath = "MapData/mapp" + std::to_string(stageIndex + 1) + ".csv";
	} else {
		stagePath = "MapData/mapp1.csv";
	}

	skydome_ = make_unique<Object3D>();
	skydome_->Initialize(Object3DCommon::GetInstance());
	skydome_->SetTranslate(Vector3{ 17.6f,15.28f,62.72f });
	skydome_->SetRotate(Vector3{ 0.0f,0.0f,-1.57f });
	skydome_->SetScale(Vector3{ 0.22f, 0.4f, 2.23f });
	skydome_->SetModel("PlaySceneBackPlane.obj");


	map = new Map;
	map->LoadMapChipCsv(stagePath);
	map->Initialize();

	//6,3
	//playerの生成
	player = std::make_unique<Player>();
	player->SetMapChipField(map);
	Vector3 playerPostion = map->GetPlayerStartPosition();
	player->Initialize(playerPostion); //プレイヤーの初期位置を設定
	player->SetDeathHeight(0.0f);

	const std::array<TutorialTextParam, TutorialTextType::Count> tutorialTextParams = { {
	{"tutorial/tutorial1.obj",  {0.5f, 0.5f, 0.5f}, {17.3f, 12.57f, 0.0f}, {8.5f, 21.44f, 1.0f}},
	{"tutorial/tutorial2.obj",  {0.5f, 0.5f, 0.5f}, {17.3f, 12.57f, 0.0f}, {15.3f, 21.4f, 0.75f}},
	{"tutorial/tutorial3.obj",  {0.5f, 0.5f, 0.5f}, {17.3f, 12.56f, 0.0f}, {8.5f, 21.45f, 1.0f} },
	{"tutorial/tutorial4.obj",  {0.5f, 0.5f, 0.5f}, {17.3f, 12.56f, 0.0f}, {15.46f, 21.4f, 0.69f} },
	{"tutorial/tutorial5.obj",  {0.75f, 0.5f, 0.5f}, {17.3f, 12.57f, 0.0f}, {12.24f, 21.45f, 1.0f} },
	{"tutorial/tutorial6.obj",  {0.5f, 0.5f, 0.5f}, {17.3f, 12.56f, 0.0f}, {8.5f, 21.4f, 1.0f} },
	{"tutorial/tutorial7.obj",  {0.5f, 0.5f, 0.5f}, {17.3f, 12.56f, 0.0f}, {15.46f, 21.4f, 1.0f} },
	{"tutorial/tutorial8.obj",  {0.5f, 0.5f, 0.5f}, {17.3f, 12.56f, 0.0f}, {12.46f, 21.4f, 1.0f} },
	{"tutorial/tutorial9.obj",  {0.5f, 0.5f, 0.5f}, {17.3f, 12.56f, 0.0f}, {12.46f, 21.4f, 1.0f} },
	{"tutorial/tutorial10.obj", {1.0f, 0.5f, 0.5f}, {17.3f, 12.56f, 0.0f}, {12.46f, 21.4f, 1.0f} },
	{"tutorial/tutorial11.obj", {1.0f, 0.5f, 0.5f}, {17.3f, 12.56f, 0.0f}, {12.46f, 21.4f, 1.0f} },
	{"tutorial/tutorial12.obj", {1.0f, 0.5f, 0.5f}, {17.3f, 12.56f, 0.0f}, {12.46f, 21.4f, 1.0f} },
	{"tutorial/tutorial13.obj", {1.0f, 0.5f, 0.5f}, {17.3f, 12.56f, 0.0f}, {12.46f, 19.5f, 1.0f} }} };

	// forでチュートリアルテキストの初期化
	for (int i = 0; i < TutorialTextType::Count; ++i) {	
		// 共通処理
		tutorialTexts[i] = std::make_unique<Object3D>();
		tutorialTexts[i]->Initialize(Object3DCommon::GetInstance());
		// 固有のパラメータを設定
		const TutorialTextParam& param = tutorialTextParams[i];
		tutorialTexts[i]->SetModel(param.modelPath);
		tutorialTexts[i]->SetScale(param.scale);
		tutorialTexts[i]->SetRotate(param.rotate);
		tutorialTexts[i]->SetTranslate(param.translate);
		// 共通処理
		tutorialTexts[i]->SetLighting(false);
		tutorialTexts[i]->SetIsTutorialActive(false);
	}

	//リセットお知らせ
	ResetNotice = std::make_unique<Object3D>();
	ResetNotice->Initialize(Object3DCommon::GetInstance());
	ResetNotice->SetModel("resetnotice.obj");
	ResetNotice->SetScale(Vector3(0.5f, 0.5f, 0.5f));
	ResetNotice->SetRotate(Vector3(17.3f, 12.56f, 0.0f));
	ResetNotice->SetTranslate(Vector3(12.46f, 23.25f, -1.0f));
	ResetNotice->SetLighting(false);

	const std::array<OperationSpriteParam, OperationTextCount> operationSpriteParams = { {
		{"Resources/xbox_stick_l.png", {43, 655}, {70, 70}},
		{"Resources/xbox_button_color_b.png", {416, 655}, {70, 70}},
		{"Resources/xbox_button_color_a.png", {228, 655}, {70, 70}},
		{"Resources/xbox_button_color_x.png", {628, 655}, {70, 70}},
		{"Resources/xbox_button_color_y.png", {792, 655}, {70, 70}},
		{"Resources/xbox_lb.png", {964, 655}, {70, 60}},
		{"Resources/xbox_rb.png", {1090, 655}, {70, 60}},
		{"Resources/idou.png", {135, 655}, {60, 60}},
		{"Resources/kirikae.png", {507, 655}, {100, 60}},
		{"Resources/toru.png", {712, 655}, {60, 60}},
		{"Resources/haiti.png", {882, 655}, {60, 60}},
		{"Resources/zyanpu.png", {326, 655}, {60, 60}},
		{"Resources/reset.png", {1167, 655}, {70, 60}},
		{"Resources/plus.png", {1031, 655}, {60, 60}}
		} };

	// forで操作説明テキストの初期化
	for (int i = 0; i < OperationTextCount; ++i) {
		// 共通処理
		operationTexts[i] = std::make_unique<Sprite>();
		// 固有のパラメータを設定
		const OperationSpriteParam& param = operationSpriteParams[i];
		operationTexts[i]->Initialize(SpriteCommon::GetInstance(), param.texturePath);
		operationTexts[i]->SetPosition(param.position);
		operationTexts[i]->SetSize(param.size);
	}

	for (uint32_t i = 0; i < 2; ++i) {
		std::unique_ptr<Sprite> newSprite = std::make_unique<Sprite>();
		if (i == 0) {
			newSprite->Initialize(SpriteCommon::GetInstance(), "Resources/Pause.png");
			newSprite->SetPosition(Vector2(1050.0f, 15.0f));
			newSprite->SetSize(Vector2(150, 50));
		} else if (i == 1) {
			newSprite->Initialize(SpriteCommon::GetInstance(), "Resources/xbox_button_menu.png");
			newSprite->SetPosition(Vector2(1210.0f, 5.0f));
			newSprite->SetSize(Vector2(70, 70));
		}
		newSprite->setColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		pauseui.push_back(std::move(newSprite));
	}

	//ブロックのスプライト
	nCopySprite = std::make_unique<Sprite>();
	nCopySprite->Initialize(SpriteCommon::GetInstance(), "Resources/ncopy.png");
	nCopySprite->SetPosition(Vector2(275, 121));
	nCopySprite->SetSize(Vector2(45, 45));

	jumpSprite = std::make_unique<Sprite>();
	jumpSprite->Initialize(SpriteCommon::GetInstance(), "Resources/jumpsprite.png");
	jumpSprite->SetPosition(Vector2(276, 214));
	jumpSprite->SetSize(Vector2(45, 45));

	timerSprite = std::make_unique<Sprite>();
	timerSprite->Initialize(SpriteCommon::GetInstance(), "Resources/timersprite.png");
	timerSprite->SetPosition(Vector2(275, 121));
	timerSprite->SetSize(Vector2(45, 45));

	//リセットメーターのスプライト
	resetMeter = std::make_unique<Sprite>();
	resetMeter->Initialize(SpriteCommon::GetInstance(), "Resources/resetmeter.png");
	resetMeter->SetPosition(Vector2(325, 70));
	resetMeter->SetSize(Vector2(200, 45));

	//フォローカメラ設定
	CameraManager::GetInstans()->GetCamera("maincam")->SetFollowTarget(player->GetObject3D(), {0, 0, -15});

	CameraManager::GetInstans()->GetCamera("maincam")->SetFollowMode(false);

	// ゲームカメラの生成
	photoCamera = new PhotoCamera;
	photoCamera->Initialize(map);

	//ポーズメニュー
	pauseMenu = std::make_unique<PauseMenu>();
	pauseMenu->Initialize(Object3DCommon::GetInstance(), PauseType::GamePlayScene);
	pauseMenu->SetCamera(CameraManager::GetInstans()->GetCamera("maincam"));

	fadeManager_.Initialize("Resources/white.png");
	fadeManager_.StartFadeIn();

	ParticleMnager::GetInstance()->CreateParticleGroup("Goal", "Resources/white.png", "block.obj");
	ParticleMnager::GetInstance()->CreateParticleGroup("Player", "Resources/white.png", "block.obj");

	// パーティクル発生器
	emitter_ = new ParticleEmitter(
		{ 0.0f,0.0f,0.0f },
		3.0f,
		0.0f,
		6,
		"Goal"
	);
	// パーティクルの位置
	Vector3 start = map->FindMapChipPosition(MapChipType::kGoalDown);
	emitter_->SetPosition(start);

	playeremitter_ = new ParticleEmitter(
		{ 0.0f,0.0f,0.0f },
		5.0f,
		0.0f,
		1,
		"Player"
	);
	playeroffset = { 0.0f,0.0f,0.0f };
}

void GamePlayScene::Finalize()
{
	CameraManager::GetInstans()->RemoveCamera("maincam");
	CameraManager::GetInstans()->RemoveCamera("subcam");
	CameraManager::GetInstans()->Finalize();

	map->Finalize();
	delete map;

	//delete gameCamera_;
	photoCamera->Finalize();
	delete photoCamera;

	delete emitter_;
	delete playeremitter_;

}

void GamePlayScene::Update()
{

	//ポーズ画面が出ている間は停止
	if (!pauseMenu->IsPaused()) {
		// フェード更新
		fadeManager_.Update();

		//ゲームの経過時間
		if (tutorial8) {
			elapsedTime += deltaTime;

			if (elapsedTime >= afterseconds) {
				secondspassed = true;
			}
		}
		//カメラの更新
		CameraManager::GetInstans()->GetActiveCamera()->Update();

		// 天球の更新
		//skydomerotate += 0.0f;
		//skydome_->SetRotate(Vector3{ 0.0f,0.0f,skydomerotate });
		skydome_->Update();

		// ゲームカメラ更新処理
		//gameCamera_->Update();
		photoCamera->Update(map, player->GetCameraMode());
		// マップの更新
		map->Update(player->GetCameraMode());
		//プレイヤーの更新
		player->Update();

		// プレイヤーが右に移動中
		if (player->GetPrayerMoveRight()) {
			playeroffset = { -0.3f,0.0f,0.0f };
			playeremitter_->SetPosition(player->GetTranslate() + playeroffset);
			// 左方向に設定
			playeremitter_->SetisRight(false);
			// プレイヤーのパーティクルを発生させる
			playeremitter_->PlayerEmit();
		}

		// プレイヤーが左に移動中
		if (player->GetPrayerMoveLeft()) {
			playeroffset = { 0.3f,0.0f,0.0f };
			playeremitter_->SetPosition(player->GetTranslate() + playeroffset);
			// 右方向に設定
			playeremitter_->SetisRight(true);
			// プレイヤーのパーティクルを発生させる
			playeremitter_->PlayerEmit();
		}

		playeremitter_->SetPosition(player->GetTranslate() + playeroffset);
		// パーティクルの更新
		playeremitter_->Update();

		//チュートリアル表示制御map2
		if (SceneManager::GetInstance()->GetStageIndex() == 1) {
			if (!tutorial9) {
				tutorialTexts[TutorialTextType::Text9]->SetIsTutorialActive(true);
				tutorial9 = true;
			}
		}

		if (player->GetCameraMode()) {
			// カメラモード時パーティクル解除
			player->SetPrayerMoveRight(false);
			player->SetPrayerMoveLeft(false);
		}

		if (player->GetCheckGoal() && !isfadesense_) {

			// クリアパーティクル発生
			emitter_->Emit();
			// フェードアウト開始
			fadeManager_.StartFadeOut();
			isfadesense_ = true;  // 一度だけ行う
		}

		if (isfadesense_) {
			CameraManager::GetInstans()->GetCamera("maincam")->SetFollowTarget(player->GetObject3D(), { 0,0, -7.0f });
			CameraManager::GetInstans()->GetCamera("maincam")->SetFollowMode(true);

			// クリアパーティクル開始
			emitter_->Update();
		}

		if (fadeManager_.IsFadeOutFinished()) {
			// シーン切り替え
			SceneManager::GetInstance()->ChangeScene("GAMECLEAR");
		}

		//チュートリアル表示制御//map1
		if (SceneManager::GetInstance()->GetStageIndex() == 0) {

			if (!tutorial1_2) {
				// text1,2を表示
				tutorialTexts[TutorialTextType::Text1]->SetIsTutorialActive(true);
				tutorialTexts[TutorialTextType::Text2]->SetIsTutorialActive(true);
				tutorial1_2 = true;
			}

			if (photoCamera->HasStarted() && !tutorial3_4) {
				tutorialTexts[TutorialTextType::Text1]->SetIsTutorialActive(false);
				tutorialTexts[TutorialTextType::Text2]->SetIsTutorialActive(false);
				tutorialTexts[TutorialTextType::Text3]->SetIsTutorialActive(true);
				tutorialTexts[TutorialTextType::Text4]->SetIsTutorialActive(true);
				tutorial3_4 = true;
			}

			if (photoCamera->HasMoved() && !tutorial5) {
				tutorialTexts[TutorialTextType::Text3]->SetIsTutorialActive(false);
				tutorialTexts[TutorialTextType::Text4]->SetIsTutorialActive(false);
				tutorialTexts[TutorialTextType::Text5]->SetIsTutorialActive(true);
				tutorial5 = true;
			}

			if (photoCamera->isFirstCopied && !tutorial6_7) {
				tutorialTexts[TutorialTextType::Text5]->SetIsTutorialActive(false);
				tutorialTexts[TutorialTextType::Text6]->SetIsTutorialActive(true);
				tutorialTexts[TutorialTextType::Text7]->SetIsTutorialActive(true);
				tutorial6_7 = true;
			}
			if (photoCamera->isFirstPasted && !tutorial8) {
				tutorialTexts[TutorialTextType::Text6]->SetIsTutorialActive(false);
				tutorialTexts[TutorialTextType::Text7]->SetIsTutorialActive(false);
				tutorialTexts[TutorialTextType::Text8]->SetIsTutorialActive(true);
				tutorial8 = true;
			}

			if (tutorial8 && secondspassed) {
				tutorialTexts[TutorialTextType::Text8]->SetIsTutorialActive(false);
				tutorialTexts[TutorialTextType::Text10]->SetIsTutorialActive(true);
				tutorial10 = true;
			}

		}

	} else {
		player->SetPrayerMoveRight(false);
		player->SetPrayerMoveLeft(false);
	}

	//チュートリアル表示制御map2
	if (SceneManager::GetInstance()->GetStageIndex() == 1) {
		if (!tutorial9) {
			tutorialTexts[TutorialTextType::Text9]->SetIsTutorialActive(true);
			tutorial9 = true;
		}
	}

	//チュートリアル表示制御map3
	if (SceneManager::GetInstance()->GetStageIndex() == 2) {
		if (!tutorial11) {
			tutorialTexts[TutorialTextType::Text11]->SetIsTutorialActive(true);
			tutorial11 = true;
		}
		nCopySprite->Update();
	}

	//チュートリアル表示制御map9
	if (SceneManager::GetInstance()->GetStageIndex() == 8) {
		tutorialTexts[TutorialTextType::Text12]->SetIsTutorialActive(true);
		tutorial12 = true;
		timerSprite->Update();
	}

	//チュートリアル表示制御map7
	if (SceneManager::GetInstance()->GetStageIndex() == 6) {
		tutorialTexts[TutorialTextType::Text13]->SetIsTutorialActive(true);
		tutorial13 = true;
		jumpSprite->Update();
	}

	//チュートリアルテキストの更新
	for (int i = 0; i < TutorialTextType::Count; ++i) {
		tutorialTexts[i]->Update();
	}

	// 操作説明テキストの更新
	if (!photoCamera->GetCameraMode()) {
		// 通常モードのときに表示するスプライト
		const std::vector<OperationTextType> visibleSprites = {
			StickL, ButtonB, ButtonA,
			LB, RB,
			Idou, Kirikae, Zyanpu,
			Reset, Plus
		};

		for (auto type : visibleSprites) {
			if (operationTexts[type]) {
				operationTexts[type]->Update();
			}
		}
	} else {
		// カメラモードのときに表示するスプライト
		const std::vector<OperationTextType> visibleSprites = {
			StickL, ButtonB, ButtonX, ButtonY,
			LB, RB,
			Idou, Kirikae, Toru, Haiti,
			Reset, Plus
		};

		for (auto type : visibleSprites) {
			if (operationTexts[type]) {
				operationTexts[type]->Update();
			}
		}
	}

	//mode切り替え
	photoCamera->SetcameraMode(player->GetCameraMode());


	for (std::unique_ptr<Sprite>& Uitext : pauseui) {
		Uitext->Update();
	}

	// ポーズ
	if (!player->GetCheckGoal()) {
		pauseMenu->Update();
	}

	//リセット
	GameReset();

	resetMeter->Update();
	ResetNotice->Update();

	DrawImgui();
}


void GamePlayScene::Draw()
{
	SpriteCommon::GetInstance()->CommonDraw();

#pragma region 3Dオブジェクト描画

	//3dオブジェクトの描画準備。3Dオブジェクトの描画に共通のグラフィックスコマンドを積む
	Object3DCommon::GetInstance()->CommonDraw();

	// 天球の描画
	skydome_->Draw();

	// ゲームカメラ
	//gameCamera_->Draw();
	photoCamera->Draw3DObject();
	////プレイヤー
	player->Draw();

	//チュートリアルテキストの描画
	for (int i = 0; i < TutorialTextType::Count; ++i) {
		tutorialTexts[i]->Draw();
	}

	//リセットお知らせ
	if (holdTime > 0.0f) {
		ResetNotice->Draw();
	}

	map->Draw();

	//ポーズメニュー
	pauseMenu->Draw();

	ParticleMnager::GetInstance()->Draw();

#pragma endregion

#pragma region スプライト描画
	//Spriteの描画準備。spriteの描画に共通のグラフィックスコマンドを積む
	SpriteCommon::GetInstance()->CommonDraw();

	// 操作説明テキストの更新
	if (!photoCamera->GetCameraMode()) {
		// 通常モードのときに表示するスプライト
		const std::vector<OperationTextType> visibleSprites = {
			StickL, ButtonB, ButtonA,
			LB, RB,
			Idou, Kirikae, Zyanpu,
			Reset, Plus
		};

		for (auto type : visibleSprites) {
			if (operationTexts[type]) {
				operationTexts[type]->Draw();
			}
		}
	} else {
		// カメラモードのときに表示するスプライト
		const std::vector<OperationTextType> visibleSprites = {
			StickL, ButtonB, ButtonX, ButtonY,
			LB, RB,
			Idou, Kirikae, Toru, Haiti,
			Reset, Plus
		};

		for (auto type : visibleSprites) {
			if (operationTexts[type]) {
				operationTexts[type]->Draw();
			}
		}
	}

	for (std::unique_ptr<Sprite>& Uitext : pauseui) {
		Uitext->Draw();
	}

	//リセットメーター描画
	if (holdTime > 0.0f) {
		resetMeter->Draw();
	}

	//チュートリアル表示制御map9
	if (SceneManager::GetInstance()->GetStageIndex() == 8) {

		timerSprite->Draw();
	}

	//チュートリアル表示制御map9
	if (SceneManager::GetInstance()->GetStageIndex() == 6) {

		jumpSprite->Draw();
	}

	nCopySprite->Draw();

	// フォトカメラ内のスプライト描画
	photoCamera->DrawSprite();

	// フェード描画
	fadeManager_.Draw();

#pragma endregion
}

//リセット
void GamePlayScene::GameReset() {
	if (
#ifdef _DEBUG
		Input::GetInstance()->PushKey(DIK_R) ||
#endif// _DEBUG
		Input::GetInstance()->PushGamePadButton(XINPUT_GAMEPAD_LEFT_SHOULDER) &&
		Input::GetInstance()->PushGamePadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER)) {

		holdTime += deltaTime;

		//メーターの進み具合
		float progress = min(holdTime / holdDuration, 1.0f);
		float maxWidth = 600.0f;
		float meterWidth = maxWidth * progress;

		resetMeter->SetSize(Vector2(meterWidth, 45));

		if (holdTime >= holdDuration) {
			holdTime = 0.0f;

			int stageIndex = SceneManager::GetInstance()->GetStageIndex();
			SceneManager::GetInstance()->SetStageIndex(stageIndex);
			SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
		}

	} else {
		//離されたらタイマーをリセット
		holdTime = 0.0f;
	}
}

void GamePlayScene::DrawImgui()
{

#ifdef _DEBUG
	ImGui::Begin("Back");

	// Transform構造体を直接編集
	Transform skydomeTransform = skydome_->GetTransform();
	bool changed = false;

	changed |= ImGui::DragFloat3("Skydome Scale", &skydomeTransform.scale.x, 0.01f);
	changed |= ImGui::DragFloat3("Skydome Rotate", &skydomeTransform.rotate.x, 0.01f);
	changed |= ImGui::DragFloat3("Skydome Position", &skydomeTransform.translate.x, 0.01f);

	if (changed) {
		skydome_->SetTransform(skydomeTransform);
	}

	ImGui::End();


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


		//プレイヤーディレクれくしょなるライト
		DirectionalLight directionalLight = player->GetObject3D()->GetDirectionalLight();
		if (ImGui::DragFloat3("Player Directional Light Direction", &directionalLight.direction.x, 0.01f)) {
			player->GetObject3D()->SetDirectionalLightDirection(directionalLight.direction);
		}

		// チュートリアルテキストの表示制御
		for (int i = 0; i < TutorialTextType::Count; ++i) {
			if (!tutorialTexts[i]) continue;

			Transform transform = tutorialTexts[i]->GetTransform();
			
			// ラベル名を作る
			std::string labelPrefix = "text" + std::to_string(i + 1);
			
			// スケール・回転・位置を編集
			bool changed = false;
			changed |= ImGui::DragFloat3((labelPrefix + " scale").c_str(), &transform.scale.x, 0.01f);
			changed |= ImGui::DragFloat3((labelPrefix + " rotate").c_str(), &transform.rotate.x, 0.01f);
			changed |= ImGui::DragFloat3((labelPrefix + " translate").c_str(), &transform.translate.x, 0.01f);

			if (changed) {
				tutorialTexts[i]->SetTransform(transform);
			}
		}		

		//UI

		Vector2 ncopy = nCopySprite->GetPosition();
		Vector2 ncopysize = nCopySprite->GetSize();
		Vector2 resetmeter = resetMeter->GetPosition();
		Vector2 resetmetersize = resetMeter->GetSize();
		Vector2 timer = timerSprite->GetPosition();
		Vector2 timersize = timerSprite->GetSize();
		Vector2 jump = jumpSprite->GetPosition();
		Vector2 jumpsize = jumpSprite->GetSize();

		if (ImGui::DragFloat2("ncopytranslate", &ncopy.x), 0.01f) {
			nCopySprite->SetPosition(ncopy);
		}
		if (ImGui::DragFloat2("ncopysize", &ncopysize.x), 0.01f) {
			nCopySprite->SetSize(ncopysize);
		}
		if (ImGui::DragFloat2("resetMetertranslate", &resetmeter.x), 0.01f) {
			resetMeter->SetPosition(resetmeter);
		}
		if (ImGui::DragFloat2("resetMetersize", &resetmetersize.x), 0.01f) {
			resetMeter->SetSize(resetmetersize);
		}
		if (ImGui::DragFloat2("timertranslate", &timer.x), 0.01f) {
			timerSprite->SetPosition(timer);
		}
		if (ImGui::DragFloat2("timersize", &timersize.x), 0.01f) {
			timerSprite->SetSize(timersize);
		}
		if (ImGui::DragFloat2("jumptranslate", &jump.x), 0.01f) {
			jumpSprite->SetPosition(jump);
		}
		if (ImGui::DragFloat2("jumpMetersize", &jumpsize.x), 0.01f) {
			jumpSprite->SetSize(jumpsize);
		}
	}

#endif // _DEBUG
}

bool GamePlayScene::GetCameraMode()
{
	if (player) {
		return player->GetCameraMode();
	}
	return false;
}