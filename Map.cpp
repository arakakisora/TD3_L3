#include "Map.h"
#include "Block.h"
#include <fstream>
#include <string>
#include <iostream>

Map::Map()
{
}

Map::~Map()
{
}

void Map::Initialize()
{
    // CSVファイルからマップデータを読み込む
    LoadMap("MapData/map2.csv");

    // ブロックのリストを初期化
    mapBlock.resize(mapHeight, vector<Block>(mapWidth));

	float blockScaleX = 0.15f;
	float blockScaleY = 0.3f;

    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
			mapBlock[y][x].Initialize(mapData[y][x], Vector3(float(x*blockScaleX), float(y*blockScaleY),0.0f ));
        }
    }
}

void Map::Update()
{
    // 各ブロックの更新処理
    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            mapBlock[y][x].Update();
        }
    }
}

void Map::Draw()
{
    // 各ブロックの描画処理
    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            mapBlock[y][x].Draw();
        }
    }
}

void Map::Finalize()
{
    // 各ブロックの終了処理
    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            mapBlock[y][x].Finalize();
        }
    }
}

void Map::LoadMap(const std::string filename)
{
    // ファイルを開く
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "ファイルが開けませんでした。" << std::endl;
        return;
    }

    // マップデータを一時的に格納するためのベクトル
    std::vector<std::string> tempMapData;

    // ファイルからマップデータを読み込む
    std::string line;
    while (std::getline(file, line)) {
        tempMapData.push_back(line);
    }

    // マップのサイズを設定
    mapHeight = tempMapData.size();
    if (mapHeight > 0) {
        mapWidth = tempMapData[0].size();
    } else {
        std::cerr << "マップデータが空です。" << std::endl;
        return;
    }

    // マップデータを初期化
    mapData.resize(mapHeight);
    for (size_t y = 0; y < mapHeight; y++) {
        mapData[y].resize(mapWidth);
        for (size_t x = 0; x < mapWidth; x++) {
            mapData[y][x] = tempMapData[mapHeight - 1 - y][x] - '0';
        }
    }

    file.close();
}

// 任意のサイズを指定
void Map::DemoMap(int width, int height) {
    mapWidth = width;
    mapHeight = height;

    // mapBlockを指定されたサイズで初期化
    mapBlock.resize(mapHeight, vector<Block>(mapWidth));

    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
			mapBlock[y][x].Initialize(0, Vector3(float(x), 0.0f, float(y)));
        }
    }
}

//ゴールの座標を取得
Vector3 Map::GetGoalPosition()const {
    for (size_t y = 0; y < mapHeight; y++) {
        for (size_t x = 0; x < mapWidth; x++) {
            if (mapData[y][x] == 2) {
                return mapBlock[y][x].GetPosition();
            }
        }
    }
    return Vector3(0.0f, 0.0f, 0.0f);//ゴールがない場合
}