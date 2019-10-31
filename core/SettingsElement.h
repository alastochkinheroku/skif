#ifndef __SETTINGS_ELEMENT__
#define __SETTINGS_ELEMENT__

#include <string>
#include <list>

namespace core
{

enum SettingType
{
    SettingType_Int,
    SettingType_Double,
    SettingType_String,
    SettingType_Bool,
};

//Описатель настроек
struct SettingsDesc
{
    std::string name;
    std::string defaultValue;
    SettingType type;
};

//Описатель настроек
struct SettingsElement
{
	std::string name;
	std::string currentValue;
};

typedef std::list<SettingsDesc> SettingsDescList;
typedef std::list<SettingsElement> SettingsElementList;

}

#endif