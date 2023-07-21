/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

// Precompiled Header
#include "stdafx.h"

/*
	This is for not modifying core vendor files
	due to using a precompiled header.
	You can force include the pch however you
	tend to lose IntelliSense.
*/

// Dear ImGui
#include <imgui/backends/imgui_impl_dx11.cpp>
#include <imgui/backends/imgui_impl_win32.cpp>
#include <imgui/misc/cpp/imgui_stdlib.cpp>
