#pragma once
#include <string>
#include <unordered_map>
#include<ResourceID.h>

// ステージセレクトのリソースID
class StageSelectResourceID {
public:

    // モデルのファイルパス指定
    static std::string GetModelPath(StageSelectID::ModelID id) {
        return "StageSelect/" + modelBaseNameTable().at(id) + ".obj";
    }

    //// スプライトのファイルパス指定
    //static std::string GetTexturePath(StageSelectID::TextureID id) {
    //    return "Resources/StageSelect/" + textureBaseNameTable().at(id) + ".png";
    //}

    //// 音声のファイルパス指定
    //static std::string GetAudioPath(StageSelectID::AudioID id) {
    //    return "Resources/Audio/" + audioBaseNameTable().at(id) + ".wav";
    //}
private:
    // モデル用：拡張子やパスを除いたベース名のみ
    static const std::unordered_map<StageSelectID::ModelID, std::string>& modelBaseNameTable() {
        static const std::unordered_map<StageSelectID::ModelID, std::string> table = {
            { StageSelectID::ModelID::Stage01, "Stage01" },
            { StageSelectID::ModelID::Stage02, "Stage02" },
            { StageSelectID::ModelID::Stage03, "Stage03" },
            { StageSelectID::ModelID::Stage04, "Stage04" },
            { StageSelectID::ModelID::Stage05, "Stage05" },
            { StageSelectID::ModelID::Stage06, "Stage06" },	
            { StageSelectID::ModelID::Stage07, "Stage07" },
			{ StageSelectID::ModelID::Stage08, "Stage08" },
			{ StageSelectID::ModelID::Stage09, "Stage09" },
			{ StageSelectID::ModelID::Stage10, "Stage10" },
			{ StageSelectID::ModelID::Stage11, "Stage11" },
			{ StageSelectID::ModelID::Stage12, "Stage12" },
			{ StageSelectID::ModelID::Stage13, "Stage13" },
			{ StageSelectID::ModelID::playercharacter, "playercharacter" },
			{ StageSelectID::ModelID::SelectSceneBackPlane, "SelectSceneBackPlane" },
        };
        return table;
    }

    //// テクスチャ用：拡張子やパスを除いたベース名のみ
    //static const std::unordered_map<StageSelectID::TextureID, std::string>& textureBaseNameTable() {
    //    static const std::unordered_map<StageSelectID::TextureID, std::string> table = {
    //        { StageSelectID::TextureID::WhiteRooms,  "WhiteRooms" },
    //        { StageSelectID::TextureID::white,  "white" },
    //    };
    //    return table;
    //}

    //// 音声用：拡張子やパスを除いたベース名のみ
    //static const std::unordered_map<StageSelectID::AudioID, std::string>& audioBaseNameTable() {
    //    static const std::unordered_map<StageSelectID::AudioID, std::string> table = {
    //        { StageSelectID::AudioID::Button,  "Button" },
    //        { StageSelectID::AudioID::Camera_copy,  "Camera_copy" },
    //        { StageSelectID::AudioID::bgm,  "bgm" },
    //    };
    //    return table;
    //}
};