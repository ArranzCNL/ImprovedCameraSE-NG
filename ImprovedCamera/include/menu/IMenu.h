/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "menu/UIMenuHelper.h"
#include "plugin.h"


namespace Interface {

	class IMenu {

	public:
		IMenu();
		virtual ~IMenu() = default;

		virtual void OnOpen() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnClose() = 0;

		virtual void OnShutdown() = 0;

	protected:
		bool m_Window = false;
		Systems::Config* m_pluginConfig = nullptr;
	};

	class Menu {

	public:
		Menu() = default;
		~Menu();

		void Register(IMenu* menu);
		void Unregister(IMenu* menu);

		std::vector<IMenu*>::iterator begin() { return m_Menus.begin(); }
		std::vector<IMenu*>::iterator end() { return m_Menus.end(); }
		std::vector<IMenu*>::reverse_iterator rbegin() { return m_Menus.rbegin(); }
		std::vector<IMenu*>::reverse_iterator rend() { return m_Menus.rend(); }

		std::vector<IMenu*>::const_iterator begin() const { return m_Menus.begin(); }
		std::vector<IMenu*>::const_iterator end() const { return m_Menus.end(); }
		std::vector<IMenu*>::const_reverse_iterator rbegin() const { return m_Menus.rbegin(); }
		std::vector<IMenu*>::const_reverse_iterator rend() const { return m_Menus.rend(); }

	private:
		std::vector<IMenu*> m_Menus{};
		std::uint32_t m_MenuInsertIndex = 0;
	};

}
