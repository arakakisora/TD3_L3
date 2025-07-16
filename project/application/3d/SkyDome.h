#pragma once
#include "engine/3d/Object3D.h"
#include "engine/3d/Object3DCommon.h"
#include <memory>

class SkyDome
{
public:
	// コンストラクタ
	SkyDome();
	// デストラクタ
	~SkyDome();
	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw();
	// 終了処理
	void Finalize();
	// 位置設定
	void SetTranslate(const Vector3& pos) {
		skyDomePosition = pos;
		if (skyDomeObject) {
			skyDomeObject->SetTranslate(pos);
		}
	}
	// 拡縮設定
	void SetScale(const Vector3& scale) {
		if (skyDomeObject) {
			skyDomeObject->SetScale(scale);
		}
	}
private:
	std::unique_ptr<Object3D> skyDomeObject; // 天球の3Dオブジェクト	
	Vector3 skyDomePosition;

};

