#include "ObjectCamera.h"
#include "Map.h"
#include "Object3DCommon.h"
#include "Input.h"
#include <imgui.h>

void ObjectCamera::Initialize(Map* map) {
    this->map_ = map;
    // 開始番号を設定
    xIndex = 5;
    yIndex = 5;
    size = { 1.0f,1.0f,-1.0f };

    // ゲームカメラのオブジェクト数をレンダリング範囲分だけ確保
    gamecameras_.resize(kRenderHeight); // 縦のサイズを最初に確保
    for (uint32_t i = 0; i < kRenderHeight; ++i) {
        gamecameras_[i].resize(kRenderWidth); // 横のサイズを確保
    }

    // 各オブジェクトを生成 & 初期化
    for (uint32_t y = 0; y < kRenderHeight; ++y) {
        for (uint32_t x = 0; x < kRenderWidth; ++x) {
            // マップチップの状態を取得
            MapChipType mapChipType = map_->GetMapChipTypeByIndex(xIndex + x, yIndex + y);

            // オブジェクト生成
            gamecameras_[y][x] = std::make_unique<Object3D>();
            gamecameras_[y][x]->Initialize(Object3DCommon::GetInstance());

            // マップのチップ座標に基づいて配置
            Vector3 position = map_->GetMapChipPostionByIndex(xIndex + x, yIndex + y);
            gamecameras_[y][x]->SetTranslate(position);
            // スケール設定
            gamecameras_[y][x]->SetScale(size);
            // モデル設定
            gamecameras_[y][x]->SetModel("cube.obj");
            // ライティング有効化
            gamecameras_[y][x]->SetLighting(false);
        }
    }
}

void ObjectCamera::Finalize() {
    gamecameras_.clear();
}

void ObjectCamera::Update() {
    // 移動
    move();
    // コピーと貼り付け
    GameCamertakeaphoto();
    GameCameraphoto();

    for (uint32_t y = 0; y < kRenderHeight; ++y) {
        for (uint32_t x = 0; x < kRenderWidth; ++x) {
            // オブジェクトが存在すれば更新
            if (gamecameras_[y][x]) {
                gamecameras_[y][x]->Update();
            }
        }
    }
}

void ObjectCamera::Draw() {
    // 各カメラの描画
    for (uint32_t y = 0; y < kRenderHeight; ++y) {
        for (uint32_t x = 0; x < kRenderWidth; ++x) {
            // オブジェクトが存在すれば更新
            if (gamecameras_[y][x]) {
                gamecameras_[y][x]->Draw();
            }
        }
    }
}

void ObjectCamera::move() {
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
            gamecameras_[y][x]->SetTranslate(adjustedPos);
        }
    }
}

void ObjectCamera::GameCamertakeaphoto() {
    // カメラごとの処理
    for (uint32_t y = 0; y < kRenderHeight; ++y) {
        for (uint32_t x = 0; x < kRenderWidth; ++x) {
            // 現在位置を取得（カメラ位置を使用）
            Vector3 cameraPos = gamecameras_[y][x]->GetTranslate();
            uint32_t currentXIndex = static_cast<uint32_t>(cameraPos.x);
            uint32_t currentYIndex = static_cast<uint32_t>(map_->GetMapHeight() - 1 - cameraPos.y);

            // 現在位置のマップチップタイプを一度だけ取得
            MapChipType mapChipType = map_->GetMapChipTypeByIndex(currentXIndex, currentYIndex);

            //コピペ不可能ブロックの場合スキップ
            if (mapChipType == MapChipType::kNCopyBlock) {
                continue;
            }

            // SPACEキーでの撮影処理
            if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
                // マップチップタイプに基づくモデル設定
                auto modelIter = mapChipTypemodelMap.find(mapChipType);
                if (modelIter != mapChipTypemodelMap.end()) {
                    // モデル名を取得し設定
                    gamecameras_[y][x]->SetModel(modelIter->second);
                }
            }
        }
    }
}

void ObjectCamera::GameCameraphoto() {
    for (uint32_t y = 0; y < kRenderHeight; ++y) {
        for (uint32_t x = 0; x < kRenderWidth; ++x) {
            // カメラの位置を取得
            Vector3 cameraPos = gamecameras_[y][x]->GetTranslate();
            uint32_t currentXIndex = static_cast<uint32_t>(cameraPos.x);
            uint32_t currentYIndex = static_cast<uint32_t>((map_->GetMapHeight() - 1) - cameraPos.y);

            // 範囲チェック
            if (currentXIndex >= map_->GetNumBlockHorizontal() || currentYIndex >= map_->GetNumBlockVirtical()) {
                continue; // 範囲外ならスキップ
            }

            // ENTERキーが押されたときの処理
            if (Input::GetInstance()->TriggerKey(DIK_RETURN)) {
                // カメラのモデルを取得
                std::string model = gamecameras_[y][x]->GetModel();

                // モデルマップから現在のモデルの対応するマップチップタイプを取得
                auto mapChipIter = std::find_if(mapChipTypemodelMap.begin(), mapChipTypemodelMap.end(),
                    [&](const std::pair<MapChipType, std::string>& pair) {
                        return pair.second == model;
                    });

                 // モデルがマップに存在する場合
                if (mapChipIter != mapChipTypemodelMap.end()) {
                    MapChipType mapChipType = mapChipIter->first;

                    //現在のマップチップ取得
                    MapChipType existingType = map_->GetMapChipTypeByIndex(currentXIndex, currentYIndex);

                    //kNCopyBlockを消せない
                    if (existingType == MapChipType::kNCopyBlock) {
                        continue;
                    }

                    //コピペ不可能ブロックの場合スキップ
                    if (mapChipType == MapChipType::kNCopyBlock) {
                        continue;
                    }

                    // 生成、削除
                    HandleMapChip(currentXIndex, currentYIndex,mapChipType);
                }
            }
        }
    }
}

void ObjectCamera::HandleMapChip(uint32_t currentXIndex, uint32_t currentYIndex, MapChipType mapChipType) {
    // マップチップタイプに対応する処理を関数ポインタで定義
    std::unordered_map<MapChipType, std::function<void(uint32_t, uint32_t)>> actions = {
        { MapChipType::kBlank, [this](uint32_t x, uint32_t y) { map_->RemoveObjectAt(x, y); } },
        { MapChipType::kCopyBlock, [this](uint32_t x, uint32_t y) { map_->GenerateObjectAt(x, y, MapChipType::kCopyBlock); } },

		{ MapChipType::kFixedTimeBlock, [this](uint32_t x, uint32_t y) { map_->GenerateObjectAt(x, y, MapChipType::kFixedTimeBlock); } },

        {MapChipType::kFallBlock,[this](uint32_t x,uint32_t y) {map_->GenerateObjectAt(x,y,MapChipType::kFallBlock); }}

        // 必要な処理を追加していく
    };

    // 該当するマップチップタイプに対応する処理を呼び出す
    if (actions.find(mapChipType) != actions.end()) {
        actions[mapChipType](currentXIndex, currentYIndex);
    }
}