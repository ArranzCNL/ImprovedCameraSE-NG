/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

// Precompiled Header
#include "PCH.h"

#include "cameras/Horse.h"

#include "skyrimse/Addresses.h"
#include "skyrimse/ImprovedCameraSE.h"
#include "utils/ICMath.h"

namespace ImprovedCamera {

	using namespace Address::Variable;

	CameraHorse::CameraHorse() :
		ICamera("Horse", RE::CameraStates::kMount)
	{
		SetData();
	}

	bool CameraHorse::OnUpdate(std::uint8_t currentCamera, std::uint8_t previousCamera)
	{
		this->Player = RE::PlayerCharacter::GetSingleton();
		// Dismounted Horse
		if (currentCamera != RE::CameraStates::kMount && m_HorseState)
		{
			m_HorseState = CameraHorse::State::kDismounted;
			return false;
		}
		// AutoVanity check
		if (currentCamera == RE::CameraStates::kMount && previousCamera == RE::CameraStates::kAutoVanity && m_HorseState == CameraHorse::State::kDismounted)
		{
			m_HorseState = CameraHorse::State::kMounted;
			return true;
		}
		// TweenMenu check
		if (currentCamera == RE::CameraStates::kMount && previousCamera == RE::CameraStates::kTween && m_HorseState == CameraHorse::State::kDismounted)
		{
			m_HorseState = CameraHorse::State::kMounted;
			return true;
		}
		// Load Game check. Note: 255
		if (currentCamera == RE::CameraStates::kMount && previousCamera == 255 && m_HorseState == CameraHorse::State::kDismounted)
		{
			m_HorseState = CameraHorse::State::kMounted;
			return true;
		}
		// Mounting Horse
		if (currentCamera == RE::CameraStates::kMount && m_HorseState == CameraHorse::State::kDismounted && Helper::IsInteractionEnter(this->Player))
		{
			m_HorseState = CameraHorse::State::kMounting;
			SetData();
			return true;
		}
		// Mounted Horse
		if (currentCamera == RE::CameraStates::kMount && m_HorseState == CameraHorse::State::kMounting && Helper::IsInteracting(this->Player))
		{
			m_HorseState = CameraHorse::State::kMounted;
			SetData();
			// Fix to face forward once mounted
			auto thirdpersonState = (RE::ThirdPersonState*)RE::PlayerCamera::GetSingleton()->currentState.get();
			thirdpersonState->freeRotation.x = this->Player->data.angle.x;

			return true;
		}
		// Riding Horse
		if (currentCamera == RE::CameraStates::kMount && m_HorseState == CameraHorse::State::kMounted)
		{
			m_HorseState = this->Player->AsActorState()->IsWeaponDrawn() ? CameraHorse::State::kWeaponDrawnEnter : CameraHorse::State::kRiding;
			SetData();
			return true;
		}
		// Weapon Drawn Enter
		if (currentCamera == RE::CameraStates::kMount && m_HorseState == CameraHorse::State::kWeaponDrawnEnter)
		{
			m_HorseState = CameraHorse::State::kWeaponDrawnIdle;
			SetData();
			return true;
		}
		// Dismounting Horse
		if (currentCamera == RE::CameraStates::kMount && m_HorseState == CameraHorse::State::kRiding && Helper::IsInteractionLeave(this->Player))
		{
			m_HorseState = CameraHorse::State::kDismounting;
			SetData();
			return true;
		}
		return false;
	}

	bool CameraHorse::HasControl()
	{
		auto pluginCamera = DLLMain::Plugin::Get()->SkyrimSE()->Camera();
		bool isFirstPerson = pluginCamera->IsFirstPerson();

		if (m_HorseState)
		{
			if (m_HorseState == CameraHorse::State::kRiding || m_HorseState == CameraHorse::State::kWeaponDrawnIdle)
			{
				// Weapon Drawn detection
				if (m_HorseState == CameraHorse::State::kRiding && this->Player->AsActorState()->IsWeaponDrawn() ||
					m_HorseState == CameraHorse::State::kWeaponDrawnIdle && !this->Player->AsActorState()->IsWeaponDrawn())
				{
					m_HorseState = CameraHorse::State::kMounted;
					return true;
				}
				// Change riding angles
				auto thirdpersonState = (RE::ThirdPersonState*)RE::PlayerCamera::GetSingleton()->currentState.get();
				float horseMaxAngle = m_pluginConfig->RestrictAngles().fMounted * (M_PI / 180.0f);  // Degrees to Radians;

				if (m_pluginConfig->Events().bHorse && m_HorseState == CameraHorse::State::kRiding ||
					m_pluginConfig->Events().bHorseCombat && m_HorseState == CameraHorse::State::kWeaponDrawnIdle)
				{
					float zoom = *fMinCurrentZoom + 0.03f;

					if (isFirstPerson && thirdpersonState->currentZoomOffset <= zoom)
					{
						if (thirdpersonState->freeRotation.x >= horseMaxAngle)
							thirdpersonState->freeRotation.x = horseMaxAngle;

						if (thirdpersonState->freeRotation.x <= -horseMaxAngle)
							thirdpersonState->freeRotation.x = -horseMaxAngle;

						*fMountedMaxLookingUp = m_pluginConfig->RestrictAngles().fMountedMaxLookingUp;
						*fMountedMaxLookingDown = m_pluginConfig->RestrictAngles().fMountedMaxLookingDown;
					}
				}
			}
			return true;
		}
		return false;
	}

	std::uint8_t CameraHorse::GetEventID()
	{
		switch (m_HorseState)
		{
			case CameraHorse::State::kMounting:
			case CameraHorse::State::kMounted:
			case CameraHorse::State::kDismounting:
				return CameraEvent::kHorseTransition;

			case CameraHorse::State::kWeaponDrawnEnter:
			case CameraHorse::State::kWeaponDrawnIdle:
				return CameraEvent::kHorseCombat;
		}
		return CameraEvent::kHorse;
	}

	void CameraHorse::SetData()
	{
		switch (m_HorseState)
		{
			case CameraHorse::State::kMounting:
			case CameraHorse::State::kMounted:
			case CameraHorse::State::kDismounting:
			{
				this->m_Data.EventActive = &m_pluginConfig->Events().bHorseTransition;
				this->m_Data.FOV = &m_pluginConfig->FOV().fHorseTransition;
				this->m_Data.NearDistance = &m_pluginConfig->NearDistance().fHorseTransition;
				break;
			}
			case CameraHorse::State::kWeaponDrawnEnter:
			case CameraHorse::State::kWeaponDrawnIdle:
			{
				this->m_Data.EventActive = &m_pluginConfig->Events().bHorseCombat;
				this->m_Data.FOV = &m_pluginConfig->FOV().fHorseCombat;
				this->m_Data.NearDistance = &m_pluginConfig->NearDistance().fHorseCombat;
				break;
			}
			default:
			{
				this->m_Data.EventActive = &m_pluginConfig->Events().bHorse;
				this->m_Data.FOV = &m_pluginConfig->FOV().fHorse;
				this->m_Data.NearDistance = &m_pluginConfig->NearDistance().fHorse;
			}
		}
	}

}
