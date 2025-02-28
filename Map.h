#pragma once
#include "Vector3.h"

#include <iostream>
#include <vector>
using namespace std;
class Block;
class Map
{
public:
	// コンストラクタ / デストラクタ
	Map();
	~Map();

	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw();
	// 終了処理
	void Finalize();

	// マップの読み込み
	void LoadMap(const string filename);

	// マップデータの疑似用
	void DemoMap(int width, int height);

public:
	//ゴールの座標を取得
	Vector3 GetGoalPosition()const;

private:

private:
	// マップデータ
	vector<vector<int>> mapData;
	// ブロックのリスト
	vector<vector<Block>> mapBlock;
	// マップのサイズ
	size_t mapWidth;
	size_t mapHeight;
	

};

