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
		m_MenuNodes.emplace_back(1, "First Person Hands", "Adjust the field of view of Hands",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_FOV.fFirstPersonHands, 1.0f, 165.0f, "%.0f");
		m_MenuNodes.emplace_back(1, "Enable FOV Override", "On by default, Turn off for manual adjustment(s)",
			ControlType::kToggle, (void*)&m_pluginConfig->m_FOV.bEnableOverride);
		m_MenuNodes.emplace_back(1, "First Person", "Adjust field of view for First Person",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_FOV.fFirstPerson, 1.0f, 165.0f, "%.0f", &MenuFOV::OnCallback);
		m_MenuNodes.emplace_back(1, "First Person Combat", "Adjust field of view for First Person Combat",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_FOV.fFirstPersonCombat, 1.0f, 165.0f, "%.0f");
		m_MenuNodes.emplace_back(1, "Furniture", "Adjust field of view for Furniture (Bed, Chair, Bench, etc)",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_FOV.fFurniture, 1.0f, 165.0f, "%.0f");
		m_MenuNodes.emplace_back(1, "Crafting", "Adjust field of view at Crafting (Forge, Smelter. Grinding Stone, ect)",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_FOV.fCrafting, 1.0f, 165.0f, "%.0f");
		m_MenuNodes.emplace_back(1, "Killmove", "Adjust field of view during Killmoves",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_FOV.fKillmove, 1.0f, 165.0f, "%.0f");
		m_MenuNodes.emplace_back(1, "Ragdoll", "Adjust field of view for Ragdolling",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_FOV.fRagdoll, 1.0f, 165.0f, "%.0f");
		m_MenuNodes.emplace_back(1, "Death", "Adjust field of view for Death",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_FOV.fDeath, 1.0f, 165.0f, "%.0f");
		m_MenuNodes.emplace_back(1, "Cannibal", "Adjust field of view for Cannibal",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_FOV.fCannibal, 1.0f, 165.0f, "%.0f");
		m_MenuNodes.emplace_back(1, "Horse", "Adjust field of view while mounted on a Horse (Idle)",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_FOV.fHorse, 1.0f, 165.0f, "%.0f");
		m_MenuNodes.emplace_back(1, "Horse Combat", "Adjust field of view for mounted Combat",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_FOV.fHorseCombat, 1.0f, 165.0f, "%.0f");
		m_MenuNodes.emplace_back(1, "Horse Transition", "Adjust field of view for Horse Transition (Mounting/Dismounting)",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_FOV.fHorseTransition, 1.0f, 165.0f, "%.0f");
		// Table 2
		m_MenuNodes.emplace_back(2, "", "", ControlType::kNone);
		m_MenuNodes.emplace_back(2, "", "", ControlType::kNone);
		m_MenuNodes.emplace_back(2, "", "", ControlType::kNone);
		m_MenuNodes.emplace_back(2, "Dragon", "Adjust field of view while Mounted On A Dragon",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_FOV.fDragon, 1.0f, 165.0f, "%.0f");
		m_MenuNodes.emplace_back(2, "Dragon Combat", "Adjust field of view for Dragon Combat",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_FOV.fDragonCombat, 1.0f, 165.0f, "%.0f");
		m_MenuNodes.emplace_back(2, "Dragon Transition", "Adjust field of view for Dragon Transition (Mounting/Dismounting)",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_FOV.fDragonTransition, 1.0f, 165.0f, "%.0f");
		m_MenuNodes.emplace_back(2, "Vampire Lord", "Adjust field of view for Vampire Lord",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_FOV.fVampireLord, 1.0f, 165.0f, "%.0f");
		m_MenuNodes.emplace_back(2, "Werewolf", "Adjust field of view for Werewolf",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_FOV.fWerewolf, 1.0f, 165.0f, "%.0f");
		m_MenuNodes.emplace_back(2, "Lich", "Adjust the field of view for Lich",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_FOV.fNecroLich, 1.0f, 165.0f, "%.0f");
		m_MenuNodes.emplace_back(2, "Scripted", "Adjust field of view for Scripted Events",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_FOV.fScripted, 1.0f, 165.0f, "%.0f");
		m_MenuNodes.emplace_back(2, "Third Person", "Adjust field of view for Third Person",
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
