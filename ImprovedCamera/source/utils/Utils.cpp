/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

 // Precompiled Header
#include "stdafx.h"

#include "utils/Utils.h"
#include "plugin.h"

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
		std::string strVersion = text;
		std::string delimiter = ".";

		std::size_t pos = 0;
		std::int32_t i = 0;

		while ((pos = strVersion.find(delimiter)) != std::string::npos)
		{
			if (i > 3)
			{
				break;
			}
			version[i] = stoi(strVersion.substr(0, pos));
			strVersion.erase(0, pos + delimiter.length());
			i++;
		}
	}

	const std::string GetCurrentDateTime(const char* fmt)
	{
		time_t now = time(0);
		struct tm tstruct{};
		char buffer[128]{};
		localtime_s(&tstruct, &now);
		// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
		// for more information about date/time format
		strftime(buffer, sizeof(buffer), fmt, &tstruct);

		return buffer;
	}

}
