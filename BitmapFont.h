#pragma once
#include <cstdint>
#include "Sprite.h"
#include "SpriteCommon.h"
#include <memory>
using namespace std;

class BitmapFont
{
public:
	// 初期化処理
	void Initialize();
	// 更新処理
	void Update(uint32_t drawNum);
	// 描画処理
	void Draw();

	// 残り枚数を更新する
private:

	// 描画する数
	uint32_t drawNumber;
	
	// 表示する番号のスプライト
	vector<unique_ptr<Sprite>> sprites;

	// 枚数表示時のｘ
	std::unique_ptr<Sprite> spriteMulti;

};

