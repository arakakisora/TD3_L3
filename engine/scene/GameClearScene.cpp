#include "GameClearScene.h"
#include "Object3DCommon.h"
#include "SpriteCommon.h"
#include "ImGuiManager.h"
#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG
#include "Input.h"
#include "SceneManager.h"
#include "CameraManager.h"
#include <ModelManager.h>
#include <TextureManager.h>

void GameClearScene::Initialize()
{
	CameraManager::GetInstans()->Initialize();

	ModelManager::GetInstans()->LoadModel("GameClear/ClearText_01.obj");
	ModelManager::GetInstans()->LoadModel("GameClear/ClearText_02.obj");
	ModelManager::GetInstans()->LoadModel("GameClear/ClearText_03.obj");

	velocity_.resize(MaxtextIndex_); // すべてのオブジェクトに対応するサイズに設定

	// クリアしたステージのindex
	nextStage = SceneManager::GetInstance()->GetStageIndex() + 1;

	// 作成してリストに追加
	for (uint32_t i = 0; i < MaxtextIndex_; ++i) {
		std::unique_ptr<Object3D> newObject = std::make_unique<Object3D>();
		newObject->Initialize(Object3DCommon::GetInstance());
		if (i == 0) {
			newObject->SetModel("GameClear/ClearText_01.obj");
		} else if (i == 1) {
			newObject->SetModel("GameClear/ClearText_02.obj");
		} else if (i == 2) {
			newObject->SetModel("GameClear/ClearText_03.obj");
		}
		newObject->SetTranslate(Vector3(-0.5f + (0.5f * i), 0.3f, 10.0f));
		newObject->SetScale(Vector3(0.0f, 0.0f, 0.0f));
		newObject->SetLighting(false);
		Cleartext_.push_back(std::move(newObject));

		objecttime_[i] = i * 1.0f;
		startDelay[i] = 0.0f;

		velocity_[i] = Vector3(0.0f, 0.0f, 0.0f); // 初期速度はゼロに設定（x, y, z）
	}


	TextureManager::GetInstance()->LoadTexture("Resources/GameClear/TextUI_Nextstage.png");
	TextureManager::GetInstance()->LoadTexture("Resources/GameClear/TextUI_Stageselect.png");
	TextureManager::GetInstance()->LoadTexture("Resources/GameClear/ArroUP.png");

	// 作成してリストに追加
	for (uint32_t i = 0; i < 3; ++i) {
		std::unique_ptr<Sprite> newSprite = std::make_unique<Sprite>();

		if (i == 0) {
			newSprite->Initialize(SpriteCommon::GetInstance(), "Resources/GameClear/TextUI_Title.png");
			newSprite->SetPosition(Vector2(250.0f, 700.0f));

		} else if (i == 1) {
			newSprite->Initialize(SpriteCommon::GetInstance(), "Resources/GameClear/TextUI_Stageselect.png");
			newSprite->SetPosition(Vector2(550.0f, 700.0f));
		} else if (i == 2) {
			newSprite->Initialize(SpriteCommon::GetInstance(), "Resources/GameClear/TextUI_Nextstage.png");
			newSprite->SetPosition(Vector2(850.0f, 700.0f));
		}

		newSprite->SetSize({ 200.0f, 50.0f });
		newSprite->SetRotation(0.0f);
		newSprite->setColor({ 1.0f, 1.0f, 1.0f, 1.0f });

		TextUI_.push_back(std::move(newSprite));
	}

	ArroTextUI_ = std::make_unique<Sprite>();

	ArroTextUI_->Initialize(SpriteCommon::GetInstance(), "Resources/GameClear/ArroUP.png");

	ArroTextUI_->SetPosition(Vector2(925.0f, 570.0f));
	ArroTextUI_->SetSize({ 50.0f, 50.0f });
	ArroTextUI_->SetRotation(0.0f);
	ArroTextUI_->setColor({ 1.0f, 1.0f, 1.0f, 1.0f });



	// 背景
	skydome_ = std::make_unique<Object3D>();
	skydome_->Initialize(Object3DCommon::GetInstance());
	skydome_->SetTranslate(Vector3{ 15.0f, 5.0f, 100.0f });
	skydome_->SetScale(Vector3{ 1.0f,1.0f,1.0f });
	skydome_->SetModel("backPlane.obj");

	// ラストステージならフラグを立てる
	if (nextStage == MaxStageIndex_) {
		nextsneneonthit = true;
		Selectindex = 1;
	}
	// セレクト用サウンド
	selectSound = Audio::GetInstance()->SoundLoadWave("Resources/Audio/Select.wav");
	// 決定用サウンド
	ButtonSound = Audio::GetInstance()->SoundLoadWave("Resources/Audio/Button.wav");
}

void GameClearScene::Finalize()
{
}

void GameClearScene::Update()
{
	CameraManager::GetInstans()->GetActiveCamera()->Update();

	skydome_->Update();

	// 音量設定
	Audio::GetInstance()->SetVolume(&selectSound, 2.0f);
	Audio::GetInstance()->SetVolume(&ButtonSound, 3.0f);
#ifdef _DEBUG


	if (ImGui::CollapsingHeader("Model", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("gameClearScene");
		if (ImGui::Button("TitleScene"))
		{
			SceneManager::GetInstance()->ChangeScene("TITELE");
		}


		//ImGui::Checkbox("start", &fige);

	}


#endif // _DEBUG


	// クリアの更新処理
	for (std::unique_ptr<Object3D>& Text : Cleartext_) {
		Text->Update();
	}

	// UIの更新
	for (uint32_t i = 0; i < 3; ++i) {
		// nextsneneonthit が true ならスプライト2だけ非表示
		if (nextsneneonthit && i == 2) {
			TextUI_[i]->setColor({ 1.0f, 1.0f, 1.0f, 0.0f }); // アルファを0にして非表示に
		}
		TextUI_[i]->Update();
	}

	// 移動開始
	EasingMove();

	if (allObjectsFinished) {

		easeT += easeSpeed;
		if (easeT > 1.0f) easeT = 1.0f;

		for (uint32_t i = 0; i < 3; ++i) {
			// 透過処理
			if (nextsneneonthit && i == 2) {
				TextUI_[i]->setColor({ 1.0f, 1.0f, 1.0f, 0.0f });
			}

			// 現在の位置
			Vector2 currentPos = { TextUI_[i]->GetPosition().x,700.0f };
			Vector2 startPos = currentPos;
			Vector2 endPos = { currentPos.x, 500.0f };

			// 線形補間で Y軸に移動（イージング）
			Vector2 newPos = {
				std::lerp(startPos.x, endPos.x, easeT),
				std::lerp(startPos.y, endPos.y, easeT)
			};
			TextUI_[i]->SetPosition(newPos);

			TextUI_[i]->Update();
		}
		Changefige = true;
		if (easeT == 1.0f) {
			// コントローラー操作
			ControllerUpdate();
		}
	}

	if (Changefige) {
		// ジャンプ開始
		StartJump();
	}

	// ↑の更新
	ArroTextUI_->Update();

}

void GameClearScene::Draw()
{
#pragma region 3Dオブジェクト描画

	//3dオブジェクトの描画準備。3Dオブジェクトの描画に共通のグラフィックスコマンドを積む
	Object3DCommon::GetInstance()->CommonDraw();

	skydome_->Draw();
	// クリアの描画処理
	for (std::unique_ptr<Object3D>& Text : Cleartext_) {
		Text->Draw();
	}

#pragma endregion


#pragma region スプライト描画
	//Spriteの描画準備。spriteの描画に共通のグラフィックスコマンドを積む
	SpriteCommon::GetInstance()->CommonDraw();

	if (allObjectsFinished) {
		// UIの描画
		for (std::unique_ptr<Sprite>& UI : TextUI_) {
			UI->Draw();
		}
		if (easeT == 1.0f) {
			ArroTextUI_->Draw();
		}
	}
#pragma endregion

}

void GameClearScene::EasingMove() {
	elapsedTime += deltaTime; // フレームごとの経過時間を加算
	allObjectsFinished = true;

	for (uint32_t i = 0; i < Cleartext_.size(); ++i) {
		float startZ = 10.0f;  // 開始位置
		float targetZ = 0.0f;    // 目標位置
		if (objecttime_[i] <= objecttime_[i]) {
			startDelay[i] += 0.1f; // 時間差
		}

		if (startDelay[i] >= objecttime_[i]) {
			float newZ = EaseOutElastic(elapsedTime, startZ, targetZ - startZ, animationDuration);

			// スケールのイージング
			float scaleFactor = EaseOutElastic(elapsedTime, 0.0f, 0.5f, animationDuration); // スケールを0から0.5fに変化


			// 現在の位置を取得
			Vector3 currentPosition = Cleartext_[i]->GetTranslate();
			currentPosition.z = newZ; // Z軸の値だけ更新
			Cleartext_[i]->SetTranslate(currentPosition); // 新しい座標をセット

			// スケールを更新
			Cleartext_[i]->SetScale(Vector3(scaleFactor, scaleFactor, scaleFactor)); // X, Y, Z軸すべてに適用
			// アニメーションが終了していない場合、フラグを偽に設定
			if (elapsedTime < animationDuration) {
				allObjectsFinished = false;
			}
		}
	}

	// アニメーション終了時、ループしないようにする
	if (elapsedTime >= animationDuration) {
		elapsedTime = animationDuration;
	}
}

void GameClearScene::StartJump() {
	// 初期化（オブジェクト数が変わった時も対応）
	if (!initialized || jumpStartTimes.size() != Cleartext_.size()) {
		size_t count = Cleartext_.size();

		jumpStartTimes.assign(count, 0.0f);
		velocities.assign(count, 0.0f);
		isJumping.assign(count, false);
		originalYPositions.clear();
		cooldownTimers.assign(count, 0.0f);
		totalElapsedTimes.assign(count, 0.0f);

		for (size_t i = 0; i < count; ++i) {
			jumpStartTimes[i] = i * jumpInterval;
			originalYPositions.push_back(Cleartext_[i]->GetTranslate().y);
		}

		initialized = true;
	}

	for (size_t i = 0; i < Cleartext_.size(); ++i) {
		float baseY = originalYPositions[i];

		// タイマー更新
		totalElapsedTimes[i] += deltaTime;

		// クールダウン中なら待機
		if (cooldownTimers[i] > 0.0f) {
			cooldownTimers[i] -= deltaTime;
			if (cooldownTimers[i] < 0.0f) cooldownTimers[i] = 0.0f;
		}

		// ジャンプ開始
		if (!isJumping[i] && cooldownTimers[i] <= 0.0f && totalElapsedTimes[i] >= jumpStartTimes[i]) {
			velocities[i] = jumpHeight;
			isJumping[i] = true;
		}

		// ジャンプ中の処理
		if (isJumping[i]) {
			velocities[i] += acceleration * deltaTime;

			Vector3 pos = Cleartext_[i]->GetTranslate();
			pos.y += velocities[i] * deltaTime;

			// 着地判定
			if (pos.y <= baseY) {
				pos.y = baseY;
				velocities[i] = 0.0f;
				isJumping[i] = false;

				// 個別にクールタイム開始
				cooldownTimers[i] = cooldownTime;

				// 次回ジャンプの遅延タイミングをリセット（オプション）
				totalElapsedTimes[i] = 0.0f;
			}

			Cleartext_[i]->SetTranslate(pos);
		} else {
			// 非ジャンプ中 → y位置をリセット
			Vector3 pos = Cleartext_[i]->GetTranslate();
			if (pos.y != baseY) {
				pos.y = baseY;
				Cleartext_[i]->SetTranslate(pos);
			}
		}
	}
}


void GameClearScene::ControllerUpdate() {

	// 長押し対応用の遅延時間
	static float holdDelay_ = 0.2f; // 押しっぱなしで再入力されるまでの時間
	static float holdTimer_ = 0.0f; // タイマー
	static bool wasStickMoved = false; // 前フレームに倒されていたか

	// 右スティックのX軸入力を取得
	float rightStickX = Input::GetInstance()->GetGamePadStickX();

	// スティックのしきい値
	const float stickThreshold = 0.5f;

	// 範囲の最大値
	const int maxIndex = nextsneneonthit ? 1 : 2; ;
#ifdef _DEBUG
	// キーボード入力
	// 入力がしきい値を超えた瞬間だけ反応
	if (!wasStickMoved) {
		if (Input::GetInstance()->PushKey(DIK_RIGHT) && Selectindex < (uint32_t)maxIndex) {
			Selectindex++;
			wasStickMoved = true;
			holdTimer_ = 0.0f;
		} else if (Input::GetInstance()->PushKey(DIK_LEFT) && Selectindex > 0) {
			Selectindex--;
			wasStickMoved = true;
			holdTimer_ = 0.0f;
		}
	}
#endif // _DEBUG

	// 入力がしきい値を超えた瞬間だけ反応
	if (!wasStickMoved) {
		if (rightStickX > stickThreshold && Selectindex < (uint32_t)maxIndex) {
			Selectindex++;
			wasStickMoved = true;
			holdTimer_ = 0.0f;
			// セレクト音声を流す
			Audio::GetInstance()->SoundPlayWave(selectSound);
		} else if (rightStickX < -stickThreshold && Selectindex > 0) {
			Selectindex--;
			wasStickMoved = true;
			holdTimer_ = 0.0f;
			// セレクト音声を流す
			Audio::GetInstance()->SoundPlayWave(selectSound);
		}
	}

	// 入力が戻ったらフラグをリセット
	if (fabsf(rightStickX) < stickThreshold) {
		wasStickMoved = false;
	}

	// タイトルの場合
	if (Selectindex == 0) {
		ArroTextUI_->SetPosition(Vector2(325.0f, 570.0f));
		if (Changefige) {
#ifdef _DEBUG
			if (Input::GetInstance()->PushKey(DIK_SPACE)) {
				SceneManager::GetInstance()->ChangeScene("TITELE");
			}
#endif // _DEBUG
			if (Input::GetInstance()->TriggerGamePadButton(XINPUT_GAMEPAD_A)) {
				// 決定の音声を流す
				Audio::GetInstance()->SoundPlayWave(ButtonSound);
				SceneManager::GetInstance()->ChangeScene("TITELE");
			}
		}
	}


	// ステージセレクトの場合
	if (Selectindex == 1) {
		ArroTextUI_->SetPosition(Vector2(625.0f, 570.0f));
		if (Changefige) {
#ifdef _DEBUG
			if (Input::GetInstance()->PushKey(DIK_SPACE)) {
				SceneManager::GetInstance()->ChangeScene("STAGESELECTSCENE");
			}
#endif // _DEBUG
			if (Input::GetInstance()->TriggerGamePadButton(XINPUT_GAMEPAD_A)) {
				// 決定の音声を流す
				Audio::GetInstance()->SoundPlayWave(ButtonSound);
				SceneManager::GetInstance()->ChangeScene("STAGESELECTSCENE");
			}
		}
	}

	// 次のステージの場合
	if (Selectindex == 2) {
		ArroTextUI_->SetPosition(Vector2(925.0f, 570.0f));
		if (Changefige) {
#ifdef _DEBUG
			if (Input::GetInstance()->PushKey(DIK_SPACE)) {
				if (nextStage < MaxStageIndex_) {
					SceneManager::GetInstance()->SetStageIndex(nextStage);
					SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
				}
			}
#endif // _DEBUG
			if (Input::GetInstance()->TriggerGamePadButton(XINPUT_GAMEPAD_A)) {

				if (nextStage < MaxStageIndex_) {
					// 決定の音声を流す
					Audio::GetInstance()->SoundPlayWave(ButtonSound);
					SceneManager::GetInstance()->SetStageIndex(nextStage);
					SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
				}
			}
		}
	}
}