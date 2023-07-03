/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <spdlog/spdlog.h>


constexpr auto LOGGER_NAME = "Logger";

#define LOG_TRACE(...) if (spdlog::get(LOGGER_NAME) != nullptr) { spdlog::get(LOGGER_NAME)->trace(__VA_ARGS__); }
#define LOG_DEBUG(...) if (spdlog::get(LOGGER_NAME) != nullptr) { spdlog::get(LOGGER_NAME)->debug(__VA_ARGS__); }
#define LOG_INFO(...) if (spdlog::get(LOGGER_NAME) != nullptr) { spdlog::get(LOGGER_NAME)->info(__VA_ARGS__); }
#define LOG_WARN(...) if (spdlog::get(LOGGER_NAME) != nullptr) { spdlog::get(LOGGER_NAME)->warn(__VA_ARGS__); }
#define LOG_ERROR(...) if (spdlog::get(LOGGER_NAME) != nullptr) { spdlog::get(LOGGER_NAME)->error(__VA_ARGS__); }
#define LOG_CRITICAL(...) if (spdlog::get(LOGGER_NAME) != nullptr) { spdlog::get(LOGGER_NAME)->critical(__VA_ARGS__); }
