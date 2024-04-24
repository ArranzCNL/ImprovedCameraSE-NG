/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

// Precompiled Header
#include "PCH.h"

#include "menu/EventsMenu.h"

namespace Menu {

	struct MENU_IDS {

		enum MENU_ID : std::int32_t
		{
			kFirstPerson = 1,
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
			kDragon,
			kDragonCombat,
			kDragonTransition,
			kTransform,
			kVampireLord,
			kWerewolf,
			kNecroLich,
			kScripted,
			kThirdPerson,

			kTotal = 21
		};
	};
	using MENU_ID = MENU_IDS::MENU_ID;

	MenuEvents::MenuEvents()
	{
		// Table 1
		m_MenuNodes.emplace_back(1, "First Person", "Enables First Person where Third Person would normally take over",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Events.bFirstPerson);
		m_MenuNodes.emplace_back(1, "First Person Combat", "Enables First Person Combat where Third Person would normally take over",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Events.bFirstPersonCombat);
		m_MenuNodes.emplace_back(1, "Furniture", "Enables First Person Furniture where Third Person would normally take over",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Events.bFurniture);
		m_MenuNodes.emplace_back(1, "Crafting", "Enables First Person Crafting where Third Person would normally take over",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Events.bCrafting);
		m_MenuNodes.emplace_back(1, "Killmove", "Enables First Person Killmove where Third Person would normally take over",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Events.bKillmove);
		m_MenuNodes.emplace_back(1, "Ragdoll", "Enables First Person Ragdoll where Third Person would normally take over",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Events.bRagdoll);
		m_MenuNodes.emplace_back(1, "Death", "Enables First Person death where Third Person would normally take over",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Events.bDeath);
		m_MenuNodes.emplace_back(1, "Cannibal", "Enables First Person cannibal where Third Person would normally take over",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Events.bCannibal);
		m_MenuNodes.emplace_back(1, "Horse", "Enables First Person Horse (Mounted) where Third Person would normally take over",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Events.bHorse);
		m_MenuNodes.emplace_back(1, "Horse Combat", "Enables First Person Horse Combat (Mounted) where Third Person would normally take over",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Events.bHorseCombat);
		m_MenuNodes.emplace_back(1, "Horse Transition", "Enables First Person Horse Trasition (Mounting/Dismounting) where Third Person would normally take over",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Events.bHorseTransition);
		// Table 2
		m_MenuNodes.emplace_back(2, "Dragon", "Enables First Person Dragon (Mounted) where Third Person would normally take over",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Events.bDragon);
		m_MenuNodes.emplace_back(2, "Dragon Combat", "Enables First Person Dragon Combat (Mounted) where Third Person would normally take over",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Events.bDragonCombat);
		m_MenuNodes.emplace_back(2, "Dragon Transition", "Enables First Person dragon Transition (Mounting/Dismounting) where Third Person would normally take over",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Events.bDragonTransition);
		m_MenuNodes.emplace_back(2, "Transform", "Enables First Person Transform where Third Person would normally take over",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Events.bTransform);
		m_MenuNodes.emplace_back(2, "Vampire Lord", "Enables First Person Vampire Lord where Third Person would normally take over",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Events.bVampireLord);
		m_MenuNodes.emplace_back(2, "Werewolf", "Enables First Person Werewolf where Third Person would normally take over",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Events.bWerewolf);
		m_MenuNodes.emplace_back(2, "Lich", "Enables First Person lich where Third Person would normally take over",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Events.bNecroLich);
		m_MenuNodes.emplace_back(2, "Scripted", "Enables First Person Scripted where Third Person would normally take over",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Events.bScripted);
		m_MenuNodes.emplace_back(2, "Third Person", "",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Events.bThirdPerson);
	}

	void MenuEvents::OnOpen()
	{
		ImGui::Checkbox("Events", &m_Window);
	}

	void MenuEvents::OnUpdate()
	{
		if (!m_Window)
			return;

		ImGui::Begin("[EVENTS]", &m_Window, ImGuiWindowFlags_NoCollapse);

		DisplayMenuNodes("EventsTable");
		ImGui::SameLine();
		DisplayMenuNodes("EventsTable", 2);

		if (ImGui::Button("Close"))
			m_Window = false;

		ImGui::End();
	}

}
