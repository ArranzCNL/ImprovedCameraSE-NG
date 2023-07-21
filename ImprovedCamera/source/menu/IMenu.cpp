/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

// Precompiled Header
#include "stdafx.h"

#include "menu/IMenu.h"

namespace Interface {

	IMenu::IMenu()
	{
		m_pluginConfig = DLLMain::Plugin::Get()->Config();
	}

	Menu::~Menu()
	{
		for (IMenu* menu : m_Menus)
		{
			menu->OnShutdown();
			delete menu;
		}
	}

	void Menu::Register(IMenu* menu)
	{
		m_Menus.emplace(m_Menus.begin() + m_MenuInsertIndex, menu);
		m_MenuInsertIndex++;
	}

	void Menu::Unregister(IMenu* menu)
	{
		auto it = std::find(m_Menus.begin(), m_Menus.begin() + m_MenuInsertIndex, menu);

		if (it != m_Menus.begin() + m_MenuInsertIndex)
		{
			menu->OnShutdown();
			m_Menus.erase(it);
			m_MenuInsertIndex--;
		}
	}

}
