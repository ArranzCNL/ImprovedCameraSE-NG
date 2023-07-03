/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

 // Precompiled Header
#include "stdafx.h"

#include "menu/Camera.h"


namespace Menu {

	void MenuCamera::OnOpen()
	{
		ImGui::Checkbox("Camera", &m_Window);
	}

	void MenuCamera::OnUpdate()
	{
		if (!m_Window) return;

		ImGui::Begin("[CAMERA]", &m_Window, ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);
		if (ImGui::BeginTable("CameraTable", 4, ImGuiTableFlags_::ImGuiTableFlags_SizingFixedFit))
		{
			{
				ImGui::TableItemSliderFloat("Player Position X", "##CameraPosX", &m_pluginConfig->m_Camera.fFirstPersonPosX, -500.0f, 500.0f, "%.1f");
				ImGui::TableItemSliderFloat("Vampire Lord Position X", "##VampireLordCameraPosX", &m_pluginConfig->m_Camera.fVampireLordPosX, -500.0f, 500.0f, "%.1f", false);
			}
			{
				ImGui::TableItemSliderFloat("Player Position Y", "##CameraPosY", &m_pluginConfig->m_Camera.fFirstPersonPosY, -500.0f, 500.0f, "%.1f");
				ImGui::TableItemSliderFloat("Vampire Lord Position Y", "##VampireLordCameraPosY", &m_pluginConfig->m_Camera.fVampireLordPosY, -500.0f, 500.0f, "%.1f", false);
			}
			{
				ImGui::TableItemSliderFloat("Player Position Z", "##CameraPosZ", &m_pluginConfig->m_Camera.fFirstPersonPosZ, -500.0f, 500.0f, "%.1f");
				ImGui::TableItemSliderFloat("Vampire Lord Position Z", "##VampireLordCameraPosZ", &m_pluginConfig->m_Camera.fVampireLordPosZ, -500.0f, 500.0f, "%.1f", false);
			}
			{
				ImGui::TableItemSliderFloat("Player Combat Position X", "##CameraCombatPosX", &m_pluginConfig->m_Camera.fFirstPersonCombatPosX, -500.0f, 500.0f, "%.1f");
				ImGui::TableItemSliderFloat("Werewolf Position X", "##WerewolfCameraPosX", &m_pluginConfig->m_Camera.fWerewolfPosX, -500.0f, 500.0f, "%.1f", false);
			}
			{
				ImGui::TableItemSliderFloat("Player Combat Position Y", "##CameraCombatPosY", &m_pluginConfig->m_Camera.fFirstPersonCombatPosY, -500.0f, 500.0f, "%.1f");
				ImGui::TableItemSliderFloat("Werewolf Position Y", "##WerewolfCameraPosY", &m_pluginConfig->m_Camera.fWerewolfPosY, -500.0f, 500.0f, "%.1f", false);
			}
			{
				ImGui::TableItemSliderFloat("Player Combat Position Z", "##CameraCombatPosZ", &m_pluginConfig->m_Camera.fFirstPersonCombatPosZ, -500.0f, 500.0f, "%.1f");
				ImGui::TableItemSliderFloat("Werewolf Position Z", "##WerewolfCameraPosZ", &m_pluginConfig->m_Camera.fWerewolfPosZ, -500.0f, 500.0f, "%.1f", false);
			}
			{
				ImGui::TableItemSliderFloat("Horse Position X", "##HorseCameraPosX", &m_pluginConfig->m_Camera.fHorsePosX, -500.0f, 500.0f, "%.1f");
				ImGui::TableItemSliderFloat("Lich Position X", "##NecroLichCameraPosX", &m_pluginConfig->m_Camera.fNecroLichPosX, -500.0f, 500.0f, "%.1f", false);
			}
			{
				ImGui::TableItemSliderFloat("Horse Position Y", "##HorseCameraPosY", &m_pluginConfig->m_Camera.fHorsePosY, -500.0f, 500.0f, "%.1f");
				ImGui::TableItemSliderFloat("Lich Position Y", "##NecroLichCameraPosY", &m_pluginConfig->m_Camera.fNecroLichPosY, -500.0f, 500.0f, "%.1f", false);
			}
			{
				ImGui::TableItemSliderFloat("Horse Position Z", "##HorseCameraPosZ", &m_pluginConfig->m_Camera.fHorsePosZ, -500.0f, 500.0f, "%.1f");
				ImGui::TableItemSliderFloat("Lich Position Z", "##NecroLichCameraPosZ", &m_pluginConfig->m_Camera.fNecroLichPosZ, -500.0f, 500.0f, "%.1f", false);
			}
			{
				ImGui::TableItemSliderFloat("Horse Combat Position X", "##HorseCombatCameraPosX", &m_pluginConfig->m_Camera.fHorseCombatPosX, -500.0f, 500.0f, "%.1f");
				ImGui::TableItemSliderFloat("Scripted Position X", "##ScriptedCameraPosX", &m_pluginConfig->m_Camera.fScriptedPosX, -500.0f, 500.0f, "%.1f", false);
			}
			{
				ImGui::TableItemSliderFloat("Horse Combat Position Y", "##HorseCombatCameraPosY", &m_pluginConfig->m_Camera.fHorseCombatPosY, -500.0f, 500.0f, "%.1f");
				ImGui::TableItemSliderFloat("Scripted Position Y", "##ScriptedCameraPosY", &m_pluginConfig->m_Camera.fScriptedPosY, -500.0f, 500.0f, "%.1f", false);
			}
			{
				ImGui::TableItemSliderFloat("Horse Combat Position Z", "##HorseCombatCameraPosZ", &m_pluginConfig->m_Camera.fHorseCombatPosZ, -500.0f, 500.0f, "%.1f");
				ImGui::TableItemSliderFloat("Scripted Position Z", "##ScriptedCameraPosZ", &m_pluginConfig->m_Camera.fScriptedPosZ, -500.0f, 500.0f, "%.1f", false);
			}

			ImGui::TableItemSliderFloat("Dragon Position X", "##DragonCameraPosX", &m_pluginConfig->m_Camera.fDragonPosX, -500.0f, 500.0f, "%.1f");
			ImGui::TableItemSliderFloat("Dragon Position Y", "##DragonCameraPosZ", &m_pluginConfig->m_Camera.fDragonPosZ, -500.0f, 500.0f, "%.1f");
			ImGui::TableItemSliderFloat("Dragon Position Z", "##DragonCameraPosY", &m_pluginConfig->m_Camera.fDragonPosY, -500.0f, 500.0f, "%.1f");
			
			ImGui::EndTable();
		}

		if (ImGui::Button("Close"))	m_Window = false;
		ImGui::End();
	}

}
