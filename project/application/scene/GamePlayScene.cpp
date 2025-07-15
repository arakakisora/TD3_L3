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

	//リセットお知らせ
	reset = std::make_unique<Reset>();
	reset->Initialize();
	
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

	//フォローカメラ設定
	CameraManager::GetInstans()->GetCamera("maincam")->SetFollowTarget(player->GetObject3D(), {0, 0, -15});

	CameraManager::GetInstans()->GetCamera("maincam")->SetFollowMode(false);

	// ゲームカメラの生成
	photoCamera = std::make_unique<PhotoCamera>();
	photoCamera->Initialize(map);

	//チュートリアル
	tutorial = std::make_unique<Tutorial>();
	tutorial->Initialize();
	tutorial->SetPhotoCamera(photoCamera.get());

	//UI
	operate = std::make_unique<Operate>();
	operate->Initialize();
	operate->SetPhotoCamera(photoCamera.get());

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
			CameraManager::GetInstans()->GetCamera("maincam")->SetFollowTarget(player->GetObject3D(), { 0,0, -7.0f });
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

	//チュートリアル
	tutorial->Update();

    //UI
	operate->Update();

	
	//mode切り替え
	photoCamera->SetcameraMode(player->GetCameraMode());


	for (std::unique_ptr<Sprite>& Uitext : pauseui) {
		Uitext->Update();
	}

	// ポーズ
	if (!player->GetCheckGoal()) {
		pauseMenu->Update();
	}

	//ステージリセット
	reset->StageReset();
	//リセット更新
	reset->Update();
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
	photoCamera->Draw3DObject();
	////プレイヤー
	player->Draw();

	//チュートリアルテキスト
	tutorial->TextDraw();

	//リセットお知らせ描画
	reset->DrawResetNotice();

	map->Draw();

	//ポーズメニュー
	pauseMenu->Draw();

	ParticleMnager::GetInstance()->Draw();

#pragma endregion

#pragma region スプライト描画
	//Spriteの描画準備。spriteの描画に共通のグラフィックスコマンドを積む
	SpriteCommon::GetInstance()->CommonDraw();

	//UI
	operate->Draw();

	for (std::unique_ptr<Sprite>& Uitext : pauseui) {
		Uitext->Draw();
	}

	//リセットメーター描画
	reset->DrawResetMeter();
	//チュートリアル描画
	tutorial->SpriteDraw();

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
		DirectionalLight directionalLight = player->GetObject3D()->GetDirectionalLight();
		if (ImGui::DragFloat3("Player Directional Light Direction", &directionalLight.direction.x, 0.01f)) {

			player->GetObject3D()->SetDirectionalLightDirection(directionalLight.direction);
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