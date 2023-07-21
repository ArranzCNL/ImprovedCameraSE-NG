/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "cameras/ICamera.h"

namespace ImprovedCamera {

	class CameraHorse : public Interface::ICamera {

	public:
		struct State {

			enum Horse : std::uint8_t
			{
				kDismounted = 0,
				kMounting,
				kMounted,
				kRiding,
				kWeaponDrawnEnter,
				kWeaponDrawnIdle,
				kDismounting,

				kTotal
			};
		};

	public:
		CameraHorse();
		virtual ~CameraHorse() = default;

		virtual bool OnUpdate(std::uint8_t currentCamera, std::uint8_t previousCamera) override;
		virtual bool HasControl() override;
		virtual std::uint8_t GetEventID() override;
		virtual std::uint8_t GetStateID() override { return m_HorseState; };
		virtual const Data& GetData() const override { return this->m_Data; };
		virtual void OnShutdown() override{};

	private:
		void SetData();

		std::uint8_t m_HorseState = 0;
	};

}
