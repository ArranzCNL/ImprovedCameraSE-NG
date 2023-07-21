/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

namespace RE {

	class BSLookAtModifier {

	public:
		inline static constexpr auto RTTI = RTTI_BSLookAtModifier;
		inline static constexpr auto VTABLE = VTABLE_BSLookAtModifier;

		virtual ~BSLookAtModifier();  // 00
	};
	//static_assert(sizeof(BSLookAtModifier) == 0x277);
}

namespace Menu {

	class UIMenu;
	class MenuGeneral;
}

namespace Patch {

	class Hooks {

	public:
		Hooks() = default;
		~Hooks();

	private:
		void Install();
		void Setup();
#ifdef _DEBUG
		void AddressDump();
#endif

	private:
		friend class SkyrimSE;
		friend class Menu::UIMenu;
		friend class Menu::MenuGeneral;
	};

}
