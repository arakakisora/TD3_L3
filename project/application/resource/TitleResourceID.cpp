#include "TitleResourceID.h"

TitleResourceID::operator std::string() const {
    switch (id_) {
    case TitlesceneModelID::Text_Title:      return ResourceIDManager::GetPath("Titlescene", "Text_Title");
    case TitlesceneModelID::BackPlane:       return ResourceIDManager::GetPath("Titlescene", "backPlane");
    case TitlesceneModelID::PlayerCharacter: return ResourceIDManager::GetPath("Titlescene", "playercharacter");
    case TitlesceneModelID::ShutterEffect:   return ResourceIDManager::GetPath("Titlescene", "shutterEffect");
    case TitlesceneModelID::UI_Title_Stsrt:  return ResourceIDManager::GetPath("Titlescene", "UI_Title_Stsrt");
    case TitlesceneModelID::WhiteRooms:      return ResourceIDManager::GetPath("Titlescene", "WhiteRooms");
    default: return "";
    }
}
