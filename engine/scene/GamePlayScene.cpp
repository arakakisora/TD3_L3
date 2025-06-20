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
	object3DPlayer = new Object3D();

	Vector3 playerPostion = map->GetPlayerStartPosition();

	object3DPlayer->Initialize(Object3DCommon::GetInstance());

	object3DPlayer->SetModel("playercharacter.obj");
	object3DPlayer->SetScale(Vector3{ 1.0f,1.0f,1.0f });
	object3DPlayer->SetLighting(true);
	object3DPlayer->SetDirectionalLightEnable(true);
	object3DPlayer->SetDirectionalLightDirection({ -1.3f,-1.82f,-4.77f });

	player->SetMapChipField(map);
	player->Initialize(object3DPlayer, playerPostion);
	player->SetDeathHeight(0.0f);

	//リセットお知らせ
	ResetNotice = std::make_unique<Object3D>();
	ResetNotice->Initialize(Object3DCommon::GetInstance());
	ResetNotice->SetModel("resetnotice.obj");
	ResetNotice->SetScale(Vector3(0.5f, 0.5f, 0.5f));
	ResetNotice->SetRotate(Vector3(17.3f, 12.56f, 0.0f));
	ResetNotice->SetTranslate(Vector3(12.46f, 23.25f, -1.0f));
	ResetNotice->SetLighting(false);

	OperationtextStickL = std::make_unique<Sprite>();
	OperationtextStickL->Initialize(SpriteCommon::GetInstance(), "Resources/xbox_stick_l.png");
	OperationtextStickL->SetPosition(Vector2(43, 655));
	OperationtextStickL->SetSize(Vector2(70, 70));

	OperationtextButtonB = std::make_unique<Sprite>();
	OperationtextButtonB->Initialize(SpriteCommon::GetInstance(), "Resources/xbox_button_color_b.png");
	OperationtextButtonB->SetPosition(Vector2(416, 655));
	OperationtextButtonB->SetSize(Vector2(70, 70));

	OperationtextButtonA = std::make_unique<Sprite>();
	OperationtextButtonA->Initialize(SpriteCommon::GetInstance(), "Resources/xbox_button_color_a.png");
	OperationtextButtonA->SetPosition(Vector2(228, 655));
	OperationtextButtonA->SetSize(Vector2(70, 70));

	OperationtextX = std::make_unique<Sprite>();
	OperationtextX->Initialize(SpriteCommon::GetInstance(), "Resources/xbox_button_color_x.png");
	OperationtextX->SetPosition(Vector2(628, 655));
	OperationtextX->SetSize(Vector2(70, 70));

	OperationtextY = std::make_unique<Sprite>();
	OperationtextY->Initialize(SpriteCommon::GetInstance(), "Resources/xbox_button_color_y.png");
	OperationtextY->SetPosition(Vector2(792, 655));
	OperationtextY->SetSize(Vector2(70, 70));

	OperationtextLB = std::make_unique<Sprite>();
	OperationtextLB->Initialize(SpriteCommon::GetInstance(), "Resources/xbox_lb.png");
	OperationtextLB->SetPosition(Vector2(964, 655));
	OperationtextLB->SetSize(Vector2(70, 60));

	OperationtextRB = std::make_unique<Sprite>();
	OperationtextRB->Initialize(SpriteCommon::GetInstance(), "Resources/xbox_rb.png");
	OperationtextRB->SetPosition(Vector2(1090, 655));
	OperationtextRB->SetSize(Vector2(70, 60));

	OperationtextIdou = std::make_unique<Sprite>();
	OperationtextIdou->Initialize(SpriteCommon::GetInstance(), "Resources/idou.png");
	OperationtextIdou->SetPosition(Vector2(135, 655));
	OperationtextIdou->SetSize(Vector2(60, 60));

	OperationtextKrikae = std::make_unique<Sprite>();
	OperationtextKrikae->Initialize(SpriteCommon::GetInstance(), "Resources/kirikae.png");
	OperationtextKrikae->SetPosition(Vector2(507, 655));
	OperationtextKrikae->SetSize(Vector2(100, 60));

	OperationtextToru = std::make_unique<Sprite>();
	OperationtextToru->Initialize(SpriteCommon::GetInstance(), "Resources/toru.png");
	OperationtextToru->SetPosition(Vector2(712, 655));
	OperationtextToru->SetSize(Vector2(60, 60));

	OperationtextHaiti = std::make_unique<Sprite>();
	OperationtextHaiti->Initialize(SpriteCommon::GetInstance(), "Resources/haiti.png");
	OperationtextHaiti->SetPosition(Vector2(882, 655));
	OperationtextHaiti->SetSize(Vector2(60, 60));

	OperationtextZyanpu = std::make_unique<Sprite>();
	OperationtextZyanpu->Initialize(SpriteCommon::GetInstance(), "Resources/zyanpu.png");
	OperationtextZyanpu->SetPosition(Vector2(326, 655));
	OperationtextZyanpu->SetSize(Vector2(60, 60));

	OperationtextReset = std::make_unique<Sprite>();
	OperationtextReset->Initialize(SpriteCommon::GetInstance(), "Resources/reset.png");
	OperationtextReset->SetPosition(Vector2(1167, 655));
	OperationtextReset->SetSize(Vector2(70, 60));

	OperationtextPlus = std::make_unique<Sprite>();
	OperationtextPlus->Initialize(SpriteCommon::GetInstance(), "Resources/plus.png");
	OperationtextPlus->SetPosition(Vector2(1031, 655));
	OperationtextPlus->SetSize(Vector2(60, 60));

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

	//リセットメーターのスプライト
	resetMeter = std::make_unique<Sprite>();
	resetMeter->Initialize(SpriteCommon::GetInstance(), "Resources/resetmeter.png");
	resetMeter->SetPosition(Vector2(325, 70));
	resetMeter->SetSize(Vector2(200, 45));

	//フォローカメラ設定
	CameraManager::GetInstans()->GetCamera("maincam")->SetFollowTarget(object3DPlayer, { 0, 0, -15 });

	CameraManager::GetInstans()->GetCamera("maincam")->SetFollowMode(false);

	// ゲームカメラの生成
	photoCamera = std::make_unique<PhotoCamera>();
	photoCamera->Initialize(map);

	//チュートリアル
	tutorial = std::make_unique<Tutorial>();
	tutorial->Initialize();
	tutorial->SetPhotoCamera(photoCamera.get());

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

	delete object3DPlayer;

	//delete gameCamera_;
	photoCamera->Finalize();


	delete emitter_;
	delete playeremitter_;

}

void GamePlayScene::Update()
{

	//ポーズ画面が出ている間は停止
	if (!pauseMenu->IsPaused()) {
		// フェード更新
		fadeManager_.Update();

		
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
			CameraManager::GetInstans()->GetCamera("maincam")->SetFollowTarget(object3DPlayer, { 0,0, -7.0f });
			CameraManager::GetInstans()->GetCamera("maincam")->SetFollowMode(true);

			// クリアパーティクル開始
			emitter_->Update();
		}

		if (fadeManager_.IsFadeOutFinished()) {
			// シーン切り替え
			SceneManager::GetInstance()->ChangeScene("GAMECLEAR");
		}

	} else {
		player->SetPrayerMoveRight(false);
		player->SetPrayerMoveLeft(false);
	}

	tutorial->Update();

	//ui

	if (!photoCamera->GetCameraMode()) {
		OperationtextStickL->Update();
		OperationtextButtonB->Update();
		OperationtextButtonA->Update();
		OperationtextLB->Update();
		OperationtextRB->Update();
		OperationtextIdou->Update();
		OperationtextKrikae->Update();
		OperationtextZyanpu->Update();
		OperationtextReset->Update();
		OperationtextPlus->Update();
	}
	if (photoCamera->GetCameraMode()) {
		OperationtextStickL->Update();
		OperationtextButtonB->Update();
		OperationtextX->Update();
		OperationtextY->Update();
		OperationtextLB->Update();
		OperationtextRB->Update();
		OperationtextIdou->Update();
		OperationtextKrikae->Update();
		OperationtextToru->Update();
		OperationtextHaiti->Update();
		OperationtextReset->Update();
		OperationtextPlus->Update();
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
	Reset();

	resetMeter->Update();
	ResetNotice->Update();

	//DrawImgui();
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

	//チュートリアルテキスト
	tutorial->TextDraw();
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

	//ui
	if (!photoCamera->GetCameraMode()) {
		OperationtextStickL->Draw();
		OperationtextButtonB->Draw();
		OperationtextButtonA->Draw();
		OperationtextLB->Draw();
		OperationtextRB->Draw();
		OperationtextIdou->Draw();
		OperationtextKrikae->Draw();
		OperationtextZyanpu->Draw();
		OperationtextReset->Draw();
		OperationtextPlus->Draw();
	}
	if (photoCamera->GetCameraMode()) {
		OperationtextStickL->Draw();
		OperationtextButtonB->Draw();
		OperationtextLB->Draw();
		OperationtextRB->Draw();
		OperationtextX->Draw();
		OperationtextY->Draw();
		OperationtextIdou->Draw();
		OperationtextKrikae->Draw();
		OperationtextToru->Draw();
		OperationtextHaiti->Draw();
		OperationtextReset->Draw();
		OperationtextPlus->Draw();
	}

	for (std::unique_ptr<Sprite>& Uitext : pauseui) {
		Uitext->Draw();
	}

	//リセットメーター描画
	if (holdTime > 0.0f) {
		resetMeter->Draw();
	}

	tutorial->SpriteDraw();

	// フォトカメラ内のスプライト描画
	photoCamera->DrawSprite();

	// フェード描画
	fadeManager_.Draw();

#pragma endregion
}

//リセット
void GamePlayScene::Reset() {
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
		DirectionalLight directionalLight = object3DPlayer->GetDirectionalLight();
		if (ImGui::DragFloat3("Player Directional Light Direction", &directionalLight.direction.x, 0.01f)) {
			object3DPlayer->SetDirectionalLightDirection(directionalLight.direction);
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