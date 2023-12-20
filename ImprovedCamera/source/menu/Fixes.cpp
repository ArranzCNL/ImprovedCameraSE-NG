/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

// Precompiled Header
#include "PCH.h"

#include "menu/Fixes.h"

namespace Menu {

	struct MENU_IDS {

		enum MENU_ID : std::int32_t
		{
			kFirstPersonOverhaul = 1,
			kOverrideVanillaArmsOnMovement,
			kArcheryGameplayOverhaul,
			kSmoothAnimationTransitions,
			kControllerBufferDepth1st,

			kTotal = 6
		};
	};
	using MENU_ID = MENU_IDS::MENU_ID;

	MenuFixes::MenuFixes()
	{
		// Table 1
		m_MenuNodes.emplace_back(1, "First Person Overhaul", "Compatibility with First Person Animation mods",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Fixes.bFirstPersonOverhaul);
		m_MenuNodes.emplace_back(1, "Override Vanilla Arms on Movement", "Restores Vanilla Arms for Movement",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Fixes.bOverrideVanillaArmsOnMovement, 0.0f, 0.0f, "", &MenuFixes::OnCallback);
		m_MenuNodes.emplace_back(1, "Archery Gameplay Overhaul", "Fixes Drawn Animation",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Fixes.bArcheryGameplayOverhaul);
		m_MenuNodes.emplace_back(1, "Smooth Animation Transitions", "Smooths First Person Movement Animation Transitions and adds Inertia",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Fixes.bSmoothAnimationTransitions);
		m_MenuNodes.emplace_back(1, "Controller Buffer First Person", "Sets The Amount of Inertia, (Requires Smooth Animation Transitions to be Enabled)",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_Fixes.fControllerBufferDepth1st, 0.01f, 0.50f, "%.2f", &MenuFixes::OnCallback);
	}

	void MenuFixes::OnOpen()
	{
		ImGui::Checkbox("Fixes", &m_Window);
	}

	void MenuFixes::OnUpdate()
	{
		if (!m_Window)
			return;

		ImGui::Begin("[FIXES]", &m_Window, ImGuiWindowFlags_NoCollapse);
		
		DisplayMenuNodes("FixesTable");

		if (ImGui::Button("Close"))
			m_Window = false;

		ImGui::End();
	}

	void MenuFixes::OnCallback(std::uint32_t node, bool begin)
	{
		auto pluginConfig = DLLMain::Plugin::Get()->Config();

		switch (node)
		{
			case MENU_ID::kOverrideVanillaArmsOnMovement:
			{
				bool fpo = pluginConfig->Fixes().bFirstPersonOverhaul;
				if (!fpo)
					begin ? ImGui::BeginDisabled() : ImGui::EndDisabled();

				break;
			}
			case MENU_ID::kControllerBufferDepth1st:
			{
				bool sat = pluginConfig->Fixes().bSmoothAnimationTransitions;
				if (!sat)
					begin ? ImGui::BeginDisabled() : ImGui::EndDisabled();

				break;
			}
		}
	}

}
