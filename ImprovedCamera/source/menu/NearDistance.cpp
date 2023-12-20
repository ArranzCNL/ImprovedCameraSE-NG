/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

// Precompiled Header
#include "PCH.h"

#include "menu/NearDistance.h"

namespace Menu {

	struct MENU_IDS {

		enum MENU_ID : std::int32_t
		{
			kEnableOverride = 1,
			kPitchThreshold,
			kFirstPerson,
			kFirstPersonCombat,
			kSitting,
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
			kDragon,
			kDragonCombat,
			kDragonTransition,
			kVampireLord,
			kWerewolf,
			kNecroLich,
			kScripted,
			kThirdPerson,

			kTotal = 23
		};
	};
	using MENU_ID = MENU_IDS::MENU_ID;

	MenuNearDistance::MenuNearDistance()
	{
		// Table 1
		m_MenuNodes.emplace_back(1, "Enable Near Distance Override", "",
			ControlType::kToggle, (void*)&m_pluginConfig->m_NearDistance.bEnableOverride);
		m_MenuNodes.emplace_back(1, "Pitch Threshold", "Pitch Threshold for Changing the fNearDistance automatically",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_NearDistance.fPitchThreshold, -90.0f, 0.0f, "%.0f", &MenuNearDistance::OnCallback);
		m_MenuNodes.emplace_back(1, "First Person", "Reduce fNearDistance when Pitch threshold is Met",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_NearDistance.fFirstPerson, 1.0f, 15.0f, "%.0f");
		m_MenuNodes.emplace_back(1, "First Person Combat", "FNearDistance Control of First Person Combat that is Independant of The Pitch Threshold",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_NearDistance.fFirstPersonCombat, 1.0f, 15.0f, "%.0f");
		m_MenuNodes.emplace_back(1, "Sitting / Sleeping", "FNearDistance Control of Sitting/Sleeping that is Independant of The Pitch Threshold",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_NearDistance.fSitting, 1.0f, 15.0f, "%.0f");
		m_MenuNodes.emplace_back(1, "Furniture", "FNearDistance Control of Furniture that is Independant of The Pitch Threshold",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_NearDistance.fFurniture, 1.0f, 15.0f, "%.0f");
		m_MenuNodes.emplace_back(1, "Crafting", "FNearDistance Control of Crafting that is Independant of The Pitch Threshold",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_NearDistance.fCrafting, 1.0f, 15.0f, "%.0f");
		m_MenuNodes.emplace_back(1, "Killmove", "FNearDistance Control of Killmove that is Independant of The Pitch Threshold",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_NearDistance.fKillmove, 1.0f, 15.0f, "%.0f");
		m_MenuNodes.emplace_back(1, "Ragdoll", "FNearDistance Control of Ragdoll that is Independant of The Pitch Threshold",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_NearDistance.fRagdoll, 1.0f, 15.0f, "%.0f");
		m_MenuNodes.emplace_back(1, "Death", "FNearDistance Control of Death that is Independant of The Pitch Threshold",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_NearDistance.fDeath, 1.0f, 15.0f, "%.0f");
		m_MenuNodes.emplace_back(1, "Cannibal", "FNearDistance Control of Cannibal that is Independant of The Pitch Threshold",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_NearDistance.fCannibal, 1.0f, 15.0f, "%.0f");
		m_MenuNodes.emplace_back(1, "Horse", "FNearDistance Control of Horse (Mounted) that is Independant of The Pitch Threshold",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_NearDistance.fHorse, 1.0f, 15.0f, "%.0f");
		m_MenuNodes.emplace_back(1, "Horse Combat", "FNearDistance Control of Horse Combat (Mounted) that is Independant of The Pitch Threshold",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_NearDistance.fHorseCombat, 1.0f, 15.0f, "%.0f");
		m_MenuNodes.emplace_back(1, "Horse Transition", "FNearDistance Control of Horse Transition (Mount/Dismount) animations that are Independant of The Pitch Threshold",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_NearDistance.fHorseTransition, 1.0f, 15.0f, "%.0f");
		// Table 2
		m_MenuNodes.emplace_back(2, "Dragon", "FNearDistance Control of Dragon (Mounted) that is Independant of The Pitch Threshold",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_NearDistance.fDragon, 1.0f, 15.0f, "%.0f");
		m_MenuNodes.emplace_back(2, "Dragon Combat", "FNearDistance Control of Dragon Combat (Mounted) that is Independant of The Pitch Threshold",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_NearDistance.fDragonCombat, 1.0f, 15.0f, "%.0f");
		m_MenuNodes.emplace_back(2, "Dragon Transition", "FNearDistance Control of Dragon Transition (Mount/Dismount) that is Independant of The Pitch Threshold",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_NearDistance.fDragonTransition, 1.0f, 15.0f, "%.0f");
		m_MenuNodes.emplace_back(2, "Vampire Lord", "FNearDistance Control of Vampire Lord that is Independant of The Pitch Threshold",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_NearDistance.fVampireLord, 1.0f, 15.0f, "%.0f");
		m_MenuNodes.emplace_back(2, "Werewolf", "FNearDistance Control of Werewolf that is Independant of The Pitch Threshold",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_NearDistance.fWerewolf, 1.0f, 15.0f, "%.0f");
		m_MenuNodes.emplace_back(2, "Lich", "FNearDistance Control of Lich that is Independant of The Pitch Threshold",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_NearDistance.fNecroLich, 1.0f, 15.0f, "%.0f");
		m_MenuNodes.emplace_back(2, "Scripted", "FNearDistance Control of Scripted Events that is Independant of The Pitch Threshold",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_NearDistance.fScripted, 1.0f, 15.0f, "%.0f");
		m_MenuNodes.emplace_back(2, "Third Person", "FNearDistance Control of Third Person that is Independant of The Pitch Threshold",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_NearDistance.fThirdPerson, 1.0f, 15.0f, "%.0f", &MenuNearDistance::OnCallback);
	}

	void MenuNearDistance::OnOpen()
	{
		ImGui::Checkbox("Near Distance", &m_Window);
	}

	void MenuNearDistance::OnUpdate()
	{
		if (!m_Window)
			return;

		ImGui::Begin("[NEAR DISTANCE]", &m_Window, ImGuiWindowFlags_NoCollapse);

		DisplayMenuNodes("NearDistanceTable");
		ImGui::SameLine();
		DisplayMenuNodes("NearDistanceTable", 2);

		if (ImGui::Button("Close"))
			m_Window = false;

		ImGui::End();
	}

	void MenuNearDistance::OnCallback(std::uint32_t node, bool begin)
	{
		auto pluginConfig = DLLMain::Plugin::Get()->Config();

		switch (node)
		{
			case MENU_ID::kPitchThreshold:
			{
				if (begin && !pluginConfig->NearDistance().bEnableOverride)
					ImGui::BeginDisabled();

				break;
			}
			case MENU_ID::kThirdPerson:
			{
				if (!begin && !pluginConfig->NearDistance().bEnableOverride)
					ImGui::EndDisabled();

				break;
			}
		}
	}

}
