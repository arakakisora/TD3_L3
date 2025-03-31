#include "PhotoCamera.h"
#include "Object3DCommon.h"
#include "Input.h"
#include <imgui.h>
#include <iostream>
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
	postion = Vector2{ 8,13 };
	object3D->SetTranslate(Vector3(postion.x, postion.y, 0));
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


	// フォトカメラのコピー / スペースキーを押したら
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		std::cout << "Space key pressed. Calling Copy() method." << std::endl; // デバッグ用
		Copy();
	}
#ifdef _DEBUG
	// ImGuiの描画
	DrawImGui();
#endif // _DEBUG


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
	if (Input::GetInstance()->TriggerKey(DIK_W)) {
		postion.y++;
	} else if (Input::GetInstance()->TriggerKey(DIK_S)) {
		postion.y--;
	} else if (Input::GetInstance()->TriggerKey(DIK_D)) {
		postion.x++;
	} else if (Input::GetInstance()->TriggerKey(DIK_A)) {
		postion.x--;
	}
	// フォトカメラの配置を変更させる
	object3D->SetTranslate(Vector3(postion.x, postion.y, 0));
}

void PhotoCamera::Copy()
{


	// デバッグ用のログを追加
	std::cout << "Copy() method called." << std::endl;
	std::cout << "postion: (" << postion.x << ", " << postion.y << ")" << std::endl;

	// @現状カメラの範囲の大きさは２Ｘ２にしているが動的に変更ができるようにする
	// コピー先の2x2の範囲を確保
	copyData = vector<vector<MapChipType>>(2, vector<MapChipType>(2));

	// フォトカメラの位置から2x2の範囲のデータをコピー
	for (int y = 0; y < 2; y++) {
		for (int x = 0; x < 2; x++) {
			int mapY = static_cast<int>(postion.y) + y;
			int mapX = static_cast<int>(postion.x) + x;
			if (mapY >= 0 && mapY < mapData.data.size() && mapX >= 0 && mapX < mapData.data[0].size()) {
				copyData[y][x] = mapData.data[mapY][mapX];
				std::cout << "copyData[" << y << "][" << x << "] = " << static_cast<int>(copyData[y][x]) << std::endl; // デバッグ用
			} else {
				std::cout << "Out of bounds: mapY = " << mapY << ", mapX = " << mapX << std::endl; // デバッグ用
			}
		}
	}



}

void PhotoCamera::DrawImGui()
{
	// ImGuiの描画コードを追加
	ImGui::Begin("PhotoCamera Data");
	ImGui::Text("Copied Map Data:");
	for (int y = 0; y < 2; y++) {
		for (int x = 0; x < 2; x++) {
			// マップチップタイプを取得
			MapChipType mapChipType = mapData.data[y][x];

			// マップチップタイプを番号として表示
			ImGui::Text("%d", static_cast<int>(mapChipType));
			if (x < 1) {
				ImGui::SameLine();
			}
		}
	}
	ImGui::End();
}
