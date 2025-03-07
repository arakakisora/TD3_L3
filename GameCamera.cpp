#include "GameCamera.h"
#include "Object3DCommon.h"
#include"Input.h"

// 要素数
uint32_t GameCamera::camesize_ = 4;

void GameCamera::Initialize(Map* map) {
	cols_ = 2;
	rows_ = 2;
	isVertical_ = true;

	// 範囲指定 (例 4 なら 4 個分要素を設定)
	gamecameras_.resize(camesize_);
	for (uint32_t i = 0; i < camesize_; ++i) {
		// 初期化
		gamecameras_[i] = std::make_unique<Object3D>();
		gamecameras_[i]->Initialize(Object3DCommon::GetInstance());
	}
	// 位置設定
	UpdateCameraPositions(map);
}

////--------------------未実装-------------------///
void GameCamera::UpdateGridSize(uint32_t size, bool vertical) {
	camesize_ = size;
	isVertical_ = vertical;

	if (isVertical_) {
		cols_ = 2; // 縦配置の時は固定値 (2列)
		rows_ = (camesize_ + cols_ - 1) / cols_; // 自動計算
	} else {
		cols_ = 5; // 横配置の場合 (例: 5列)
		rows_ = (camesize_ + cols_ - 1) / cols_; // 縦の数を計算
	}

	// gamecameras_のサイズを変更
	gamecameras_.resize(camesize_);
	for (uint32_t i = 0; i < camesize_; ++i) {
		if (!gamecameras_[i]) {
			gamecameras_[i] = std::make_unique<Object3D>();
			gamecameras_[i]->Initialize(Object3DCommon::GetInstance());
		}
	}
}

void GameCamera::UpdateCameraPositions(Map* map) {
	Vector3 position;
	uint32_t x, y;

	for (uint32_t i = 0; i < camesize_; ++i) {

		if (isVertical_) {
			x = i % cols_; // 横のインデックス
			y = i / rows_; // 縦のインデックス
		} else {
			x = i / rows_; // 横のインデックス
			y = i % cols_; // 縦のインデックス
		}

		position = map->GetBlockPosition(x, y);

		if (camesize_ == 2) {
			// 例: 右の列を少し右にずらす
			if (isVertical_) {
				if (x == 0 || y == 1) {
				} else if (i % cols_) {
					position.x += 0.15f;
				}
			} else if (!isVertical_) {
				if (x == 0 || y == 1) {
				} else if (i % cols_) {
					position.x += 0.15f;
				}
			}
		}

		if (camesize_ == 4) {
			// 例: 右の列を少し右にずらす
			if (isVertical_) {
				if (x == 0 || y == 1 && x == 0 || y == 2) {
				} else if (i % cols_) {
					position.x += 0.15f;
				}
			} else if (!isVertical_) {
				if (x == 0 || y == 1 && x == 0 || y == 2) {
				} else if (i % cols_) {
					position.x += 0.15f;
				}
			}
		}

		if (camesize_ == 6) {
			// 例: 右の列を少し右にずらす
			if (isVertical_) {
				if (x == 0 || y == 1 && x == 0 || y == 2 && x == 0 || y == 3) {
				} else if (i % cols_) {
					position.x += 0.15f;
				}
			} else if (!isVertical_) {
				if (x == 0 || y == 1 && x == 0 || y == 2 && x == 0 || y == 3) {
				} else if (i % cols_) {
					position.x += 0.15f;
				}
			}
		}

		// オブジェクトの更新
		gamecameras_[i]->SetTranslate(position);
		gamecameras_[i]->SetScale(Vector3(0.3f, 0.3f, 0.3f));
		gamecameras_[i]->SetModel("cube.obj");
		gamecameras_[i]->SetLighting(true);
	}
}

void GameCamera::Finalize() {
	for (auto& camera : gamecameras_) {
		camera.reset();
	}
}

void GameCamera::Update(Map* map) {
	// カメラ移動
	GameCameraMove();
	// 写真を撮って貼る
	GameCamertakeaphoto(map);
	// 更新処理
	for (auto& camera : gamecameras_) {
		camera->Update();
	}
}

void GameCamera::Draw() {
	// 描画処理
	for (auto& camera : gamecameras_) {
		camera->Draw();
	}
}

void GameCamera::GameCameraMove() {
	// 1マス分の移動量
	float blockScaleX = 0.30f;
	float blockScaleY = 0.3f;

	// 全ての要素に適用
	for (auto& camera : gamecameras_) {
		// 現在位置を取得
		Vector3 cameraposition = camera->GetTranslate();

		// Y軸移動
		if (Input::GetInstans()->TriggerKey(DIK_UP)) {
			cameraposition.y += blockScaleY;  // 上移動
		} else if (Input::GetInstans()->TriggerKey(DIK_DOWN)) {
			cameraposition.y -= blockScaleY;  // 下移動
		}

		// X軸移動
		if (Input::GetInstans()->TriggerKey(DIK_RIGHT)) {
			cameraposition.x += blockScaleX;  // 右移動
		} else if (Input::GetInstans()->TriggerKey(DIK_LEFT)) {
			cameraposition.x -= blockScaleX;  // 左移動
		}
		// 新しい位置を適用
		camera->SetTranslate(cameraposition);
	}
}

void GameCamera::GameCamertakeaphoto(Map* map) {
	// 1マス分の移動量
	float blockScaleX = 0.15f;
	float blockScaleY = 0.3f;

	// 全ての要素に適用
	for (auto& camera : gamecameras_) {
		// 現在位置を取得
		Vector3 cameraposition = camera->GetTranslate();

		// マップ上の座標を計算
		uint32_t playerX = static_cast<int>(cameraposition.x / blockScaleX);
		uint32_t playerY = static_cast<int>(cameraposition.y / blockScaleY);

		// マップの範囲チェック
		if (playerX >= 0 && playerX < map->GetMapWidth() &&
			playerY >= 0 && playerY < map->GetMapHeight()) {

			// SPACEで撮影
			if (Input::GetInstans()->TriggerKey(DIK_SPACE)) {
				uint32_t tileType = map->GetMapData()[playerY][playerX]; // ここは変更不要
				switch (tileType) {
				case 0: // 何もない
					camera->SetModel("axis.obj");
					camera->SetColor(Vector4(1.0f,1.0f,0.0f,1.0f));
					break;
				case 1: // ブロック
					camera->SetModel("cube.obj");
					camera->SetColor(Vector4(1.0f, 0.0f, 1.0f, 1.0f));
					break;
				}
				camera->SetScale(Vector3(0.3f, 0.3f, 0.3f));
			}

			// ENTERで貼り付け
			if (Input::GetInstans()->TriggerKey(DIK_RETURN)) {
				// 何もない
				if (camera->GetModel() == "axis.obj") {
					// マップデータの変更
					map->SetMapData(playerX, playerY, 0); // 修正：playerX と playerY を逆にする

					// ブロック
				} else if (camera->GetModel() == "cube.obj") {
					// マップデータの変更
					map->SetMapData(playerX, playerY, 1); // 修正：playerX と playerY を逆にする
				}
			}
		}
	}
}