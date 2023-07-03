/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

 // Precompiled Header
#include "stdafx.h"

#include "systems/Logging.h"

#include "plugin.h"
#include "version.h"
#include "utils/Log.h"

#include <ShlObj.h>
#include <KnownFolders.h>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>


namespace Systems {

	Logging::~Logging()
	{
		spdlog::shutdown();
	}

	void Logging::Initialize()
	{
		if (!m_Initialized)
		{
			m_Initialized = true;

			PWSTR myDocuments{ nullptr };
			SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_DEFAULT, nullptr, &myDocuments);
			std::filesystem::path logFile(myDocuments);
			CoTaskMemFree(myDocuments);

			auto plugin = DLLMain::Plugin::Get();
			char logPath[MAX_PATH]{};
			snprintf(logPath, sizeof(logPath), "\\My Games\\%s\\SKSE\\%s.log", plugin->Config()->ModuleData().sWindowName.c_str(), plugin->Name().c_str());
			logFile += logPath;

			auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFile.string(), true);
			fileSink->set_pattern("%v");

#ifdef _DEBUG
			auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
			consoleSink->set_pattern("%^[%d/%m/%C %H:%M:%S] %v%$");

			std::vector<spdlog::sink_ptr> sinks{ fileSink, consoleSink };
#else
			std::vector<spdlog::sink_ptr> sinks{ fileSink };
#endif

			auto logger = std::make_shared<spdlog::logger>(LOGGER_NAME, sinks.begin(), sinks.end());

			logger->set_level(spdlog::level::trace);
			logger->flush_on(spdlog::level::trace);
			spdlog::register_logger(logger);

			logger->debug("{} v{}", VERSION_PRODUCTNAME_STR, VERSION_STR);
			fileSink->set_pattern("[%d/%m/%C %H:%M:%S] %v");
		}
	}

}
