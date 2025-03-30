#include "PhotoCamera.h"
#include "Object3DCommon.h"
#include "Input.h"
// MAPクラスとのループキャストに注意
void PhotoCamera::Initialize()
{
	// フォトカメラの範囲モデル
	object3D = make_unique<Object3D>();
	object3D->Initialize(Object3DCommon::GetInstance());
	// @枠組みのモデルを用意するように
	object3D->SetModel("axis.obj");
	// @値を後に調整する
	object3D->SetScale(Vector3{ 1.0f,1.0f,1.0f });
	postion = Vector2{ 6,13 };
	object3D->SetTranslate(Vector3(postion.x,postion.y,0));
	object3D->SetRotate(Vector3{ 0,0,0 });
	

}

void PhotoCamera::Update(Map* map)
{
	this->map = map;
	// mapDataを受け取る
	mapData.data = this->map->GetMap();

	// フォトカメラの枠モデルの更新
	object3D->Update();

	// フォトカメラの移動
	Move();

	// 変更したmapDataをmapにセット / mapクラスに送り返し更新させる
	this->map->SetMap(mapData.data);
}

void PhotoCamera::Draw()
{
	// フォトカメラの枠モデルの描画
	object3D->Draw();
}

void PhotoCamera::Finalize()
{

}

void PhotoCamera::Move()
{
	// @範囲外に移動しないようにする
	// @カメラの移動方法をコントローラー操作に変更する
	if (Input::GetInstance()->TriggerKey(DIK_W)){
		postion.y++;
	} 
	else if (Input::GetInstance()->TriggerKey(DIK_S)) {
		postion.y--;
	} 
	else if (Input::GetInstance()->TriggerKey(DIK_D)) {
		postion.x++;
	} 
	else if (Input::GetInstance()->TriggerKey(DIK_A)) {
		postion.x--;
	} 
	// フォトカメラの配置を変更させる
	object3D->SetTranslate(Vector3(postion.x, postion.y, 0));
}
