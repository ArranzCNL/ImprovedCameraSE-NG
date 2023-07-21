/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

// Precompiled Header
#include "stdafx.h"

#include "cameras/Furniture.h"

namespace ImprovedCamera {

	CameraFurniture::CameraFurniture() :
		ICamera("Furniture", RE::CameraStates::kFurniture)
	{
		SetData();
	}

	bool CameraFurniture::OnUpdate(std::uint8_t currentCamera, std::uint8_t)
	{
		this->Player = RE::PlayerCharacter::GetSingleton();
		// Capture Furniture State.
		// Sawmill:       kCameraState_Furniture to kCameraState_Transition to kCameraState_ThirdPerson1
		// The rest do:   kCameraState_Furniture
		bool wakeUp = false;
		auto actorState = this->Player->AsActorState();
		if (actorState->GetSitSleepState() == RE::SIT_SLEEP_STATE::kWantToWake && m_FurnitureState != CameraFurniture::State::kStopping)
			wakeUp = true;

		// Exited Furniture
		if (currentCamera != RE::CameraStates::kFurniture && m_FurnitureState && !Helper::IsInteracting(this->Player))
		{
			m_FurnitureState = CameraFurniture::State::kStopped;
			return false;
		}
		// Unequipping weapon
		if (currentCamera == RE::CameraStates::kFurniture && m_FurnitureState == CameraFurniture::State::kStopped && this->Player->AsActorState()->IsWeaponDrawn())
		{
			m_FurnitureState = CameraFurniture::State::kUnequip;
			return true;
		}
		// Entering Furniture
		if (currentCamera == RE::CameraStates::kFurniture && (m_FurnitureState == CameraFurniture::State::kStopped || m_FurnitureState == CameraFurniture::State::kUnequip) && Helper::IsInteractionEnter(this->Player))
		{
			m_FurnitureState = CameraFurniture::State::kPlaying;
			return true;
		}
		// Using Furniture
		if (currentCamera != RE::CameraStates::kFurniture && m_FurnitureState == CameraFurniture::State::kPlaying && Helper::IsInteracting(this->Player))
		{
			// Sawmill fix
			if (currentCamera == RE::CameraStates::kPCTransition)
			{
				m_FurnitureState = CameraFurniture::State::kStopped;
				return false;
			}
			m_FurnitureState = CameraFurniture::State::kIdle;
			return true;
		}
		// Exiting Furniture
		if (currentCamera == RE::CameraStates::kFurniture && (m_FurnitureState == CameraFurniture::State::kIdle && Helper::IsInteractionLeave(this->Player) || wakeUp))
		{
			m_FurnitureState = CameraFurniture::State::kStopping;
			if (wakeUp)
			{
				auto controlMap = RE::ControlMap::GetSingleton();
				controlMap->enabledControls.set(RE::UserEvents::USER_EVENT_FLAG::kLooking);  // Unblock Looking
			}
			return true;
		}
		return false;
	}

	bool CameraFurniture::HasControl()
	{
		if (m_FurnitureState)
		{
			return true;
		}
		return false;
	}

	std::uint8_t CameraFurniture::GetEventID()
	{
		return CameraEvent::kFurniture;
	}

	void CameraFurniture::SetData()
	{
		this->m_Data.EventActive = &m_pluginConfig->Events().bFurniture;
		this->m_Data.FOV = &m_pluginConfig->FOV().fFurniture;
		this->m_Data.NearDistance = &m_pluginConfig->NearDistance().fFurniture;
	}

}
