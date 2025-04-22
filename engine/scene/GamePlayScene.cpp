#include "GamePlayScene.h"
#include <ModelManager.h>
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

	ModelManager::GetInstans()->LoadModel("Player.obj");

	// ブロック関連モデル
	ModelManager::GetInstans()->LoadModel("block.obj");
	ModelManager::GetInstans()->LoadModel("Timer.obj");
	ModelManager::GetInstans()->LoadModel("fallblock.obj");
	ModelManager::GetInstans()->LoadModel("ncopyblock.obj");
	ModelManager::GetInstans()->LoadModel("GoalBase.obj");
	ModelManager::GetInstans()->LoadModel("GoreFag.obj");
	// 天球モデル
	ModelManager::GetInstans()->LoadModel("backPlane.obj");

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
	object3DPlayer->SetModel("Player.obj");
	object3DPlayer->SetScale(Vector3{ 0.1f,0.1f,0.1f });
	player->SetMapChipField(map);
	player->Initialize(object3DPlayer, playerPostion);
	player->SetDeathHeight(0.0f);

	//フォローカメラ設定
	CameraManager::GetInstans()->GetCamera("maincam")->SetFollowTarget(object3DPlayer, { 0, 0, -15 });

	CameraManager::GetInstans()->GetCamera("maincam")->SetFollowMode(false);



	// ゲームカメラの生成
	//gameCamera_ = new ObjectCamera();
	//gameCamera_->Initialize(map);
	photoCamera = new PhotoCamera;
	photoCamera->Initialize(map);

	// ビットマップフォント
	bitmapFont = make_unique<BitmapFont>();
	bitmapFont->Initialize((uint32_t)1);

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
	//カメラの更新
	CameraManager::GetInstans()->GetActiveCamera()->Update();

	// 天球の更新
	skydomerotate+=0.0f;
	skydome_->SetRotate(Vector3{ 0.0f,0.0f,skydomerotate });
	skydome_->Update();

	// ゲームカメラ更新処理
	//gameCamera_->Update();
	photoCamera->Update(map);

	// ビットマップフォントの更新処理
	bitmapFont->Update();
	
	map->Update();


	////プレイヤーの更新
	player->Update();
	//mode切り替え
	photoCamera->SetcameraMode(player->GetcamerMode());

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




	map->Draw();

	ParticleMnager::GetInstance()->Draw();

#pragma endregion

#pragma region スプライト描画
	//Spriteの描画準備。spriteの描画に共通のグラフィックスコマンドを積む
	SpriteCommon::GetInstance()->CommonDraw();
	// フォトカメラ内のスプライト描画
	photoCamera->DrawSprite();

	// ビットマップフォントのスプライト描画
	bitmapFont->Draw();
#pragma endregion
}





