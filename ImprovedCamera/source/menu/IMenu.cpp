/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

// Precompiled Header
#include "PCH.h"

#include "menu/IMenu.h"

namespace Interface {

	IMenu::IMenu()
	{
		m_pluginConfig = DLLMain::Plugin::Get()->Config();
	}

	void IMenu::DisplayMenuNodes(const char* tableName, const std::uint32_t tableIndex)
	{
		static std::string tblName;
		tblName = {};
		tblName = tableName + std::to_string(tableIndex);

		if (ImGui::BeginTable(tblName.c_str(), 2, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoHostExtendX, ImVec2(0.0f, 0.0f)))
		{
			static std::string label;
			static std::uint32_t index;
			static float posX;
			index = 0;

			for (auto& node : m_MenuNodes)
			{
				index++;

				if (node.TableIndex != tableIndex)
					continue;

				// Setup label from the name
				label = {};
				label = node.Name;
				label.erase(remove(label.begin(), label.end(), ' '), label.end());
				label.insert(0, 2, '#');
				// Setup next row and column
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				// Begin callback
				if (node.MenuCallback)
					node.MenuCallback(index, true);
				// Display Name
				ImGui::Text(node.Name);
				// Setup next column for the control
				ImGui::TableNextColumn();

				ImGui::PushItemWidth(100.0f);
				switch (node.Control)
				{
					case ControlType::kNone:
					{
						break;
					}
					case ControlType::kToggle:
					{
						posX = (ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - 55 - ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
						if (posX > ImGui::GetCursorPosX())
							ImGui::SetCursorPosX(posX);

						ImGui::ToggleButton(label.c_str(), (bool*)node.Variable);

						break;
					}
					case ControlType::kSliderFloat:
					{
						ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);
						ImGui::SliderFloat(label.c_str(), (float*)node.Variable, node.MinValue, node.MaxValue, node.Format);
						ImGui::PopStyleVar();

						break;
					}
					case ControlType::kSliderInt:
					{
						ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);
						ImGui::SliderInt(label.c_str(), (std::int32_t*)node.Variable, (std::int32_t)node.MinValue, (std::int32_t)node.MaxValue, node.Format);
						ImGui::PopStyleVar();

						break;
					}
				}
				ImGui::PopItemWidth();
				// Setup tooltip
				if (strcmp(node.Tooltip, "") != 0 && ImGui::IsItemHovered())
					ImGui::SetTooltip(node.Tooltip);

				// End callback
				if (node.MenuCallback)
					node.MenuCallback(index, false);
			}
			ImGui::EndTable();
		}
	}

	Menu::~Menu()
	{
		for (IMenu* menu : m_Menus)
		{
			menu->OnShutdown();
			delete menu;
		}
	}

	void Menu::Register(IMenu* menu)
	{
		m_Menus.emplace(m_Menus.begin() + m_MenuInsertIndex, menu);
		m_MenuInsertIndex++;
	}

	void Menu::Unregister(IMenu* menu)
	{
		auto it = std::find(m_Menus.begin(), m_Menus.begin() + m_MenuInsertIndex, menu);

		if (it != m_Menus.begin() + m_MenuInsertIndex)
		{
			menu->OnShutdown();
			m_Menus.erase(it);
			m_MenuInsertIndex--;
		}
	}

}
