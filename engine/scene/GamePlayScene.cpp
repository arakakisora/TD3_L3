#include "GamePlayScene.h"
#include <ModelManager.h>
#include "Object3DCommon.h"
#include "SpriteCommon.h"
#include "ImGuiManager.h"
#include "imgui.h"
#include "Input.h"
#include "TitleScene.h"
#include "CameraManager.h"
#include "ParticleMnager.h"
#include <Logger.h>
 
void GamePlayScene::Initialize()
{
	//カメラの生成
	camera1 = std::make_unique<Camera>();
	camera1->SetTranslate({ 0,0,-30, });//カメラの位置
	CameraManager::GetInstans()->AddCamera("maincam", camera1.get());

	//カメラの生成
	camera2 = std::make_unique<Camera>();
	camera2->SetTranslate({ 0,6,-20, });//カメラの位置
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

	map = new Map;
	map->LoadMapChipCsv("MapData/blocks.csv");
	GenerateObject3D();

	



	//playerの生成	
	player = std::make_unique<Player>();
	object3DPlayer = new Object3D();
	Vector3 playerPostion = map->GetMapChipPostionByIndex(6, 18);
	object3DPlayer->Initialize(Object3DCommon::GetInstance());
	object3DPlayer->SetModel("cube.obj");
	object3DPlayer->SetScale(Vector3{1.0f,1.0f,1.0f });
	player->SetMapChipField(map);
	player->Initialize(object3DPlayer, playerPostion);
	player->SetDeathHeight(0.0f);

	//フォローカメラ設定
	CameraManager::GetInstans()->GetCamera("maincam")->SetFollowTarget(object3DPlayer, { 0, 0, -15 });
	CameraManager::GetInstans()->GetCamera("maincam")->SetFollowMode(true);
	

	gameCamera_ = new GameCamera();
	gameCamera_->Initialize(map);

}

void GamePlayScene::Finalize()
{
	CameraManager::GetInstans()->RemoveCamera("maincam");
	CameraManager::GetInstans()->RemoveCamera("subcam");
	CameraManager::GetInstans()->Finalize();

	//マップの更新
	for (std::vector<Object3D*>& objext3dLine : blockobject3D)
	{
		for (Object3D* obj : objext3dLine)
		{
			delete obj;
		}
	}
	blockobject3D.clear();

	delete map;

	delete object3DPlayer;

	delete gameCamera_;

}

void GamePlayScene::Update()
{
	//カメラの更新
	CameraManager::GetInstans()->GetActiveCamera()->Update();


	////プレイヤーの更新
	player->Update();

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



	// ゲームカメラ更新処理
	gameCamera_->Update(map);
	
#endif // _DEBUG
	//3Dオブジェクトの更新
	for (std::vector<Object3D*>& objext3dLine : blockobject3D)
	{
		for (Object3D* obj : objext3dLine)
		{
			if (!obj)
				continue;
			obj->Update();
		}
	}
	
}

void GamePlayScene::Draw()
{
#pragma region 3Dオブジェクト描画

	//3dオブジェクトの描画準備。3Dオブジェクトの描画に共通のグラフィックスコマンドを積む
	Object3DCommon::GetInstance()->CommonDraw();


	////プレイヤー
	player->Draw();

	for (std::vector<Object3D*>& objext3dLine : blockobject3D)
	{
		for (Object3D* obj : objext3dLine)
		{
			if (!obj) {
				continue;
			}
			obj->Draw();
		}
	}
	

	// ゲームカメラ
	gameCamera_->Draw();

	ParticleMnager::GetInstance()->Draw();

#pragma endregion

#pragma region スプライト描画
	//Spriteの描画準備。spriteの描画に共通のグラフィックスコマンドを積む
	SpriteCommon::GetInstance()->CommonDraw();
	

#pragma endregion
}

void GamePlayScene::GenerateObject3D()
{
	// 要素数
	uint32_t numBlokVirtical = map->GetNumBlockVirtical();     // 縦
	uint32_t numBlokHorizontal = map->GetNumBlockHorizontal(); // 横


	blockobject3D.resize(numBlokVirtical);

	for (uint32_t i = 0; i < numBlokVirtical; ++i)
	{
		blockobject3D[i].resize(numBlokHorizontal);

	}
	// キューブ生成
	for (uint32_t i = 0; i < numBlokVirtical; ++i) {
		for (uint32_t j = 0; j < numBlokHorizontal; ++j) {

			if (map->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {


				Object3D* object3D_ = new Object3D();
				object3D_->Initialize(Object3DCommon::GetInstance());
				object3D_->SetModel("cube.obj");
				blockobject3D[i][j] = object3D_;
				blockobject3D[i][j]->SetTranslate(map->GetMapChipPostionByIndex(j, i));


			}
		}
	}



}



