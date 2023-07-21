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
		if (insertRow)
			ImGui::TableNextRow();

		ImGui::TableNextColumn();
		ImGui::Text(text);
		ImGui::TableNextColumn();
		ImGui::ToggleButton(buttonText, setting);
	}

	void TableItemSliderFloat(const char* text, const char* sliderText, float* setting, float v_min, float v_max, const char* format, bool insertRow)
	{
		if (insertRow)
			ImGui::TableNextRow();

		ImGui::TableNextColumn();
		ImGui::Text(text);
		ImGui::TableNextColumn();
		ImGui::PushItemWidth(100);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);
		ImGui::SliderFloat(sliderText, setting, v_min, v_max, format, ImGuiSliderFlags_::ImGuiSliderFlags_AlwaysClamp);
		ImGui::PopStyleVar();
		ImGui::PopItemWidth();
	}

	const ImGuiKey VirtualKeyToImGuiKey(const std::uint32_t key)
	{
		switch (key)
		{
			case VK_TAB:
				return ImGuiKey_Tab;
			case VK_LEFT:  // Left Arrow
				return ImGuiKey_LeftArrow;
			case VK_RIGHT:  // Right Arrow
				return ImGuiKey_RightArrow;
			case VK_UP:  // Up Arrow
				return ImGuiKey_UpArrow;
			case VK_DOWN:  // Down Arrow
				return ImGuiKey_DownArrow;
			case VK_PRIOR:  // Page Up
				return ImGuiKey_PageUp;
			case VK_NEXT:  // Page Down
				return ImGuiKey_PageDown;
			case VK_HOME:
				return ImGuiKey_Home;
			case VK_END:
				return ImGuiKey_End;
			case VK_INSERT:
				return ImGuiKey_Insert;
			case VK_DELETE:
				return ImGuiKey_Delete;
			case VK_BACK:
				return ImGuiKey_Backspace;
			case VK_SPACE:
				return ImGuiKey_Space;
			case VK_RETURN:
				return ImGuiKey_Enter;
			case VK_ESCAPE:
				return ImGuiKey_Escape;
			case VK_LCONTROL:
				return ImGuiKey_LeftCtrl;
			case VK_LSHIFT:
				return ImGuiKey_LeftShift;
			case VK_LMENU:  // Left Alt
				return ImGuiKey_LeftAlt;
			case VK_LWIN:
				return ImGuiKey_LeftSuper;
			case VK_RCONTROL:
				return ImGuiKey_RightCtrl;
			case VK_RSHIFT:
				return ImGuiKey_RightShift;
			case VK_RMENU:  // Right Alt
				return ImGuiKey_RightAlt;
			case VK_RWIN:
				return ImGuiKey_RightSuper;
			case VK_APPS:
				return ImGuiKey_Menu;
			case '0':
				return ImGuiKey_0;
			case '1':
				return ImGuiKey_1;
			case '2':
				return ImGuiKey_2;
			case '3':
				return ImGuiKey_3;
			case '4':
				return ImGuiKey_4;
			case '5':
				return ImGuiKey_5;
			case '6':
				return ImGuiKey_6;
			case '7':
				return ImGuiKey_7;
			case '8':
				return ImGuiKey_8;
			case '9':
				return ImGuiKey_9;
			case 'A':
				return ImGuiKey_A;
			case 'B':
				return ImGuiKey_B;
			case 'C':
				return ImGuiKey_C;
			case 'D':
				return ImGuiKey_D;
			case 'E':
				return ImGuiKey_E;
			case 'F':
				return ImGuiKey_F;
			case 'G':
				return ImGuiKey_G;
			case 'H':
				return ImGuiKey_H;
			case 'I':
				return ImGuiKey_I;
			case 'J':
				return ImGuiKey_J;
			case 'K':
				return ImGuiKey_K;
			case 'L':
				return ImGuiKey_L;
			case 'M':
				return ImGuiKey_M;
			case 'N':
				return ImGuiKey_N;
			case 'O':
				return ImGuiKey_O;
			case 'P':
				return ImGuiKey_P;
			case 'Q':
				return ImGuiKey_Q;
			case 'R':
				return ImGuiKey_R;
			case 'S':
				return ImGuiKey_S;
			case 'T':
				return ImGuiKey_T;
			case 'U':
				return ImGuiKey_U;
			case 'V':
				return ImGuiKey_V;
			case 'W':
				return ImGuiKey_W;
			case 'X':
				return ImGuiKey_X;
			case 'Y':
				return ImGuiKey_Y;
			case 'Z':
				return ImGuiKey_Z;
			case VK_F1:
				return ImGuiKey_F1;
			case VK_F2:
				return ImGuiKey_F2;
			case VK_F3:
				return ImGuiKey_F3;
			case VK_F4:
				return ImGuiKey_F4;
			case VK_F5:
				return ImGuiKey_F5;
			case VK_F6:
				return ImGuiKey_F6;
			case VK_F7:
				return ImGuiKey_F7;
			case VK_F8:
				return ImGuiKey_F8;
			case VK_F9:
				return ImGuiKey_F9;
			case VK_F10:
				return ImGuiKey_F10;
			case VK_F11:
				return ImGuiKey_F11;
			case VK_F12:
				return ImGuiKey_F12;
			case VK_OEM_7:  // Apostrophe
				return ImGuiKey_Apostrophe;
			case VK_OEM_COMMA:
				return ImGuiKey_Comma;
			case VK_OEM_MINUS:
				return ImGuiKey_Minus;
			case VK_OEM_PERIOD:
				return ImGuiKey_Period;
			case VK_OEM_2:  // Forward Slash
				return ImGuiKey_Slash;
			case VK_OEM_1:  // Semicolon
				return ImGuiKey_Semicolon;
			case VK_OEM_PLUS:
				return ImGuiKey_Equal;
			case VK_OEM_4:  // Left Bracket
				return ImGuiKey_LeftBracket;
			case VK_OEM_5:  // Backslash
				return ImGuiKey_Backslash;
			case VK_OEM_6:  // Right Bracket
				return ImGuiKey_RightBracket;
			case VK_OEM_3:  // Grave Accent
				return ImGuiKey_GraveAccent;
			case VK_CAPITAL:
				return ImGuiKey_CapsLock;
			case VK_SCROLL:
				return ImGuiKey_ScrollLock;
			case VK_NUMLOCK:
				return ImGuiKey_NumLock;
			case VK_SNAPSHOT:  // Print Screen
				return ImGuiKey_PrintScreen;
			case VK_PAUSE:
				return ImGuiKey_Pause;
			case VK_NUMPAD0:
				return ImGuiKey_Keypad0;
			case VK_NUMPAD1:
				return ImGuiKey_Keypad1;
			case VK_NUMPAD2:
				return ImGuiKey_Keypad2;
			case VK_NUMPAD3:
				return ImGuiKey_Keypad3;
			case VK_NUMPAD4:
				return ImGuiKey_Keypad4;
			case VK_NUMPAD5:
				return ImGuiKey_Keypad5;
			case VK_NUMPAD6:
				return ImGuiKey_Keypad6;
			case VK_NUMPAD7:
				return ImGuiKey_Keypad7;
			case VK_NUMPAD8:
				return ImGuiKey_Keypad8;
			case VK_NUMPAD9:
				return ImGuiKey_Keypad9;
			case VK_DECIMAL:
				return ImGuiKey_KeypadDecimal;
			case VK_DIVIDE:
				return ImGuiKey_KeypadDivide;
			case VK_MULTIPLY:
				return ImGuiKey_KeypadMultiply;
			case VK_SUBTRACT:
				return ImGuiKey_KeypadSubtract;
			case VK_ADD:
				return ImGuiKey_KeypadAdd;
			case VK_RETURN + 256:  // Numpad Enter (Special handling from ImGui)
				return ImGuiKey_KeypadEnter;
			case VK_OEM_NEC_EQUAL:  // Numpad Equal
				return ImGuiKey_KeypadEqual;
			default:
				return ImGuiKey_None;
		}
	}

}
