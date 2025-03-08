#include "GameCamera.h"
#include "Map.h"
#include "Object3DCommon.h"
#include "Input.h"

void GameCamera::Initialize(Map* map) {
    this->map_ = map;

    // ゲームカメラのオブジェクト数をレンダリング範囲分だけ確保
    gamecameras_.resize(kRenderWidth * kRenderHeight);
    // 各オブジェクトを生成 & 初期化
    for (uint32_t y = 0; y < kRenderHeight; ++y) {
        for (uint32_t x = 0; x < kRenderWidth; ++x) {
            uint32_t index = y * kRenderWidth + x; // 2D を 1D インデックスに変換
            gamecameras_[index] = std::make_unique<Object3D>();
            gamecameras_[index]->Initialize(Object3DCommon::GetInstance());
            // 位置設定
            Vector3 position(x * 1.0f, y * 1.0f, 0.0f); // X, Y を配置
            gamecameras_[index]->SetTranslate(position);
            // スケール設定
            gamecameras_[index]->SetScale(Vector3(1.0f, 1.0f, 1.0f));
            // モデル設定
            gamecameras_[index]->SetModel("cube.obj");
            // ライティング有効化
            gamecameras_[index]->SetLighting(true);
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
    if (Input::GetInstans()->TriggerKey(DIK_W)) {
        if (yIndex > 0) { // 範囲チェック
            yIndex--;
        }
    }
    if (Input::GetInstans()->TriggerKey(DIK_S)) {
        if (yIndex < map_->GetMapWidth() - 1) { // 範囲チェック
            yIndex++;
        }
    }
    if (Input::GetInstans()->TriggerKey(DIK_A)) {
        if (xIndex > 0) { // 範囲チェック
            xIndex--;
        }
    }
    if (Input::GetInstans()->TriggerKey(DIK_D)) {
        if (xIndex < map_->GetMapWidth() - 1) { // 範囲チェック
            xIndex++;
        }
    }

    // 新しいマップチップの位置を取得
    Vector3 newPos = map_->GetMapChipPostionByIndex(xIndex, yIndex);

    // gamecameras_ のすべてのオブジェクトを移動
    for (uint32_t y = 0; y < kRenderHeight; ++y) {
        for (uint32_t x = 0; x < kRenderWidth; ++x) {
            uint32_t index = y * kRenderWidth + x; // 2D を 1D インデックスに変換
            Vector3 offset(x * 1.0f, y * 1.0f, 0.0f);  // x, y ごとにずらす
            Vector3 adjustedPos = newPos + offset; // ずらした位置を設定
            // カメラに新しい位置を設定
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
        if (Input::GetInstans()->TriggerKey(DIK_SPACE)) {
            // 現在位置に基づいてモデルを変更
            switch (mapChipType) {
            case MapChipType::kBlank:
                camera->SetModel("axis.obj");
                break;
            case MapChipType::kBlock:
                camera->SetModel("cube.obj");
                break;
            }
        }
    }
}

void GameCamera::GameCameraphoto(std::vector<std::vector<Object3D*>>& blockobject3D) {
    for (auto& camera : gamecameras_) {
        // 現在のカメラ位置を取得
        Vector3 cameraPos = camera->GetTranslate();

        // カメラ位置に基づくインデックス計算
        uint32_t currentXIndex = static_cast<uint32_t>(cameraPos.x);
        uint32_t currentYIndex = static_cast<uint32_t>((map_->GetMapHeight() - 1) - cameraPos.y);

        // 現在位置のマップチップタイプを取得
        MapChipType mapChipType = map_->GetMapChipTypeByIndex(currentXIndex, currentYIndex);

        // ENTERキーでの貼り付け
        if (Input::GetInstans()->TriggerKey(DIK_RETURN)) {
            // 現在のカメラモデルを取得
            const std::string& currentModel = camera->GetModel();

            // モデルに基づいてマップデータを変更
            if (currentModel == "axis.obj") {
                // 空白（削除）
                map_->SetMapData(currentXIndex, currentYIndex, MapChipType::kBlank);

                // その位置にある Object3D のモデルを空白に変更
                if (blockobject3D[currentYIndex][currentXIndex]) {
                    delete blockobject3D[currentYIndex][currentXIndex]; // メモリ解放
                    blockobject3D[currentYIndex][currentXIndex] = nullptr;
                }
            } else if (currentModel == "cube.obj") {
                // ブロック（追加）
                map_->SetMapData(currentXIndex, currentYIndex, MapChipType::kBlock);

                // その位置に Object3D が存在しない場合は生成
                if (!blockobject3D[currentYIndex][currentXIndex]) {
                    Object3D* newObject = new Object3D();
                    newObject->Initialize(Object3DCommon::GetInstance());
                    newObject->SetModel("cube.obj");
                    newObject->SetTranslate(map_->GetMapChipPostionByIndex(currentXIndex, currentYIndex));
                    blockobject3D[currentYIndex][currentXIndex] = newObject;
                } else {
                    // 既存のオブジェクトがあればモデルを更新
                    blockobject3D[currentYIndex][currentXIndex]->SetModel("cube.obj");
                }
            }
        }
    }
}
