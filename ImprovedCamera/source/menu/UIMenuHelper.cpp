/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

 // Precompiled Header
#include "stdafx.h"

#include "menu/UIMenuHelper.h"

#include <imgui_internal.h>


// Custom ImGui
namespace ImGui {
	// All credits to ocornut (ImGui Author) for ToggleButton, just tweaked a little.
	void ToggleButton(const char* str_id, bool* v)
	{
		ImVec2 p = ImGui::GetCursorScreenPos();
		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		float height = ImGui::GetFrameHeight();
		float width = height * 1.75f;
		float radius = height * 0.50f;

		ImGui::InvisibleButton(str_id, ImVec2(width, height));
		if (ImGui::IsItemClicked())
		{
			*v = !*v;
		}
		float t = *v ? 1.0f : 0.0f;

		ImGuiContext& g = *GImGui;
		float ANIM_SPEED = 0.08f;
		if (g.LastActiveId == g.CurrentWindow->GetID(str_id))
		{
			float t_anim = ImSaturate(g.LastActiveIdTimer / ANIM_SPEED);
			t = *v ? (t_anim) : (1.0f - t_anim);
		}

		ImU32 col_bg;
		if (ImGui::IsItemHovered())
		{
			col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.78f, 0.78f, 0.78f, 1.0f), ImVec4(0.34f, 0.67f, 0.98f, 1.0f), t));
		}
		else
		{
			col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.85f, 0.85f, 0.85f, 1.0f), ImVec4(0.26f, 0.59f, 0.98f, 1.0f), t));
		}
		draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + (height - 4.0f)), col_bg, height * 0.5f);
		draw_list->AddCircleFilled(ImVec2(p.x + radius + t * (width - radius * 2.0f), p.y + (radius - 2.0f)), radius - 0.5f, IM_COL32(255, 255, 255, 255));
	}
	// Default theme
	void StyleColorsDefault(ImGuiStyle* dst)
	{
		ImGuiStyle* style = dst ? dst : &ImGui::GetStyle();
		ImVec4* colors = ImGui::GetStyle().Colors;

		style->WindowTitleAlign = ImVec2(0.5, 0.5);
		style->FramePadding = ImVec2(4, 4);

		// Rounded slider grabber
		style->GrabRounding = 12.0f;

		// Window
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.118f, 0.118f, 0.118f, 0.784f };
		colors[ImGuiCol_ResizeGrip] = ImVec4{ 0.2f, 0.2f, 0.2f, 0.5f };
		colors[ImGuiCol_ResizeGripHovered] = ImVec4{ 0.3f, 0.3f, 0.3f, 0.75f };
		colors[ImGuiCol_ResizeGripActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

		// Header
		colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

		// Title
		colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

		// Frame Background
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

		// Button
		colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

		// Tab
		colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.38f, 0.38f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.28f, 0.28f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };

		// Seperator - Inner borders from Visual Studio are this colour when grabbing.
		//colors[ImGuiCol_SeparatorActive] = ImVec4(0.09f, 0.09f, 0.09f, 1.00f);
	}

	void TableItemInputText(const char* text, const char* inputText, std::string* setting)
	{
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text(text);
		ImGui::TableNextColumn();
		ImGui::PushItemWidth(250);
		ImGui::InputText(inputText, setting);
		ImGui::PopItemWidth();
	}

	void TableItemInputInt(const char* text, const char* inputText, std::int32_t* setting)
	{
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text(text);
		ImGui::TableNextColumn();
		ImGui::PushItemWidth(250);
		ImGui::InputInt(inputText, setting);
		ImGui::PopItemWidth();
	}

	void TableItemInputFloat(const char* text, const char* inputText, float* setting, float step, float step_fast, const char* format)
	{
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text(text);
		ImGui::TableNextColumn();
		ImGui::PushItemWidth(250);
		ImGui::InputFloat(inputText, setting, step, step_fast, format);
		ImGui::PopItemWidth();
	}

	void TableItemToggleButton(const char* text, const char* buttonText, bool* setting, bool insertRow)
	{
		if (insertRow) ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text(text);
		ImGui::TableNextColumn();
		ImGui::ToggleButton(buttonText, setting);
	}

	void TableItemSliderFloat(const char* text, const char* sliderText, float* setting, float v_min, float v_max, const char* format, bool insertRow)
	{
		if (insertRow) ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text(text);
		ImGui::TableNextColumn();
		ImGui::PushItemWidth(100);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);
		ImGui::SliderFloat(sliderText, setting, v_min, v_max, format, ImGuiSliderFlags_::ImGuiSliderFlags_AlwaysClamp);
		ImGui::PopStyleVar();
		ImGui::PopItemWidth();
	}

}
