#pragma once
#include <string>
#include<ResourceID.h>

// タイトルのリソースID
class TitleResourceID {
public:
    // モデルのファイルパス指定
    static std::string GetModelPath(TitleID::ModelID id) {
        return "Title/" + modelBaseNameTable().at(id) + ".obj";
    }
    
    // スプライトのファイルパス指定
    static std::string GetTexturePath(TitleID::TextureID id) {
        return "Resources/Title/" + textureBaseNameTable().at(id) + ".png";
    }
    
    // 音声のファイルパス指定
    static std::string GetAudioPath(TitleID::AudioID id) {
        return "Resources/Audio/" + audioBaseNameTable().at(id) + ".wav";
    }
private:       
    // モデル用：拡張子やパスを除いたベース名のみ
    static const std::unordered_map<TitleID::ModelID, std::string>& modelBaseNameTable() {
        static const std::unordered_map<TitleID::ModelID, std::string> table = {
            { TitleID::ModelID::Text_Title, "Text_Title" },
            { TitleID::ModelID::BackPlane, "backPlane" },
            { TitleID::ModelID::PlayerCharacter, "playercharacter" },
            { TitleID::ModelID::ShutterEffect,    "shutterEffect" },
            { TitleID::ModelID::UI_Title_Stsrt,    "UI_Title_Stsrt" },
            { TitleID::ModelID::WhiteRooms,    "WhiteRooms" },
        };
        return table;
    }

    // テクスチャ用：拡張子やパスを除いたベース名のみ
    static const std::unordered_map<TitleID::TextureID, std::string>& textureBaseNameTable() {
        static const std::unordered_map<TitleID::TextureID, std::string> table = {
            { TitleID::TextureID::WhiteRooms,  "WhiteRooms" },            
            { TitleID::TextureID::white,  "white" },           
        };
        return table;
    }

    // 音声用：拡張子やパスを除いたベース名のみ
    static const std::unordered_map<TitleID::AudioID, std::string>& audioBaseNameTable() {
        static const std::unordered_map<TitleID::AudioID, std::string> table = {
            { TitleID::AudioID::Button,  "Button" },            
            { TitleID::AudioID::Camera_copy,  "Camera_copy" },           
            { TitleID::AudioID::bgm,  "bgm" },         
        };
        return table;
    }
};