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

	object3 = std::make_unique<Object3D>();
	object3->Initialize(object3dcommon_);
	object3->SetModel("plane.obj");
	object3->SetScale({ 2.0f,2.0f,1.0f });

	// 作成してでリストに追加
	for (uint32_t i = 0; i < 3; ++i) {	
		std::unique_ptr<Object3D> newObject = std::make_unique<Object3D>();
		newObject->Initialize(Object3DCommon::GetInstance());
		if (i == 0) {
			newObject->SetModel("StageSelect/return.obj");
		} else if (i == 1) {
			newObject->SetModel("StageSelect/explanation.obj");
		} else if (i == 2) {
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

	// コントローラの動き
	ControllerUpdate(); 

	//遷移速度
	const float easeSpeed = 0.02f;

	if (isPaused_) {
		easeTimer_ += easeSpeed;

		if (easeTimer_ > 1.0f) {
			easeTimer_ = 1.0f;
		}
	} else
	{
		easeTimer_ = 0.0f;

	}

	if (isPaused_) {
		float easedValue = float(easeInOutCirc(easeTimer_));
		// カメラのXYに追従させる（Zはそのまま）
		Vector3 cameraPos = camera_->GetTransform().translate;
		transform.translate.x = cameraPos.x;
		transform.translate.y = cameraPos.y;
		transform.translate.z = 5.0f + (-6.0f * easedValue);

		//xスケール
		float minScaleX = 0.0f;
		float maxScaleX = 1.0f;
		float currentScaleX = minScaleX + (maxScaleX - minScaleX) * easedValue;
		//yスケール
		float minScaleY = 0.0f;
		float maxScaleY = 1.0f;
		float currentScaleY = minScaleY + (maxScaleY - minScaleY) * easedValue;
		//zスケール
		float minScaleZ = 0.0f;
		float maxScaleZ = 1.0f;
		float currentScaleZ = minScaleZ + (maxScaleZ - minScaleZ) * easedValue;

		transform.scale = { currentScaleX, currentScaleY, currentScaleZ };

		object->SetTranslate(transform.translate);
		object->SetScale(transform.scale);
		object->Update();

		Vector3 basePos = object->GetTransform().translate;

		float offsetY = 1.8f;
		for (size_t i = 0; i < TextObjects.size(); ++i) {
			Vector3 followPos = basePos;
			followPos.z -= 5.0f;
			followPos.y += 2.0f + (offsetY * -static_cast<float>(i)); // objectの位置から下方向にずらす

			TextObjects[i]->SetTranslate(followPos);

			// textindex 番号のやつだけスケールを 1.0f にする
			if (i == textindex && easeTimer_ == 1.0f) {
				TextObjects[i]->SetScale(Vector3(1.5f, 1.5f, 1.5f));
			} else {
				// 通常の最大0.5fスケール
				Vector3 limitedScale;
				limitedScale.x = std::min(transform.scale.x, 1.0f);
				limitedScale.y = std::min(transform.scale.y, 1.0f);
				limitedScale.z = std::min(transform.scale.z, 1.0f);
				TextObjects[i]->SetScale(limitedScale);
			}

			TextObjects[i]->Update();
		}
	}

	//ポーズ画面が出ているとき
	if (isPaused_) {
		//ポーズ画面を閉じる
		if (textindex == 0 && Input::GetInstance()->TriggerGamePadButton(XINPUT_GAMEPAD_A)) {
			isPaused_ = !isPaused_;
			if (isOperation_) {
				isOperation_ = false;
			}
		}

		//ポーズ画面が出ているときWで操作説明を表示(仮)
		if (textindex == 1 && Input::GetInstance()->TriggerGamePadButton(XINPUT_GAMEPAD_A)) {
			isOperation_ = !isOperation_;
		}

		//ポーズ画面が出ているときTでタイトルへ(仮)
		if (textindex == 2 && Input::GetInstance()->TriggerGamePadButton(XINPUT_GAMEPAD_A)) {
			if (scenefige) {
				SceneManager::GetInstance()->ChangeScene("STAGESELECTSCENE");
			} else if(!scenefige){
				SceneManager::GetInstance()->ChangeScene("TITELE");
			}
		}
	}

	//操作説明
	if (isOperation_) {
		easeTimer2_ += easeSpeed;
		if (easeTimer2_ > 1.0f) {
			easeTimer2_ = 1.0f;
		}
	} else
	{
		easeTimer2_ -= easeSpeed;
		if (easeTimer2_ < 0.0f) {
			easeTimer2_ = 0.0f;
		}
	}

	if (isOperation_) {
		float operationEasedValue = float(easeInOutCirc(easeTimer2_));
		Transform operationTransform;
		operationTransform.translate = { 0.0f,0.0f,6.0f + (-8.0f * operationEasedValue) };

		float opScaleX = 2.0f * operationEasedValue;
		float opScaleY = 1.0f * operationEasedValue;
		float opScaleZ = 1.0f * operationEasedValue;

		operationTransform.scale = { opScaleX,opScaleY,opScaleZ };

		object3->SetTranslate(operationTransform.translate);
		object3->SetScale(operationTransform.scale);
		object3->Update();
	}
}

//描画
void PauseMenu::Draw() {

	if (isPaused_) {
		//ポーズ画面
		object->Draw();
		if (isOperation_) {
			//操作説明画面
			object3->Draw();
		}
	}

	for (std::unique_ptr<Object3D>& text : TextObjects) {
		text->Draw();
	}
}

//フラグのゲッター
bool PauseMenu::IsPaused()const {
	return isPaused_;
}


void PauseMenu::ControllerUpdate() {

	// アニメーションが完了しているかチェック
	bool isPauseAnimationFinished = (easeTimer_ >= 1.0f || easeTimer_ <= 0.0f);

	// キーを押したらフラグオン(仮)
	if (isPauseAnimationFinished && Input::GetInstance()->TriggerGamePadButton(XINPUT_GAMEPAD_Y)) {
		isPaused_ = !isPaused_;
		if (isOperation_) {
			isOperation_ = false;
		}
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
	if (!isOperation_) {
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



}