/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "menu/IMenu.h"

namespace Menu {

	class MenuGeneral final : public Interface::IMenu {

	public:
		MenuGeneral();
		virtual ~MenuGeneral() = default;

		virtual void OnOpen() override;
		virtual void OnUpdate() override;
		virtual void OnClose() override{};

		virtual void OnShutdown() override{};

		static void OnCallback(std::uint32_t node, bool begin);
	};

}
