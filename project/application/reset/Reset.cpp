#include "Reset.h"
#include "Object3DCommon.h"
#include "SpriteCommon.h"
#include "SceneManager.h"
#include "Input.h"

//初期化
void Reset::Initialize() {
	//リセット
	resetNotice = std::make_unique<Object3D>();
	resetNotice->Initialize(Object3DCommon::GetInstance());
	resetNotice->SetModel("resetnotice.obj");
	resetNotice->SetScale(Vector3(0.5f, 0.5f, 0.5f));
	resetNotice->SetRotate(Vector3(17.3f, 12.56f, 0.0f));
	resetNotice->SetTranslate(Vector3(12.46f, 23.25f, -1.0f));
	resetNotice->SetLighting(false);
	//リセットメーター
	resetMeter = std::make_unique<Sprite>();
	resetMeter->Initialize(SpriteCommon::GetInstance(), "Resources/resetmeter.png");
	resetMeter->SetPosition(Vector2(325, 70));
	resetMeter->SetSize(Vector2(200, 45));
}
//更新
void Reset::Update() {
	resetNotice->Update();
	resetMeter->Update();
}
//リセット描画
void Reset::DrawResetNotice() {
	if (holdTime > 0.0f) {
		resetNotice->Draw();
	}
}
//リセットメーター描画
void Reset::DrawResetMeter() {
	if (holdTime > 0.0f) {
		resetMeter->Draw();
	}
}
//ステージリセット
void Reset::StageReset() {
	if (
#ifdef _DEBUG
		Input::GetInstance()->PushKey(DIK_R) ||
#endif// _DEBUG
		Input::GetInstance()->PushGamePadButton(XINPUT_GAMEPAD_LEFT_SHOULDER) &&
		Input::GetInstance()->PushGamePadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER)) {

		holdTime += deltaTime;

		//メーターの進み具合
		float progress = std::min(holdTime / holdDuration, 1.0f);
		float maxWidth = 600.0f;
		float meterWidth = maxWidth * progress;

		resetMeter->SetSize(Vector2(meterWidth, 45));

		if (holdTime >= holdDuration) {
			holdTime = 0.0f;

			int stageIndex = SceneManager::GetInstance()->GetStageIndex();
			SceneManager::GetInstance()->SetStageIndex(stageIndex);
			SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
		}

	} else {
		//離されたらタイマーをリセット
		holdTime = 0.0f;
	}
}