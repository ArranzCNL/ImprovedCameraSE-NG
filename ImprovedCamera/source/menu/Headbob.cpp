/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

// Precompiled Header
#include "PCH.h"

#include "menu/Headbob.h"

namespace Menu {

	struct MENU_IDS {

		enum MENU_ID : std::int32_t
		{
			kIdle = 1,
			kWalk,
			kRun,
			kSprint,
			kCombat,
			kSneak,
			kSneakRoll,
			// Table 2
			kRotationIdle,
			kRotationWalk,
			kRotationRun,
			kRotationSprint,
			kRotationCombat,
			kRotationSneak,
			kRotationSneakRoll,

			kTotal = 15
		};
	};
	using MENU_ID = MENU_IDS::MENU_ID;

	MenuHeadbob::MenuHeadbob()
	{
		// Table 1
		m_MenuNodes.emplace_back(1, "Idle", "Enables Headbob When in First Person for Idles (May Not be Compatible With Some Animations)",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Headbob.bIdle);
		m_MenuNodes.emplace_back(1, "Walk", "Enables Headbob When in First Person for Walk (May Not be Compatible With Some Animations)",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Headbob.bWalk);
		m_MenuNodes.emplace_back(1, "Run", "Enables Headbob When in First Person for Run (May Not be Compatible With Some Animations)",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Headbob.bRun);
		m_MenuNodes.emplace_back(1, "Sprint", "Enables Headbob When in First Person for Sprint (May Not be Compatible With Some Animations)",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Headbob.bSprint);
		m_MenuNodes.emplace_back(1, "Combat", "Enables Headbob When in First Person for Combat (May Not be Compatible With Some Animations)",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Headbob.bCombat);
		m_MenuNodes.emplace_back(1, "Sneak", "Enables Headbob When in First Person for Sneak (May Not be Compatible With Some Animations)",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Headbob.bSneak);
		m_MenuNodes.emplace_back(1, "Sneak Roll", "Enables Headbob When in First Person for Sneak Roll (May Not be Compatible With Some Animations)",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Headbob.bSneakRoll);
		// Table 2
		m_MenuNodes.emplace_back(2, "Idle Rotation", "Rotation settings match camera rotation to head rotation. Valid values between 0.0 and 1.0 controlling the amount of rotation ",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_Headbob.fRotationIdle, 0.0f, 1.0f, "%.1f", &MenuHeadbob::OnCallback);
		m_MenuNodes.emplace_back(2, "Walk Rotation", "Rotation settings match camera rotation to head rotation. Valid values between 0.0 and 1.0 controlling the amount of rotation",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_Headbob.fRotationWalk, 0.0f, 1.0f, "%.1f", &MenuHeadbob::OnCallback);
		m_MenuNodes.emplace_back(2, "Run Rotation", "Rotation settings match camera rotation to head rotation. Valid values between 0.0 and 1.0 controlling the amount of rotation",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_Headbob.fRotationRun, 0.0f, 1.0f, "%.1f", &MenuHeadbob::OnCallback);
		m_MenuNodes.emplace_back(2, "Sprint Rotation", "Rotation settings match camera rotation to head rotation. Valid values between 0.0 and 1.0 controlling the amount of rotation",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_Headbob.fRotationSprint, 0.0f, 1.0f, "%.1f", &MenuHeadbob::OnCallback);
		m_MenuNodes.emplace_back(2, "Combat Rotation", "Rotation settings match camera rotation to head rotation. Valid values between 0.0 and 1.0 controlling the amount of rotation",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_Headbob.fRotationCombat, 0.0f, 1.0f, "%.1f", &MenuHeadbob::OnCallback);
		m_MenuNodes.emplace_back(2, "Sneak Rotation", "Rotation settings match camera rotation to head rotation. Valid values between 0.0 and 1.0 controlling the amount of rotation",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_Headbob.fRotationSneak, 0.0f, 1.0f, "%.1f", &MenuHeadbob::OnCallback);
		m_MenuNodes.emplace_back(2, "Sneak Roll Rotation", "Rotation settings match camera rotation to head rotation. Valid values between 0.0 and 1.0 controlling the amount of rotation",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_Headbob.fRotationSneakRoll, 0.0f, 1.0f, "%.1f", &MenuHeadbob::OnCallback);
	}

	void MenuHeadbob::OnOpen()
	{
		ImGui::Checkbox("Headbob", &m_Window);
	}

	void MenuHeadbob::OnUpdate()
	{
		if (!m_Window)
			return;

		ImGui::Begin("[HEADBOB]", &m_Window, ImGuiWindowFlags_NoCollapse);

		DisplayMenuNodes("HeadbobTable");
		ImGui::SameLine();
		DisplayMenuNodes("HeadbobTable", 2);

		if (ImGui::Button("Close"))
			m_Window = false;

		ImGui::End();
	}

	void MenuHeadbob::OnCallback(std::uint32_t node, bool begin)
	{
		auto pluginConfig = DLLMain::Plugin::Get()->Config();

		switch (node)
		{
			case MENU_ID::kRotationIdle:
			{
				bool hbi = pluginConfig->Headbob().bIdle;
				if (!hbi)
					begin ? ImGui::BeginDisabled() : ImGui::EndDisabled();

				break;
			}
			case MENU_ID::kRotationWalk:
			{
				bool hbw = pluginConfig->Headbob().bWalk;
				if (!hbw)
					begin ? ImGui::BeginDisabled() : ImGui::EndDisabled();

				break;
			}
			case MENU_ID::kRotationRun:
			{
				bool hbr = pluginConfig->Headbob().bRun;
				if (!hbr)
					begin ? ImGui::BeginDisabled() : ImGui::EndDisabled();

				break;
			}
			case MENU_ID::kRotationSprint:
			{
				bool hbs = pluginConfig->Headbob().bSprint;
				if (!hbs)
					begin ? ImGui::BeginDisabled() : ImGui::EndDisabled();

				break;
			}
			case MENU_ID::kRotationCombat:
			{
				bool hbc = pluginConfig->Headbob().bCombat;
				if (!hbc)
					begin ? ImGui::BeginDisabled() : ImGui::EndDisabled();

				break;
			}
			case MENU_ID::kRotationSneak:
			{
				bool hbsk = pluginConfig->Headbob().bSneak;
				if (!hbsk)
					begin ? ImGui::BeginDisabled() : ImGui::EndDisabled();

				break;
			}
			case MENU_ID::kRotationSneakRoll:
			{
				bool hbskr = pluginConfig->Headbob().bSneakRoll;
				if (!hbskr)
					begin ? ImGui::BeginDisabled() : ImGui::EndDisabled();

				break;
			}
		}
	}

}
