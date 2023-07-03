/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

 // Precompiled Header
#include "stdafx.h"

#include "menu/EventsMenu.h"


namespace Menu {

	void MenuEvents::OnOpen()
	{
		ImGui::Checkbox("Events", &m_Window);
	}

	void MenuEvents::OnUpdate()
	{
		if (!m_Window) return;

		ImGui::Begin("[EVENTS]", &m_Window, ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);
		if (ImGui::BeginTable("EventsTable", 4, ImGuiTableFlags_::ImGuiTableFlags_SizingFixedFit))
		{
			{
				ImGui::TableItemToggleButton("First Person", "##EventFirstPerson", &m_pluginConfig->m_Events.bFirstPerson);
				ImGui::TableItemToggleButton("Dragon", "##EventDragon", &m_pluginConfig->m_Events.bDragon, false);
			}
			{
				ImGui::TableItemToggleButton("First Person Combat", "##EventFirstPersonCombat", &m_pluginConfig->m_Events.bFirstPersonCombat);
				ImGui::TableItemToggleButton("Dragon Combat", "##EventDragonCombat", &m_pluginConfig->m_Events.bDragonCombat, false);
			}
			{
				ImGui::TableItemToggleButton("Furniture", "##EventFurniture", &m_pluginConfig->m_Events.bFurniture);
				ImGui::TableItemToggleButton("Dragon Transition", "##EventDragonTransition", &m_pluginConfig->m_Events.bDragonTransition, false);
			}
			{
				ImGui::TableItemToggleButton("Crafting", "##EventCrafting", &m_pluginConfig->m_Events.bCrafting);
				ImGui::TableItemToggleButton("Transform", "##EventTransform", &m_pluginConfig->m_Events.bTransform, false);
			}
			{
				ImGui::TableItemToggleButton("Killmove", "##EventKillmove", &m_pluginConfig->m_Events.bKillmove);
				ImGui::TableItemToggleButton("Vampire Lord", "##EventVampireLord", &m_pluginConfig->m_Events.bVampireLord, false);
			}
			{
				ImGui::TableItemToggleButton("Ragdoll", "##EventRagdoll", &m_pluginConfig->m_Events.bRagdoll);
				ImGui::TableItemToggleButton("Werewolf", "##EventWerewolf", &m_pluginConfig->m_Events.bWerewolf, false);
			}
			{
				ImGui::TableItemToggleButton("Death", "##EventDeath", &m_pluginConfig->m_Events.bDeath);
				ImGui::TableItemToggleButton("Lich", "##EventNecroLich", &m_pluginConfig->m_Events.bNecroLich, false);
			}
			{
				ImGui::TableItemToggleButton("Cannibal", "##EventCannibal", &m_pluginConfig->m_Events.bCannibal);
				ImGui::TableItemToggleButton("Scripted", "##EventScripted", &m_pluginConfig->m_Events.bScripted, false);
			}
			{
				ImGui::TableItemToggleButton("Horse", "##EventHorse", &m_pluginConfig->m_Events.bHorse);
				ImGui::TableItemToggleButton("Third Person", "##EventThirdPerson", &m_pluginConfig->m_Events.bThirdPerson, false);
			}

			ImGui::TableItemToggleButton("Horse Combat", "##EventHorseCombat", &m_pluginConfig->m_Events.bHorseCombat);
			ImGui::TableItemToggleButton("Horse Transition", "##EventHorseTransition", &m_pluginConfig->m_Events.bHorseTransition);

			ImGui::EndTable();
		}

		if (ImGui::Button("Close"))	m_Window = false;
		ImGui::End();
	}

}
