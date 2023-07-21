/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "cameras/ICamera.h"

namespace ImprovedCamera {

	class CameraThirdPerson : public Interface::ICamera {

	public:
		struct State {

			enum ThirdPerson : std::uint8_t
			{
				kExit = 0,
				kEnter,
				kIdle,
				kWeaponDrawnEnter,
				kWeaponDrawnIdle,
				kSleepingEnter,
				kSleepingIdle,
				kCraftingEnter,
				kCraftingIdle,
				kDeathCinematicEnter,
				kDeathCinematicIdle,
				kVampireLordKillmove,
				kScriptedEnter,
				kScriptedIdle,
				kAnimationEnter,
				kAnimationIdle,
				kVampireLordEnter,
				kVampireLordIdle,
				kWerewolfEnter,
				kWerewolfIdle,
				kNecroLichEnter,
				kNecroLichIdle,

				kTotal
			};
		};

	public:
		CameraThirdPerson();
		virtual ~CameraThirdPerson() = default;

		virtual bool OnUpdate(std::uint8_t currentCamera, std::uint8_t previousCamera) override;
		virtual bool HasControl() override;
		virtual std::uint8_t GetEventID() override;
		virtual std::uint8_t GetStateID() override { return m_ThirdPersonState; };
		virtual const Data& GetData() const override { return this->m_Data; };
		virtual void OnShutdown() override{};

	private:
		void SetData();

		std::uint8_t m_ThirdPersonState = 0;
	};

}
