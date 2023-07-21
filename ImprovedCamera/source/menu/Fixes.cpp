/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

// Precompiled Header
#include "stdafx.h"

#include "menu/Fixes.h"

namespace Menu {

	void MenuFixes::OnOpen()
	{
		ImGui::Checkbox("Fixes", &m_Window);
	}

	void MenuFixes::OnUpdate()
	{
		if (!m_Window)
			return;

		ImGui::Begin("[FIXES]", &m_Window, ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);
		if (ImGui::BeginTable("FixesTable", 4, ImGuiTableFlags_::ImGuiTableFlags_SizingFixedFit))
		{
			ImGui::TableItemToggleButton("QuickLight Lighting", "##QuickLightLighting", &m_pluginConfig->m_Fixes.bQuickLightLighting);
			ImGui::TableItemToggleButton("First Person Overhaul", "##FirstPersonOverhaul", &m_pluginConfig->m_Fixes.bFirstPersonOverhaul);
			ImGui::TableItemToggleButton("Archery Gameplay Overhaul", "##ArcheryGameplayOverhaul", &m_pluginConfig->m_Fixes.bArcheryGameplayOverhaul);
			ImGui::TableItemSliderFloat("Switch POV Detect Distance", "##SwitchPOVDetectDistance", &m_pluginConfig->m_Fixes.fSwitchPOVDetectDistance, 0.01f, 0.20f, "%.2f");
			ImGui::NewLine();
			ImGui::TableItemToggleButton("Smooth Animation Transitions", "##SmoothAnimationTransitions", &m_pluginConfig->m_Fixes.bSmoothAnimationTransitions);

			if (!m_pluginConfig->m_Fixes.bSmoothAnimationTransitions)
				ImGui::BeginDisabled();

			ImGui::TableItemSliderFloat("Controller Buffer First Person", "##ControllerBuffer1st", &m_pluginConfig->m_Fixes.fControllerBufferDepth1st, 0.01f, 0.50f, "%.2f");

			if (!m_pluginConfig->m_Fixes.bSmoothAnimationTransitions)
				ImGui::EndDisabled();

			ImGui::TableItemSliderFloat("Controller Buffer Third Person", "##ControllerBuffer3rd", &m_pluginConfig->m_Fixes.fControllerBufferDepth3rd, 0.01f, 0.50f, "%.2f");
			ImGui::EndTable();
		}

		if (ImGui::Button("Close"))
			m_Window = false;

		ImGui::End();
	}

}
