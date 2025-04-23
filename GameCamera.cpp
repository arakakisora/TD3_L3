#include "GameCamera.h"
#include "Map.h"
#include "Object3DCommon.h"
#include "Input.h"

void GameCamera::Initialize(Map* map) {
    this->map_ = map;
    // 開始番号を設定
    xIndex = 7;
    yIndex = 16;
    size = { 1.0f,1.0f,-1.0f };
    // ゲームカメラのオブジェクト数をレンダリング範囲分だけ確保
    gamecameras_.resize(kRenderWidth * kRenderHeight);
    // 各オブジェクトを生成 & 初期化
    for (uint32_t y = 0; y < kRenderHeight; ++y) {
        for (uint32_t x = 0; x < kRenderWidth; ++x) {
            uint32_t index = y * kRenderWidth + x; // 2D を 1D インデックスに変換
            gamecameras_[index] = std::make_unique<Object3D>();
            gamecameras_[index]->Initialize(Object3DCommon::GetInstance());
            // マップのチップ座標に基づいて配置
            position = map_->GetMapChipPostionByIndex(xIndex + x, yIndex + y);
            gamecameras_[index]->SetTranslate(position);
            // スケール設定
            gamecameras_[index]->SetScale(size);
            // モデル設定
            gamecameras_[index]->SetModel("cube.obj");
            // ライティング有効化
            gamecameras_[index]->SetLighting(false);
        }
    }
}

void GameCamera::Finalize() {
    for (auto& camera : gamecameras_) {
        camera.reset();
    }
}

void GameCamera::Update() {
    // 移動
    move();
    // コピーと貼り付け
    GameCamertakeaphoto();

    // カメラの更新
    for (auto& camera : gamecameras_) {
        camera->Update();
    }
}

void GameCamera::Draw() {
    // 各カメラの描画
    for (auto& camera : gamecameras_) {
        camera->Draw();
    }
}

void GameCamera::move() {
    // キー入力でマップのインデックスを変更
    if (Input::GetInstance()->TriggerKey(DIK_W)) {
        // マップチップ内に動くよう制限




        if (yIndex > 1) {
            yIndex--;
        }
    }
    if (Input::GetInstance()->TriggerKey(DIK_S)) {
        // マップチップ内に動くよう制限
        if (yIndex < map_->GetMapHeight() - kRenderHeight) {
            yIndex++;
        }
    }
    if (Input::GetInstance()->TriggerKey(DIK_A)) {
        // マップチップ内に動くよう制限
        if (xIndex > 0) {
            xIndex--;
        }
    }
    if (Input::GetInstance()->TriggerKey(DIK_D)) {
        // マップチップ内に動くよう制限
        if (xIndex < map_->GetMapWidth() - kRenderWidth) {
            xIndex++;
        }
    }

    // 新しいマップチップの位置を取得
    position = map_->GetMapChipPostionByIndex(xIndex, yIndex);
    // 各カメラの位置を更新
    for (uint32_t y = 0; y < kRenderHeight; ++y) {
        for (uint32_t x = 0; x < kRenderWidth; ++x) {
            uint32_t index = y * kRenderWidth + x;
            Vector3 offset(x * size.x, y * size.y, size.z);
            Vector3 adjustedPos = position + offset;
            gamecameras_[index]->SetTranslate(adjustedPos);
        }
    }
}

void GameCamera::GameCamertakeaphoto() {
    // 現在位置の取得（マップインデックスではなくカメラ位置を使う）
    for (auto& camera : gamecameras_) {
        Vector3 cameraPos = camera->GetTranslate();
        uint32_t currentXIndex = static_cast<uint32_t>(cameraPos.x);
        uint32_t currentYIndex = static_cast<uint32_t>((map_->GetMapHeight() - 1) - cameraPos.y);

        // 現在位置のマップチップタイプを取得
        MapChipType mapChipType = map_->GetMapChipTypeByIndex(currentXIndex, currentYIndex);

        // SPACEキーでの撮影処理
        if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
            // 現在位置に基づいてモデルを変更
            switch (mapChipType) {
            case MapChipType::kBlank:
                camera->SetModel("axis.obj");
                break;
            case MapChipType::kCopyBlock:
                camera->SetModel("cube.obj");
                break;
            }
        }
    }
}

void GameCamera::GameCameraphoto(std::vector<std::vector<Block*>>& blockGrid) {
    for (auto& camera : gamecameras_) {
        // 現在のカメラ位置を取得
        Vector3 cameraPos = camera->GetTranslate();

        // カメラ位置に基づくインデックス計算
        uint32_t currentXIndex = static_cast<uint32_t>(cameraPos.x);
        uint32_t currentYIndex = static_cast<uint32_t>((map_->GetMapHeight() - 1) - cameraPos.y);

        // 現在位置のマップチップタイプを取得
        MapChipType mapChipType = map_->GetMapChipTypeByIndex(currentXIndex, currentYIndex);

        // ENTERキーでの貼り付け
        if (Input::GetInstance()->TriggerKey(DIK_RETURN)) {
            // 現在のカメラモデルを取得
            const std::string& currentModel = camera->GetModel();

            if (currentModel == "axis.obj") {
                // 空白（削除）
                map_->SetMapData(currentXIndex, currentYIndex, MapChipType::kBlank);

                // その位置にある Block を削除
                if (blockGrid[currentYIndex][currentXIndex]) {
                    delete blockGrid[currentYIndex][currentXIndex]; // メモリ解放
                    blockGrid[currentYIndex][currentXIndex] = nullptr;
                }
            } else if (currentModel == "cube.obj") {
                // ブロック（追加）
                map_->SetMapData(currentXIndex, currentYIndex, MapChipType::kCopyBlock);

                // その位置に Block が存在しない場合は生成
                if (!blockGrid[currentYIndex][currentXIndex]) {
                    Block* newBlock = Block::CreateBlock(MapChipType::kCopyBlock,
                        map_->GetMapChipPostionByIndex(currentXIndex, currentYIndex),map_);
                    blockGrid[currentYIndex][currentXIndex] = newBlock;
                }
            }
        }
    }
}
