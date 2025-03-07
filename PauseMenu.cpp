#include "PauseMenu.h"
#include <SceneManager.h>

double easeInOutCirc(double x) {
	return (x < 0.5)
		? (1 - std::sqrt(1 - std::pow(2 * x, 2))) / 2
		: (std::sqrt(1 - std::pow(-2 * x + 2, 2)) + 1) / 2;
}

//初期化
void PauseMenu::Initialize(Object3DCommon* object3dcommon) {
	object3dcommon_ = object3dcommon;
	object = std::make_unique<Object3D>();
	object->Initialize(object3dcommon_);
	object->SetModel("plane.obj");
	object->SetScale({ 0.5f,0.25f,0.5f });
	transform.translate = { -2.6f,1.5f,-2.0f };
	object2 = std::make_unique<Object3D>();
	object2->Initialize(object3dcommon_);
	object2->SetModel("plane.obj");
	object2->SetScale({ 1.0f,2.0f,1.0f });
	object3 = std::make_unique<Object3D>();
	object3->Initialize(object3dcommon_);
	object3->SetModel("plane.obj");
	object3->SetScale({ 2.0f,1.0f,1.0f });
	input = Input::GetInstans();
}

//更新
void PauseMenu::Update() {

	object->SetTranslate(transform.translate);

	object->Update();

	//Qキーを押したらフラグオン(仮)
	if (input->TriggerKey(DIK_Q)) {
		if (isPaused_) {
			isPausedClosed_ = true;

			if (isOperation_) {
				isOperationClosed_ = true;
			}
		} else {
			isPaused_ = true;
			isPausedClosed_ = false;
		}
	}

	//遷移速度
	const float easeSpeed = 0.02f;

	if (isPaused_ && !isPausedClosed_) {
		easeTimer_ += easeSpeed;
		if (easeTimer_ > 1.0f) {
			easeTimer_ = 1.0f;
		}
	} else if(isPausedClosed_)
	{
		easeTimer_ -= easeSpeed;
		if (easeTimer_ < 0.0f) {
			easeTimer_ = 0.0f;
			isPaused_ = false;
			isPausedClosed_ = false;
			isOperation_ = false;
			isOperationClosed_ = false;
			easeTimer2_ = 0.0f;  
		}
	}

	if (isPaused_) {
		float easedValue = float(easeInOutCirc(easeTimer_));
		transform.translate = { 0.0f,0.0f,0.0f };
		transform.translate.z = 5.0f + (-6.0f * easedValue);

		//xスケール
		float minScaleX = 0.0f;
		float maxScaleX = 1.0f;
		float currentScaleX = minScaleX + (maxScaleX - minScaleX) * easedValue;
		//yスケール
		float minScaleY = 0.0f;
		float maxScaleY = 2.0f;
		float currentScaleY = minScaleY + (maxScaleY - minScaleY) * easedValue;
		//zスケール
		float minScaleZ = 0.0f;
		float maxScaleZ = 2.0f;
		float currentScaleZ = minScaleZ + (maxScaleZ - minScaleZ) * easedValue;

		transform.scale = {currentScaleX, currentScaleY, currentScaleZ };

		object2->SetTranslate(transform.translate);
		object2->SetScale(transform.scale);
		object2->Update();
	}

	//ポーズ画面が出ているときTでタイトルへ(仮)
	if (isPaused_) {
		if (input->TriggerKey(DIK_T)) {
			//エラー発生中多分カメラのオンオフ????
			//SceneManager::GetInstance()->ChangeScene("TITELE");
		}
	}
	//ポーズ画面が出ているときWで操作説明を表示(仮)
	if (isPaused_) {
		if (input->TriggerKey(DIK_W)) {
			if (isOperation_) {
				isOperationClosed_ = true;
			}
			else {
				isOperation_ = true;
				isOperationClosed_ = false;
			}
		}
	}

	//操作説明
	if (isOperation_&& !isOperationClosed_) {
		easeTimer2_ += easeSpeed;
		if (easeTimer2_ > 1.0f) {
			easeTimer2_ = 1.0f;
		}
	}
	else if(isOperationClosed_)
	{
		easeTimer2_ -= easeSpeed;
		if (easeTimer2_ < 0.0f) {
			easeTimer2_ = 0.0f;
			isOperation_ = false;
			isOperationClosed_ = false;
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

	//ポーズの文字(未定)
	//object->Draw();

	if (isPaused_) {
		//ポーズ画面
		object2->Draw();
		if (isOperation_) {
			//操作説明画面
			object3->Draw();
		}
	}
}

//フラグのゲッター
bool PauseMenu::IsPaused()const {
	return isPaused_;
}