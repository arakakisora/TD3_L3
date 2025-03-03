#include "PauseMenu.h"

//初期化
void PauseMenu::Initialize(Object3DCommon* object3dcommon) {
	object3dcommon_ = object3dcommon;
	object = std::make_unique<Object3D>();
	object->Initialize(object3dcommon_);
	object->SetModel("plane.obj");
	object->SetScale({ 1.0f,1.5f,0.0f });
	transform.translate = { 0.0f,0.0f,0.0f };
	//各メニューの壱とサイズ
	menuItems[0] = { {0.0f,2.0f},{0.5f,0.5f} };//タイトル
	menuItems[1] = { {0.0f,1.0f}, {0.5f,0.5f} };//セレクト
	menuItems[2] = { {0.0f,0.0f},{0.5f,0.5f} };//リトライ
	menuItems[3] = { {0.0f,-1.0f},{0.5f,0.5f} };//操作説明表示
}

//更新
void PauseMenu::Update() {

	//タイトルへ戻る処理

	//セレクトへ戻る処理

	//リトライ
	
	//操作説明表示

	Vector2 mousePos = Input::GetInstans()->GetMousePos();

	for (size_t i = 0; i < menuItems.size(); i++) {
		const auto& item = menuItems[i];
		//マウスがボタンの範囲内か
		bool isHovered =
			mousePos.x >= item.position.x &&
			mousePos.x <= item.position.x + item.size.x &&
			mousePos.y >= item.position.y &&
			mousePos.y <= item.position.y + item.size.y;
		
		if (isHovered && Input::GetInstans()->TriggerMouse(0)) {
			switch (i) {
			case 0:
			    //タイトルへ
				break;
			case 1:
				//セレクトへ
				break;
			case 2:
				//リトライ
				break;
			case 3:
				//操作説明
				break;
			}
		}
	}

	object->SetTranslate(transform.translate);

	object->Update();
}

//描画
void PauseMenu::Draw() {
	object->Draw();
}