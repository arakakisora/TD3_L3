#pragma once
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

