/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

// Precompiled Header
#include "PCH.h"

#include "menu/Fov.h"

namespace Menu {

	struct MENU_IDS {

		enum MENU_ID : std::int32_t
		{
			kFirstPersonHands = 1,
			kEnableOverride,
			kFirstPerson,
			kFirstPersonCombat,
			kFurniture,
			kCrafting,
			kKillmove,
			kRagdoll,
			kDeath,
			kCannibal,
			kHorse,
			kHorseCombat,
			kHorseTransition,

			// Table 2
			kEmptyControl1,
			kEmptyControl2,
			kEmptyControl3,

			kDragon,
			kDragonCombat,
			kDragonTransition,
			kVampireLord,
			kWerewolf,
			kNecroLich,
			kScripted,
			kThirdPerson,

			kTotal = 25
		};
	};
	using MENU_ID = MENU_IDS::MENU_ID;

	MenuFOV::MenuFOV()
	{
		// Table 1
		m_MenuNodes.emplace_back(1, "First Person Hands", "Adjust the FOV of Hands",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_FOV.fFirstPersonHands, 1.0f, 165.0f, "%.0f");
		m_MenuNodes.emplace_back(1, "Enable FOV Override", "On by default, Turn off for manual adjustments",
			ControlType::kToggle, (void*)&m_pluginConfig->m_FOV.bEnableOverride);
		m_MenuNodes.emplace_back(1, "First Person", "Manually Adjust First Person FOV",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_FOV.fFirstPerson, 1.0f, 165.0f, "%.0f", &MenuFOV::OnCallback);
		m_MenuNodes.emplace_back(1, "First Person Combat", "Manually Adjust First Person Combat",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_FOV.fFirstPersonCombat, 1.0f, 165.0f, "%.0f");
		m_MenuNodes.emplace_back(1, "Furniture", "Manually Adjust Furniture FOV",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_FOV.fFurniture, 1.0f, 165.0f, "%.0f");
		m_MenuNodes.emplace_back(1, "Crafting", "Manually Adjust FOV at Crafting (Forge, Smelter. Grinding Stone, Ect.)",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_FOV.fCrafting, 1.0f, 165.0f, "%.0f");
		m_MenuNodes.emplace_back(1, "Killmove", "Manually Adjust FOV during Killmoves",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_FOV.fKillmove, 1.0f, 165.0f, "%.0f");
		m_MenuNodes.emplace_back(1, "Ragdoll", "Manually Adjust FOV for Ragdoll",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_FOV.fRagdoll, 1.0f, 165.0f, "%.0f");
		m_MenuNodes.emplace_back(1, "Death", "Manually Adjust FOV for Death",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_FOV.fDeath, 1.0f, 165.0f, "%.0f");
		m_MenuNodes.emplace_back(1, "Cannibal", "Manually Adjust FOV for Cannibal",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_FOV.fCannibal, 1.0f, 165.0f, "%.0f");
		m_MenuNodes.emplace_back(1, "Horse", "Manually Adjust FOV while mounted on a Horse (Idle)",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_FOV.fHorse, 1.0f, 165.0f, "%.0f");
		m_MenuNodes.emplace_back(1, "Horse Combat", "Manually Adjust FOV for mounted Combat",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_FOV.fHorseCombat, 1.0f, 165.0f, "%.0f");
		m_MenuNodes.emplace_back(1, "Horse Transition", "Manually Adjust FOV for Horse Transition (Mount/Dismount)",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_FOV.fHorseTransition, 1.0f, 165.0f, "%.0f");
		// Table 2
		m_MenuNodes.emplace_back(2, "", "", ControlType::kNone);
		m_MenuNodes.emplace_back(2, "", "", ControlType::kNone);
		m_MenuNodes.emplace_back(2, "", "", ControlType::kNone);
		m_MenuNodes.emplace_back(2, "Dragon", "Manually Adjust FOV while Mounted On A Dragon",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_FOV.fDragon, 1.0f, 165.0f, "%.0f");
		m_MenuNodes.emplace_back(2, "Dragon Combat", "Manually Adjust FOV for Dragon Combat",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_FOV.fDragonCombat, 1.0f, 165.0f, "%.0f");
		m_MenuNodes.emplace_back(2, "Dragon Transition", "Manually Adjust FOV for Dragon Transition (Mount/Dismount)",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_FOV.fDragonTransition, 1.0f, 165.0f, "%.0f");
		m_MenuNodes.emplace_back(2, "Vampire Lord", "Manually Adjust The FOV for Vampire Lord",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_FOV.fVampireLord, 1.0f, 165.0f, "%.0f");
		m_MenuNodes.emplace_back(2, "Werewolf", "Manually Adjust FOV for Werewolf",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_FOV.fWerewolf, 1.0f, 165.0f, "%.0f");
		m_MenuNodes.emplace_back(2, "Lich", "Manually Adjust the FOV for Lich",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_FOV.fNecroLich, 1.0f, 165.0f, "%.0f");
		m_MenuNodes.emplace_back(2, "Scripted", "Manually Adjust DOV for Scripted Events",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_FOV.fScripted, 1.0f, 165.0f, "%.0f");
		m_MenuNodes.emplace_back(2, "Third Person", "Manually Adjust FOV for Third Person",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_FOV.fThirdPerson, 1.0f, 165.0f, "%.0f", &MenuFOV::OnCallback);
	}

	void MenuFOV::OnOpen()
	{
		ImGui::Checkbox("FOV", &m_Window);
	}

	void MenuFOV::OnUpdate()
	{
		if (!m_Window)
			return;

		ImGui::Begin("[FOV]", &m_Window, ImGuiWindowFlags_NoCollapse);

		DisplayMenuNodes("FOVTable");
		ImGui::SameLine();
		DisplayMenuNodes("FOVTable", 2);

		if (ImGui::Button("Close"))
			m_Window = false;

		ImGui::End();
	}

	void MenuFOV::OnCallback(std::uint32_t node, bool begin)
	{
		auto pluginConfig = DLLMain::Plugin::Get()->Config();

		switch (node)
		{
			case MENU_ID::kFirstPerson:
			{
				if (begin && !pluginConfig->FOV().bEnableOverride)
					ImGui::BeginDisabled();

				break;
			}
			case MENU_ID::kThirdPerson:
			{
				if (!begin && !pluginConfig->FOV().bEnableOverride)
					ImGui::EndDisabled();

				break;
			}
		}
	}

}
