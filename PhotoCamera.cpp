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

	// フォトカメラの移動
	Move();
	// フォトカメラの枠モデルの更新
	object3D->Update();



	// フォトカメラのコピー / スペースキーを押したら
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		Copy();
		for (uint32_t y = 0; y < 2; ++y) {
			for (uint32_t x = 0; x < 2; ++x) {
				MapChipType type = mapData.data[static_cast<int>(position.y) - y][static_cast<int>(position.x) + x];
				if (type != MapChipType::kBlank) {
					// ブロックのインスタンスを生成する
					blockPosition = Vector3(position.x, position.y, -1.0F);
					Block* block = new Block();
					block = block->CreateBlock(type, blockPosition, map);
					blocks.push_back(block);
				}
			}
		}
	}
	// フォトカメラのペースト / Pキーを押したら
	if (Input::GetInstance()->TriggerKey(DIK_P)) {
		Paste();
	}
	

	// 生成されたブロックの更新
	for (auto& block : blocks) {
		block->Update();
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

	// 生成されたブロックの描画
	for (auto& block : blocks) {
		block->Draw();
	}
}

void PhotoCamera::Finalize()
{
	// 生成されたブロックの終了処理
	for (auto& block : blocks) {
		block->Finalize();
		delete block;
	}
	blocks.clear();
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

	// ブロックの位置を更新
	for (size_t i = 0; i < blocks.size(); ++i) {
		uint32_t x = static_cast<uint32_t>(i % 2);
		uint32_t y = static_cast<uint32_t>(i / 2);
		Vector3 blockPosition = Vector3(position.x + x, position.y + y, -1.0F);
		blocks[i]->SetObject3DPosiition(blockPosition);
	}

}

void PhotoCamera::Copy() {
	// マップデータが読み込めていないときはコピー不可
	if (!map) return;

	// カメラの位置からマップチップのインデックスを取得

	// 2x2 のマップチップ番号をコピー
	copyData.clear();
	blocks.clear();
	for (uint32_t y = 0; y < 2; ++y) {
		vector<MapChipType> row;
		for (uint32_t x = 0; x < 2; ++x) {
			MapChipType type = mapData.data[static_cast<int>(position.y) - y][static_cast<int>(position.x) + x];
			row.push_back(type);

			// コピーしたマップチップを基にブロックのインスタンスを生成する
			//if (type != MapChipType::kBlank) {
			//	// 直接座標を設定
			//	blockPosition = Vector3(position.x + x, position.y + y, -1.0F);
			//	Block* block = new Block();
			//	block = block->CreateBlock(type, blockPosition, map);
			//	
			//	blocks.push_back(block);
			//}
		}
		copyData.push_back(row);
	}
}

void PhotoCamera::Paste()
{
	// マップデータが読み込めていないときはペースト不可
	if (!map) return;

	// コピーデータをマップデータにペースト
	for (uint32_t y = 0; y < 2; ++y) {
		for (uint32_t x = 0; x < 2; ++x) {
			MapChipType type = copyData[y][x];
			int positionX = static_cast<int>(position.x) + x;
			int positionY = static_cast<int>(position.y) + y;
			mapData.data[positionY][positionX] = type;
			// 変更したマップデータをマップにセット
			map->SetMap(mapData.data);
		}
	}
}


void PhotoCamera::DrawImGui()
{
	// ImGuiの描画コードを追加
	ImGui::Begin("PhotoCamera Data");

	// カメラの現在位置を表示
	ImGui::Text("Camera Position: (%.2f, %.2f)", position.x, position.y);
	ImGui::Separator();

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
			ImGui::SameLine();

			// マップチップタイプの名前を表示
			std::string mapChipName;
			switch (mapChipType) {
			case MapChipType::kBlank: mapChipName = "Blank"; break;
			case MapChipType::kPlayer: mapChipName = "Player"; break;
			case MapChipType::kNCopyBlock: mapChipName = "NCopyBlock"; break;
			case MapChipType::kCopyBlock: mapChipName = "CopyBlock"; break;
			case MapChipType::kGoalUp: mapChipName = "GoalUp"; break;
			case MapChipType::kGoalDown: mapChipName = "GoalDown"; break;
			case MapChipType::kFallBlock: mapChipName = "FallBlock"; break;
			case MapChipType::kFixedTimeBlock: mapChipName = "FixedTimeBlock"; break;
			case MapChipType::kPutFixedTimeBlock: mapChipName = "PutFixedTimeBlock"; break;
			default: mapChipName = "Unknown"; break;
			}
			ImGui::Text("%s", mapChipName.c_str());

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
			Vector3 position = blockPosition;
			ImGui::Text("Block Position: (%.2f, %.2f, %.2f)", position.x, position.y, position.z);
		}
	} else {
		ImGui::Text("No blocks generated.");
	}

	ImGui::End();
}
