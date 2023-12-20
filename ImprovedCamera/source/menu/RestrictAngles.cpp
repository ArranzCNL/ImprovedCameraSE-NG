/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

// Precompiled Header
#include "PCH.h"

#include "menu/RestrictAngles.h"

namespace Menu {

	struct MENU_IDS {

		enum MENU_ID : std::int32_t
		{
			kSitting = 1,
			kSittingMaxLookingUp,
			kSittingMaxLookingDown,
			kMounted,
			kMountedMaxLookingUp,
			kMountedMaxLookingDown,
			kFlying,
			kFlyingMaxLookingUp,
			kFlyingMaxLookingDown,
			kVampireLord,
			kWerewolf,
			kNecroLich,
			kScripted,
			kScriptedPitch,

			kTotal = 15
		};
	};
	using MENU_ID = MENU_IDS::MENU_ID;

	MenuRestrictAngles::MenuRestrictAngles()
	{
		m_MenuNodes.emplace_back(1, "Sitting Restrict Angle", "Restricts left/right Angle to Prevent Clipping into Head/Body",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_RestrictAngles.fSitting, 0.0f, 180.0f, "%.1f");
		m_MenuNodes.emplace_back(1, "Sitting Max Looking Up", "Restricts looking Up Angle to Prevent Clipping into Head/Body",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_RestrictAngles.fSittingMaxLookingUp, 0.0f, 180.0f, "%.1f");
		m_MenuNodes.emplace_back(1, "Sitting Max Looking Down", "Restricts looking down Angle to Prevent Clipping into Head/Body",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_RestrictAngles.fSittingMaxLookingDown, 0.0f, 180.0f, "%.1f");
		m_MenuNodes.emplace_back(1, "Mounted Restrict Angle", "Restricts Left/Right Angles to Prevent Clipping into Head/Body",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_RestrictAngles.fMounted, 0.0f, 180.0f, "%.1f");
		m_MenuNodes.emplace_back(1, "Mounted Max Looking Up", "Restricts Looking Up Angle to Prevent Clipping into Head/Body",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_RestrictAngles.fMountedMaxLookingUp, 0.0f, 180.0f, "%.1f");
		m_MenuNodes.emplace_back(1, "Mounted Max Looking Down", "Restricts looking down Angle to Prevent Clipping into Head/Body",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_RestrictAngles.fMountedMaxLookingDown, 0.0f, 180.0f, "%.1f");
		m_MenuNodes.emplace_back(1, "Flying Restrict Angle", "Restricts Left/Right Angles to Prevent Clipping into Head/Body",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_RestrictAngles.fFlying, 0.0f, 180.0f, "%.1f");
		m_MenuNodes.emplace_back(1, "Flying Max Looking Up", "Restricts looking up Angle to Prevent Clipping into Head/Body",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_RestrictAngles.fFlyingMaxLookingUp, 0.0f, 180.0f, "%.1f");
		m_MenuNodes.emplace_back(1, "Flying Max Looking Down", "Restricts looking down Angle to Prevent Clipping into Head/Body",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_RestrictAngles.fFlyingMaxLookingDown, 0.0f, 180.0f, "%.1f");
		m_MenuNodes.emplace_back(1, "VampireLord Restrict Angle", "Restricts Angles to Prevent Clipping into Head/Body",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_RestrictAngles.fVampireLord, 0.0f, 180.0f, "%.1f");
		m_MenuNodes.emplace_back(1, "Werewolf Restrict Angle", "Restricts Angles to Prevent Clipping into Head/Body",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_RestrictAngles.fWerewolf, 0.0f, 180.0f, "%.1f");
		m_MenuNodes.emplace_back(1, "Lich Restrict Angle", "Restricts Angles to Prevent Clipping into Head/Body",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_RestrictAngles.fNecroLich, 0.0f, 180.0f, "%.1f");
		m_MenuNodes.emplace_back(1, "Scripted Restrict Angle", "Restricts Angles to Prevent Clipping into Head/Body during Scripted Events",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_RestrictAngles.fScripted, 0.0f, 180.0f, "%.1f");
		m_MenuNodes.emplace_back(1, "Scripted Restrict Pitch", "Restricts Pitch to Prevent Clipping into Head/Body during Scripted Events",
			ControlType::kSliderFloat, (void*)&m_pluginConfig->m_RestrictAngles.fScriptedPitch, 0.0f, 180.0f, "%.1f");
	}

	void MenuRestrictAngles::OnOpen()
	{
		ImGui::Checkbox("Restrict Angles", &m_Window);
	}

	void MenuRestrictAngles::OnUpdate()
	{
		if (!m_Window)
			return;

		ImGui::Begin("[RESTRICT ANGLES]", &m_Window, ImGuiWindowFlags_NoCollapse);
		
		DisplayMenuNodes("RestrictAnglesTable");
		if (ImGui::Button("Close"))
			m_Window = false;

		ImGui::End();
	}

}
