/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

 // Precompiled Header
#include "stdafx.h"

#include "menu/RestrictAngles.h"


namespace Menu {

	void MenuRestrictAngles::OnOpen()
	{
		ImGui::Checkbox("Restrict Angles", &m_Window);
	}

	void MenuRestrictAngles::OnUpdate()
	{
		if (!m_Window) return;

		ImGui::Begin("[RESTRICT ANGLES]", &m_Window, ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);
		if (ImGui::BeginTable("RestrictAnglesTable", 2, ImGuiTableFlags_::ImGuiTableFlags_SizingFixedFit))
		{
			ImGui::TableItemSliderFloat("Sitting Restrict Angle", "##SittingRestrictAngle", &m_pluginConfig->m_RestrictAngles.fSitting, 0.0f, 180.0f, "%.1f");
			ImGui::TableItemSliderFloat("Sitting Max Looking Up", "##SittingMaxLookingUp", &m_pluginConfig->m_RestrictAngles.fSittingMaxLookingUp, 0.0f, 180.0f, "%.1f");
			ImGui::TableItemSliderFloat("Sitting Max Looking Down", "##SittingMaxLookingDown", &m_pluginConfig->m_RestrictAngles.fSittingMaxLookingDown, 0.0f, 180.0f, "%.1f");
			ImGui::TableItemSliderFloat("Mounted Restrict Angle", "##MountedRestrictAngle", &m_pluginConfig->m_RestrictAngles.fMounted, 0.0f, 180.0f, "%.1f");
			ImGui::TableItemSliderFloat("Mounted Max Looking Up", "##MountedMaxLookingUp", &m_pluginConfig->m_RestrictAngles.fMountedMaxLookingUp, 0.0f, 180.0f, "%.1f");
			ImGui::TableItemSliderFloat("Mounted Max Looking Down", "##MountedMaxLookingDown", &m_pluginConfig->m_RestrictAngles.fMountedMaxLookingDown, 0.0f, 180.0f, "%.1f");
			ImGui::TableItemSliderFloat("Flying Restrict Angle", "##FlyingRestrictAngle", &m_pluginConfig->m_RestrictAngles.fFlying, 0.0f, 180.0f, "%.1f");
			ImGui::TableItemSliderFloat("Flying Max Looking Up", "##FlyingMaxLookingUp", &m_pluginConfig->m_RestrictAngles.fFlyingMaxLookingUp, 0.0f, 180.0f, "%.1f");
			ImGui::TableItemSliderFloat("Flying Max Looking Down", "##FlyingMaxLookingDown", &m_pluginConfig->m_RestrictAngles.fFlyingMaxLookingDown, 0.0f, 180.0f, "%.1f");
			ImGui::TableItemSliderFloat("VampireLord Restrict Angle", "##VampireLordRestrictAngle", &m_pluginConfig->m_RestrictAngles.fVampireLord, 0.0f, 180.0f, "%.1f");
			ImGui::TableItemSliderFloat("Werewolf Restrict Angle", "##WerewolfRestrictAngle", &m_pluginConfig->m_RestrictAngles.fWerewolf, 0.0f, 180.0f, "%.1f");
			ImGui::TableItemSliderFloat("Lich Restrict Angle", "##NecroLichRestrictAngle", &m_pluginConfig->m_RestrictAngles.fNecroLich, 0.0f, 180.0f, "%.1f");
			ImGui::TableItemSliderFloat("Scripted Restrict Angle", "##ScriptedRestrictAngle", &m_pluginConfig->m_RestrictAngles.fScripted, 0.0f, 180.0f, "%.1f");
			ImGui::TableItemSliderFloat("Scripted Restrict Pitch", "##ScriptedRestrictPitch", &m_pluginConfig->m_RestrictAngles.fScriptedPitch, 0.0f, 180.0f, "%.1f");

			ImGui::EndTable();
		}

		if (ImGui::Button("Close"))	m_Window = false;
		ImGui::End();
	}

}
