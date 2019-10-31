#ifndef __SETTINGS_ELEMENT__
#define __SETTINGS_ELEMENT__

#include <string>
#include <list>

enum SettingType
{
    SettingType_Int,
    SettingType_Double,
    SettingType_String,
    SettingType_Bool,
};

struct SettingsElement
{
    std::string name;
    std::string currentValue;
    std::string defaultValue;
    SettingType type;
};

typedef std::list<SettingsElement> SettingsList;

#endif