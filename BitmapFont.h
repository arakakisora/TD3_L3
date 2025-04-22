#pragma once
#include <cstdint>
#include "Sprite.h"
#include "SpriteCommon.h"
class BitmapFont
{
public:
	// 初期化処理
	void Initialize(uint32_t drawNum);
	// 更新処理
	void Update();
	// 描画処理
	void Draw();

private:

	// 描画する数
	uint32_t drawNumber;

	// 表示する番号のスプライト
	std::unique_ptr<Sprite> sprite;

	// 枚数表示時のｘ


};

