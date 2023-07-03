/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

 // Precompiled Header
#include "stdafx.h"

#include "menu/NearDistance.h"


namespace Menu {

	void MenuNearDistance::OnOpen()
	{
		ImGui::Checkbox("Near Distance", &m_Window);
	}

	void MenuNearDistance::OnUpdate()
	{
		if (!m_Window) return;

		ImGui::Begin("[NEAR DISTANCE]", &m_Window, ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);
		if (ImGui::BeginTable("NearDistanceTable", 4, ImGuiTableFlags_::ImGuiTableFlags_SizingFixedFit))
		{
			ImGui::TableItemToggleButton("Enable Near Distance Override", "##EnableNearDistanceOverride", &m_pluginConfig->m_NearDistance.bEnableOverride);

			if (!m_pluginConfig->m_NearDistance.bEnableOverride)
				ImGui::BeginDisabled();

			{
				ImGui::TableItemSliderFloat("Pitch Threshold", "##NearDistancePitchThreshold", &m_pluginConfig->m_NearDistance.fPitchThreshold, -90.0f, 0.0f, "%.0f");
				ImGui::TableItemSliderFloat("Dragon", "##NearDistanceDragon", &m_pluginConfig->m_NearDistance.fDragon, 1.0f, 15.0f, "%.0f", false);
			}
			{
				ImGui::TableItemSliderFloat("First Person", "##NearDistanceFirstPerson", &m_pluginConfig->m_NearDistance.fFirstPerson, 1.0f, 15.0f, "%.0f");
				ImGui::TableItemSliderFloat("Dragon Combat", "##NearDistanceDragonCombat", &m_pluginConfig->m_NearDistance.fDragonCombat, 1.0f, 15.0f, "%.0f", false);
			}
			{
				ImGui::TableItemSliderFloat("First Person Combat", "##NearDistanceFirstPersonCombat", &m_pluginConfig->m_NearDistance.fFirstPersonCombat, 1.0f, 15.0f, "%.0f");
				ImGui::TableItemSliderFloat("Dragon Transition", "##NearDistanceDragonTransition", &m_pluginConfig->m_NearDistance.fDragonTransition, 1.0f, 15.0f, "%.0f", false);
			}
			{
				ImGui::TableItemSliderFloat("Sitting / Sleeping", "##NearDistanceSitting", &m_pluginConfig->m_NearDistance.fSitting, 1.0f, 15.0f, "%.0f");
				ImGui::TableItemSliderFloat("Vampire Lord", "##NearDistanceVampireLord", &m_pluginConfig->m_NearDistance.fVampireLord, 1.0f, 15.0f, "%.0f", false);
			}
			{
				ImGui::TableItemSliderFloat("Furniture", "##NearDistanceFurniture", &m_pluginConfig->m_NearDistance.fFurniture, 1.0f, 15.0f, "%.0f");
				ImGui::TableItemSliderFloat("Werewolf", "##NearDistanceWerewolf", &m_pluginConfig->m_NearDistance.fWerewolf, 1.0f, 15.0f, "%.0f", false);
			}
			{
				ImGui::TableItemSliderFloat("Crafting", "##NearDistanceCrafting", &m_pluginConfig->m_NearDistance.fCrafting, 1.0f, 15.0f, "%.0f");
				ImGui::TableItemSliderFloat("Lich", "##NearDistanceNecroLich", &m_pluginConfig->m_NearDistance.fNecroLich, 1.0f, 15.0f, "%.0f", false);
			}
			{
				ImGui::TableItemSliderFloat("Killmove", "##NearDistanceKillmove", &m_pluginConfig->m_NearDistance.fKillmove, 1.0f, 15.0f, "%.0f");
				ImGui::TableItemSliderFloat("Scripted", "##NearDistanceScripted", &m_pluginConfig->m_NearDistance.fScripted, 1.0f, 15.0f, "%.0f", false);
			}
			{
				ImGui::TableItemSliderFloat("Ragdoll", "##NearDistanceRagdoll", &m_pluginConfig->m_NearDistance.fRagdoll, 1.0f, 15.0f, "%.0f");
				ImGui::TableItemSliderFloat("Third Person", "##NearDistanceThirdPerson", &m_pluginConfig->m_NearDistance.fThirdPerson, 1.0f, 15.0f, "%.0f", false);
			}

			ImGui::TableItemSliderFloat("Death", "##NearDistanceDeath", &m_pluginConfig->m_NearDistance.fDeath, 1.0f, 15.0f, "%.0f");
			ImGui::TableItemSliderFloat("Cannibal", "##NearDistanceCannibal", &m_pluginConfig->m_NearDistance.fCannibal, 1.0f, 15.0f, "%.0f");
			ImGui::TableItemSliderFloat("Horse", "##NearDistanceHorse", &m_pluginConfig->m_NearDistance.fHorse, 1.0f, 15.0f, "%.0f");
			ImGui::TableItemSliderFloat("Horse Combat", "##NearDistanceHorseCombat", &m_pluginConfig->m_NearDistance.fHorseCombat, 1.0f, 15.0f, "%.0f");
			ImGui::TableItemSliderFloat("Horse Transition", "##NearDistanceHorseTransition", &m_pluginConfig->m_NearDistance.fHorseTransition, 1.0f, 15.0f, "%.0f");

			if (!m_pluginConfig->m_NearDistance.bEnableOverride)
				ImGui::EndDisabled();

			ImGui::EndTable();
		}

		if (ImGui::Button("Close"))	m_Window = false;
		ImGui::End();
	}

}
