/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

// Precompiled Header
#include "stdafx.h"

#include "plugin.h"

#include "utils/Log.h"
#include "utils/Utils.h"
#include "version.h"

namespace DLLMain {

	Plugin* Plugin::s_Instance = nullptr;

	Plugin::Plugin()
	{
		if (s_Instance)
			return;

		s_Instance = this;

		m_Name = VERSION_PRODUCTNAME_STR;
		m_Description = VERSION_PRODUCTNAME_DESCRIPTION_STR;
		m_VersionMajor = VERSION_MAJOR;
		m_VersionMinor = VERSION_MINOR;
		m_VersionRevision = VERSION_REVISION;
		m_VersionBuild = VERSION_BUILD;

		char currentDirectory[MAX_PATH]{};
		const DWORD directorySize = GetCurrentDirectory(MAX_PATH, currentDirectory);

		if (directorySize && directorySize <= MAX_PATH)
		{
			std::string path{};
			path = currentDirectory;
			path += "\\";

			m_Path = path + PLUGIN_PATH;
		}

		m_Config = std::make_unique<Systems::Config>();
		m_SkyrimSE = std::make_unique<Patch::SkyrimSE>();

		// Append " GOG" to the window name for 1.6.659
		if (m_SkyrimSE->Build() == Patch::SkyrimSE::BuildInfo::k16659)
			m_Config->m_ModuleData.sWindowName += " GOG";
	}

	Plugin::~Plugin()
	{
#ifdef _DEBUG
		std::cout << "Debug Console Terminated." << std::endl;
		FreeConsole();
#endif
	}

	bool Plugin::Load()
	{
		if (m_Loaded)
			return true;

		m_Loaded = true;

		if (!m_Config->m_PreInitialized)
		{
			LOG_ERROR("Failed to load {}", m_Config->FileName().c_str());
			return false;
		}

		m_Config->m_Initialized = true;
		LOG_DEBUG("Profile:\t\t\t\t{}", m_Config->ModuleData().sProfileName.c_str());

		if (!CheckSkyrim())
			return false;

		if (m_Config->ModuleData().iCheckCompatibility)
			CheckCompatibility();

		return true;
	}

	void Plugin::CreateMenu()
	{
		if (m_InitializeMenu)
			return;

		m_InitializeMenu = true;

		// No need to run this if a menu is not required
		if (m_Config->ModuleData().iMenuMode > Systems::Window::UIDisplay::kNone)
		{
			m_Graphics = std::make_unique<Systems::Graphics>(m_Config->ModuleData().iMenuMode);
		}
	}

	bool Plugin::CheckSkyrim()
	{
		LOG_INFO("Checking {} information...", m_SkyrimSE->FullName().c_str());

		std::uint32_t fileVersionMin[4]{};
		std::uint32_t fileVersionMax[4]{};
		std::uint32_t skseVersionMin[4]{};

		Utils::GetVersionFromString(m_Config->ModuleData().sFileVersionMin, fileVersionMin);
		Utils::GetVersionFromString(m_Config->ModuleData().sFileVersionMax, fileVersionMax);

		if (m_SkyrimSE->VersionMajor() >= fileVersionMin[0] && m_SkyrimSE->VersionMajor() <= fileVersionMax[0] &&
			m_SkyrimSE->VersionMinor() >= fileVersionMin[1] && m_SkyrimSE->VersionMinor() <= fileVersionMax[1] &&
			m_SkyrimSE->VersionRevision() >= fileVersionMin[2] && m_SkyrimSE->VersionRevision() <= fileVersionMax[2] &&
			m_SkyrimSE->VersionBuild() >= fileVersionMin[3] && m_SkyrimSE->VersionBuild() <= fileVersionMax[3])
		{
			LOG_TRACE("  Version:\t\t\t\tv{}.{}.{}.{}", m_SkyrimSE->VersionMajor(), m_SkyrimSE->VersionMinor(), m_SkyrimSE->VersionRevision(), m_SkyrimSE->VersionBuild());
			LOG_TRACE("  Base Address:\t\t\t0x{:016X}", m_SkyrimSE->BaseAddress());
			LOG_TRACE("  Image Size:\t\t\t0x{:016X}", m_SkyrimSE->ImageSize());

			std::string fullFilePath = m_SkyrimSE->Path() + "skse64_loader.exe";
			std::string productName = Utils::GetFileInfo(fullFilePath, "ProductName");
			std::string fileVersion = Utils::GetFileInfo(fullFilePath, "ProductVersion");
			Utils::GetVersionFromString(fileVersion, skseVersionMin);

			if (productName.empty())
			{
				LOG_CRITICAL("Unable to find: skse64_loader.exe to verify SKSE64 version, aborting.");
				return false;
			}
			if (skseVersionMin[0] == 0 && skseVersionMin[1] == 2 && skseVersionMin[2] == 0 && skseVersionMin[3] < 18)
			{
				LOG_CRITICAL("{}: v{} not supported. (Requirement: SKSE64 v0.2.0.18 or higher)", productName.c_str(), fileVersion.c_str());
				return false;
			}
			LOG_TRACE("  {}:\t\t\t\tv{}", productName.c_str(), fileVersion.c_str());
		}
		else
		{
			LOG_CRITICAL("{}: v{}.{}.{}.{} not supported.", m_SkyrimSE->Name().c_str(), m_SkyrimSE->VersionMajor(), m_SkyrimSE->VersionMinor(), m_SkyrimSE->VersionRevision(), m_SkyrimSE->VersionBuild());
			return false;
		}
		return true;
	}

	void Plugin::CheckCompatibility()
	{
		std::string fullFilePath{};
		std::string productName{};
		std::string fileVersion{};

		LOG_INFO("Checking for compatibility issues...");

		/* Check for any known compatibility issues */
		bool ReShade = Utils::PluginExists("dxgi.dll", true);
		// Mods to check
		bool SkyrimTogether = Utils::PluginExists("Data\\SkyrimTogetherReborn\\SkyrimTogether.exe", true);

		if (ReShade)
		{
			// Pull file information.
			fullFilePath = m_SkyrimSE->Path() + "dxgi.dll";
			productName = Utils::GetFileInfo(fullFilePath, "ProductName");

			if (productName.compare("ReShade") != 0)
				ReShade = false;
		}
		if (SkyrimTogether)
		{
			// Compatibility.
			if (m_Config->m_ModuleData.iMenuMode)
				m_Config->m_ModuleData.iMenuMode = Systems::Window::UIDisplay::kOverlay;

			m_Config->m_ModuleData.sWindowName = "Skyrim Together";
			// Pull file information.
			fullFilePath = m_SkyrimSE->Path() + "Data\\SkyrimTogetherReborn\\SkyrimTogetherServer.exe";
			productName = Utils::GetFileInfo(fullFilePath, "ProductName");
			fileVersion = Utils::GetFileInfo(fullFilePath, "ProductVersion");

			if (!productName.empty())
			{
				LOG_TRACE("  Found:\t\t\t\t{} v{} ({} re-configured)", productName.c_str(), fileVersion.c_str(), VERSION_PRODUCTNAME_DESCRIPTION_STR);
			}
			else
			{
				LOG_TRACE("  Found:\t\t\t\tSkyrimTogetherServer.exe ({} re-configured)", VERSION_PRODUCTNAME_DESCRIPTION_STR);
			}
		}
		LOG_INFO("Finished checking for compatibility issues.");
	}

}
