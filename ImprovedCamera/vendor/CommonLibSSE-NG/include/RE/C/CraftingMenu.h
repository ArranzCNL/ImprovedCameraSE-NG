#pragma once

#include "RE/I/IMenu.h"

namespace RE
{
	namespace CraftingSubMenus
	{
		class CraftingSubMenu;
	}

	// menuDepth = 0
	// flags = kUsesMenuContext | kDisablePauseMenu | kUpdateUsesCursor | kInventoryItemMenu | kDontHideCursorWhenTopmost
	// context = kItemMenu
	class CraftingMenu : public IMenu
	{
	public:
		inline static constexpr auto      RTTI = RTTI_CraftingMenu;
		constexpr static std::string_view MENU_NAME = "Crafting Menu";

		~CraftingMenu() override;  // 00

		// override (IMenu)
		UI_MESSAGE_RESULTS ProcessMessage(UIMessage& a_message) override;                         // 04
		void               AdvanceMovie(float a_interval, std::uint32_t a_currentTime) override;  // 05
		void               PostDisplay() override;                                                // 06

		[[nodiscard]] inline CraftingSubMenus::CraftingSubMenu* GetCraftingSubMenu() const noexcept
		{
			return REL::RelocateMember<CraftingSubMenus::CraftingSubMenu*>(this, 0x30, 0x40);
		}

		inline void SetCraftingSubMenu(CraftingSubMenus::CraftingSubMenu* craftingSubMenu) noexcept
		{
			REL::RelocateMember<CraftingSubMenus::CraftingSubMenu*>(this, 0x30, 0x40) = craftingSubMenu;
		}

		// members
#ifndef SKYRIM_CROSS_VR
		CraftingSubMenus::CraftingSubMenu* subMenu;  // 30, 40
#endif
	};
#ifndef ENABLE_SKYRIM_VR
	static_assert(sizeof(CraftingMenu) == 0x38);
#elif !defined(ENABLE_SKYRIM_AE) && !defined(ENABLE_SKYRIM_SE)
	static_assert(sizeof(CraftingMenu) == 0x48);
#endif
}
