/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

// Precompiled Header
#include "PCH.h"

#include "menu/General.h"

#include "skyrimse/ImprovedCameraSE.h"

namespace Menu {

	struct MENU_IDS {

		enum MENU_ID : std::int32_t
		{
			kEnableBody = 1,
			kEnableShadows,
			kEnableBodyConsole,
			kAdjustPlayerScale,
			kBodyHeightOffset,
			kEnableHead,
			kEnableHeadCombat,
			kEnableHeadHorse,
			kEnableHeadDragon,
			kEnableHeadVampireLord,
			kEnableHeadWerewolf,
			kEnableHeadScripted,
			// Table 2
			kEnableThirdPersonArms,
			kEnableThirdPersonTorch,
			kEnableThirdPersonTorchBlock,
			kEnableThirdPersonShield,
			kEnableThirdPersonShieldBlock,
			kEnableThirdPersonBow,
			kEnableThirdPersonBowAim,
			kEnableThirdPersonCrossbow,
			kEnableThirdPersonCrossbowAim,
			kFirstPersonOverhaul,
			kOverrideVanillaArmsOnMovement,

			kTotal = 24
		};
	};
	using MENU_ID = MENU_IDS::MENU_ID;

	MenuGeneral::MenuGeneral()
	{
		// Table 1
		m_MenuNodes.emplace_back(1, "First Person Body", "Enable/Disable First Person Body visibility",
			ControlType::kToggle, (void*)&m_pluginConfig->m_General.bEnableBody, 0.0f, 0.0f, "", &MenuGeneral::OnCallback);
		m_MenuNodes.emplace_back(1, "First Person Shadows", "Enable/Disable First Person Shadows on Character (Only Working Outdoors!)",
			ControlType::kToggle, (void*)&m_pluginConfig->m_General.bEnableShadows);
		m_MenuNodes.emplace_back(1, "First Person Body Console", "Enable/disable First Person Body visibility while the console is open",
			ControlType::kToggle, (void*)&m_pluginConfig->m_General.bEnableBodyConsole, 0.0f, 0.0f, "", &MenuGeneral::OnCallback);
		m_MenuNodes.emplace_back(1, "Adjust Player Scale", "Adjust First Person Camera Height to match character's race scale",
			ControlType::kToggle, (void*)&m_pluginConfig->m_General.bAdjustPlayerScale);
		m_MenuNodes.emplace_back(1, "Body Height Offset", "Moves the character's body in First Person Up/Down",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_General.fBodyHeightOffset, -8192.0f, 8192.0f, "%.1f");
		m_MenuNodes.emplace_back(1, "Head First Person", "Enable/Disable First Person Head (May Make Hair Visible)",
			ControlType::kToggle, (void*)&m_pluginConfig->m_General.bEnableHead);
		m_MenuNodes.emplace_back(1, "Head First Person Combat", "Enable/Disable First Person Head whilst weapons are drawn (May Make Hair Visible)",
			ControlType::kToggle, (void*)&m_pluginConfig->m_General.bEnableHeadCombat);
		m_MenuNodes.emplace_back(1, "Head First Person Horse", "Enable/Disable First Person Head while mounted on a horse (May Make Hair Visible)",
			ControlType::kToggle, (void*)&m_pluginConfig->m_General.bEnableHeadHorse);
		m_MenuNodes.emplace_back(1, "Head First Person Dragon", "Enable/Disable First Person Head while mounted On a dragon (May Make Hair Visible)",
			ControlType::kToggle, (void*)&m_pluginConfig->m_General.bEnableHeadDragon);
		m_MenuNodes.emplace_back(1, "Head First Person Vampire Lord", "Enable/Disable First Person Head for Vampire Lord",
			ControlType::kToggle, (void*)&m_pluginConfig->m_General.bEnableHeadVampireLord);
		m_MenuNodes.emplace_back(1, "Head First Person Werewolf", "Enable/Disable First Person Head For Werewolf",
			ControlType::kToggle, (void*)&m_pluginConfig->m_General.bEnableHeadWerewolf);
		m_MenuNodes.emplace_back(1, "Head First Person Scripted", "Enable/Disable First Person Head during scripted animations",
			ControlType::kToggle, (void*)&m_pluginConfig->m_General.bEnableHeadScripted);
		// Table 2
		m_MenuNodes.emplace_back(2, "Third Person Arms", "Enable/Disable Third person Arms when in First Person",
			ControlType::kToggle, (void*)&m_pluginConfig->m_General.bEnableThirdPersonArms, 0.0f, 0.0f, "", &MenuGeneral::OnCallback);
		m_MenuNodes.emplace_back(2, "Third Person Torch", "Enable/Disable Third Person Torch when in First Person",
			ControlType::kToggle, (void*)&m_pluginConfig->m_General.bEnableThirdPersonTorch, 0.0f, 0.0f, "", &MenuGeneral::OnCallback);
		m_MenuNodes.emplace_back(2, "Third Person Torch Block", "Enable/Disable Torch Blocking when in First Person",
			ControlType::kToggle, (void*)&m_pluginConfig->m_General.bEnableThirdPersonTorchBlock);
		m_MenuNodes.emplace_back(2, "Third Person Shield", "Enable/Disable Third Person Shield when in First Person",
			ControlType::kToggle, (void*)&m_pluginConfig->m_General.bEnableThirdPersonShield, 0.0f, 0.0f, "", &MenuGeneral::OnCallback);
		m_MenuNodes.emplace_back(2, "Third Person Shield Block", "Enable/Disable Third Person Shield Blocking when in First Person",
			ControlType::kToggle, (void*)&m_pluginConfig->m_General.bEnableThirdPersonShieldBlock);
		m_MenuNodes.emplace_back(2, "Third Person Bow", "Enables/Disables Third Person Bow when in First Person",
			ControlType::kToggle, (void*)&m_pluginConfig->m_General.bEnableThirdPersonBow);
		m_MenuNodes.emplace_back(2, "Third Person Bow Aim", "Enable/Disable Third Person Bow Aiming when in First Person",
			ControlType::kToggle, (void*)&m_pluginConfig->m_General.bEnableThirdPersonBowAim);
		m_MenuNodes.emplace_back(2, "Third Person Crossbow", "Enable/Disable Ther Person Crossbow when in First Person",
			ControlType::kToggle, (void*)&m_pluginConfig->m_General.bEnableThirdPersonCrossbow);
		m_MenuNodes.emplace_back(2, "Third Person Crossbow Aim", "Enable/Disable Third Person Crossbow Aiming when in First Person",
			ControlType::kToggle, (void*)&m_pluginConfig->m_General.bEnableThirdPersonCrossbowAim);
		m_MenuNodes.emplace_back(2, "First Person Overhaul", "Compatibility with First Person Animation mods",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Fixes.bFirstPersonOverhaul, 0.0f, 0.0f, "", &MenuGeneral::OnCallback);
		m_MenuNodes.emplace_back(2, "Override Vanilla Arms on Movement", "Restores vanilla arms for movement",
			ControlType::kToggle, (void*)&m_pluginConfig->m_Fixes.bOverrideVanillaArmsOnMovement, 0.0f, 0.0f, "", &MenuGeneral::OnCallback);
	}

	void MenuGeneral::OnOpen()
	{
		ImGui::Checkbox("General", &m_Window);
	}

	void MenuGeneral::OnUpdate()
	{
		if (!m_Window)
			return;

		ImGui::Begin("[GENERAL]", &m_Window, ImGuiWindowFlags_NoCollapse);

		DisplayMenuNodes("GeneralTable");
		ImGui::SameLine();
		DisplayMenuNodes("GeneralTable", 2);

		if (ImGui::Button("Close"))
			m_Window = false;

		ImGui::End();
	}

	void MenuGeneral::OnCallback(std::uint32_t node, bool begin)
	{
		auto pluginSkyrimSE = DLLMain::Plugin::Get()->SkyrimSE();
		auto pluginConfig = DLLMain::Plugin::Get()->Config();

		switch (node)
		{
			case MENU_ID::kEnableBody:
			case MENU_ID::kEnableBodyConsole:
			{
				if (!begin && ImGui::IsItemClicked())
					pluginSkyrimSE->Camera()->ResetState();

				break;
			}
			case MENU_ID::kEnableThirdPersonArms:
			{
				if (!begin && ImGui::IsItemClicked())
					pluginSkyrimSE->Camera()->ResetPlayerNodes();

				break;
			}
			case MENU_ID::kEnableThirdPersonTorch:
			{
				if (!begin && ImGui::IsItemClicked())
					pluginSkyrimSE->Camera()->ResetPlayerNodes();

				break;
			}
			case MENU_ID::kEnableThirdPersonShield:
			{
				if (!begin && ImGui::IsItemClicked())
					pluginSkyrimSE->Camera()->ResetPlayerNodes();

				break;
			}
			case MENU_ID::kFirstPersonOverhaul:
			{
				if (begin)
					ImGui::Separator();

				break;
			}
			case MENU_ID::kOverrideVanillaArmsOnMovement:
			{
				bool fpo = pluginConfig->Fixes().bFirstPersonOverhaul;

				if (!fpo)
					begin ? ImGui::BeginDisabled() : ImGui::EndDisabled();

				break;
			}
		}
	}

}
