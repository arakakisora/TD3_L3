#pragma once
#include <iostream>
#include <vector>

#include "Vector3.h"
#include "Block.h"

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
	
public:
	// getter
	Vector3 GetBlockPosition(int x, int y);
	const vector<vector<int>>& GetMapData() const { return mapData; }
	const vector<vector<Block>>& GetMapBlock() const { return mapBlock; }
	size_t GetMapWidth() const { return mapWidth; }
	size_t GetMapHeight() const { return mapHeight; }
	Block* GetBlock(int x, int y) {
		if (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight) {
			return &mapBlock[mapHeight - 1 - y][x];
		}
		return nullptr;
	}
	// setter
	void SetMapData(int x, int y, int value)
	{
		// 範囲チェック
		if (y >= 0 && y < mapHeight && x >= 0 && x < mapWidth) { // ここは変更なし
			// 既存ブロックのリソース解放
			mapBlock[y][x].Finalize();
			// mapDataの更新
			mapData[y][x] = value;  // マップデータを更新
			// mapBlockの見た目を更新
			Vector3 blockPosition = GetBlockPosition(x, y);  // x と y の順番を修正
			mapBlock[y][x].Initialize(value, blockPosition);  // ブロックを再初期化
		}
	}

};

