#include"GameClearResourceID.h"

//std::string GameClearResourceID::GetModelPath(GameClearModelID id) {
//    return "GameClear/" + modelBaseNameTable().at(id) + ".obj";
//}
//
//std::string GameClearResourceID::GetTexturePath(GameClearTextureID id) {
//    return "Resources/GameClear/" + textureBaseNameTable().at(id) + ".png";
//}
//
//const std::unordered_map<GameClearModelID, std::string>& GameClearResourceID::modelBaseNameTable() {
//    static const std::unordered_map<GameClearModelID, std::string> table = {
//        { GameClearModelID::ClearText_01, "ClearText_01" },
//        { GameClearModelID::ClearText_02, "ClearText_02" },
//        { GameClearModelID::ClearText_03, "ClearText_03" },
//        { GameClearModelID::backPlane,     "backPlane" },
//    };
//    return table;
//}
//
//const std::unordered_map<GameClearTextureID, std::string>& GameClearResourceID::textureBaseNameTable() {
//    static const std::unordered_map<GameClearTextureID, std::string> table = {
//        { GameClearTextureID::Title,       "TextUI_Title" },           
//        { GameClearTextureID::NextStage,   "TextUI_Nextstage" },
//        { GameClearTextureID::StageSelect, "TextUI_Stageselect" },
//        { GameClearTextureID::ArrowUp,     "ArroUP" },
//    };
//    return table;
//}