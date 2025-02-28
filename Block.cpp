#include "Block.h"
#include "Object3D.h"
#include "Object3DCommon.h"
#include "ModelManager.h"
Block::Block()
{
}

Block::~Block()
{
}

void Block::Initialize(const int mapID, const Vector3& position) {
    this->mapID = mapID;
    this->position = position; // 座標を設定

    // mapIDに応じて異なるオブジェクトを初期化
    if (mapID == 0) {
		//ModelManager::GetInstans()->LoadModel("cube.obj");
  //      mapBlock0 = new Object3D();
  //      mapBlock0->Initialize(Object3DCommon::GetInstance()); // 適切な引数を渡す
  //      mapBlock0->SetTranslate(position); // 座標を設定
		//mapBlock0->SetScale(Vector3(0.5f, 0.5f, 0.5f));
		//mapBlock0->SetModel("cube.obj");
		//mapBlock0->SetLighting(true);
    } else if (mapID == 1) {
        ModelManager::GetInstans()->LoadModel("cube.obj");
        mapBlock1 = new Object3D();
        mapBlock1->Initialize(Object3DCommon::GetInstance()); // 適切な引数を渡す
        mapBlock1->SetTranslate(position); // 座標を設定
		mapBlock1->SetScale(Vector3(0.3f, 0.3f, 0.3f));
		mapBlock1->SetModel("cube.obj");
		mapBlock1->SetLighting(true);
    } else if (mapID == 2) {
        //ゴール
        
        ModelManager::GetInstans()->LoadModel("sphere.obj");
        mapBlock2 = new Object3D();
        mapBlock2->Initialize(Object3DCommon::GetInstance());
        mapBlock2->SetTranslate(position);
        mapBlock2->SetScale(Vector3(0.3f, 0.3f, 0.3f));
        mapBlock2->SetModel("sphere.obj");
        mapBlock2->SetLighting(true);
        
    }
}
void Block::Update()
{
    // mapIDに応じて異なるオブジェクトを更新
    if (mapID == 0 && mapBlock0) {
      //  mapBlock0->Update();
    } else if (mapID == 1 && mapBlock1) {
        mapBlock1->Update();
    } else if (mapID == 2 && mapBlock2) {
       mapBlock2->Update();
    }
}

void Block::Draw()
{
    // mapIDに応じて異なるオブジェクトを描画
    if (mapID == 0 && mapBlock0) {
       // mapBlock0->Draw();
    } else if (mapID == 1 && mapBlock1) {
        mapBlock1->Draw();
    } else if (mapID == 2 && mapBlock2) {
        mapBlock2->Draw();
    }
}

void Block::Finalize()
{
    // mapIDに応じて異なるオブジェクトを終了処理
    if (mapID == 0 && mapBlock0) {
       // delete mapBlock0;
       // mapBlock0 = nullptr;
    } else if (mapID == 1 && mapBlock1) {
        delete mapBlock1;
        mapBlock1 = nullptr;
    } else if (mapID == 2 && mapBlock2) {
        delete mapBlock2;
        mapBlock2 = nullptr;
    }
}
