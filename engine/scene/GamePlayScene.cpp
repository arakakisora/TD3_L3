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
	ModelManager::GetInstans()->LoadModel("terrain.obj");
	ModelManager::GetInstans()->LoadModel("cube.obj");

	//ModelManager::GetInstans()->LoadModel("Player.obj");
	ModelManager::GetInstans()->LoadModel("playercharacter.obj");

	// ブロック関連モデル
	ModelManager::GetInstans()->LoadModel("block.obj");
	ModelManager::GetInstans()->LoadModel("Timer.obj");
	ModelManager::GetInstans()->LoadModel("fallblock.obj");
	ModelManager::GetInstans()->LoadModel("ncopyblock.obj");
	ModelManager::GetInstans()->LoadModel("GoalBase.obj");
	ModelManager::GetInstans()->LoadModel("GoreFag.obj");
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

	// ポーズテキスト
	ModelManager::GetInstans()->LoadModel("Pause.obj");
	ModelManager::GetInstans()->LoadModel("StageSelect/return.obj");
	ModelManager::GetInstans()->LoadModel("StageSelect/title.obj");
	ModelManager::GetInstans()->LoadModel("StageSelect/explanation.obj");
	ModelManager::GetInstans()->LoadModel("StageSelect/StageSelect.obj");


	//操作説明UI

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

	int stageIndex = SceneManager::GetInstance()->GetStageIndex();

	std::string stagePath;
	switch (stageIndex) {
	case 0: stagePath = "MapData/mapp1.csv"; break;
	case 1: stagePath = "MapData/mapp2.csv"; break;
	case 2: stagePath = "MapData/mapp3.csv"; break;
	}

	skydome_ = make_unique<Object3D>();
	skydome_->Initialize(Object3DCommon::GetInstance());
	skydome_->SetTranslate(Vector3{ 15.0f, 5.0f, 100.0f });
	skydome_->SetScale(Vector3{ 1.0f,1.0f,1.0f });
	skydome_->SetModel("backPlane.obj");


	map = new Map;
	map->LoadMapChipCsv(stagePath);
	map->Initialize();


	//playerの生成
	player = std::make_unique<Player>();
	object3DPlayer = new Object3D();
	Vector3 playerPostion = Vector3((float)map->GetPlayerStartX(), (float)map->GetPlayerStartY(), 0.0f);
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
	Tutorialtext9->SetScale(Vector3(0.5f,0.5f,0.5f));
	Tutorialtext9->SetRotate(Vector3(17.3f,12.56f,0.0f));
	Tutorialtext9->SetTranslate(Vector3(12.46f,21.4f,1.0f));
	Tutorialtext9->SetLighting(false);
	Tutorialtext9->SetIsTutorialActive(false);

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
	OperationtextStickL->SetPosition(Vector2(90, 655));
	OperationtextStickL->SetSize(Vector2(70, 70));

	OperationtextButtonB = std::make_unique<Sprite>();
	OperationtextButtonB->Initialize(SpriteCommon::GetInstance(), "Resources/xbox_button_color_b.png");
	OperationtextButtonB->SetPosition(Vector2(557, 655));
	OperationtextButtonB->SetSize(Vector2(70, 70));

	OperationtextButtonA = std::make_unique<Sprite>();
	OperationtextButtonA->Initialize(SpriteCommon::GetInstance(), "Resources/xbox_button_color_a.png");
	OperationtextButtonA->SetPosition(Vector2(319, 655));
	OperationtextButtonA->SetSize(Vector2(70, 70));

	OperationtextLB = std::make_unique<Sprite>();
	OperationtextLB->Initialize(SpriteCommon::GetInstance(), "Resources/xbox_lb.png");
	OperationtextLB->SetPosition(Vector2(826, 655));
	OperationtextLB->SetSize(Vector2(70, 70));

	OperationtextRB = std::make_unique<Sprite>();
	OperationtextRB->Initialize(SpriteCommon::GetInstance(), "Resources/xbox_rb.png");
	OperationtextRB->SetPosition(Vector2(1039, 655));
	OperationtextRB->SetSize(Vector2(70, 70));

	OperationtextIdou = std::make_unique<Sprite>();
	OperationtextIdou->Initialize(SpriteCommon::GetInstance(), "Resources/idou.png");
	OperationtextIdou->SetPosition(Vector2(203, 655));
	OperationtextIdou->SetSize(Vector2(60, 60));

	OperationtextKrikae = std::make_unique<Sprite>();
	OperationtextKrikae->Initialize(SpriteCommon::GetInstance(), "Resources/kirikae.png");
	OperationtextKrikae->SetPosition(Vector2(672, 655));
	OperationtextKrikae->SetSize(Vector2(100, 60));

	OperationtextToru = std::make_unique<Sprite>();
	OperationtextToru->Initialize(SpriteCommon::GetInstance(), "Resources/toru.png");
	OperationtextToru->SetPosition(Vector2(938, 655));
	OperationtextToru->SetSize(Vector2(60, 60));

	OperationtextHaiti = std::make_unique<Sprite>();
	OperationtextHaiti->Initialize(SpriteCommon::GetInstance(), "Resources/haiti.png");
	OperationtextHaiti->SetPosition(Vector2(1143, 655));
	OperationtextHaiti->SetSize(Vector2(60, 60));

	OperationtextZyanpu = std::make_unique<Sprite>();
	OperationtextZyanpu->Initialize(SpriteCommon::GetInstance(), "Resources/zyanpu.png");
	OperationtextZyanpu->SetPosition(Vector2(430, 655));
	OperationtextZyanpu->SetSize(Vector2(60, 60));

	for (uint32_t i = 0; i < 2; ++i) {
		std::unique_ptr<Sprite> newSprite = std::make_unique<Sprite>();
		if (i == 0) {
			newSprite->Initialize(SpriteCommon::GetInstance(), "Resources/Pause.png");
			newSprite->SetPosition(Vector2(1050.0f, 15.0f));
			newSprite->SetSize(Vector2(150, 50));
		} else if (i == 1) {
			newSprite->Initialize(SpriteCommon::GetInstance(), "Resources/xbox_button_color_y.png");
			newSprite->SetPosition(Vector2(1210.0f, 5.0f));
			newSprite->SetSize(Vector2(70, 70));
		}
		newSprite->setColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		pauseui.push_back(std::move(newSprite));
	}

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
	pauseMenu->Initialize(Object3DCommon::GetInstance(), true);
	pauseMenu->SetCamera(CameraManager::GetInstans()->GetCamera("maincam"));

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
}

void GamePlayScene::Update()
{

	//ポーズ画面が出ている間は停止
	if (!pauseMenu->IsPaused()) {

		//カメラの更新
		CameraManager::GetInstans()->GetActiveCamera()->Update();

		// 天球の更新
		skydomerotate += 0.0f;
		skydome_->SetRotate(Vector3{ 0.0f,0.0f,skydomerotate });
		skydome_->Update();

		// ゲームカメラ更新処理
		//gameCamera_->Update();
		photoCamera->Update(map);

		map->Update();
		////プレイヤーの更新
		player->Update();

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
		}

	}
	
	//チュートリアル表示制御map2
	if (SceneManager::GetInstance()->GetStageIndex() == 1) {
		if (!tutorial9) {
			Tutorialtext9->SetIsTutorialActive(true);
			tutorial9 = true;
		}
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
			OperationtextIdou->Update();
			OperationtextKrikae->Update();
			OperationtextZyanpu->Update();
		}
		if (photoCamera->GetCameraMode()) {
			OperationtextStickL->Update();
			OperationtextButtonB->Update();
			OperationtextLB->Update();
			OperationtextRB->Update();
			OperationtextIdou->Update();
			OperationtextKrikae->Update();
			OperationtextToru->Update();
			OperationtextHaiti->Update();
		}
		//mode切り替え
		photoCamera->SetcameraMode(player->GetcamerMode());

	
		for (std::unique_ptr<Sprite>& Uitext : pauseui) {
			Uitext->Update();
		}

	// ポーズ
	pauseMenu->Update();

	//リセット
	if (Input::GetInstance()->PushGamePadButton(XINPUT_GAMEPAD_LEFT_SHOULDER) &&
		Input::GetInstance()->PushGamePadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER)) {

		holdTime += deltaTime;

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
		Vector2 uistickL = OperationtextStickL->GetPosition();
		Vector2 uibuttonB = OperationtextButtonB->GetPosition();
		Vector2 uibuttonA = OperationtextButtonA->GetPosition();
		Vector2 uiLB = OperationtextLB->GetPosition();
		Vector2 uiRB = OperationtextRB->GetPosition();
		Vector2 uitoru = OperationtextToru->GetPosition();
		Vector2 uihaiti = OperationtextHaiti->GetPosition();
		Vector2 uikirikae = OperationtextKrikae->GetPosition();
		Vector2 uiidou = OperationtextIdou->GetPosition();
		Vector2 uizyanpu = OperationtextZyanpu->GetPosition();
		if (ImGui::DragFloat2("uiStickLtranslate", &uistickL.x), 0.01f) {
			OperationtextStickL->SetPosition(uistickL);
		}
		if (ImGui::DragFloat2("uibuttonBtranslate", &uibuttonB.x), 0.01f) {
			OperationtextButtonB->SetPosition(uibuttonB);
		}
		if (ImGui::DragFloat2("uibuttonAtranslate", &uibuttonA.x), 0.01f) {
			OperationtextButtonA->SetPosition(uibuttonA);
		}
		if (ImGui::DragFloat2("uiLBtranslate", &uiLB.x), 0.01f) {
			OperationtextLB->SetPosition(uiLB);
		}
		if (ImGui::DragFloat2("uiRBtranslate", &uiRB.x), 0.01f) {
			OperationtextRB->SetPosition(uiRB);
		}
		if (ImGui::DragFloat2("uitorutranslate", &uitoru.x), 0.01f) {
			OperationtextToru->SetPosition(uitoru);
		}
		if (ImGui::DragFloat2("uihaititranslate", &uihaiti.x), 0.01f) {
			OperationtextHaiti->SetPosition(uihaiti);
		}
		if (ImGui::DragFloat2("uikirikaetranslate", &uikirikae.x), 0.01f) {
			OperationtextKrikae->SetPosition(uikirikae);
		}
		if (ImGui::DragFloat2("uiidoutranslate", &uiidou.x), 0.01f) {
			OperationtextIdou->SetPosition(uiidou);
		}
		if (ImGui::DragFloat2("uizyanputranslate", &uizyanpu.x), 0.01f) {
			OperationtextZyanpu->SetPosition(uizyanpu);
		}

	}

#endif // _DEBUG
}


void GamePlayScene::Draw()
{
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
		OperationtextIdou->Draw();
		OperationtextKrikae->Draw();
		OperationtextZyanpu->Draw();
	}
	if (photoCamera->GetCameraMode()) {
		OperationtextStickL->Draw();
		OperationtextButtonB->Draw();
		OperationtextLB->Draw();
		OperationtextRB->Draw();
		OperationtextIdou->Draw();
		OperationtextKrikae->Draw();
		OperationtextToru->Draw();
		OperationtextHaiti->Draw();
	}
	
	for (std::unique_ptr<Sprite>& Uitext : pauseui) {
		Uitext->Draw();
	}

	// フォトカメラ内のスプライト描画
	photoCamera->DrawSprite();

#pragma endregion
}





