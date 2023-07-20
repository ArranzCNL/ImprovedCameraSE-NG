/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

 // Precompiled Header
#include "stdafx.h"

#include "api/skse_api.h"

#include "plugin.h"
#include "version.h"
#include "utils/Log.h"
#include "skyrimse/ImprovedCameraSE.h"


namespace SKSE {

	void SKSEMessageHandler(SKSE::MessagingInterface::Message* message)
	{
		auto plugin = DLLMain::Plugin::Get();

		switch (message->type)
		{
			case SKSE::MessagingInterface::kPostPostLoad:
			{
				plugin->SkyrimSE()->Camera()->RequestAPIs();
				break;
			}
			case SKSE::MessagingInterface::kPostLoadGame:
			{
				plugin->SkyrimSE()->LoadGame();
				break;
			}
			case SKSE::MessagingInterface::kInputLoaded:
			{
				plugin->CreateMenu();
				break;
			}
			case SKSE::MessagingInterface::kDataLoaded:
			{
				if (plugin->Graphics()->IsInitalized())
					plugin->m_GraphicsInitialized = true;

				break;
			}
		}
	}

	SKSE_API constinit auto SKSEPlugin_Version = []() noexcept
	{
		SKSE::PluginVersionData pluginData{};
		REL::Version version{VERSION_MAJOR, VERSION_MINOR, VERSION_REVISION, VERSION_BUILD};

		pluginData.PluginName(VERSION_PRODUCTNAME_STR);
		pluginData.PluginVersion(version);
		pluginData.AuthorName(VERSION_AUTHOR_STR);
		pluginData.UsesAddressLibrary(true);
		pluginData.HasNoStructUse(true);

		return pluginData;
	}();

	SKSE_API bool SKSEPlugin_Query(const SKSE::QueryInterface* skse, SKSE::PluginInfo* pluginInfo)
	{
		pluginInfo->name = SKSEPlugin_Version.pluginName;
		pluginInfo->version = SKSEPlugin_Version.pluginVersion;
		pluginInfo->infoVersion = SKSE::PluginInfo::kVersion;

		if (skse->IsEditor())
		{
			LOG_CRITICAL("Loaded in editor, marking as incompatible");
			return false;
		}

		const auto version = skse->RuntimeVersion();
		if (version < SKSE::RUNTIME_SSE_1_5_39)
		{
			LOG_CRITICAL("Unsupported runtime version {}", version.string());
			return false;
		}
		return true;
	}

	SKSE_API bool SKSEPlugin_Load(const SKSE::LoadInterface* skse)
	{
		auto plugin = DLLMain::Plugin::Get();
		plugin->m_Logging.Initialize();

		SKSE::Init(skse);
		
		bool loaded = plugin->Load();

		if (loaded)
		{
			auto msgInterface = SKSE::GetMessagingInterface();
			msgInterface->RegisterListener(SKSEMessageHandler);
			plugin->SkyrimSE()->OnLoad();
		}
		return loaded;
	}

}
