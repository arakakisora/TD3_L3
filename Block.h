#pragma once
#include "Vector3.h"
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

class Map;
struct MapChipData;

enum class MapChipType {

	kBlank,      	// 空白				No.0
	kPlayer,        // Player初期位置		No.1
	kNCopyBlock,    // コピー不可			No.2
	kCopyBlock,    	// コピー可能			No.3
	kGoalUp,       	// ゴール上			No.4
	kGoalDown,		// ゴール下			No.5
	kFallBlock,		// 重力の影響受		No.6
	kFixedTimeBlock,// 貼り付け後一定時間有	No.7
};

namespace {

	std::map<std::string, MapChipType> mapChipTable = {
		{"0", MapChipType::kBlank},
		{"1", MapChipType::kPlayer},
		{"2", MapChipType::kNCopyBlock},
		{"3", MapChipType::kCopyBlock},
		{"4", MapChipType::kGoalUp},
		{"5", MapChipType::kGoalDown},
		{"6", MapChipType::kFallBlock},
		{"7", MapChipType::kFixedTimeBlock },

		/*０：空
	１：プレイヤーの初期位置
	２：コピー不可能ブロック
	３：コピー可能ブロック
	４：ゴール（上）
	５：ゴール（下）
	６：重力の影響を受けるブロック（FallBlock）
	７：一定時間経過したら消えるブロック（FixedTime）*/
	};
}
class Object3D;
class Block
{
public:
	Block();
	~Block();

	// 初期化
	void Initialize(MapChipType type, const Vector3& position);
	// 更新
	void Update();
	// 描画
	void Draw();

	// 終了処理
	void Finalize();

	// SetPosition

	static Block* CreateBlock(MapChipType type, const Vector3& position);

private:
	int mapID;
	MapChipType type;
	Object3D* object3D;
	Map* map;

	Vector3 position = { 0.0f,0.0f,0.0f };
};

