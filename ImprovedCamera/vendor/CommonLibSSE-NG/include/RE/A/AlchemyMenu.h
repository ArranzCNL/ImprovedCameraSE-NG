#pragma once

#include "RE/B/BSFixedString.h"
#include "RE/B/BSTArray.h"
#include "RE/C/CraftingSubMenu.h"
#include "RE/I/IMessageBoxCallback.h"

namespace RE::CraftingSubMenus
{
	class AlchemyMenu : public CraftingSubMenu
	{
	public:
		inline static constexpr auto RTTI = RTTI_CraftingSubMenus__AlchemyMenu;

		class QuitMenuCallback : public IMessageBoxCallback
		{
		public:
			inline static constexpr auto RTTI = RTTI_CraftingSubMenus__AlchemyMenu__QuitMenuCallback;

			~QuitMenuCallback() override;  // 00

			// override (IMessageBoxCallback)
			void Run(Message a_msg) override;  // 01

			// members
			AlchemyMenu* subMenu;  // 10
		};
		static_assert(sizeof(QuitMenuCallback) == 0x18);

		class UsableEffectMap
		{
		public:
			std::uint32_t ingredientIndex;               // 00
			std::uint32_t effectIndex;                   // 04
			std::uint32_t nextCompatibleEffectMapIndex;  // 08
		};
		static_assert(sizeof(UsableEffectMap) == 0xC);

			class MenuIngredientEntry
			{
			public:
				InventoryEntryData* ingredient;       // 00
				std::uint8_t        effect1FilterID;  // 09
				std::uint8_t        effect2FilterID;  // 0A
				std::uint8_t        effect3FilterID;  // 0B
				std::uint8_t        effect4FilterID;  // 0C
				std::uint8_t        isSelected;       // 0D
				std::uint8_t        isNotGreyed;      // 0E
				std::uint16_t       padE;             // 0F
			};
			static_assert(sizeof(MenuIngredientEntry) == 0x10);

		class PotionCreationData
		{
		public:
			BSTArray<UsableEffectMap>      usableEffectsMaps;  // 0
			BSTArray<MenuIngredientEntry>* ingredientEntries;  // 18
		};
		static_assert(sizeof(PotionCreationData) == 0x20);

		~AlchemyMenu() override;  // 00

		// override (CraftingSubMenu)
		void Accept(CallbackProcessor* a_cbReg) override;  // 01
		void Unk_05(void) override;                        // 05

		// members
		RE::BSTArray<MenuIngredientEntry> ingredientsEntries;       // 100
		std::int32_t                      unk118;                   // 118
		std::int32_t                      unk11c;                   // 11c
		std::uint32_t                     ingredientIndex1;         // 120
		std::uint32_t                     ingredientIndex2;         // 124
		std::uint32_t                     ingredientIndex3;         // 128
		std::uint32_t                     unk12c;                   // 12c
		std::uint32_t                     selectedIngredientCount;  // 130
		std::uint32_t                     unk134;                   // 134
		PotionCreationData                potionCreationData;       // 138
		RE::GFxValue                      unk158;                   // 158
		std::uint64_t                     unk170;                   // 170
		RE::TESForm*                      outputPotion;             // 178
		RE::TESForm*                      outputPotion2;            // 180
		std::uint32_t                     unk188;                   // 188
		std::uint32_t                     unk18c;                   // 18c
		RE::BSFixedString                 newRecipeFormatString;    // 190
		std::uint64_t                     playerHasPurityPerk;      // 198
	};
	static_assert(sizeof(AlchemyMenu) == 0x1A0);
}
