/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <SKSE/SKSE.h>

#define SKSE_API extern "C" __declspec(dllexport)

namespace SKSE {

	extern void SKSEMessageHandler(SKSE::MessagingInterface::Message* message);
	SKSE_API bool SKSEPlugin_Load(const SKSE::LoadInterface* skse);

}
