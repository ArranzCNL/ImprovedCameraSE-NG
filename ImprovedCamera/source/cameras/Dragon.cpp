/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

 // Precompiled Header
#include "stdafx.h"

#include "cameras/Dragon.h"

#include "skyrimse/ImprovedCameraSE.h"
#include "skyrimse/Addresses.h"
#include "utils/ICMath.h"


namespace ImprovedCamera {

	using namespace Address::Variable;

	CameraDragon::CameraDragon()
		: ICamera("Dragon", RE::CameraStates::kDragon)
	{
		SetData();
	}

	bool CameraDragon::OnUpdate(std::uint8_t currentCamera, std::uint8_t previousCamera)
	{
		this->Player = RE::PlayerCharacter::GetSingleton();

		// Dismounted Dragon
		if (currentCamera != RE::CameraStates::kDragon && m_DragonState)
		{
			m_DragonState = CameraDragon::State::kDismounted;
			return false;
		}
		// AutoVanity check
		if (currentCamera == RE::CameraStates::kDragon && previousCamera == RE::CameraStates::kAutoVanity && m_DragonState == CameraDragon::State::kDismounted)
		{
			m_DragonState = CameraDragon::State::kMounted;
			return true;
		}
		// TweenMenu check
		if (currentCamera == RE::CameraStates::kDragon && previousCamera == RE::CameraStates::kTween && m_DragonState == CameraDragon::State::kDismounted)
		{
			m_DragonState = CameraDragon::State::kMounted;
			return true;
		}
		// Load Game check. Note: 255
		if (currentCamera == RE::CameraStates::kDragon && previousCamera == 255 && m_DragonState == CameraDragon::State::kDismounted)
		{
			m_DragonState = CameraDragon::State::kMounted;
			return true;
		}
		// Mounting Dragon
		if (currentCamera == RE::CameraStates::kDragon && m_DragonState == CameraDragon::State::kDismounted && Helper::IsInteractionEnter(this->Player))
		{
			m_DragonState = CameraDragon::State::kMounting;
			SetData();
			return true;
		}
		// Dismounting Dragon
		if (currentCamera == RE::CameraStates::kDragon && previousCamera == RE::CameraStates::kAnimated && m_DragonState == CameraDragon::State::kDismounted && Helper::IsInteractionLeave(this->Player))
		{
			m_DragonState = CameraDragon::State::kDismounting;
			SetData();
			return true;
		}
		// Mounted Dragon
		if (currentCamera == RE::CameraStates::kDragon && previousCamera == RE::CameraStates::kAnimated && m_DragonState == CameraDragon::State::kDismounted)
		{
			m_DragonState = CameraDragon::State::kMounted;
			SetData();
			// Fix to face forward once mounted
			auto thirdpersonState = (RE::ThirdPersonState*)RE::PlayerCamera::GetSingleton()->currentState.get();
			thirdpersonState->freeRotation.x = this->Player->data.angle.x;

			return true;
		}
		// Riding Dragon
		if (currentCamera == RE::CameraStates::kDragon && m_DragonState == CameraDragon::State::kMounted)
		{
			m_DragonState = CameraDragon::State::kRiding;
			SetData();
			return true;
		}
		// Dismounting Dragon
		if (currentCamera == RE::CameraStates::kDragon && m_DragonState == CameraDragon::State::kRiding && Helper::IsInteractionLeave(this->Player))
		{
			m_DragonState = CameraDragon::State::kDismounting;
			SetData();
			return true;
		}
		return false;
	}

	bool CameraDragon::HasControl()
	{
		auto pluginCamera = DLLMain::Plugin::Get()->SkyrimSE()->Camera();
		bool isFirstPerson = pluginCamera->IsFirstPerson();

		if (m_DragonState)
		{
			if (m_DragonState == CameraDragon::State::kRiding)
			{
				auto thirdpersonState = (RE::ThirdPersonState*)RE::PlayerCamera::GetSingleton()->currentState.get();

				float dragonMaxAngle = m_pluginConfig->RestrictAngles().fFlying * (M_PI / 180.0f); // Degrees to Radians;

				if (m_pluginConfig->Events().bDragon && m_DragonState == CameraDragon::State::kRiding)
				{
					float zoom = *fMinCurrentZoom + m_pluginConfig->Fixes().fSwitchPOVDetectDistance;

					if (isFirstPerson && thirdpersonState->currentZoomOffset <= zoom)
					{
						if (thirdpersonState->freeRotation.x >= dragonMaxAngle) thirdpersonState->freeRotation.x = dragonMaxAngle;
						if (thirdpersonState->freeRotation.x <= -dragonMaxAngle) thirdpersonState->freeRotation.x = -dragonMaxAngle;

						*fFlyingMaxLookingUp = m_pluginConfig->RestrictAngles().fFlyingMaxLookingUp;
						*fFlyingMaxLookingDown = m_pluginConfig->RestrictAngles().fFlyingMaxLookingDown;
					}
				}
			}
			return true;
		}
		return false;
	}

	std::uint8_t CameraDragon::GetEventID()
	{
		switch (m_DragonState)
		{
			case CameraDragon::State::kMounting:
			case CameraDragon::State::kMounted:
			case CameraDragon::State::kDismounting:
				return CameraEvent::kDragonTransition;
		}
		return CameraEvent::kDragon;
	}

	void CameraDragon::SetData()
	{
		switch (m_DragonState)
		{
			case CameraDragon::State::kMounting:
			case CameraDragon::State::kMounted:
			case CameraDragon::State::kDismounting:
			{
				this->m_Data.EventActive = &m_pluginConfig->Events().bDragonTransition;
				this->m_Data.FOV = &m_pluginConfig->FOV().fDragonTransition;
				this->m_Data.NearDistance = &m_pluginConfig->NearDistance().fDragonTransition;
				break;
			}
			default:
			{
				this->m_Data.EventActive = &m_pluginConfig->Events().bDragon;
				this->m_Data.FOV = &m_pluginConfig->FOV().fDragon;
				this->m_Data.NearDistance = &m_pluginConfig->NearDistance().fDragon;
			}
		}
	}

}
