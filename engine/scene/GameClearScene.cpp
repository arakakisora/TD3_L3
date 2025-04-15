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
		newObject->SetTranslate(Vector3(-0.5f + (0.5f * i), 0.0f, 10.0f));
		newObject->SetScale(Vector3(0.0f, 0.0f, 0.0f));
		newObject->SetLighting(false);
		Cleartext_.push_back(std::move(newObject));

		objecttime_[i] = i * 1.0f;
		startDelay[i] = 0.0f;

		velocity_[i] = Vector3(0.0f, 0.0f, 0.0f); // 初期速度はゼロに設定（x, y, z）
	}


	TextureManager::GetInstance()->LoadTexture("Resources/TextUI_Nextstage.png");
	TextureManager::GetInstance()->LoadTexture("Resources/TextUI_Stageselect.png");
	TextureManager::GetInstance()->LoadTexture("Resources/ArroUP.png");
	
	// 作成してリストに追加
	for (uint32_t i = 0; i < 3; ++i) {
		std::unique_ptr<Sprite> newSprite = std::make_unique<Sprite>();
	
		if (i == 0) {
			newSprite->Initialize(SpriteCommon::GetInstance(), "Resources/TextUI_Title.png");
			newSprite->SetPosition(Vector2(224.0f, 500.0f));

		} else if (i == 1) {
			newSprite->Initialize(SpriteCommon::GetInstance(), "Resources/TextUI_Stageselect.png");
			newSprite->SetPosition(Vector2(528.0f, 500.0f));
		} else if (i == 2) {
			newSprite->Initialize(SpriteCommon::GetInstance(), "Resources/TextUI_Nextstage.png");
			newSprite->SetPosition(Vector2(796.0f, 500.0f));
		}

		newSprite->SetSize({ 224.0f, 50.0f });
		newSprite->SetRotation(0.0f);
		newSprite->setColor({ 1.0f, 1.0f, 1.0f, 1.0f });

		TextUI_.push_back(std::move(newSprite));
	}

	ArroTextUI_ = std::make_unique<Sprite>();

	ArroTextUI_->Initialize(SpriteCommon::GetInstance(), "Resources/ArroUP.png");
	ArroTextUI_->SetPosition(Vector2(890.0f, 575.0f));
	ArroTextUI_->SetSize({ 50.0f, 50.0f });
	ArroTextUI_->SetRotation(0.0f);
	ArroTextUI_->setColor({ 1.0f, 1.0f, 1.0f, 1.0f });


}

void GameClearScene::Finalize()
{
}

void GameClearScene::Update()
{
	CameraManager::GetInstans()->GetActiveCamera()->Update();

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
	for (std::unique_ptr<Sprite>& UI : TextUI_) {
		UI->Update();
	}

	ArroTextUI_->Update();

	// 移動開始
	EasingMove();	

	if (allObjectsFinished) {
		Changefige = true;
		// コントローラー操作
		ControllerUpdate();
	}

	//次のステージへ(リターンは仮)
	if (Input::GetInstance()->TriggerKey(DIK_RETURN)) {
		uint32_t nextStage = SceneManager::GetInstance()->GetStageIndex() + 1;

		if (nextStage < MaxStageIndex_) {
			SceneManager::GetInstance()->SetStageIndex(nextStage);
			SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
		}
	}
}

void GameClearScene::Draw()
{
#pragma region 3Dオブジェクト描画

	//3dオブジェクトの描画準備。3Dオブジェクトの描画に共通のグラフィックスコマンドを積む
	Object3DCommon::GetInstance()->CommonDraw();

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

		ArroTextUI_->Draw();
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
	static float acceleration = -9.8f;
	static float jumpHeight = 2.5f;
	static float totalElapsedTime = 0.0f;
	static float jumpInterval = 0.2f;  // 各オブジェクトのジャンプ開始時間差
	static float cooldownTime = 120.0f;

	static std::vector<float> jumpStartTimes;
	static std::vector<float> velocities;
	static std::vector<bool> isJumping;
	static float cooldownTimer = 0.0f;

	// 初期化（初回または再ジャンプ時）
	if (jumpStartTimes.size() != Cleartext_.size()) {
		jumpStartTimes.clear();
		velocities.clear();
		isJumping.clear();

		for (size_t i = 0; i < Cleartext_.size(); ++i) {
			jumpStartTimes.push_back(i * jumpInterval);
			velocities.push_back(0.0f);
			isJumping.push_back(false);
		}
		totalElapsedTime = 0.0f;
		cooldownTimer = 0.0f;
	}

	totalElapsedTime += deltaTime;

	for (size_t i = 0; i < Cleartext_.size(); ++i) {
		// ジャンプ開始
		if (!isJumping[i] && totalElapsedTime >= jumpStartTimes[i]) {
			velocities[i] = jumpHeight;
			isJumping[i] = true;
		}

		// ジャンプ処理
		if (isJumping[i]) {
			velocities[i] += acceleration * deltaTime;

			Vector3 pos = Cleartext_[i]->GetTranslate();
			pos.y += velocities[i] * deltaTime;

			if (pos.y <= 0.0f) {
				pos.y = 0.0f;
				velocities[i] = 0.0f;
			}

			Cleartext_[i]->SetTranslate(pos);
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
	const int maxIndex = 2;

	// 入力がしきい値を超えた瞬間だけ反応
	if (!wasStickMoved) {
		if (rightStickX > stickThreshold && Selectindex < maxIndex) {
			Selectindex++;
			wasStickMoved = true;
			holdTimer_ = 0.0f;
		} else if (rightStickX < -stickThreshold && Selectindex > 0) {
			Selectindex--;
			wasStickMoved = true;
			holdTimer_ = 0.0f;
		}
	}

	// 入力が戻ったらフラグをリセット
	if (fabsf(rightStickX) < stickThreshold) {
		wasStickMoved = false;
	}
	
	// タイトルの場合
	if (Selectindex == 0) {
		ArroTextUI_->SetPosition(Vector2(300.0f, 575.0f));
		if (Changefige) {
			if (Input::GetInstance()->TriggerGamePadButton(XINPUT_GAMEPAD_A)) {
				SceneManager::GetInstance()->ChangeScene("TITELE");
			}
		}
	}


	// ステージセレクトの場合
	if (Selectindex == 1) {
		ArroTextUI_->SetPosition(Vector2(600.0f, 565.0f));
		if (Changefige) {
			if (Input::GetInstance()->TriggerGamePadButton(XINPUT_GAMEPAD_A)) {
				SceneManager::GetInstance()->ChangeScene("STAGESELECTSCENE");
			}
		}
	}

	// 次のステージの場合
	if (Selectindex == 2) {
		ArroTextUI_->SetPosition(Vector2(890.0f, 575.0f));
	}
}