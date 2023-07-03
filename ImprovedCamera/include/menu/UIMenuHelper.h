/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h> // allows std:string

// We cheat and pollute ImGui's namespace!
namespace ImGui {

	extern void ToggleButton(const char* str_id, bool* v);
	extern void StyleColorsDefault(ImGuiStyle* dst = NULL);

	extern void TableItemInputText(const char* text, const char* inputText, std::string* setting);
	extern void TableItemInputInt(const char* text, const char* inputText, std::int32_t* setting);
	extern void TableItemInputFloat(const char* text, const char* inputText, float* setting, float step, float step_fast, const char* format);
	extern void TableItemToggleButton(const char* text, const char* buttonText, bool* setting, bool insertRow = true);
	extern void TableItemSliderFloat(const char* text, const char* sliderText, float* setting, float v_min, float v_max, const char* format, bool insertRow = true);

}
