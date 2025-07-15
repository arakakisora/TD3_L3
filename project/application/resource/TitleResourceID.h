#pragma once
#include <string>

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

// タイトルのリソースID
class TitleResourceID {
public:
    // モデルのファイルパス指定
    static std::string GetModelPath(ModelID id) {
        return "Title/" + modelBaseNameTable().at(id) + ".obj";
    }
    
    // スプライトのファイルパス指定
    static std::string GetTexturePath(TextureID id) {
        return "Resources/Title/" + textureBaseNameTable().at(id) + ".png";
    }
    
    // 音声のファイルパス指定
    static std::string GetAudioPath(AudioID id) {
        return "Resources/Audio/" + audioBaseNameTable().at(id) + ".wav";
    }
private:       
    // モデル用：拡張子やパスを除いたベース名のみ
    static const std::unordered_map<ModelID, std::string>& modelBaseNameTable() {
        static const std::unordered_map<ModelID, std::string> table = {
            { ModelID::Text_Title, "Text_Title" },
            { ModelID::BackPlane, "backPlane" },
            { ModelID::PlayerCharacter, "playercharacter" },
            { ModelID::ShutterEffect,    "shutterEffect" },                        
            { ModelID::UI_Title_Stsrt,    "UI_Title_Stsrt" },
            { ModelID::WhiteRooms,    "WhiteRooms" },
        };
        return table;
    }

    // テクスチャ用：拡張子やパスを除いたベース名のみ
    static const std::unordered_map<TextureID, std::string>& textureBaseNameTable() {
        static const std::unordered_map<TextureID, std::string> table = {
            { TextureID::WhiteRooms,  "WhiteRooms" },            
            { TextureID::white,  "white" },           
        };
        return table;
    }

    // 音声用：拡張子やパスを除いたベース名のみ
    static const std::unordered_map<AudioID, std::string>& audioBaseNameTable() {
        static const std::unordered_map<AudioID, std::string> table = {
            { AudioID::Button,  "Button" },            
            { AudioID::Camera_copy,  "Camera_copy" },           
            { AudioID::bgm,  "bgm" },         
        };
        return table;
    }
};