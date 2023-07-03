/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

 // Precompiled Header
#include "stdafx.h"

#include "systems/UI.h"

#include "menu/UIMenu.h"


namespace Systems {

	std::unique_ptr<UI> UI::CreateMenu(HWND windowMenu)
	{
		return std::make_unique<Menu::UIMenu>(windowMenu);
	}

}
