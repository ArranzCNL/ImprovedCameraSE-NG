/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

// Precompiled Header
#include "PCH.h"

#include "menu/Hide.h"

namespace Menu {

	struct MENU_IDS {

		enum MENU_ID : std::int32_t
		{
			kEmptyControl1 = 1,

			k2HWeapon,
			kBow,
			kQuiver,

			kEmptyControl2,

			kSitting,
			kSleeping,
			kJumping,
			kSwimming,
			kSneakRoll,
			kAttack,
			kPowerAttack,
			kAttackBow,
			kKillmove,

			kTotal = 15
		};
	};
	using MENU_ID = MENU_IDS::MENU_ID;

	static bool s_iedActive = false;

	MenuHide::MenuHide()
	{
		if (GetModuleHandle("ImmersiveEquipmentDisplays.dll") && !s_iedActive)
			s_iedActive = true;

		// Table 1
		m_MenuNodes.emplace_back(1, "Weaponary", "", ControlType::kNone);
		m_MenuNodes.emplace_back(1, "Two-Handed Weapon", "Setting to hide Two Handed Weapon(s) on back to prevent camera clipping",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Hide.b2HWeapon, 0.0f, 0.0f, "", &MenuHide::OnCallback);
		m_MenuNodes.emplace_back(1, "Bow", "Setting to hide Bow(s) on back to prevent camera clipping",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Hide.bBow);
		m_MenuNodes.emplace_back(1, "Quiver", "Setting to hide Quiver(s) on back to prevent camera clipping",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Hide.bQuiver, 0.0f, 0.0f, "", &MenuHide::OnCallback);

		m_MenuNodes.emplace_back(1, "Movements", "", ControlType::kNone);
		m_MenuNodes.emplace_back(1, "Sitting", "Setting to hide the body while Sitting",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Hide.bSitting, 0.0f, 0.0f, "", &MenuHide::OnCallback);
		m_MenuNodes.emplace_back(1, "Sleeping", "Setting to hide the body while Sleeping",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Hide.bSleeping);
		m_MenuNodes.emplace_back(1, "Jumping", "Setting to hide the body while Jumping",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Hide.bJumping);
		m_MenuNodes.emplace_back(1, "Swimming", "Setting to hide the body while Swimming",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Hide.bSwimming);
		m_MenuNodes.emplace_back(1, "Sneak Roll", "Setting to hide the body during Sneak Rolling",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Hide.bSneakRoll);
		m_MenuNodes.emplace_back(1, "Attack", "Setting to hide the body while Attacking",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Hide.bAttack);
		m_MenuNodes.emplace_back(1, "Power Attack", "Setting to hide the body while Power Attacking",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Hide.bPowerAttack);
		m_MenuNodes.emplace_back(1, "Attack Bow", "Setting to hide the body while Attacking with a Bow",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Hide.bAttackBow);
		m_MenuNodes.emplace_back(1, "Killmove", "Setting to hide the body while in a Killmove",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Hide.bKillmove, 0.0f, 0.0f, "", &MenuHide::OnCallback);
	}

	void MenuHide::OnOpen()
	{
		ImGui::Checkbox("Hide", &m_Window);
	}

	void MenuHide::OnUpdate()
	{
		if (!m_Window)
			return;

		ImGui::Begin("[HIDE]", &m_Window, ImGuiWindowFlags_NoCollapse);

		if (s_iedActive)
		{
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Immersive Equipment Displays detected!");
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Please use IED to hide weapons.");
			ImGui::Separator();
		}
		DisplayMenuNodes("HideTable");

		if (ImGui::Button("Close"))
			m_Window = false;

		ImGui::End();
	}

	void MenuHide::OnCallback(std::uint32_t node, bool begin)
	{
		switch (node)
		{
			case MENU_ID::k2HWeapon:
			{
				if (begin && s_iedActive)
					ImGui::BeginDisabled();

				break;
			}
			case MENU_ID::kQuiver:
			{
				if (!begin && s_iedActive)
					ImGui::EndDisabled();

				break;
			}
			case MENU_ID::kSitting:
			{
				if (begin)
					ImGui::BeginDisabled();

				break;
			}
			case MENU_ID::kKillmove:
			{
				if (!begin)
					ImGui::EndDisabled();

				break;
			}
		}
	}

}
