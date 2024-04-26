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
			kFirstPersonDefault,
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
			// Table 2
			kHorse,
			kHorseCombat,
			kHorseTransition,
			kDragon,
			kDragonCombat,
			kDragonTransition,
			kVampireLord,
			kWerewolf,
			kNecroLich,
			kScripted,
			kThirdPerson,

			kTotal = 24
		};
	};
	using MENU_ID = MENU_IDS::MENU_ID;

	MenuNearDistance::MenuNearDistance()
	{
		// Table 1
		m_MenuNodes.emplace_back(1, "Enable Near Distance Override", "",
			ControlType::kToggle, (void*)&m_pluginConfig->m_NearDistance.bEnableOverride);
		m_MenuNodes.emplace_back(1, "First Person Default", "Control for Default that works outside of the pitch threshold",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_NearDistance.fFirstPersonDefault, 1.0f, 30.0f, "%.0f", &MenuNearDistance::OnCallback);
		m_MenuNodes.emplace_back(1, "Pitch Threshold", "Pitch Threshold for changing the fNearDistance automatically",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_NearDistance.fPitchThreshold, -90.0f, 0.0f, "%.0f", &MenuNearDistance::OnCallback);
		m_MenuNodes.emplace_back(1, "First Person", "Reduce Near Distance when Pitch threshold is met",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_NearDistance.fFirstPerson, 1.0f, 30.0f, "%.0f");
		m_MenuNodes.emplace_back(1, "First Person Combat", "Control for First Person Combat",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_NearDistance.fFirstPersonCombat, 1.0f, 30.0f, "%.0f", &MenuNearDistance::OnCallback);
		m_MenuNodes.emplace_back(1, "Sitting / Sleeping", "Control for Sitting/Sleeping",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_NearDistance.fSitting, 1.0f, 30.0f, "%.0f");
		m_MenuNodes.emplace_back(1, "Furniture", "Control for Furniture",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_NearDistance.fFurniture, 1.0f, 30.0f, "%.0f");
		m_MenuNodes.emplace_back(1, "Crafting", "Control for Crafting",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_NearDistance.fCrafting, 1.0f, 30.0f, "%.0f");
		m_MenuNodes.emplace_back(1, "Killmove", "Control for Killmove",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_NearDistance.fKillmove, 1.0f, 30.0f, "%.0f");
		m_MenuNodes.emplace_back(1, "Ragdoll", "Control for Ragdoll",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_NearDistance.fRagdoll, 1.0f, 30.0f, "%.0f");
		m_MenuNodes.emplace_back(1, "Death", "Control for Death",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_NearDistance.fDeath, 1.0f, 30.0f, "%.0f");
		m_MenuNodes.emplace_back(1, "Cannibal", "Control for Cannibal",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_NearDistance.fCannibal, 1.0f, 30.0f, "%.0f");
		// Table 2
		m_MenuNodes.emplace_back(2, "Horse", "Control for Horse (Mounted)",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_NearDistance.fHorse, 1.0f, 30.0f, "%.0f");
		m_MenuNodes.emplace_back(2, "Horse Combat", "Control for Horse Combat (Mounted)",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_NearDistance.fHorseCombat, 1.0f, 30.0f, "%.0f");
		m_MenuNodes.emplace_back(2, "Horse Transition", "Control for Horse Transition (Mounting/Dismounting)",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_NearDistance.fHorseTransition, 1.0f, 30.0f, "%.0f");
		m_MenuNodes.emplace_back(2, "Dragon", "Control for Dragon (Mounted)",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_NearDistance.fDragon, 1.0f, 30.0f, "%.0f");
		m_MenuNodes.emplace_back(2, "Dragon Combat", "Control for Dragon Combat (Mounted)",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_NearDistance.fDragonCombat, 1.0f, 30.0f, "%.0f");
		m_MenuNodes.emplace_back(2, "Dragon Transition", "Control for Dragon Transition (Mounting/Dismounting)",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_NearDistance.fDragonTransition, 1.0f, 30.0f, "%.0f");
		m_MenuNodes.emplace_back(2, "Vampire Lord", "Control for Vampire Lord",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_NearDistance.fVampireLord, 1.0f, 30.0f, "%.0f");
		m_MenuNodes.emplace_back(2, "Werewolf", "Control for Werewolf",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_NearDistance.fWerewolf, 1.0f, 30.0f, "%.0f");
		m_MenuNodes.emplace_back(2, "Lich", "Control for Lich",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_NearDistance.fNecroLich, 1.0f, 30.0f, "%.0f");
		m_MenuNodes.emplace_back(2, "Scripted", "Control for Scripted Events",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_NearDistance.fScripted, 1.0f, 30.0f, "%.0f");
		m_MenuNodes.emplace_back(2, "Third Person", "Control for Third Person",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_NearDistance.fThirdPerson, 1.0f, 30.0f, "%.0f", &MenuNearDistance::OnCallback);
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
			case MENU_ID::kFirstPersonDefault:
			{
				if (begin && !pluginConfig->NearDistance().bEnableOverride)
					ImGui::BeginDisabled();

				break;
			}
			case MENU_ID::kPitchThreshold:
			{
				if (begin)
					ImGui::Separator();

				break;
			}
			case MENU_ID::kFirstPersonCombat:
			{
				if (begin)
					ImGui::Separator();

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
