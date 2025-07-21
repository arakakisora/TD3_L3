#pragma once
#include <map>
#include <string>
#include <unordered_map>
#include <unordered_set>

enum class MapChipType {

	kBlank,			 	// 空白				No.0
	kPlayer,		    // Player初期位置		No.1
	kNCopyBlock,		// コピー不可			No.2
	kCopyBlock,    		// コピー可能			No.3
	kGoalUp,       		// ゴール上			No.4
	kGoalDown,			// ゴール下			No.5
	kFallBlock,			// 重力の影響受		No.6
	kFixedTimeBlock,	// 貼り付け前一定時間	No.7
	kPutFixedTimeBlock,	// 貼り付け後一定時間　	No.8
	kjumpBlock,         // プレイヤーの上昇 No.9
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
		{"8",MapChipType::kPutFixedTimeBlock},
		{"9",MapChipType::kjumpBlock},

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

// MapChipType に対応するモデル名
static const std::unordered_map<MapChipType, std::string> modelTable = {
	{ MapChipType::kBlank, "GamePlay/nullBlock.obj" },
	{ MapChipType::kNCopyBlock, "GamePlay/ncopyblock.obj" },
	{ MapChipType::kCopyBlock, "GamePlay/block.obj" },
	{ MapChipType::kGoalUp, "GamePlay/goalup.obj" },
	{ MapChipType::kGoalDown, "GamePlay/gall.obj" },
	{ MapChipType::kFallBlock, "GamePlay/fallblock.obj" },
	{ MapChipType::kFixedTimeBlock, "GamePlay/Timer.obj" },
	{ MapChipType::kPutFixedTimeBlock, "GamePlay/putTimer.obj" },
	{ MapChipType::kjumpBlock, "GamePlay/jump.obj" },
};

static const std::unordered_set<MapChipType> simpleUpdateTypes = {
		MapChipType::kCopyBlock,
		MapChipType::kGoalUp,
		MapChipType::kNCopyBlock,
		MapChipType::kFixedTimeBlock,
		MapChipType::kPutFixedTimeBlock,
		MapChipType::kFallBlock,
		MapChipType::kjumpBlock,
};

static const std::unordered_set<MapChipType> drawTypes = {
	MapChipType::kCopyBlock,
	MapChipType::kGoalUp,
	MapChipType::kGoalDown,
	MapChipType::kNCopyBlock,
	MapChipType::kFixedTimeBlock,
	MapChipType::kPutFixedTimeBlock,
	MapChipType::kFallBlock,
	MapChipType::kjumpBlock
};

struct MapChipMeta {
	std::string modelName;
	bool needsUpdate = false;
	bool needsDraw = false;
};



