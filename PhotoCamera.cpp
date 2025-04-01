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
	position = Vector2{ 8,13 };
	object3D->SetTranslate(Vector3(position.x, position.y, 0));
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

	// コピーしたマップデータの描画
	for (auto& block : blocks) {
		block->Draw();
	}
}

void PhotoCamera::Finalize()
{

}

void PhotoCamera::Move()
{
	// @範囲外に移動しないようにする
	// @カメラの移動方法をコントローラー操作に変更する
	if (Input::GetInstance()->TriggerKey(DIK_W)) {
		position.y++;
	} else if (Input::GetInstance()->TriggerKey(DIK_S)) {
		position.y--;
	} else if (Input::GetInstance()->TriggerKey(DIK_D)) {
		position.x++;
	} else if (Input::GetInstance()->TriggerKey(DIK_A)) {
		position.x--;
	}
	// フォトカメラの配置を変更させる
	object3D->SetTranslate(Vector3(position.x, position.y, 0));
}

void PhotoCamera::Copy() {
	// マップデータが読み込めていないときはコピー不可
	if (!map) return;

	// カメラの位置からマップチップのインデックスを取得
	IndexSet indexSet = map->GetMapChipIndexSetByPosition(Vector3(position.x, position.y, 0));

	// 2x2 のマップチップ番号をコピー
	copyData.clear();
	blocks.clear();
	for (uint32_t y = 0; y < 2; ++y) {
		vector<MapChipType> row;
		for (uint32_t x = 0; x < 2; ++x) {
			MapChipType type = map->GetMapChipTypeByIndex(indexSet.xIndex + x, indexSet.yIndex + y);
			row.push_back(type);

			// コピーしたマップチップを基にブロックのインスタンスを生成する
			if (type != MapChipType::kBlank) {
				// 直接座標を設定
				Vector3 blockPosition = Vector3(position.x + x, position.y + y, -5.0F);
				unique_ptr<Block> block = make_unique<Block>();
				block->CreateBlock(type, blockPosition, map);
				blocks.push_back(move(block));
			}

		}
		copyData.push_back(row);
	}


}

void PhotoCamera::DrawImGui()
{
	// ImGuiの描画コードを追加
	ImGui::Begin("PhotoCamera Data");

	// コピーしたマップチップタイプを表示
	for (int y = 0; y < 2; y++) {
		for (int x = 0; x < 2; x++) {
			// もしコピーデータが空の場合は処理をスキップ
			if (copyData.empty()) {
				break;
			}

			// コピーしたマップチップタイプを取得
			MapChipType mapChipType = copyData[y][x];

			// マップチップタイプを番号として表示
			ImGui::Text("%d", static_cast<int>(mapChipType));
			if (x < 1) {
				ImGui::SameLine();
			}
		}
	}

	// 生成されたブロックの座標を表示
	ImGui::Separator();
	ImGui::Text("Generated Blocks:");
	if (!blocks.empty()) {
		for (const auto& block : blocks) {
			Vector3 position = block->GetPosition();
			ImGui::Text("Block Position: (%.2f, %.2f, %.2f)", position.x, position.y, position.z);
		}
	} else {
		ImGui::Text("No blocks generated.");
	}

	ImGui::End();
}


