/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once


namespace Utils {

	extern const std::string GetFileInfo(const std::string& fullFilePath, const std::string& keyValue);
	extern bool PluginExists(const std::string& name, bool appPath = false);
	extern void GetVersionFromString(const std::string& text, std::uint32_t version[]);
	extern const std::string GetCurrentDateTime(const char* fmt);

	extern void CorrectExtendedKeys(const std::uint32_t scanCode, std::uint32_t* virtualKey);
}
