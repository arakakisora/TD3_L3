#include "PhotoCamera.h"
#include "Object3DCommon.h"
#include "Input.h"
#include <imgui.h>
#include <iostream>
#include "TextureManager.h"
#include "SpriteCommon.h"
#include "Easing.h"
#include "ModelManager.h"

// MAPクラスとのループキャストに注意
void PhotoCamera::Initialize(Map* map)
{
	this->map = map;
	// フォトカメラの範囲モデル
	object3D = make_unique<Object3D>();
	object3D->Initialize(Object3DCommon::GetInstance());
	// @枠組みのモデルを用意するように
	object3D->SetModel("Frame.obj");
	// @値を後に調整する
	object3D->SetScale(Vector3{ 1.0f,1.0f,1.0f });
	position = Vector2{ 2,13 };
	object3D->SetTranslate(Vector3(position.x, position.y - 1, -30.0f));
	object3D->SetRotate(Vector3{ 0,0,0 });

	initialPos = position;

	isFirstCopied = false;
	isFirstPasted = false;

	ModelManager::GetInstans()->LoadModel("shutterEffect.obj");
	//sahtter演出用のオブジェクト
	shuttertopObject = new Object3D();
	shuttertopObject->Initialize(Object3DCommon::GetInstance());
	shuttertopObject->SetModel("shutterEffect.obj");
	//サイズは画面いっぱいにする
	shuttertopObject->SetScale(Vector3{ 13.0f,6.0f,1.0f });
	shuttertopObject->SetTranslate(Vector3(12.5f, 30.0f, -1.5f));
	shuttertopObject->SetRotate(Vector3{ 0,0,0 });
	//bottm
	shutterbottomObject = new Object3D();
	shutterbottomObject->Initialize(Object3DCommon::GetInstance());
	shutterbottomObject->SetModel("shutterEffect.obj");
	//サイズは画面いっぱいにする
	shutterbottomObject->SetScale(Vector3{ 13.0f,6.0f,1.0f });
	shutterbottomObject->SetTranslate(Vector3(12.5f, -30.0f, -1.5f));
	shutterbottomObject->SetRotate(Vector3{ 0,0,0 });
	

	



	// 残りシャッター枚数表示画像
	TextureManager::GetInstance()->LoadTexture("Resources/shutter.png");

	// フォトカメラのシャッター回数
	shutterLimitCountMax = this->map->GetShutterCount();
	// 残りシャッター枚数表示スプライト
	for (int i = 0; i < (int)shutterLimitCountMax; ++i) {
		auto shutter_ = make_unique<Sprite>();
		shutter_->Initialize(SpriteCommon::GetInstance(), "Resources/shutter.png");
		shutter_->SetSize({ 80.0f,80.0f });
		shutter_->SetRotation(0.0f);
		shutter_->SetPosition({ 50.0f,20.0f });
		shutter_->setColor({ 1.0f,1.0f,1.0f,1.0f });
		shutterRests_.push_back(move(shutter_));
	}

	currentPos = targetPos = position;
	moveTimer = 1.0f;
	isMoving = false;

	// ビットマップフォント
	bitmapFont = make_unique<BitmapFont>();
	bitmapFont->Initialize();

	// 移動用サウンド
	moveSound = Audio::GetInstance()->SoundLoadWave("Resources/Audio/Select.wav");
	// コピー用サウンド
	copeSound = Audio::GetInstance()->SoundLoadWave("Resources/Audio/Camera_copy.wav");
	// ペースト用サウンド
	pasteSound = Audio::GetInstance()->SoundLoadWave("Resources/Audio/Camera_paste.wav");
}

void PhotoCamera::Update(Map* map, const bool cameraMode)
{
	this->map = map;
	this->cameraMode_ = cameraMode;
	// フォトカメラの範囲
	cameraSizeX = this->map->GetkameraSizeX();
	cameraSizeY = this->map->GetkameraSizeY();
	// フォトカメラのシャッター回数
	shutterLimitCountMax = this->map->GetShutterCount();

	// mapDataを受け取る
	mapData.data = this->map->GetMap();
	// シャッターの残り枚数表示画像の更新
	for (auto& shutter : shutterRests_) {
		shutter->Update();
	}
	
	shutterEffectUpdate();	
	//sahtter演出用のオブジェクト
	shuttertopObject->Update();
	shutterbottomObject->Update();


	// 音量設定
	Audio::GetInstance()->SetVolume(&moveSound, 0.2f);
	Audio::GetInstance()->SetVolume(&copeSound, 0.2f);
	Audio::GetInstance()->SetVolume(&pasteSound, 1.0f);

	if (CamerMode) {
		// フォトカメラの移動
		Move();


		// フォトカメラのコピー / スペースキーを押したら
		if (
#ifdef _DEBUG
			Input::GetInstance()->TriggerKey(DIK_SPACE) ||
#endif // DEBUG
			Input::GetInstance()->TriggerGamePadButton(XINPUT_GAMEPAD_X)
			)//LB
		{
			shatterEffect();
			Copy();
			for (uint32_t y = 0; y < cameraSizeY; ++y) {
				for (uint32_t x = 0; x < cameraSizeX; ++x) {
					// コピーしたマップデータの描画用Blockクラスの位置
					Vector3 blockPosition = Vector3(currentPos.x + x, currentPos.y - y, -1.0F);
					// コピーしたマップデータの描画用Blockクラスのマップチップタイプ
					MapChipType mapChipType = copyData[y][x];
					// コピーしたマップデータの描画用Blockクラスのマップチップタイプが空白でないとき
					if (mapChipType != MapChipType::kBlank) {
						// ブロックのインスタンスを生成する
						Block* block = new Block();
						block->Initialize(mapChipType, blockPosition, this->map);
						//block->SetPosition(blockPosition);
						blocks.push_back(block);
					} else if (mapChipType == MapChipType::kBlank) {
						// コピーしたマップデータの描画用Blockクラスのマップチップタイプが空白のとき
						// ブロックのインスタンスを生成する
						Block* block = new Block();
						block->Initialize(MapChipType::kBlank, blockPosition, this->map);
						blocks.push_back(block);
					}
				}
			}
			// コピーサウンド開始
			Audio::GetInstance()->SoundPlayWave(copeSound);
		}
		// フォトカメラのペースト / Pキーを押したら

		if (
#ifdef _DEBUG
			Input::GetInstance()->TriggerKey(DIK_P) ||
#endif // _DEBUG

			Input::GetInstance()->TriggerGamePadButton(XINPUT_GAMEPAD_Y)) {//RB


			// シャッター上限に達していったら使用不可
			if (shutterCount >= shutterLimitCountMax) {
				// シャッター上限に達しているのでペースト不可
				return;
			}
			// ペーストサウンド開始
			Audio::GetInstance()->SoundPlayWave(pasteSound);
			Paste();
		}
	}
	// ビットマップフォントの更新処理
	bitmapFont->Update(shutterLimitCountMax - shutterCount);
	// フォトカメラの枠モデルの更新
	object3D->Update();

	// 生成されたブロックの更新
	for (auto& block : blocks) {
		block->Update(cameraMode_);
	}

#ifdef _DEBUG
	// ImGuiの描画
	DrawImGui();
#endif // _DEBUG


	// 変更したmapDataをmapにセット / mapクラスに送り返し更新させる
	this->map->SetMap(mapData);


}

void PhotoCamera::Draw3DObject()
{
	if (CamerMode) {
		// フォトカメラの枠モデルの描画
		object3D->Draw();
		// 生成されたブロックの描画
		for (auto& block : blocks) {
			block->Draw();
		}
	}
	//shutter演出用のオブジェクト
	shuttertopObject->Draw();
	shutterbottomObject->Draw();

	

}

void PhotoCamera::DrawSprite()
{
	int remainingShutter = shutterLimitCountMax - shutterCount;

	// 表示するのは1枚だけ
	if (remainingShutter >= 0 && !shutterRests_.empty()) {
		float x = 65.0f;
		float y = 20.0f;
		shutterRests_[0]->SetPosition(Vector2(x, y));
		shutterRests_[0]->Draw();
	}

	// ビットマップフォントのスプライト描画
	bitmapFont->Draw();
}



void PhotoCamera::Finalize()
{
	// 生成されたブロックの終了処理
	for (auto& block : blocks) {
		block->Finalize();
		delete block;
	}
	blocks.clear();

	// フォトカメラの枠モデルの終了処理
	if (object3D) {
		object3D.reset();
	}
	delete shuttertopObject;
	delete shutterbottomObject;
}



void PhotoCamera::Move()
{

	Vector2 input = { 0, 0 };

#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_W)) {
		input.y++;
	} else if (Input::GetInstance()->TriggerKey(DIK_S)) {
		input.y--;
	} else if (Input::GetInstance()->TriggerKey(DIK_D)) {
		input.x++;
	} else if (Input::GetInstance()->TriggerKey(DIK_A)) {
		input.x--;
	}
#endif // _DEBUG

	if (Input::GetInstance()->TriggerGamePadButton(XINPUT_GAMEPAD_DPAD_UP)) {
		input.y++;
	} else if (Input::GetInstance()->TriggerGamePadButton(XINPUT_GAMEPAD_DPAD_DOWN)) {
		input.y--;
	} else if (Input::GetInstance()->TriggerGamePadButton(XINPUT_GAMEPAD_DPAD_RIGHT)) {
		input.x++;
	} else if (Input::GetInstance()->TriggerGamePadButton(XINPUT_GAMEPAD_DPAD_LEFT)) {
		input.x--;
	}

	if ((input.x != 0 || input.y != 0) && !isMoving) {
		Vector2 nextTargetPos = targetPos;
		nextTargetPos.x += input.x;
		nextTargetPos.y += input.y;

		// 固定された範囲で移動制限
		if (nextTargetPos.x >= 1 && nextTargetPos.x <= 22 &&
			nextTargetPos.y >= 13 && nextTargetPos.y <= 23) {
			targetPos = nextTargetPos;
			moveTimer = 0.0f;
			isMoving = true;
		}
	}



	// スティック移動
	stickMove();



	// イージング補間
	if (isMoving) {
		moveTimer += moveSpeed;
		if (moveTimer >= 1.0f) {
			moveTimer = 1.0f;
			isMoving = false;
			currentPos = targetPos;
		} else {
			currentPos = Easing::EaseLerp(currentPos, targetPos, moveTimer, Easing::EaseOutQuad);
		}
	}


	// イージング結果を object3D に反映
	object3D->SetTranslate(Vector3(currentPos.x, currentPos.y, 0));

	// photo_ConvertYの代わりにposition.yをそのまま使用
	for (size_t i = 0; i < blocks.size(); ++i) {
		uint32_t x = static_cast<uint32_t>(i % cameraSizeX);
		uint32_t y = static_cast<uint32_t>(i / cameraSizeX);
		Vector3 blockPosition = Vector3(currentPos.x + x, currentPos.y - y, -1.0F);
		blocks[i]->SetObject3DPosiition(blockPosition);
	}
	position = targetPos;

}

void PhotoCamera::stickMove()
{

	static int stickCoolTimeX = 0;
	static int stickCoolTimeY = 0;

	float stickX = Input::GetInstance()->GetGamePadStickX();
	float stickY = Input::GetInstance()->GetGamePadStickY();

	const float threshold = 0.5f;
	const int maxCoolTime = 10;

	Vector2 nextTargetPos = targetPos;

	// X方向
	if (std::abs(stickX) > threshold) {
		if (stickCoolTimeX <= 0 && !isMoving) {
			nextTargetPos.x += (stickX > 0) ? 1 : -1;

			if (nextTargetPos.x >= 1 && nextTargetPos.x <= 22) {
				targetPos.x = nextTargetPos.x;
				moveTimer = 0.0f;
				isMoving = true;
			}
			// 移動サウンド開始
			Audio::GetInstance()->SoundPlayWave(moveSound);
			stickCoolTimeX = maxCoolTime;
		} else {
			stickCoolTimeX--;
		}
	} else {
		stickCoolTimeX = 0;
	}

	// Y方向
	if (std::abs(stickY) > threshold) {
		if (stickCoolTimeY <= 0 && !isMoving) {
			nextTargetPos.y += (stickY > 0) ? 1 : -1;

			if (nextTargetPos.y >= 13 && nextTargetPos.y <= 23) {
				targetPos.y = nextTargetPos.y;
				moveTimer = 0.0f;
				isMoving = true;
			}
			// 移動サウンド開始
			Audio::GetInstance()->SoundPlayWave(moveSound);
			stickCoolTimeY = maxCoolTime;
		} else {
			stickCoolTimeY--;
		}
	} else {
		stickCoolTimeY = 0;
	}





	// イージング結果を object3D に反映
	object3D->SetTranslate(Vector3(currentPos.x, currentPos.y, -1.0f));

	// photo_ConvertYの代わりにposition.yをそのまま使用
	for (size_t i = 0; i < blocks.size(); ++i) {
		uint32_t x = static_cast<uint32_t>(i % cameraSizeX);
		uint32_t y = static_cast<uint32_t>(i / cameraSizeX);
		Vector3 blockPosition = Vector3(currentPos.x + x, currentPos.y - y, -1.0F);
		blocks[i]->SetObject3DPosiition(blockPosition);
	}
	position = targetPos;


}
void PhotoCamera::Copy() {
	// マップデータが読み込めていないときはコピー不可
	if (!map) return;
	for (auto& block : blocks) {
		block->Finalize();
		delete block;
	}
	blocks.clear();

	// 2x2 のマップチップ番号をコピー
	copyData.clear();
	blocks.clear();

	for (uint32_t y = 0; y < cameraSizeY; y++) {
		vector<MapChipType> row;
		for (uint32_t x = 0; x < cameraSizeX; x++) {
			// マップの座標変換を適切に行う
			int targetX = static_cast<int>(position.x) + x;
			int targetY = static_cast<int>(Map::kNumBlockVirtical - position.y - 1) + y;

			MapChipType type = mapData.data[targetY][targetX];

			// マップの範囲外をチェック
			if (targetX < 0 || targetY < 0 || targetX >= mapData.data[0].size() || targetY >= mapData.data.size()) {
				row.push_back(MapChipType::kBlank);
			} else if (type == MapChipType::kBlank) {
				row.push_back(MapChipType::kBlank);
			} else if (type == MapChipType::kGoalUp) {
				row.push_back(MapChipType::kBlank);
			} else if (type == MapChipType::kGoalDown) {
				row.push_back(MapChipType::kBlank);
			} else if (type == MapChipType::kNCopyBlock) {
				row.push_back(MapChipType::kBlank);
			} else if (type == MapChipType::kPutFixedTimeBlock) {
				row.push_back(MapChipType::kFixedTimeBlock);
			} else {
				type = mapData.data[targetY][targetX];
				row.push_back(type);
			}
		}
		copyData.push_back(row);
	}
	//初回コピーがしたか
	isFirstCopied = true;
}

void PhotoCamera::Paste()
{
	// マップデータが読み込めていないときはペースト不可
	if (!map) return;
	// コピーデータがないときはペースト不可
	if (copyData.empty()) return;

	// 貼り付けたかどうかのフラグ
	bool ispasted = false;

	// コピーデータをマップデータにペースト
	for (uint32_t y = 0; y < cameraSizeY; ++y) {
		for (uint32_t x = 0; x < cameraSizeX; ++x) {
			MapChipType type = copyData[y][x];

			// コピー元のデータが貼り付け禁止またはゴールならスキップ
			if (type == MapChipType::kNCopyBlock ||
				type == MapChipType::kGoalUp ||
				type == MapChipType::kGoalDown) {
				continue;
			}

			int positionX = static_cast<int>(position.x) + x;
			int positionY = static_cast<int>(Map::kNumBlockVirtical - position.y - 1) + y;

			// マップの範囲外チェック
			if (positionX < 0 || positionY < 0 ||
				positionX >= mapData.data[0].size() ||
				positionY >= mapData.data.size()) {
				continue;
			}

			// 貼り付け先が貼り付け禁止ブロック or ゴールならスキップ
			MapChipType target = mapData.data[positionY][positionX];
			if (target == MapChipType::kNCopyBlock ||
				target == MapChipType::kGoalUp ||
				target == MapChipType::kGoalDown) {
				continue;
			}

			// 貼り付け実行
			if (type == MapChipType::kFixedTimeBlock) {
				mapData.data[positionY][positionX] = MapChipType::kPutFixedTimeBlock;
			} else {
				mapData.data[positionY][positionX] = type;
			}
			ispasted = true;
		}
	}

	// 一つでも貼り付けていたら、マップ更新とカウンタ更新
	if (ispasted) {
		map->SetMap(mapData);
		shutterCount++;
		isFirstPasted = true;
	}
}


void PhotoCamera::DrawImGui()
{
	// ImGuiの描画コードを追加
	ImGui::Begin("PhotoCamera Data");

	// カメラの現在位置を表示
	ImGui::Text("Camera Position: (%.2f, %.2f)", position.x, position.y);
	ImGui::Text("Camera ConvertY: %d", photo_ConvertY);
	ImGui::Separator();

	//イージング用
	ImGui::DragFloat("Move Speed", &moveSpeed, 0.01f, 0.0f, 1.0f);
	//movetimer
	ImGui::DragFloat("Move Timer", &moveTimer, 0.01f, 0.0f, 1.0f);


	// カメラのサイズを表示
	ImGui::Text("Camera Size: (%d, %d)", cameraSizeX, cameraSizeY);
	// カメラのシャッター回数を表示
	ImGui::Text("Shutter Count: %d / %d", shutterCount, shutterLimitCountMax);
	// カメラのモードを表示
	ImGui::Text("Camera Mode: %s", CamerMode ? "On" : "Off");
	ImGui::Separator();

	// コピーしたマップチップタイプを表示
	if (!copyData.empty()) {
		for (int y = 0; y < 2; y++) {
			for (int x = 0; x < 2; x++) {
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
	} else {
		ImGui::Text("No map chip data copied.");
	}

	ImGui::End();

	ImGui::Begin("ShutterCountSprite");

	// シャッター回数の情報を表示
	ImGui::Text("Shutter Count Information");
	ImGui::Separator();
	ImGui::Text("Current Shutter Count: %d", shutterCount);
	ImGui::Text("Shutter Limit: %d", shutterLimitCountMax);
	ImGui::Text("Remaining Shutter Count: %d", shutterLimitCountMax - shutterCount);

	// プログレスバーで残りシャッター回数を視覚化
	float progress = static_cast<float>(shutterCount) / static_cast<float>(shutterLimitCountMax);
	ImGui::ProgressBar(progress, ImVec2(0.0f, 0.0f), "Usage");

	ImGui::End();

	//shatter演出用のオブジェクトImGui
	ImGui::Begin("ShutterTopObject");


	

	if (ImGui::CollapsingHeader("shtter", ImGuiTreeNodeFlags_DefaultOpen)) {


		//Transformのドラッグ
		
		Transform transformtop = shuttertopObject->GetTransform();
		if (ImGui::DragFloat3("obj3Position", &transformtop.translate.x, 0.01f)) {
			shuttertopObject->SetTransform(transformtop);
		}
		//rotation
		if (ImGui::DragFloat3("obj3Rotation", &transformtop.rotate.x, 0.01f)) {
			shuttertopObject->SetTransform(transformtop);
		}
		//scale
		if (ImGui::DragFloat3("obj3Scale", &transformtop.scale.x, 0.01f)) {
			shuttertopObject->SetTransform(transformtop);
		}
		
		Transform transformbot = shutterbottomObject->GetTransform();
		if (ImGui::DragFloat3("Position", &transformbot.translate.x, 0.01f)) {
			shutterbottomObject->SetTransform(transformbot);
		}
		//rotation
		if (ImGui::DragFloat3("Rotation", &transformbot.rotate.x, 0.01f)) {
			shutterbottomObject->SetTransform(transformbot);
		}
		//scale
		if (ImGui::DragFloat3("Scale", &transformbot.scale.x, 0.01f)) {
			shutterbottomObject->SetTransform(transformbot);
		}


	}

	//effectをかけるボタン
	if (ImGui::Button("ShutterEffect")) {
		shatterEffect();
	}

	ImGui::End();

}

void PhotoCamera::shatterEffect()
{
	if (isShutterEffectPlaying) return;

	isShutterEffectPlaying = true;
	shutterAnimTime = 0.0f;

	// 初期位置にリセット
	shuttertopObject->SetTranslate(Vector3(12.5f, 30.0f, -1.5f));
	shutterbottomObject->SetTranslate(Vector3(12.5f, 4.0f, -1.5f));

	

}

void PhotoCamera::shutterEffectUpdate()
{
	if (!isShutterEffectPlaying) return;

	shutterAnimTime += 0.01f; // ← deltaTime に置き換えOK
	float t = shutterAnimTime / shutterAnimDuration;
	t = std::min(t, 1.0f);

	// 閉じる：Top 30 → 17, Bottom 5 → -17
	// 開く  ：Top 17 → 30, Bottom -17 → 5
	float topY, bottomY;

	if (t < 0.5f) {
		// 閉じるフェーズ（0.0〜0.5）
		float p = t / 0.5f;
		topY = Easing::EaseLerp(30.0f, 20.0f, p, Easing::EaseOutQuad);
		bottomY = Easing::EaseLerp(4.0f, 12.0f, p, Easing::EaseOutQuad);
	} else {
		// 開くフェーズ（0.5〜1.0）
		float p = (t - 0.5f) / 0.5f;
		topY = Easing::EaseLerp(20.0f, 30.0f, p, Easing::EaseInQuad);
		bottomY = Easing::EaseLerp(12.0f, 4.0f, p, Easing::EaseInQuad);
	}

	shuttertopObject->SetTranslate(Vector3(12.5f, topY, -1.5f));
	shutterbottomObject->SetTranslate(Vector3(12.5f, bottomY, -1.5f));

	if (t >= 1.0f) {
		isShutterEffectPlaying = false;
	}

	//for (int i = 0; i < kShutterBladeCount; ++i) {
	//	float angle = (360.0f / kShutterBladeCount) * i;
	//	float rad = DirectX::XMConvertToRadians(angle);

	//	float radius = Easing::EaseLerp(10.0f, 0.0f, t, Easing::EaseInOutQuad); // 中心に集まる

	//	Vector3 pos = {
	//		std::cos(rad) * radius + 12.5f,
	//		std::sin(rad) * radius + 17.5f,
	//		-1.5f
	//	};
	//	float bladeAngle = Easing::EaseLerp(angle, angle + 30.0f, t, Easing::EaseInOutQuad); // 回転

	//	shutterBlades_[i]->SetTranslate(pos);
	//	shutterBlades_[i]->SetRotate(Vector3{ 0, 0, bladeAngle });
	//}

	//if (t >= 1.0f) {
	//	isShutterEffectPlaying = false;
	//}



}
