#pragma once
#include <string>
#include "ResourceIDManager.h"

enum class GameClearModelID {
    ClearText_01, 
    ClearText_02, 
    ClearText_03,
    backPlane,
};

enum class GameClearTextureID {
    Title,
    NextStage,
    StageSelect,
    ArrowUp,
};

class GameClearResourceID {
public:
    static std::string GetModelPath(GameClearModelID id) {
        return "GameClear/" + modelBaseNameTable().at(id) + ".obj";
    }

    static std::string GetTexturePath(GameClearTextureID id) {
        return "Resources/GameClear/" + textureBaseNameTable().at(id) + ".png";
    }

private:
     // モデル用：拡張子やパスを除いたベース名のみ
    static const std::unordered_map<GameClearModelID, std::string>& modelBaseNameTable() {
        static const std::unordered_map<GameClearModelID, std::string> table = {
            { GameClearModelID::ClearText_01, "ClearText_01" },
            { GameClearModelID::ClearText_02, "ClearText_02" },
            { GameClearModelID::ClearText_03, "ClearText_03" },
            { GameClearModelID::backPlane,    "backPlane" },
        };
        return table;
    }

    // テクスチャ用：こちらも拡張子を除いたベース名のみ
    static const std::unordered_map<GameClearTextureID, std::string>& textureBaseNameTable() {
        static const std::unordered_map<GameClearTextureID, std::string> table = {
            { GameClearTextureID::Title,  "TextUI_Title" },
            { GameClearTextureID::NextStage,  "TextUI_Nextstage" },
            { GameClearTextureID::StageSelect,"TextUI_Stageselect" },
            { GameClearTextureID::ArrowUp,           "ArroUP" }, 
        };
        return table;
    }
};