#include "ObjectCamera.h"
#include "Map.h"
#include "Object3DCommon.h"
#include "Input.h"
#include <imgui.h>

void ObjectCamera::Initialize(Map* map) {
    this->map_ = map;
    // 開始番号を設定
    xIndex = 7;
    yIndex = 16;
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
    // ImGuiウィンドウの開始
    ImGui::Begin("Model Map Editor");

    // modelMapの各要素に対してUIを作成
    for (auto& pair : mapChipTypemodelMap) {
        // マップチップタイプとモデル名のペア
        MapChipType mapChipType = pair.first;
        std::string& modelName = pair.second;

        // MapChipTypeに基づくラベル表示
        const char* chipTypeLabel = nullptr;
        switch (mapChipType) {
        case MapChipType::kBlank: chipTypeLabel = "Blank"; break;
        case MapChipType::kCopyBlock: chipTypeLabel = "CopyBlock"; break;
            // 他のMapChipTypeに対応するラベルを追加
        default: chipTypeLabel = "Unknown"; break;
        }

        // ドロップダウンリストを作成して、モデル名を選択できるようにする
        std::vector<std::string> modelChoices = { "axis.obj", "cube.obj", "sphere.obj" }; // 例: モデルの選択肢
        int selectedIndex = -1;

        // 現在のモデル名のインデックスを選択肢から探す
        for (int i = 0; i < modelChoices.size(); ++i) {
            if (modelChoices[i] == modelName) {
                selectedIndex = i;
                break;
            }
        }

        // ドロップダウンメニューを表示
        if (ImGui::BeginCombo(chipTypeLabel, modelName.c_str())) {
            for (int i = 0; i < modelChoices.size(); ++i) {
                // 選択肢を表示
                bool isSelected = (selectedIndex == i);
                if (ImGui::Selectable(modelChoices[i].c_str(), isSelected)) {
                    // モデルが選択されたときに、modelNameを更新
                    modelName = modelChoices[i];
                }

                // 最初の選択肢を表示状態にする
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
    }

    // 変更を保存するボタン
    if (ImGui::Button("Save Changes")) {
        // モデルの変更を保存する処理（必要に応じて）
        // 例えば、ゲームシーンに反映したり、ファイルに保存したり
    }

    ImGui::End();
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
        { MapChipType::kCopyBlock, [this](uint32_t x, uint32_t y) { map_->GenerateObjectAt(x, y, MapChipType::kCopyBlock); } }
        // 必要な処理を追加していく
    };

    // 該当するマップチップタイプに対応する処理を呼び出す
    if (actions.find(mapChipType) != actions.end()) {
        actions[mapChipType](currentXIndex, currentYIndex);
    }
}