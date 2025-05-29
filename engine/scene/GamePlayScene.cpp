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

	//モデルの読み込み
	ModelManager::GetInstans()->LoadModel("axis.obj");
	ModelManager::GetInstans()->LoadModel("plane.obj");
	ModelManager::GetInstans()->LoadModel("sphere.obj");
	ModelManager::GetInstans()->LoadModel("cube.obj");
	ModelManager::GetInstans()->LoadModel("jump.obj");

	//ModelManager::GetInstans()->LoadModel("Player.obj");
	ModelManager::GetInstans()->LoadModel("playercharacter.obj");

	// ブロック関連モデル
	ModelManager::GetInstans()->LoadModel("block.obj");
	ModelManager::GetInstans()->LoadModel("Timer.obj");
	ModelManager::GetInstans()->LoadModel("fallblock.obj");
	ModelManager::GetInstans()->LoadModel("ncopyblock.obj");
	ModelManager::GetInstans()->LoadModel("GoalBase.obj");
	ModelManager::GetInstans()->LoadModel("GoreFag.obj");
	ModelManager::GetInstans()->LoadModel("gall.obj");
	ModelManager::GetInstans()->LoadModel("goalup.obj");
	ModelManager::GetInstans()->LoadModel("nullBlock.obj");
	ModelManager::GetInstans()->LoadModel("putTimer.obj");
	// 天球モデル / 背景のプレーン
	ModelManager::GetInstans()->LoadModel("backPlane.obj");
	// フォトカメラフレーム
	ModelManager::GetInstans()->LoadModel("Frame.obj");

	//チュートリアルテキスト
	ModelManager::GetInstans()->LoadModel("tutorial/tutorial1.obj");
	ModelManager::GetInstans()->LoadModel("tutorial/tutorial2.obj");
	ModelManager::GetInstans()->LoadModel("tutorial/tutorial3.obj");
	ModelManager::GetInstans()->LoadModel("tutorial/tutorial4.obj");
	ModelManager::GetInstans()->LoadModel("tutorial/tutorial5.obj");
	ModelManager::GetInstans()->LoadModel("tutorial/tutorial6.obj");
	ModelManager::GetInstans()->LoadModel("tutorial/tutorial7.obj");
	ModelManager::GetInstans()->LoadModel("tutorial/tutorial8.obj");
	ModelManager::GetInstans()->LoadModel("tutorial/tutorial9.obj");
	ModelManager::GetInstans()->LoadModel("tutorial/tutorial10.obj");
	ModelManager::GetInstans()->LoadModel("tutorial/tutorial11.obj");
	ModelManager::GetInstans()->LoadModel("tutorial/tutorial12.obj");
	ModelManager::GetInstans()->LoadModel("tutorial/tutorial13.obj");

	// ポーズテキスト
	ModelManager::GetInstans()->LoadModel("Pause.obj");
	ModelManager::GetInstans()->LoadModel("StageSelect/return.obj");
	ModelManager::GetInstans()->LoadModel("StageSelect/title.obj");
	ModelManager::GetInstans()->LoadModel("StageSelect/explanation.obj");
	ModelManager::GetInstans()->LoadModel("StageSelect/StageSelect.obj");


	//操作説明UI
	/*
	std::vector<OperationText>spriteInfos = {
		{"Resources/xbox_stick_l.png",{90,655},{70,70}},
		{"Resources/xbox_button_color_b.png",{557,655},{70,70}},
		{"Resources/xbox_button_color_a.png",{319,655},{70,70}},
		{"Resources/xbox_lb.png",{826,655}, {70,70}},
		{"Resources/xbox_rb.png",{1039,655},{70,70}},
		{"Resources/idou.png",{203,655},{60,60}},
		{"Resources/kirikae.png",{672,655},{100,60}},
		{"Resources/toru.png",{938,655},{60,60}},
		{"Resources/haiti.png",{1143,655},{60,60}},
		{"Resources/zyanpu.png",{430,655},{60,60}},
	};
	*/
	int stageIndex = SceneManager::GetInstance()->GetStageIndex();

	std::string stagePath;
	switch (stageIndex) {
	case 0: stagePath = "MapData/mapp1.csv"; break;
	case 1: stagePath = "MapData/mapp2.csv"; break;
	case 2: stagePath = "MapData/mapp3.csv"; break;
	case 3: stagePath = "MapData/mapp4.csv"; break;
	case 4: stagePath = "MapData/mapp5.csv"; break;
	case 5: stagePath = "MapData/mapp6.csv"; break;
	case 6: stagePath = "MapData/mapp7.csv"; break;
	case 7: stagePath = "MapData/mapp8.csv"; break;
	case 8: stagePath = "MapData/mapp9.csv"; break;
	case 9: stagePath = "MapData/mapp10.csv"; break;
	}

	skydome_ = make_unique<Object3D>();
	skydome_->Initialize(Object3DCommon::GetInstance());
	skydome_->SetTranslate(Vector3{ 17.6f,16.67f,62.72f });
	skydome_->SetRotate(Vector3{ 0.0f,0.0f,-1.57f });
	skydome_->SetScale(Vector3{ 0.2f, 0.4f, 2.23f });
	skydome_->SetModel("backPlane.obj");


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

	//チュートリアルテキスト
	/*
	for (int i = 1; i <= 8; ++i) {
		tutorialTexts[i] = std::make_unique<Object3D>();
		tutorialTexts[i]->Initialize(Object3DCommon::GetInstance());
		tutorialTexts[i]->SetModel("tutorial/tutorial" + std::to_string(i) + ".obj");
		tutorialTexts[i]->SetScale(Vector3(0.5f, 0.5f, 0.5f));
		tutorialTexts[i]->SetRotate(Vector3(17.3f, 12.57f, 0.0f));
		tutorialTexts[i]->SetLighting(false);
		tutorialTexts[i]->SetIsTutorialActive(false);
	}
	tutorialTexts[1]->SetTranslate(Vector3(8.5f, 21.44f, 1.0f));
	tutorialTexts[2]->SetTranslate(Vector3(15.3f, 21.4f, 0.75f));
	tutorialTexts[3]->SetTranslate(Vector3(8.5f, 21.45f, 1.0f));
	tutorialTexts[4]->SetTranslate(Vector3(15.46f, 21.4f, 0.69f));
	tutorialTexts[5]->SetTranslate(Vector3(12.24f, 21.45f, 1.0f));
	tutorialTexts[6]->SetTranslate(Vector3(8.5f, 21.4f, 1.0f));
	tutorialTexts[7]->SetTranslate(Vector3(15.46f, 21.4f, 1.0f));
	tutorialTexts[8]->SetTranslate(Vector3(12.46f, 21.4f, 1.0f));
	*/
	Tutorialtext1 = std::make_unique<Object3D>();
	Tutorialtext1->Initialize(Object3DCommon::GetInstance());
	Tutorialtext1->SetModel("tutorial/tutorial1.obj");
	Tutorialtext1->SetScale(Vector3(0.5f, 0.5f, 0.5f));
	Tutorialtext1->SetRotate(Vector3(17.3f, 12.57f, 0.0f));
	Tutorialtext1->SetTranslate(Vector3(8.5f, 21.44f, 1.0f));
	Tutorialtext1->SetLighting(false);
	Tutorialtext1->SetIsTutorialActive(false);

	Tutorialtext2 = std::make_unique<Object3D>();
	Tutorialtext2->Initialize(Object3DCommon::GetInstance());
	Tutorialtext2->SetModel("tutorial/tutorial2.obj");
	Tutorialtext2->SetScale(Vector3(0.5f, 0.5f, 0.5f));
	Tutorialtext2->SetRotate(Vector3(17.3f, 12.57f, 0.0f));
	Tutorialtext2->SetTranslate(Vector3(15.3f, 21.4f, 0.75f));
	Tutorialtext2->SetLighting(false);
	Tutorialtext2->SetIsTutorialActive(false);

	Tutorialtext3 = std::make_unique<Object3D>();
	Tutorialtext3->Initialize(Object3DCommon::GetInstance());
	Tutorialtext3->SetModel("tutorial/tutorial3.obj");
	Tutorialtext3->SetScale(Vector3(0.5f, 0.5f, 0.5f));
	Tutorialtext3->SetRotate(Vector3(17.3f, 12.56f, 0.0f));
	Tutorialtext3->SetTranslate(Vector3(8.5f, 21.45f, 1.0f));
	Tutorialtext3->SetLighting(false);
	Tutorialtext3->SetIsTutorialActive(false);

	Tutorialtext4 = std::make_unique<Object3D>();
	Tutorialtext4->Initialize(Object3DCommon::GetInstance());
	Tutorialtext4->SetModel("tutorial/tutorial4.obj");
	Tutorialtext4->SetScale(Vector3(0.5f, 0.5f, 0.5f));
	Tutorialtext4->SetRotate(Vector3(17.3f, 12.56f, 0.0f));
	Tutorialtext4->SetTranslate(Vector3(15.46f, 21.4f, 0.69f));
	Tutorialtext4->SetLighting(false);
	Tutorialtext4->SetIsTutorialActive(false);

	Tutorialtext5 = std::make_unique<Object3D>();
	Tutorialtext5->Initialize(Object3DCommon::GetInstance());
	Tutorialtext5->SetModel("tutorial/tutorial5.obj");
	Tutorialtext5->SetScale(Vector3(0.75f, 0.5f, 0.5f));
	Tutorialtext5->SetRotate(Vector3(17.3f, 12.57f, 0.0f));
	Tutorialtext5->SetTranslate(Vector3(12.24f, 21.45f, 1.0f));
	Tutorialtext5->SetLighting(false);
	Tutorialtext5->SetIsTutorialActive(false);

	Tutorialtext6 = std::make_unique<Object3D>();
	Tutorialtext6->Initialize(Object3DCommon::GetInstance());
	Tutorialtext6->SetModel("tutorial/tutorial6.obj");
	Tutorialtext6->SetScale(Vector3(0.5f, 0.5f, 0.5f));
	Tutorialtext6->SetRotate(Vector3(17.3f, 12.56f, 0.0f));
	Tutorialtext6->SetTranslate(Vector3(8.5f, 21.4f, 1.0f));
	Tutorialtext6->SetLighting(false);
	Tutorialtext6->SetIsTutorialActive(false);

	Tutorialtext7 = std::make_unique<Object3D>();
	Tutorialtext7->Initialize(Object3DCommon::GetInstance());
	Tutorialtext7->SetModel("tutorial/tutorial7.obj");
	Tutorialtext7->SetScale(Vector3(0.5f, 0.5f, 0.5f));
	Tutorialtext7->SetRotate(Vector3(17.3f, 12.56f, 0.0f));
	Tutorialtext7->SetTranslate(Vector3(15.46f, 21.4f, 1.0f));
	Tutorialtext7->SetLighting(false);
	Tutorialtext7->SetIsTutorialActive(false);

	Tutorialtext8 = std::make_unique<Object3D>();
	Tutorialtext8->Initialize(Object3DCommon::GetInstance());
	Tutorialtext8->SetModel("tutorial/tutorial8.obj");
	Tutorialtext8->SetScale(Vector3(0.5f, 0.5f, 0.5f));
	Tutorialtext8->SetRotate(Vector3(17.3f, 12.56f, 0.0f));
	Tutorialtext8->SetTranslate(Vector3(12.46f, 21.4f, 1.0f));
	Tutorialtext8->SetLighting(false);
	Tutorialtext8->SetIsTutorialActive(false);

	Tutorialtext9 = std::make_unique<Object3D>();
	Tutorialtext9->Initialize(Object3DCommon::GetInstance());
	Tutorialtext9->SetModel("tutorial/tutorial9.obj");
	Tutorialtext9->SetScale(Vector3(0.5f, 0.5f, 0.5f));
	Tutorialtext9->SetRotate(Vector3(17.3f, 12.56f, 0.0f));
	Tutorialtext9->SetTranslate(Vector3(12.46f, 21.4f, 1.0f));
	Tutorialtext9->SetLighting(false);
	Tutorialtext9->SetIsTutorialActive(false);

	Tutorialtext10 = std::make_unique<Object3D>();
	Tutorialtext10->Initialize(Object3DCommon::GetInstance());
	Tutorialtext10->SetModel("tutorial/tutorial10.obj");
	Tutorialtext10->SetScale(Vector3(1.0f, 0.5f, 0.5f));
	Tutorialtext10->SetRotate(Vector3(17.3f, 12.56f, 0.0f));
	Tutorialtext10->SetTranslate(Vector3(12.46f, 21.4f, 1.0f));
	Tutorialtext10->SetLighting(false);
	Tutorialtext10->SetIsTutorialActive(false);

	Tutorialtext11 = std::make_unique<Object3D>();
	Tutorialtext11->Initialize(Object3DCommon::GetInstance());
	Tutorialtext11->SetModel("tutorial/tutorial11.obj");
	Tutorialtext11->SetScale(Vector3(1.0f, 0.5f, 0.5f));
	Tutorialtext11->SetRotate(Vector3(17.3f, 12.56f, 0.0f));
	Tutorialtext11->SetTranslate(Vector3(12.46f, 21.4f, 1.0f));
	Tutorialtext11->SetLighting(false);
	Tutorialtext11->SetIsTutorialActive(false);

	Tutorialtext12 = std::make_unique<Object3D>();
	Tutorialtext12->Initialize(Object3DCommon::GetInstance());
	Tutorialtext12->SetModel("tutorial/tutorial12.obj");
	Tutorialtext12->SetScale(Vector3(1.0f, 0.5f, 0.5f));
	Tutorialtext12->SetRotate(Vector3(17.3f, 12.56f, 0.0f));
	Tutorialtext12->SetTranslate(Vector3(12.46f, 21.4f, 1.0f));
	Tutorialtext12->SetLighting(false);
	Tutorialtext12->SetIsTutorialActive(false);

	Tutorialtext13 = std::make_unique<Object3D>();
	Tutorialtext13->Initialize(Object3DCommon::GetInstance());
	Tutorialtext13->SetModel("tutorial/tutorial13.obj");
	Tutorialtext13->SetScale(Vector3(1.0f, 0.5f, 0.5f));
	Tutorialtext13->SetRotate(Vector3(17.3f, 12.56f, 0.0f));
	Tutorialtext13->SetTranslate(Vector3(12.46f, 21.4f, 1.0f));
	Tutorialtext13->SetLighting(false);
	Tutorialtext13->SetIsTutorialActive(false);



	//操作説明UI
	/*
	operationTexts.clear();
	for (const auto& info : spriteInfos) {
		auto sprite = std::make_unique<Sprite>();
		sprite->Initialize(SpriteCommon::GetInstance(), info.texturePath);
		sprite->SetPosition(info.position);
		sprite->SetSize(info.size);
		operationTexts.emplace_back(std::move(sprite));
	}
	*/
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

	//ブロックのスプライト
	nCopySprite = std::make_unique<Sprite>();
	nCopySprite->Initialize(SpriteCommon::GetInstance(), "Resources/ncopy.png");
	nCopySprite->SetPosition(Vector2(275, 121));
	nCopySprite->SetSize(Vector2(45, 45));

	jumpSprite = std::make_unique<Sprite>();
	jumpSprite->Initialize(SpriteCommon::GetInstance(), "Resources/jumpsprite.png");
	jumpSprite->SetPosition(Vector2(275, 121));
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
	CameraManager::GetInstans()->GetCamera("maincam")->SetFollowTarget(object3DPlayer, { 0, 0, -15 });

	CameraManager::GetInstans()->GetCamera("maincam")->SetFollowMode(false);

	// ゲームカメラの生成
	//gameCamera_ = new ObjectCamera();
	//gameCamera_->Initialize(map);
	photoCamera = new PhotoCamera;
	photoCamera->Initialize(map);

	//ポーズメニュー
	pauseMenu = std::make_unique<PauseMenu>();
	pauseMenu->Initialize(Object3DCommon::GetInstance(), PauseType::GamePlayScene);
	pauseMenu->SetCamera(CameraManager::GetInstans()->GetCamera("maincam"));

	fadeManager_.Initialize("Resources/white.png");
	fadeManager_.StartFadeIn();


	ParticleMnager::GetInstance()->CreateParticleGroup("Goal", "Resources/block.png", "block.obj");
	ParticleMnager::GetInstance()->CreateParticleGroup("Player", "Resources/block.png", "block.obj");

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
				Tutorialtext9->SetIsTutorialActive(true);
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
			CameraManager::GetInstans()->GetCamera("maincam")->SetFollowTarget(object3DPlayer, { 0,0, -7.0f });
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
			/*

			if (!tutorial1_2) {
				tutorialTexts[1]->SetIsTutorialActive(true);
				tutorialTexts[2]->SetIsTutorialActive(true);
				tutorial1_2 = true;
			}

			if(photoCamera->HasStarted()&&!turorial3_4){
			   tutorialTexts[1]->SetIsTutorialActive(false);
			   tutorialTexts[2]->SetIsTutorialActive(false);
			   tutorialTexts[3]->SetIsTutorialActive(true);
			   tutorialTexts[4]->SetIsTutorialActive(true);
			   tutorial3_4 = true;
			   }

			if (photoCamera->HasMoved() && !tutorial5) {
				tutorialtext[3]->SetIsTutorialActive(false);
				tutorialtext[4]->SetIsTutorialActive(false);
				tutorialtext[5]->SetIsTutorialActive(true);
				tutorial5 = true;
			}

			if (photoCamera->isFirstCopied && !tutorial6_7) {
				tutorialtext[5]->SetIsTutorialActive(false);
				tutorialtext[6]->SetIsTutorialActive(true);
				tutorialtext[7]->SetIsTutorialActive(true);
				tutorial6_7 = true;
			}
			if (photoCamera->isFirstPasted && !tutorial8) {
				tutorialtext[6]->SetIsTutorialActive(false);
				tutorialtext[7]->SetIsTutorialActive(false);
				tutorialtext[8]->SetIsTutorialActive(true);
				tutorial8 = true;
			}

			for (auto& text : tutorialTexts) {
				if (text->GetIsTutorialActive()) {
					text->Update();
				}
			}
			*/

			if (!tutorial1_2) {
				// text1,2を表示
				Tutorialtext1->SetIsTutorialActive(true);
				Tutorialtext2->SetIsTutorialActive(true);
				tutorial1_2 = true;
			}

			if (photoCamera->HasStarted() && !tutorial3_4) {
				Tutorialtext1->SetIsTutorialActive(false);
				Tutorialtext2->SetIsTutorialActive(false);
				Tutorialtext3->SetIsTutorialActive(true);
				Tutorialtext4->SetIsTutorialActive(true);
				tutorial3_4 = true;
			}

			if (photoCamera->HasMoved() && !tutorial5) {
				Tutorialtext3->SetIsTutorialActive(false);
				Tutorialtext4->SetIsTutorialActive(false);
				Tutorialtext5->SetIsTutorialActive(true);
				tutorial5 = true;
			}

			if (photoCamera->isFirstCopied && !tutorial6_7) {
				Tutorialtext5->SetIsTutorialActive(false);
				Tutorialtext6->SetIsTutorialActive(true);
				Tutorialtext7->SetIsTutorialActive(true);
				tutorial6_7 = true;
			}
			if (photoCamera->isFirstPasted && !tutorial8) {
				Tutorialtext6->SetIsTutorialActive(false);
				Tutorialtext7->SetIsTutorialActive(false);
				Tutorialtext8->SetIsTutorialActive(true);
				tutorial8 = true;
			}

			if (tutorial8 && secondspassed) {
				Tutorialtext8->SetIsTutorialActive(false);
				Tutorialtext10->SetIsTutorialActive(true);
				tutorial10 = true;
			}

		}

	} else {
		player->SetPrayerMoveRight(false);
		player->SetPrayerMoveLeft(false);
	}

	//// プレイヤー用のパーティクルの位置を常に更新
	//Vector3 pos = player->GetTranslate();

	//// 右に移動中
	//if (player->GetPrayerMoveRight()) {
	//	// 左方向に設定
	//	playeremitter_->SetisRight(false);
	//	Vector3 offset = { -0.3f,0.0f,0.0f };
	//	playeremitter_->SetPosition(pos + offset);
	//	playeremitter_->PlayerEmit();
	//}

	//// 左に移動中
	//if (player->GetPrayerMoveLeft()) {
	//	// 右方向に設定
	//	playeremitter_->SetisRight(true);
	//	Vector3 offset = { 0.3f,0.0f,0.0f };
	//	playeremitter_->SetPosition(pos + offset);
	//	playeremitter_->PlayerEmit();
	//}

	//チュートリアル表示制御map2
	if (SceneManager::GetInstance()->GetStageIndex() == 1) {
		if (!tutorial9) {
			Tutorialtext9->SetIsTutorialActive(true);
			tutorial9 = true;
		}
	}

	//チュートリアル表示制御map3
	if (SceneManager::GetInstance()->GetStageIndex() == 2) {
		if (!tutorial11) {
			Tutorialtext11->SetIsTutorialActive(true);
			tutorial11 = true;
		}
		nCopySprite->Update();
	}

	if (Tutorialtext1->GetIsTutorialActive()) Tutorialtext1->Update();
	if (Tutorialtext2->GetIsTutorialActive()) Tutorialtext2->Update();
	if (Tutorialtext3->GetIsTutorialActive()) Tutorialtext3->Update();
	if (Tutorialtext4->GetIsTutorialActive()) Tutorialtext4->Update();
	if (Tutorialtext5->GetIsTutorialActive()) Tutorialtext5->Update();
	if (Tutorialtext6->GetIsTutorialActive()) Tutorialtext6->Update();
	if (Tutorialtext7->GetIsTutorialActive()) Tutorialtext7->Update();
	if (Tutorialtext8->GetIsTutorialActive())Tutorialtext8->Update();
	if (Tutorialtext9->GetIsTutorialActive())Tutorialtext9->Update();
	if (Tutorialtext10->GetIsTutorialActive())Tutorialtext10->Update();
	if (Tutorialtext11->GetIsTutorialActive())Tutorialtext11->Update();

	//ui
	/*
	if (!photoCamera->GetCameraMode()) {
		for (int i : {0, 1, 2, 5, 6, 9}) {
			operationTexts[i]->Update();
		}
		else {
			for (int i : {0, 1, 3, 4, 5, 6, 7, 8}) {
				operationTexts[i]->Update();
			}
		}
		*/
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
	if (Input::GetInstance()->PushGamePadButton(XINPUT_GAMEPAD_LEFT_SHOULDER) &&
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
	resetMeter->Update();

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

	//チュートリアルテキスト
	/*
	for (auto& text : tutorialTexts) {
		if (text->GetIsTutorialActive()) {
			text->Draw();
		}
	}
	*/

	if (Tutorialtext1->GetIsTutorialActive()) Tutorialtext1->Draw();
	if (Tutorialtext2->GetIsTutorialActive()) Tutorialtext2->Draw();
	if (Tutorialtext3->GetIsTutorialActive()) Tutorialtext3->Draw();
	if (Tutorialtext4->GetIsTutorialActive()) Tutorialtext4->Draw();
	if (Tutorialtext5->GetIsTutorialActive()) Tutorialtext5->Draw();
	if (Tutorialtext6->GetIsTutorialActive()) Tutorialtext6->Draw();
	if (Tutorialtext7->GetIsTutorialActive()) Tutorialtext7->Draw();
	if (Tutorialtext8->GetIsTutorialActive()) Tutorialtext8->Draw();
	if (Tutorialtext9->GetIsTutorialActive())Tutorialtext9->Draw();
	if (Tutorialtext10->GetIsTutorialActive())Tutorialtext10->Draw();
	if (Tutorialtext11->GetIsTutorialActive())Tutorialtext11->Draw();

	map->Draw();

	//ポーズメニュー
	pauseMenu->Draw();

	ParticleMnager::GetInstance()->Draw();

#pragma endregion

#pragma region スプライト描画
	//Spriteの描画準備。spriteの描画に共通のグラフィックスコマンドを積む
	SpriteCommon::GetInstance()->CommonDraw();

	//ui
	/*
	if (!photoCamera->GetCameraMode()) {
		for (int i : {0, 1, 2, 5, 6, 9}) {
			operationTexts[i]->Draw();
		}
	} else {
		for (int i : {0, 1, 3, 4, 5, 6, 7, 8}) {
			operationTexts[i]->Draw();
		}
	}
	*/
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

	nCopySprite->Draw();

	// フォトカメラ内のスプライト描画
	photoCamera->DrawSprite();

	// フェード描画
	fadeManager_.Draw();

#pragma endregion
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




		/*
		if (ImGui::CollapsingHeader("Tutorial Text Transforms")) {
			for (int i = 1; i <= tutorialTexts.size(); ++i) {
				std::string label = "text" + std::to_string(i);
				Transform t = tutorialTexts[i]->GetTransform();
				if (ImGui::DragFloat3((label + "scale").c_str(), &t.scale.x, 0.01f) ||
					ImGui::DragFloat3((label + "rotate").c_str(), &t.rotate.x, 0.01f) ||
					ImGui::DragFloat3((label + "translate").c_str(), &t.translate.x, 0.01f)) {
					tutorialTexts[i]->SetTransform(t);
				}
			}
		}
		*/
		Transform text = Tutorialtext1->GetTransform();
		Transform text2 = Tutorialtext2->GetTransform();
		Transform text3 = Tutorialtext3->GetTransform();
		Transform text4 = Tutorialtext4->GetTransform();
		Transform text5 = Tutorialtext5->GetTransform();
		Transform text6 = Tutorialtext6->GetTransform();
		Transform text7 = Tutorialtext7->GetTransform();
		Transform text8 = Tutorialtext8->GetTransform();
		Transform text9 = Tutorialtext9->GetTransform();
		if (ImGui::DragFloat3("text1scale", &text.scale.x, 0.01f)) {
			Tutorialtext1->SetTransform(text);
		}
		if (ImGui::DragFloat3("text1rotate", &text.rotate.x, 0.01f)) {
			Tutorialtext1->SetTransform(text);
		}
		if (ImGui::DragFloat3("text1translate", &text.translate.x, 0.01f)) {
			Tutorialtext1->SetTransform(text);
		}
		if (ImGui::DragFloat3("text2scale", &text2.scale.x, 0.01f)) {
			Tutorialtext2->SetTransform(text2);
		}
		if (ImGui::DragFloat3("text2rotate", &text2.rotate.x, 0.01f)) {
			Tutorialtext2->SetTransform(text2);
		}
		if (ImGui::DragFloat3("text2translate", &text2.translate.x, 0.01f)) {
			Tutorialtext2->SetTransform(text2);
		}
		if (ImGui::DragFloat3("text3scale", &text3.scale.x, 0.01f)) {
			Tutorialtext3->SetTransform(text3);
		}
		if (ImGui::DragFloat3("text3rotate", &text3.rotate.x, 0.01f)) {
			Tutorialtext3->SetTransform(text3);
		}
		if (ImGui::DragFloat3("text3translate", &text3.translate.x, 0.01f)) {
			Tutorialtext3->SetTransform(text3);
		}
		if (ImGui::DragFloat3("text4scale", &text4.scale.x, 0.01f)) {
			Tutorialtext4->SetTransform(text4);
		}
		if (ImGui::DragFloat3("text4rotate", &text4.rotate.x, 0.01f)) {
			Tutorialtext4->SetTransform(text4);
		}
		if (ImGui::DragFloat3("text4translate", &text4.translate.x, 0.01f)) {
			Tutorialtext4->SetTransform(text4);
		}
		if (ImGui::DragFloat3("text5scale", &text5.scale.x, 0.01f)) {
			Tutorialtext5->SetTransform(text5);
		}
		if (ImGui::DragFloat3("text5rotate", &text5.rotate.x, 0.01f)) {
			Tutorialtext5->SetTransform(text5);
		}
		if (ImGui::DragFloat3("text5translate", &text5.translate.x, 0.01f)) {
			Tutorialtext5->SetTransform(text5);
		}
		if (ImGui::DragFloat3("text6scale", &text6.scale.x, 0.01f)) {
			Tutorialtext6->SetTransform(text6);
		}
		if (ImGui::DragFloat3("text6rotate", &text6.rotate.x, 0.01f)) {
			Tutorialtext6->SetTransform(text6);
		}
		if (ImGui::DragFloat3("text6translate", &text6.translate.x, 0.01f)) {
			Tutorialtext6->SetTransform(text6);
		}
		if (ImGui::DragFloat3("text7scale", &text7.scale.x, 0.01f)) {
			Tutorialtext7->SetTransform(text7);
		}
		if (ImGui::DragFloat3("text7rotate", &text7.rotate.x, 0.01f)) {
			Tutorialtext7->SetTransform(text7);
		}
		if (ImGui::DragFloat3("text7translate", &text7.translate.x, 0.01f)) {
			Tutorialtext7->SetTransform(text7);
		}
		if (ImGui::DragFloat3("text8translate", &text8.translate.x), 0.01f) {
			Tutorialtext8->SetTransform(text8);
		}

		if (ImGui::DragFloat3("text9translate", &text9.translate.x), 0.01f) {
			Tutorialtext9->SetTransform(text9);
		}

		//UI

		Vector2 ncopy = nCopySprite->GetPosition();
		Vector2 ncopysize = nCopySprite->GetSize();
		Vector2 resetmeter = resetMeter->GetPosition();
		Vector2 resetmetersize = resetMeter->GetSize();

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

		/*
		if (ImGui::CollapsingHeader("UI Translate")) {
			for (int i = 0; i < operationTexts.size(); ++i) {
				Vector2 pos = operationTexts[i]->GetPosition();
				if (ImGui::DragFloat2(("Sprite" + std::to_string(i)).c_str(), &pos.x, 0.01f)) {
					operationTexts[i]->SetPosition(pos);
				}
			}
		}
		*/
		/*
		Vector2 uistickL = OperationtextStickL->GetPosition();
		Vector2 uistickLsize = OperationtextStickL->GetSize();
		Vector2 uibuttonB = OperationtextButtonB->GetPosition();
		Vector2 uibuttonBsize = OperationtextButtonB->GetSize();
		Vector2 uibuttonA = OperationtextButtonA->GetPosition();
		Vector2 uibuttonAsize = OperationtextButtonA->GetSize();
		Vector2 uiX = OperationtextX->GetPosition();
		Vector2 uiXsize = OperationtextX->GetSize();
		Vector2 uiY = OperationtextY->GetPosition();
		Vector2 uiYsize = OperationtextY->GetSize();
		Vector2 uiLB = OperationtextLB->GetPosition();
		Vector2 uiLBsize = OperationtextLB->GetSize();
		Vector2 uiRB = OperationtextRB->GetPosition();
		Vector2 uiRBsize = OperationtextRB->GetSize();
		Vector2 uitoru = OperationtextToru->GetPosition();
		Vector2 uitorusize = OperationtextToru->GetSize();
		Vector2 uihaiti = OperationtextHaiti->GetPosition();
		Vector2 uihaitisize = OperationtextHaiti->GetSize();
		Vector2 uikirikae = OperationtextKrikae->GetPosition();
		Vector2 uikirikaesize = OperationtextKrikae->GetSize();
		Vector2 uiidou = OperationtextIdou->GetPosition();
		Vector2 uiidousize = OperationtextIdou->GetSize();
		Vector2 uizyanpu = OperationtextZyanpu->GetPosition();
		Vector2 uizyanpusize = OperationtextZyanpu->GetSize();
		Vector2 uireset = OperationtextReset->GetPosition();
		Vector2 uiresetsize = OperationtextReset->GetSize();
		Vector2 uiPlus = OperationtextPlus->GetPosition();
		Vector2 uiPlussize = OperationtextPlus->GetSize();

		if (ImGui::DragFloat2("uiStickLtranslate", &uistickL.x), 0.01f) {
			OperationtextStickL->SetPosition(uistickL);
		}
		if (ImGui::DragFloat2("uiStickLsize", &uistickLsize.x), 0.01f) {
			OperationtextStickL->SetSize(uistickLsize);
		}
		if (ImGui::DragFloat2("uibuttonBtranslate", &uibuttonB.x), 0.01f) {
			OperationtextButtonB->SetPosition(uibuttonB);
		}
		if (ImGui::DragFloat2("uibuttonBsize", &uibuttonBsize.x), 0.01f) {
			OperationtextButtonB->SetSize(uibuttonBsize);
		}
		if (ImGui::DragFloat2("uibuttonAtranslate", &uibuttonA.x), 0.01f) {
			OperationtextButtonA->SetPosition(uibuttonA);
		}
		if (ImGui::DragFloat2("uibuttonAsize", &uibuttonAsize.x), 0.01f) {
			OperationtextButtonA->SetSize(uibuttonAsize);
		}
		if (ImGui::DragFloat2("uiXtranslate", &uiX.x), 0.01f) {
			OperationtextX->SetPosition(uiX);
		}
		if (ImGui::DragFloat2("uiXsize", &uiXsize.x), 0.01f) {
			OperationtextX->SetSize(uiXsize);
		}
		if (ImGui::DragFloat2("uiYtranslate", &uiY.x), 0.01f) {
			OperationtextY->SetPosition(uiY);
		}
		if (ImGui::DragFloat2("uiYsize", &uiYsize.x), 0.01f) {
			OperationtextY->SetSize(uiYsize);
		}
		if (ImGui::DragFloat2("uiLBtranslate", &uiLB.x), 0.01f) {
			OperationtextLB->SetPosition(uiLB);
		}
		if (ImGui::DragFloat2("uiLBsize", &uiLBsize.x), 0.01f) {
			OperationtextLB->SetSize(uiLBsize);
		}
		if (ImGui::DragFloat2("uiRBtranslate", &uiRB.x), 0.01f) {
			OperationtextRB->SetPosition(uiRB);
		}
		if (ImGui::DragFloat2("uiRBsize", &uiRBsize.x), 0.01f) {
			OperationtextRB->SetSize(uiRBsize);
		}
		if (ImGui::DragFloat2("uitorutranslate", &uitoru.x), 0.01f) {
			OperationtextToru->SetPosition(uitoru);
		}
		if (ImGui::DragFloat2("uitorusize", &uitorusize.x), 0.01f) {
			OperationtextToru->SetSize(uitorusize);
		}
		if (ImGui::DragFloat2("uihaititranslate", &uihaiti.x), 0.01f) {
			OperationtextHaiti->SetPosition(uihaiti);
		}
		if (ImGui::DragFloat2("uihaitisize", &uihaitisize.x), 0.01f) {
			OperationtextHaiti->SetSize(uihaitisize);
		}
		if (ImGui::DragFloat2("uikirikaetranslate", &uikirikae.x), 0.01f) {
			OperationtextKrikae->SetPosition(uikirikae);
		}
		if (ImGui::DragFloat2("uikirikaesize", &uikirikaesize.x), 0.01f) {
			OperationtextKrikae->SetSize(uikirikaesize);
		}
		if (ImGui::DragFloat2("uiidoutranslate", &uiidou.x), 0.01f) {
			OperationtextIdou->SetPosition(uiidou);
		}
		if (ImGui::DragFloat2("uiidousize", &uiidousize.x), 0.01f) {
			OperationtextIdou->SetSize(uiidousize);
		}
		if (ImGui::DragFloat2("uizyanputranslate", &uizyanpu.x), 0.01f) {
			OperationtextZyanpu->SetPosition(uizyanpu);
		}
		if (ImGui::DragFloat2("uizyanpusize", &uizyanpusize.x), 0.01f) {
			OperationtextZyanpu->SetSize(uizyanpusize);
		}
		if (ImGui::DragFloat2("uiResettranslate", &uireset.x), 0.01f) {
			OperationtextReset->SetPosition(uireset);
		}
		if (ImGui::DragFloat2("uiResetsize", &uiresetsize.x), 0.01f) {
			OperationtextReset->SetSize(uiresetsize);
		}
		if (ImGui::DragFloat2("uiPlustranslate", &uiPlus.x), 0.01f) {
			OperationtextPlus->SetPosition(uiPlus);
		}
		if (ImGui::DragFloat2("uiPlussize", &uiPlussize.x), 0.01f) {
			OperationtextPlus->SetSize(uiPlussize);
		}
		*/
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

