#include "PauseMenu.h"
#include <SceneManager.h>
#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

double easeInOutCirc(double x) {
	return (x < 0.5)
		? (1 - std::sqrt(1 - std::pow(2 * x, 2))) / 2
		: (std::sqrt(1 - std::pow(-2 * x + 2, 2)) + 1) / 2;
}

//初期化
void PauseMenu::Initialize(Object3DCommon* object3dcommon, bool isPlayScene) {
	object3dcommon_ = object3dcommon;
	scenefige = isPlayScene;

	transform.translate = { -2.6f,1.5f,-2.0f };
	object = std::make_unique<Object3D>();
	object->Initialize(object3dcommon_);
	object->SetModel("Pause.obj");
	object->SetScale({ 1.0f,2.0f,1.0f });
	input = Input::GetInstance();

	// 作成してでリストに追加
	for (uint32_t i = 0; i < 2; ++i) {	
		std::unique_ptr<Object3D> newObject = std::make_unique<Object3D>();
		newObject->Initialize(Object3DCommon::GetInstance());
		if (i == 0) {
			newObject->SetModel("StageSelect/return.obj");
		} else if (i == 1) {
			if (isPlayScene) {
				newObject->SetModel("StageSelect/StageSelect.obj");
			} else {
				newObject->SetModel("StageSelect/title.obj");
			}
		} else {
			newObject->SetModel("StageSelect/return.obj");
		}

		texttransform[i] = { transform.translate };
		texttransform[i].translate.y = 0.0f;
		newObject->SetTranslate(texttransform[i].translate);
		newObject->SetScale(Vector3(0.0f,0.0f,0.0f));
		newObject->SetLighting(false);
		TextObjects.push_back(std::move(newObject));
	}

}

//更新
void PauseMenu::Update() {
	object->SetTranslate(transform.translate);

	object->Update();

#ifdef _DEBUG
	ImGui::Begin("Textindex");
	ImGui::Text("check: %d", textindex);
	ImGui::End();
#endif // _DEBUG

	for (std::unique_ptr<Object3D>& text : TextObjects) {
		text->SetScale(Vector3(0.0f, 0.0f, 0.0f));
		text->Update();
	}

	//遷移速度
	const float easeSpeed = 0.04f;

	if (isPaused_) {
		easeTimer_ += easeSpeed;
		if (easeTimer_ > 1.0f) easeTimer_ = 1.0f;
	} else {
		easeTimer_ -= easeSpeed;
		if (easeTimer_ < 0.0f) easeTimer_ = 0.0f;
	}

	// ポーズ画面表示
	PausedStart();
	// コントローラの動き
	ControllerUpdate();
}

//描画
void PauseMenu::Draw() {

	if (isPaused_) {
		//ポーズ画面
		object->Draw();
	}

	for (std::unique_ptr<Object3D>& text : TextObjects) {
		text->Draw();
	}
}

//フラグのゲッター
bool PauseMenu::IsPaused()const {
	// 「ポーズ中」または「イージングがまだ戻っている最中」なら true
	return isPaused_ || easeTimer_ > 0.0f;
}

void PauseMenu::PausedStart() {
	if (isPaused_ || easeTimer_ > 0.0f) {
		// イージングの値（0.0〜1.0）の計算（※逆再生に対応）
		float easedValue = float(easeInOutCirc(easeTimer_));

		Vector3 cameraPos = camera_->GetTransform().translate;
		transform.translate.x = cameraPos.x;
		transform.translate.y = cameraPos.y;
		transform.translate.z = 5.0f + (-6.0f * easedValue);

		float minScale = 0.1f;
		float limitedScale = (easedValue > 1.0f) ? 1.0f :
			(easedValue < minScale) ? minScale : easedValue;

		// ポーズを閉じている途中なら、0.0f までしっかり戻す
		if (easeTimer_ < 1.0f && !isPaused_) {
			limitedScale = (easedValue > 0.0f) ? easedValue : 0.0f;
		} else if (easeTimer_ >= 1.0f) {
			limitedScale = 1.0f;
		}

		transform.scale = { limitedScale, limitedScale, limitedScale };

		object->SetTranslate(transform.translate);
		object->SetScale(transform.scale);
		object->Update();

		Vector3 basePos = object->GetTransform().translate;

		float currentScale = (easedValue > minScale) ? easedValue : minScale;	
		float offsetY = 1.8f;
		for (size_t i = 0; i < TextObjects.size(); ++i) {
			Vector3 followPos = basePos;
			followPos.z -= 5.0f;
			followPos.y += 1.0f + (offsetY * -static_cast<float>(i));

			TextObjects[i]->SetTranslate(followPos);

			if (i == textindex && easeTimer_ == 1.0f) {
				TextObjects[i]->SetScale(Vector3(1.5f, 1.5f, 1.5f));
			} else {
				Vector3 limitedScale = { std::min(currentScale, 1.0f), std::min(currentScale, 1.0f), std::min(currentScale, 1.0f) };
				TextObjects[i]->SetScale(limitedScale);
			}
			TextObjects[i]->Update();
		}
	}
}

void PauseMenu::ControllerUpdate() {

	// アニメーションが完了しているかチェック
	bool isPauseAnimationFinished = (easeTimer_ >= 1.0f || easeTimer_ <= 0.0f);

	// キーを押したらフラグオン
	if (isPauseAnimationFinished && Input::GetInstance()->TriggerGamePadButton(XINPUT_GAMEPAD_START)) {
		isPaused_ = !isPaused_;
	}

	// スティックのしきい値
	const float stickThreshold = 0.5f;

	// 右スティックのY軸入力（先に取得！）
	float rightStickY = Input::GetInstance()->GetGamePadStickY();

	// 前フレームのY軸入力（staticにして保持）
	static float prevRightStickY = 0.0f;

	// 押し始め検出
	bool stickUpPressed = (rightStickY < -stickThreshold && prevRightStickY >= -stickThreshold);
	bool stickDownPressed = (rightStickY > stickThreshold && prevRightStickY <= stickThreshold);
	if (easeTimer_ ==1.0f) {
		// 上向き
		if (stickUpPressed && textindex < TextObjects.size() - 1 && !easingmoveFlag_ && !easingsceneFlag_) {
			textindex++;
		}

		// 下向き
		if (stickDownPressed && textindex > 0 && !easingmoveFlag_ && !easingsceneFlag_) {
			textindex--;
		}
	}
	// 前フレームの値を保存
	prevRightStickY = rightStickY;


	//ポーズ画面が出ているとき
	if (isPaused_ && easeTimer_ == 1.0f) {
		//ポーズ画面を閉じる
		if (textindex == 0 && Input::GetInstance()->TriggerGamePadButton(XINPUT_GAMEPAD_A)) {
			isPaused_ = !isPaused_;
		}

		//ポーズ画面が出ているときTでタイトルへ(仮)
		if (textindex == 1 && Input::GetInstance()->TriggerGamePadButton(XINPUT_GAMEPAD_A)) {
			if (scenefige) {
				SceneManager::GetInstance()->ChangeScene("STAGESELECTSCENE");
			} else if (!scenefige) {
				SceneManager::GetInstance()->ChangeScene("TITELE");
			}
		}
	}

}