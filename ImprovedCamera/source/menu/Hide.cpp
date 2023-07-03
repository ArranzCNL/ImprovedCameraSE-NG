/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

 // Precompiled Header
#include "stdafx.h"

#include "menu/Hide.h"


namespace Menu {

    void MenuHide::OnOpen()
    {
        ImGui::BeginDisabled();
        ImGui::Checkbox("Hide", &m_Window);
        ImGui::EndDisabled();
    }

    void MenuHide::OnUpdate()
    {
		if (!m_Window) return;

		ImGui::Begin("[HIDE]", &m_Window, ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);
		if (ImGui::BeginTable("HideTable", 2, ImGuiTableFlags_::ImGuiTableFlags_SizingFixedFit))
		{
			ImGui::TableItemToggleButton("Two-Handed Weapon", "##Hide2HWeapon", &m_pluginConfig->m_Hide.b2HWeapon);
			ImGui::TableItemToggleButton("Bow", "##HideBow", &m_pluginConfig->m_Hide.bBow);
			ImGui::TableItemToggleButton("Quiver", "##HideQuiver", &m_pluginConfig->m_Hide.bQuiver);
			ImGui::TableItemToggleButton("Body Sitting", "##HideBodySitting", &m_pluginConfig->m_Hide.bSitting);
			ImGui::TableItemToggleButton("Body Sleeping", "##HideBodySleeping", &m_pluginConfig->m_Hide.bSleeping);
			ImGui::TableItemToggleButton("Body Jumping", "##HideBodyJumping", &m_pluginConfig->m_Hide.bJumping);
			ImGui::TableItemToggleButton("Body Swimming", "##HideBodySwimming", &m_pluginConfig->m_Hide.bSwimming);
			ImGui::TableItemToggleButton("Body Sneak Roll", "##HideBodySneakRoll", &m_pluginConfig->m_Hide.bSneakRoll);
			ImGui::TableItemToggleButton("Body Attack", "##HideBodyAttack", &m_pluginConfig->m_Hide.bAttack);
			ImGui::TableItemToggleButton("Body Power Attack", "##HideBodyPowerAttack", &m_pluginConfig->m_Hide.bPowerAttack);
			ImGui::TableItemToggleButton("Body Attack Bow", "##HideBodyAttackBow", &m_pluginConfig->m_Hide.bAttackBow);
			ImGui::TableItemToggleButton("Body Killmove", "##HideBodyKillmove", &m_pluginConfig->m_Hide.bKillmove);

			ImGui::EndTable();
		}

		if (ImGui::Button("Close"))	m_Window = false;
		ImGui::End();
    }

}
