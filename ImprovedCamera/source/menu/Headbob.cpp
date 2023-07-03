/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

 // Precompiled Header
#include "stdafx.h"

#include "menu/Headbob.h"


namespace Menu {

	void MenuHeadbob::OnOpen()
	{
		ImGui::Checkbox("Headbob", &m_Window);
	}

	void MenuHeadbob::OnUpdate()
	{
		if (!m_Window) return;

		ImGui::Begin("[HEADBOB]", &m_Window, ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);
		if (ImGui::BeginTable("HeadbobTable", 4, ImGuiTableFlags_::ImGuiTableFlags_SizingFixedFit))
		{
			{
				ImGui::TableItemToggleButton("Idle", "##EnableHeadBobPositionIdle", &m_pluginConfig->m_Headbob.bIdle);

				if (!m_pluginConfig->m_Headbob.bIdle)
					ImGui::BeginDisabled();

				ImGui::TableItemSliderFloat("Idle Rotation", "##HeadBobRotationIdle", &m_pluginConfig->m_Headbob.fRotationIdle, 0.0f, 1.0f, "%.1f", false);

				if (!m_pluginConfig->m_Headbob.bIdle)
					ImGui::EndDisabled();
			}
			{
				ImGui::TableItemToggleButton("Walk", "##EnableHeadBobPositionWalk", &m_pluginConfig->m_Headbob.bWalk);

				if (!m_pluginConfig->m_Headbob.bWalk)
					ImGui::BeginDisabled();

				ImGui::TableItemSliderFloat("Walk Rotation", "##HeadBobRotationWalk", &m_pluginConfig->m_Headbob.fRotationWalk, 0.0f, 1.0f, "%.1f", false);

				if (!m_pluginConfig->m_Headbob.bWalk)
					ImGui::EndDisabled();
			}
			{
				ImGui::TableItemToggleButton("Run", "##EnableHeadBobPositionRun", &m_pluginConfig->m_Headbob.bRun);

				if (!m_pluginConfig->m_Headbob.bRun)
					ImGui::BeginDisabled();

				ImGui::TableItemSliderFloat("Run Rotation", "##HeadBobRotationRun", &m_pluginConfig->m_Headbob.fRotationRun, 0.0f, 1.0f, "%.1f", false);

				if (!m_pluginConfig->m_Headbob.bRun)
					ImGui::EndDisabled();
			}
			{
				ImGui::TableItemToggleButton("Sprint", "##EnableHeadBobPositionSprint", &m_pluginConfig->m_Headbob.bSprint);

				if (!m_pluginConfig->m_Headbob.bSprint)
					ImGui::BeginDisabled();

				ImGui::TableItemSliderFloat("Sprint Rotation", "##HeadBobRotationSprint", &m_pluginConfig->m_Headbob.fRotationSprint, 0.0f, 1.0f, "%.1f", false);

				if (!m_pluginConfig->m_Headbob.bSprint)
					ImGui::EndDisabled();
			}
			{
				ImGui::TableItemToggleButton("Combat", "##EnableHeadBobPositionCombat", &m_pluginConfig->m_Headbob.bCombat);

				if (!m_pluginConfig->m_Headbob.bCombat)
					ImGui::BeginDisabled();

				ImGui::TableItemSliderFloat("Combat Rotation", "##HeadBobRotationCombat", &m_pluginConfig->m_Headbob.fRotationCombat, 0.0f, 1.0f, "%.1f", false);

				if (!m_pluginConfig->m_Headbob.bCombat)
					ImGui::EndDisabled();
			}
			{
				ImGui::TableItemToggleButton("Sneak", "##EnableHeadBobPositionSneak", &m_pluginConfig->m_Headbob.bSneak);

				if (!m_pluginConfig->m_Headbob.bSneak)
					ImGui::BeginDisabled();

				ImGui::TableItemSliderFloat("Sneak Rotation", "##HeadBobRotationSneak", &m_pluginConfig->m_Headbob.fRotationSneak, 0.0f, 1.0f, "%.1f", false);

				if (!m_pluginConfig->m_Headbob.bSneak)
					ImGui::EndDisabled();
			}
			{
				ImGui::TableItemToggleButton("Sneak Roll", "##EnableHeadBobPositionSneakRoll", &m_pluginConfig->m_Headbob.bSneakRoll);

				if (!m_pluginConfig->m_Headbob.bSneakRoll)
					ImGui::BeginDisabled();

				ImGui::TableItemSliderFloat("Sneak Roll Rotation", "##HeadBobRotationSneakRoll", &m_pluginConfig->m_Headbob.fRotationSneakRoll, 0.0f, 1.0f, "%.1f", false);

				if (!m_pluginConfig->m_Headbob.bSneakRoll)
					ImGui::EndDisabled();
			}

			ImGui::EndTable();
		}

		if (ImGui::Button("Close"))	m_Window = false;
		ImGui::End();
	}

}
