#include "RE/I/INISettingCollection.h"

namespace RE
{
	INISettingCollection* INISettingCollection::GetSingleton()
	{
		REL::Relocation<INISettingCollection**> singleton{ Offset::INISettingCollection::Singleton };
		return *singleton;
	}

	Setting* INISettingCollection::GetSetting(std::string_view a_name)
	{
		for (auto& setting : settings) {
			if (a_name.size() == strlen(setting->name) &&
				_strnicmp(setting->name, a_name.data(), a_name.size()) == 0) {
				return setting;
			}
		}
		return nullptr;
	}
}
