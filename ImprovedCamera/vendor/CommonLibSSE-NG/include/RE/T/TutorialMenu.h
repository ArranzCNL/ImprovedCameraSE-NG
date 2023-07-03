#pragma once

#include "RE/G/GFxValue.h"
#include "RE/I/IMenu.h"

namespace RE
{
	// menuDepth = 10
	// flags = kPausesGame | kModal | kUpdateUsesCursor
	// context = kMenuMode
	class TutorialMenu : public IMenu
	{
	public:
		inline static constexpr auto      RTTI = RTTI_TutorialMenu;
		constexpr static std::string_view MENU_NAME = "Tutorial Menu";

		~TutorialMenu() override;  // 00

		// override (IMenu)
		void               Accept(CallbackProcessor* a_processor) override;  // 01
		UI_MESSAGE_RESULTS ProcessMessage(UIMessage& a_message) override;    // 04

		[[nodiscard]] inline GFxValue GetRoot() const noexcept
		{
			return REL::RelocateMember<GFxValue>(this, 0x30, 0x40);
		}

		inline void SetRoot(GFxValue a_root) noexcept
		{
			REL::RelocateMember<GFxValue>(this, 0x30, 0x40) = a_root;
		}

		// members
#ifndef SKYRIM_CROSS_VR
		GFxValue root;  // 30, 40 - "Menu_mc"
#endif
	};
#ifndef ENABLE_SKYRIM_VR
	static_assert(sizeof(TutorialMenu) == 0x48);
#elif !defined(ENABLE_SKYRIM_AE) && !defined(ENABLE_SKYRIM_SE)
	static_assert(sizeof(TutorialMenu) == 0x58);
#endif
}
