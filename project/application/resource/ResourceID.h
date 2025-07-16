#pragma once

namespace TitleID  {
    enum class ModelID { // モデルのID
        Text_Title,
        BackPlane,
        PlayerCharacter,
        ShutterEffect,
        UI_Title_Stsrt,
        WhiteRooms,
        white,
    };

    enum class TextureID {// スプライトのID
        WhiteRooms,
        white,
    };

    enum class AudioID {// 音声のID
        Button,
        Camera_copy,
        bgm,
    };
}

namespace GameClearID  {
    enum class ModelID { // モデルのID
        ClearText_01,
        ClearText_02,
        ClearText_03,
        backPlane,
    };

    enum class TextureID {// スプライトのID
        Title,
        NextStage,
        StageSelect,
        ArrowUp,
    };
}

namespace StageSelectID {
    enum class ModelID { // モデルのID 	
        Stage01, Stage02, Stage03, Stage04, Stage05,
        Stage06, Stage07, Stage08, Stage09, Stage10,
        Stage11, Stage12, Stage13,
        playercharacter,
        SelectSceneBackPlane,
    };

    enum class TextureID {// スプライトのID



    };
    
    enum class AudioID {// 音声のID
    
    
    };
}