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

	MenuHide::MenuHide()
	{
		// Table 1
		m_MenuNodes.emplace_back(1, "Weaponary", "", ControlType::kNone);
		m_MenuNodes.emplace_back(1, "Two-Handed Weapon", "Setting to Hide Two Handed Weapons on Back to Prevent Camera Clipping",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Hide.b2HWeapon);
		m_MenuNodes.emplace_back(1, "Bow", "Setting to Hide Bows on Back to Prevent Camera Clipping",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Hide.bBow);
		m_MenuNodes.emplace_back(1, "Quiver", "Setting to Hide Quiver on Back to Prevent Camera Clipping",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Hide.bQuiver);

		m_MenuNodes.emplace_back(1, "Movements", "", ControlType::kNone);
		m_MenuNodes.emplace_back(1, "Sitting", "Setting For Hiding The Body While Sitting",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Hide.bSitting, 0.0f, 0.0f, "", &MenuHide::OnCallback);
		m_MenuNodes.emplace_back(1, "Sleeping", "Setting For Hiding The Body While Sleeping",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Hide.bSleeping);
		m_MenuNodes.emplace_back(1, "Jumping", "Setting For Hiding The Body While Jumping",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Hide.bJumping);
		m_MenuNodes.emplace_back(1, "Swimming", "Setting For Hiding The Body While Swimming",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Hide.bSwimming);
		m_MenuNodes.emplace_back(1, "Sneak Roll", "Setting For Hiding The Body During Sneak Roll",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Hide.bSneakRoll);
		m_MenuNodes.emplace_back(1, "Attack", "Setting For Hiding The Body During Attack",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Hide.bAttack);
		m_MenuNodes.emplace_back(1, "Power Attack", "Setting For Hiding The Body During Power Attack",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Hide.bPowerAttack);
		m_MenuNodes.emplace_back(1, "Attack Bow", "Setting For Hiding The Body While Attacking with Bow ",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Hide.bAttackBow);
		m_MenuNodes.emplace_back(1, "Killmove", "Setting For Hiding The Body During Killmove",
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

		DisplayMenuNodes("HideTable");

		if (ImGui::Button("Close"))
			m_Window = false;

		ImGui::End();
	}

	void MenuHide::OnCallback(std::uint32_t node, bool begin)
	{
		switch (node)
		{
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
