/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

// Precompiled Header
#include "stdafx.h"

#include "menu/General.h"

#include "skyrimse/ImprovedCameraSE.h"

namespace Menu {

	MenuGeneral::MenuGeneral()
	{
		m_pluginSkyrimSE = DLLMain::Plugin::Get()->SkyrimSE();
	}

	void MenuGeneral::OnOpen()
	{
		ImGui::Checkbox("General", &m_Window);
	}

	void MenuGeneral::OnUpdate()
	{
		if (!m_Window)
			return;

		ImGui::Begin("[GENERAL]", &m_Window, ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);
		if (ImGui::BeginTable("GeneralTable", 4, ImGuiTableFlags_::ImGuiTableFlags_SizingFixedFit))
		{
			{
				ImGui::TableItemToggleButton("First Person Body", "##EnableBody", &m_pluginConfig->m_General.bEnableBody);
				if (ImGui::IsItemClicked())
					m_pluginSkyrimSE->Camera()->ResetState();

				ImGui::TableItemToggleButton("Third Person Arms", "##EnableThirdPersonArms", &m_pluginConfig->m_General.bEnableThirdPersonArms, false);
				if (ImGui::IsItemClicked())
					m_pluginSkyrimSE->Camera()->ResetPlayerNodes();
			}
			{
				ImGui::TableItemToggleButton("First Person Shadows", "##EnableShadows", &m_pluginConfig->m_General.bEnableShadows);
				ImGui::TableItemToggleButton("Third Person Torch", "##EnableThirdPersonTorch", &m_pluginConfig->m_General.bEnableThirdPersonTorch, false);
				if (ImGui::IsItemClicked())
					m_pluginSkyrimSE->Camera()->ResetPlayerNodes();
			}
			{
				ImGui::TableItemToggleButton("First Person Body Console", "##EnableBodyConsole", &m_pluginConfig->m_General.bEnableBodyConsole);
				if (ImGui::IsItemClicked())
					m_pluginSkyrimSE->Camera()->ResetState();

				ImGui::TableItemToggleButton("Third Person Shield", "##EnableThirdPersonShield", &m_pluginConfig->m_General.bEnableThirdPersonShield, false);
				if (ImGui::IsItemClicked())
					m_pluginSkyrimSE->Camera()->ResetPlayerNodes();
			}
			{
				ImGui::TableItemToggleButton("Adjust Player Scale", "##AdjustPlayerScale", &m_pluginConfig->m_General.bAdjustPlayerScale);
				ImGui::TableItemToggleButton("Third Person Shield Block", "##EnableThirdPersonShieldBlock", &m_pluginConfig->m_General.bEnableThirdPersonShieldBlock, false);
			}
			{
				ImGui::TableItemSliderFloat("Body Height Offset", "##BodyHeightOffset", &m_pluginConfig->m_General.fBodyHeightOffset, -500.0f, 500.0f, "%.1f");
				ImGui::TableItemToggleButton("Third Person Bow", "##EnableThirdPersonBow", &m_pluginConfig->m_General.bEnableThirdPersonBow, false);
			}
			{
				ImGui::TableItemToggleButton("Head First Person", "##EnableHead", &m_pluginConfig->m_General.bEnableHead);
				ImGui::TableItemToggleButton("Third Person Bow Aim", "##EnableThirdPersonBowAim", &m_pluginConfig->m_General.bEnableThirdPersonBowAim, false);
			}
			{
				ImGui::TableItemToggleButton("Head First Person Combat", "##EnableHeadCombat", &m_pluginConfig->m_General.bEnableHeadCombat);
				ImGui::TableItemToggleButton("Third Person Crossbow", "##EnableThirdPersonCrossbow", &m_pluginConfig->m_General.bEnableThirdPersonCrossbow, false);
			}
			{
				ImGui::TableItemToggleButton("Head First Person Horse", "##EnableHeadHorse", &m_pluginConfig->m_General.bEnableHeadHorse);
				ImGui::TableItemToggleButton("Third Person Crossbow Aim", "##EnableThirdPersonCrossbowAim", &m_pluginConfig->m_General.bEnableThirdPersonCrossbowAim, false);
			}
			ImGui::TableItemToggleButton("Head First Person Dragon", "##EnableHeadDragon", &m_pluginConfig->m_General.bEnableHeadDragon);
			ImGui::TableItemToggleButton("Head First Person Vampire Lord", "##EnableHeadVampireLord", &m_pluginConfig->m_General.bEnableHeadVampireLord);
			ImGui::TableItemToggleButton("Head First Person Werewolf", "##EnableHeadWerewolf", &m_pluginConfig->m_General.bEnableHeadWerewolf);
			ImGui::TableItemToggleButton("Head First Person Scripted", "##EnableHeadScripted", &m_pluginConfig->m_General.bEnableHeadScripted);

			ImGui::EndTable();
		}

		if (ImGui::Button("Close"))
			m_Window = false;

		ImGui::End();
	}

}
