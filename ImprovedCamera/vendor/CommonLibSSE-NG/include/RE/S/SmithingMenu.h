#pragma once

#include "RE/B/BSTArray.h"
#include "RE/B/BSTHashMap.h"
#include "RE/C/CraftingSubMenu.h"
#include "RE/I/IMessageBoxCallback.h"

namespace RE
{
	class InventoryEntryData;

	namespace CraftingSubMenus
	{
		class SmithingMenu : public CraftingSubMenu
		{
		public:
			inline static constexpr auto RTTI = RTTI_CraftingSubMenus__SmithingMenu;

			class SmithingConfirmCallback : public IMessageBoxCallback
			{
			public:
				inline static constexpr auto RTTI = RTTI_CraftingSubMenus__SmithingMenu__SmithingConfirmCallback;

				~SmithingConfirmCallback() override;  // 00

				// override (IMessageBoxCallback)
				void Run(Message a_msg) override;  // 01

				// members
				SmithingMenu* subMenu;  // 10
			};
			static_assert(sizeof(SmithingConfirmCallback) == 0x18);

			~SmithingMenu() override;  // 00

			// override (CraftingSubMenu)
			void Accept(CallbackProcessor* a_cbReg) override;  // 01
			void Unk_02(void) override;                        // 02
			void Unk_05(void) override;                        // 05

			// members
			BSTArray<void*>              unk100;        // 100
			BSTHashMap<UnkKey, UnkValue> unk118;        // 118 - constructibleObject map?
			NiPointer<TESObjectREFR>     furnitureRef;  // 148
			std::uint32_t                unk150;        // 150
			std::int32_t                 unk154;        // 154
			FormType                     smithingType;  // 158
			std::uint32_t                unk15C;        // 15C
			InventoryEntryData*          unk160;        // 160
		};
		static_assert(sizeof(SmithingMenu) == 0x168);
	}
}
