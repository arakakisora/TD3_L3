#pragma once
#include <string>
#include "ResourceIDManager.h"

enum class TitlesceneModelID {
    Text_Title,
    BackPlane,
    PlayerCharacter,
    ShutterEffect,
    UI_Title_Stsrt,
    WhiteRooms,
};

class TitleResourceID {
public:
    explicit TitleResourceID(TitlesceneModelID id) : id_(id) {}
    operator std::string() const;

private:
    TitlesceneModelID id_;
};
