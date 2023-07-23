/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

// Precompiled Header
#include "stdafx.h"

#include "plugin.h"
#include "utils/Utils.h"

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <time.h>

namespace Utils {

	const std::string GetFileInfo(const std::string& fullFilePath, const std::string& keyValue)
	{
		char File[MAX_PATH]{};
		sprintf_s(File, "%s", fullFilePath.c_str());

		const DWORD versionSize = GetFileVersionInfoSize(File, nullptr);
		DWORD versionLen = 0, langID = 0;
		std::string strValue{};

		if (versionSize)
		{
			std::uint8_t* versionBuffer = new std::uint8_t[versionSize];

			if (versionBuffer && GetFileVersionInfo(File, versionLen, versionSize, versionBuffer))
			{
				if (keyValue.compare("ProductVersion") == 0)
				{
					VS_FIXEDFILEINFO* productVersion = nullptr;
					std::uint32_t productVersionLen = 0;

					if (VerQueryValue(versionBuffer, TEXT("\\"), reinterpret_cast<LPVOID*>(&productVersion), reinterpret_cast<std::uint32_t*>(&productVersionLen)) && productVersionLen && productVersion)
					{
						char buffer[64]{};
						sprintf_s(buffer, "%d.%d.%d.%d", (productVersion->dwProductVersionMS >> 16) & 0xffff,
							(productVersion->dwProductVersionMS >> 0) & 0xffff,
							(productVersion->dwProductVersionLS >> 16) & 0xffff,
							(productVersion->dwProductVersionLS >> 0) & 0xffff);

						strValue = buffer;
					}
				}
				else
				{
					char fileInfo[256]{};
					LPVOID fileInfoValue = nullptr;
					std::uint32_t fileInfoLen = 0;
					bool retVal = false;

					sprintf_s(fileInfo, "\\VarFileInfo\\Translation");
					retVal = VerQueryValue(versionBuffer, fileInfo, &fileInfoValue, reinterpret_cast<std::uint32_t*>(&fileInfoLen));

					if (retVal && fileInfoLen == 4)
					{
						memcpy(&langID, fileInfoValue, 4);
						sprintf_s(fileInfo, "\\StringFileInfo\\%02X%02X%02X%02X\\%s", (langID & 0xff00) >> 8, langID & 0xff, (langID & 0xff000000) >> 24, (langID & 0xff0000) >> 16, keyValue.c_str());
					}
					else
					{
						// Default language
						sprintf_s(fileInfo, "\\StringFileInfo\\040904B0\\%s", keyValue.c_str());
					}
					retVal = VerQueryValue(versionBuffer, fileInfo, &fileInfoValue, reinterpret_cast<std::uint32_t*>(&fileInfoLen));

					if (!retVal)
					{
						// Alien language
						sprintf_s(fileInfo, "\\StringFileInfo\\00000000\\%s", keyValue.c_str());
						retVal = VerQueryValue(versionBuffer, fileInfo, &fileInfoValue, reinterpret_cast<std::uint32_t*>(&fileInfoLen));
					}

					if (retVal)
					{
						char fileInfoValueBuffer[128]{};
						sprintf_s(fileInfoValueBuffer, "%s", reinterpret_cast<char*>(fileInfoValue));
						strValue = fileInfoValueBuffer;
					}
				}
			}
			delete[] versionBuffer;
		}
		return strValue;
	}

	bool PluginExists(const std::string& name, bool appPath)
	{
		auto plugin = DLLMain::Plugin::Get();

		std::string fullFilePath{};
		FILE* pFile = nullptr;

		if (appPath)
			fullFilePath = plugin->SkyrimSE()->Path() + name;
		else
			fullFilePath = plugin->Path() + name;

		errno_t err = fopen_s(&pFile, fullFilePath.c_str(), "r");

		if (!err)
		{
			fclose(pFile);
			return true;
		}
		return false;
	}

	void GetVersionFromString(const std::string& text, std::uint32_t version[])
	{
		if (text.empty())
			return;

		std::string strVersion = text;
		std::string delimiter = ".";

		std::size_t pos = 0;
		std::size_t end = -1 * delimiter.size();
		std::int32_t i = 0;

		do {
			if (i > 3)
				break;

			pos = end + delimiter.size();
			end = strVersion.find(delimiter, pos);
			version[i++] = stoi(strVersion.substr(pos, end - pos));

		} while (end != -1);
	}

	const std::string GetCurrentDateTime(const char* fmt)
	{
		time_t now = time(0);
		struct tm tstruct {};
		char buffer[128]{};
		localtime_s(&tstruct, &now);
		// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
		// for more information about date/time format
		strftime(buffer, sizeof(buffer), fmt, &tstruct);

		return buffer;
	}

	// Note: This only supports US/GB keyboards maybe others, untested!
	void CorrectExtendedKeys(const std::uint32_t scanCode, std::uint32_t* virtualKey)
	{
		switch (scanCode)
		{
			case DIK_LWIN:
			{
				*virtualKey = VK_LWIN;
				break;
			}
			case DIK_RMENU:
			{
				*virtualKey = VK_RMENU;
				break;
			}
			case DIK_RWIN:
			{
				*virtualKey = VK_RWIN;
				break;
			}
			case DIK_APPS:
			{
				*virtualKey = VK_APPS;
				break;
			}
			case DIK_RCONTROL:
			{
				*virtualKey = VK_RCONTROL;
				break;
			}
			case DIK_SYSRQ:
			{
				// Print Screen
				*virtualKey = VK_SNAPSHOT;
				break;
			}
			// Not an extended key
			case DIK_PAUSE:
			{
				*virtualKey = VK_PAUSE;
				break;
			}
			case DIK_INSERT:
			{
				*virtualKey = VK_INSERT;
				break;
			}
			case DIK_HOME:
			{
				*virtualKey = VK_HOME;
				break;
			}
			case DIK_PRIOR:
			{
				// Page Up
				*virtualKey = VK_PRIOR;
				break;
			}
			case DIK_DELETE:
			{
				*virtualKey = VK_DELETE;
				break;
			}
			case DIK_END:
			{
				*virtualKey = VK_END;
				break;
			}
			case DIK_NEXT:
			{
				// Page Down
				*virtualKey = VK_NEXT;
				break;
			}
			case DIK_UP:
			{
				*virtualKey = VK_UP;
				break;
			}
			case DIK_LEFT:
			{
				*virtualKey = VK_LEFT;
				break;
			}
			case DIK_DOWN:
			{
				*virtualKey = VK_DOWN;
				break;
			}
			case DIK_RIGHT:
			{
				*virtualKey = VK_RIGHT;
				break;
			}
			case DIK_DIVIDE:
			{
				*virtualKey = VK_DIVIDE;
				break;
			}
			case DIK_NUMPAD7:
			{
				*virtualKey = VK_NUMPAD7;
				break;
			}
			case DIK_NUMPAD8:
			{
				*virtualKey = VK_NUMPAD8;
				break;
			}
			case DIK_NUMPAD9:
			{
				*virtualKey = VK_NUMPAD9;
				break;
			}
			case DIK_NUMPAD4:
			{
				*virtualKey = VK_NUMPAD4;
				break;
			}
			case DIK_NUMPAD5:
			{
				*virtualKey = VK_NUMPAD5;
				break;
			}
			case DIK_NUMPAD6:
			{
				*virtualKey = VK_NUMPAD6;
				break;
			}
			case DIK_NUMPAD1:
			{
				*virtualKey = VK_NUMPAD1;
				break;
			}
			case DIK_NUMPAD2:
			{
				*virtualKey = VK_NUMPAD2;
				break;
			}
			case DIK_NUMPAD3:
			{
				*virtualKey = VK_NUMPAD3;
				break;
			}
			case DIK_NUMPAD0:
			{
				*virtualKey = VK_NUMPAD0;
				break;
			}
			case DIK_DECIMAL:
			{
				*virtualKey = VK_DECIMAL;
				break;
			}
			case DIK_NUMPADENTER:
			{
				*virtualKey = VK_RETURN + 256;  // Numpad Enter (Special handling)
				break;
			}
			// Unsure of this one should be fine.
			case DIK_NUMPADEQUALS:
			{
				*virtualKey = VK_OEM_NEC_EQUAL;
				break;
			}
		}
	}

}
