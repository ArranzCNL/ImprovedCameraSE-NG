/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

// Precompiled Header
#include "stdafx.h"

#include "skyrimse/SkyrimSE.h"

#include "plugin.h"
#include "skyrimse/EventsSkyrim.h"
#include "skyrimse/ImprovedCameraSE.h"
#include "utils/Utils.h"

namespace Patch {

	SkyrimSE::SkyrimSE()
	{
		auto pluginConfig = DLLMain::Plugin::Get()->Config();

		m_FullName = pluginConfig->ModuleData().sFileName;
		m_Name = m_FullName.substr(0, m_FullName.size() - 4);

		ExecutableInfo();
		BuildInfo();
	}

	void SkyrimSE::OnLoad()
	{
		if (m_OnLoaded)
			return;

		m_OnLoaded = true;

		// Initialize Hooks
		m_Hooks = std::make_unique<Patch::Hooks>();
		m_Camera = std::make_unique<ImprovedCamera::ImprovedCameraSE>();
		// Install Hooks
		m_Hooks->Install();
	}

	void SkyrimSE::InstallInput()
	{
		m_Hooks->Input();
		Events::Observer::Register();
	}

	void SkyrimSE::LoadGame()
	{
		m_Camera->ResetState();
	}

	void SkyrimSE::ExecutableInfo()
	{
		char currentDirectory[MAX_PATH]{};
		const DWORD directorySize = GetCurrentDirectory(MAX_PATH, currentDirectory);

		if (directorySize && directorySize <= MAX_PATH)
		{
			m_Path = currentDirectory;
			m_Path += "\\";
		}

		char File[MAX_PATH]{};
		sprintf_s(File, "%s%s", m_Path.c_str(), m_FullName.c_str());

		const DWORD versionSize = GetFileVersionInfoSize(File, NULL);
		std::uint32_t version[4]{};

		if (versionSize)
		{
			std::uint8_t* versionBuffer = new std::uint8_t[versionSize];

			if (versionBuffer && GetFileVersionInfo(File, NULL, versionSize, versionBuffer))
			{
				char* productVersion = nullptr;
				std::uint32_t productVersionLen = 0;

				if (VerQueryValueA(versionBuffer, TEXT("\\StringFileInfo\\040904B0\\ProductVersion"), reinterpret_cast<LPVOID*>(&productVersion), reinterpret_cast<std::uint32_t*>(&productVersionLen)) && productVersionLen && productVersion)
				{
					Utils::GetVersionFromString(productVersion, version);

					MODULEINFO moduleInfo{};
					HMODULE hModule = GetModuleHandleA(m_FullName.c_str());

					if (hModule)
					{
						GetModuleInformation(GetCurrentProcess(), hModule, &moduleInfo, sizeof(MODULEINFO));
						m_BaseAddress = reinterpret_cast<std::uintptr_t>(moduleInfo.lpBaseOfDll);
						m_ImageSize = (std::uintptr_t)moduleInfo.SizeOfImage;
					}
				}
			}
			delete[] versionBuffer;
		}

		m_VersionMajor = version[0];
		m_VersionMinor = version[1];
		m_VersionRevision = version[2];
		m_VersionBuild = version[3];
	}

	void SkyrimSE::BuildInfo()
	{
		if (m_VersionMajor == 1 && m_VersionMinor == 5 && m_VersionRevision >= 39 && m_VersionRevision <= 97)
			m_Build = BuildInfo::k15970;

		else if (m_VersionMajor == 1 && m_VersionMinor == 6 && m_VersionRevision >= 317 && m_VersionRevision <= 353)
			m_Build = BuildInfo::k16353;

		else if (m_VersionMajor == 1 && m_VersionMinor == 6 && m_VersionRevision >= 629 && m_VersionRevision < 659)
			m_Build = BuildInfo::k16640;

		else if (m_VersionMajor == 1 && m_VersionMinor == 6 && m_VersionRevision == 659)
			m_Build = BuildInfo::k16659;

		else
			m_Build = BuildInfo::kInvalid;
	}

}
