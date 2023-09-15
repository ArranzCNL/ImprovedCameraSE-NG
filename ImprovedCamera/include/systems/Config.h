/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "settings/Settings.h"

// Forward declare Plugin
namespace DLLMain {

	class Plugin;
}
// Forward declare Menus
namespace Menu {

	class UIMenu;
	class MenuGeneral;
	class MenuHide;
	class MenuFixes;
	class MenuRestrictAngles;
	class MenuEvents;
	class MenuFOV;
	class MenuNearDistance;
	class MenuHeadbob;
	class MenuCamera;
}

namespace Systems {

	class Config {

	public:
		Config();
		~Config() = default;

	public:
		const std::string& Name() const { return m_Name; };
		const std::string& Path() const { return m_Path; };
		const std::string& FontPath() const { return m_FontPath; };
		const std::string& ProfilePath() const { return m_ProfilePath; };
		const std::string& FileName() const { return m_FileName; };

	public:
		const Settings::ModuleData& ModuleData() const { return m_ModuleData; };
		// Data from ImprovedCameraSE.ini
		const Settings::General& General() const { return m_General; };
		const Settings::Hide& Hide() const { return m_Hide; };
		const Settings::Fixes& Fixes() const { return m_Fixes; };
		const Settings::RestrictAngles& RestrictAngles() const { return m_RestrictAngles; };
		const Settings::Events& Events() const { return m_Events; };
		const Settings::FOV& FOV() const { return m_FOV; };
		const Settings::NearDistance& NearDistance() const { return m_NearDistance; };
		const Settings::Headbob& Headbob() const { return m_Headbob; };
		const Settings::Camera& Camera() const { return m_Camera; };
		const Settings::Logging& Logging() const { return m_Logging; };

	private:
		bool ReadIni(std::string& name);
		void WriteIni(std::string& name, bool updateMain = false);

	private:
		bool m_Initialized = false;
		bool m_PreInitialized = false;

		std::string m_Name{};
		std::string m_FileName{};
		std::string m_Path{};
		std::string m_FontPath{};
		std::string m_ProfilePath{};

	private:
		Settings::ModuleData m_ModuleData{};
		// Data from ImprovedCameraSE.ini
		Settings::General m_General{};
		Settings::Hide m_Hide{};
		Settings::Fixes m_Fixes{};
		Settings::RestrictAngles m_RestrictAngles{};
		Settings::Events m_Events{};
		Settings::FOV m_FOV{};
		Settings::NearDistance m_NearDistance{};
		Settings::Headbob m_Headbob{};
		Settings::Camera m_Camera{};
		Settings::Logging m_Logging{};

	private:
		friend class DLLMain::Plugin;

		friend class Menu::UIMenu;
		friend class Menu::MenuGeneral;
		friend class Menu::MenuHide;
		friend class Menu::MenuFixes;
		friend class Menu::MenuRestrictAngles;
		friend class Menu::MenuEvents;
		friend class Menu::MenuFOV;
		friend class Menu::MenuNearDistance;
		friend class Menu::MenuHeadbob;
		friend class Menu::MenuCamera;
	};

}
