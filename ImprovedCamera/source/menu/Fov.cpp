/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

// Precompiled Header
#include "stdafx.h"

#include "menu/Fov.h"

namespace Menu {

	void MenuFOV::OnOpen()
	{
		ImGui::Checkbox("FOV", &m_Window);
	}

	void MenuFOV::OnUpdate()
	{
		if (!m_Window)
			return;

		ImGui::Begin("[FOV]", &m_Window, ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);
		if (ImGui::BeginTable("FOVTable", 4, ImGuiTableFlags_::ImGuiTableFlags_SizingFixedFit))
		{
			ImGui::TableItemSliderFloat("First Person Hands", "##FOVFirstPersonHands", &m_pluginConfig->m_FOV.fFirstPersonHands, 1.0f, 165.0f, "%.0f");

			ImGui::TableItemToggleButton("Enable FOV Override", "##EnableFOVOverride", &m_pluginConfig->m_FOV.bEnableOverride);

			if (!m_pluginConfig->m_FOV.bEnableOverride)
				ImGui::BeginDisabled();

			{
				ImGui::TableItemSliderFloat("First Person", "##FOVFirstPerson", &m_pluginConfig->m_FOV.fFirstPerson, 1.0f, 165.0f, "%.0f");
				ImGui::TableItemSliderFloat("Dragon", "##FOVDragon", &m_pluginConfig->m_FOV.fDragon, 1.0f, 165.0f, "%.0f", false);
			}
			{
				ImGui::TableItemSliderFloat("First Person Combat", "##FOVFirstPersonCombat", &m_pluginConfig->m_FOV.fFirstPersonCombat, 1.0f, 165.0f, "%.0f");
				ImGui::TableItemSliderFloat("Dragon Combat", "##FOVDragonCombat", &m_pluginConfig->m_FOV.fDragonCombat, 1.0f, 165.0f, "%.0f", false);
			}
			{
				ImGui::TableItemSliderFloat("Furniture", "##FOVFurniture", &m_pluginConfig->m_FOV.fFurniture, 1.0f, 165.0f, "%.0f");
				ImGui::TableItemSliderFloat("Dragon Transition", "##FOVDragonTransition", &m_pluginConfig->m_FOV.fDragonTransition, 1.0f, 165.0f, "%.0f", false);
			}
			{
				ImGui::TableItemSliderFloat("Crafting", "##FOVCrafting", &m_pluginConfig->m_FOV.fCrafting, 1.0f, 165.0f, "%.0f");
				ImGui::TableItemSliderFloat("Vampire Lord", "##FOVVampireLord", &m_pluginConfig->m_FOV.fVampireLord, 1.0f, 165.0f, "%.0f", false);
			}
			{
				ImGui::TableItemSliderFloat("Killmove", "##FOVKillmove", &m_pluginConfig->m_FOV.fKillmove, 1.0f, 165.0f, "%.0f");
				ImGui::TableItemSliderFloat("Werewolf", "##FOVWerewolf", &m_pluginConfig->m_FOV.fWerewolf, 1.0f, 165.0f, "%.0f", false);
			}
			{
				ImGui::TableItemSliderFloat("Ragdoll", "##FOVRagdoll", &m_pluginConfig->m_FOV.fRagdoll, 1.0f, 165.0f, "%.0f");
				ImGui::TableItemSliderFloat("Lich", "##FOVNecroLich", &m_pluginConfig->m_FOV.fNecroLich, 1.0f, 165.0f, "%.0f", false);
			}
			{
				ImGui::TableItemSliderFloat("Death", "##FOVDeath", &m_pluginConfig->m_FOV.fDeath, 1.0f, 165.0f, "%.0f");
				ImGui::TableItemSliderFloat("Scripted", "##FOVScripted", &m_pluginConfig->m_FOV.fScripted, 1.0f, 165.0f, "%.0f", false);
			}
			{
				ImGui::TableItemSliderFloat("Cannibal", "##FOVCannibal", &m_pluginConfig->m_FOV.fCannibal, 1.0f, 165.0f, "%.0f");
				ImGui::TableItemSliderFloat("Third Person", "##FOVThirdPerson", &m_pluginConfig->m_FOV.fThirdPerson, 1.0f, 165.0f, "%.0f", false);
			}

			ImGui::TableItemSliderFloat("Horse", "##FOVHorse", &m_pluginConfig->m_FOV.fHorse, 1.0f, 165.0f, "%.0f");
			ImGui::TableItemSliderFloat("Horse Combat", "##FOVHorseCombat", &m_pluginConfig->m_FOV.fHorseCombat, 1.0f, 165.0f, "%.0f");
			ImGui::TableItemSliderFloat("Horse Transition", "##FOVHorseTransition", &m_pluginConfig->m_FOV.fHorseTransition, 1.0f, 165.0f, "%.0f");

			if (!m_pluginConfig->m_FOV.bEnableOverride)
				ImGui::EndDisabled();

			ImGui::EndTable();
		}

		if (ImGui::Button("Close"))
			m_Window = false;

		ImGui::End();
	}

}
