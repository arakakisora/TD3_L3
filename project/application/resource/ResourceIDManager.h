#pragma once
#include <json.hpp>
#include <string>

class ResourceIDManager {
public:
    static void Load(const std::string& jsonPath);
    static std::string GetPath(const std::string& category, const std::string& id);

private:
    static nlohmann::json json_;
};