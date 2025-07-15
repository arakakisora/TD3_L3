#pragma once
#include "Object3D.h"
#include "Sprite.h"
class Reset
{
public:
	//初期化
	void Initialize();
	//更新
	void Update();
	//リセットお知らせ描画
	void DrawResetNotice();
	//リセットメーター描画
	void DrawResetMeter();
	//ステージリセット
	void StageReset();
private:
	std::unique_ptr<Object3D>resetNotice;
	std::unique_ptr<Sprite>resetMeter;
	//リセット用タイマー
	float holdTime = 0.0f;
	const float holdDuration = 1.7f;
	//リセットテキスト用タイマー

	//経過時間
	float elapsedTime = 0.0f;
	//30秒後に表示
	const float afterseconds = 30.0f;
	//30秒経過フラグ
	bool secondspassed = false;

	const float deltaTime = 1.0f / 60.0f;
};

