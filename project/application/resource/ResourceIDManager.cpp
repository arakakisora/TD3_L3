#include "ResourceIDManager.h"
#include <fstream>

nlohmann::json ResourceIDManager::json_;

void ResourceIDManager::Load(const std::string& jsonPath) {
    std::ifstream file(jsonPath);
    if (!file) throw std::runtime_error("JSONファイルが開けません: " + jsonPath);
    file >> json_;
}

std::string ResourceIDManager::GetPath(const std::string& category, const std::string& id) {
    if (json_["Models"].contains(category) && json_["Models"][category].contains(id)) {
        std::string fullPath = std::string(json_["Models"][category][id]);
        // ここで必要に応じて "Resources/" 削除や拡張子削除もできる
        return fullPath;
    }
    return "";
}
