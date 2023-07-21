/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

// Precompiled Header
#include "stdafx.h"

#include "cameras/DeathCinematic.h"

namespace ImprovedCamera {

	CameraDeathCinematic::CameraDeathCinematic() :
		ICamera("DeathCinematic", RE::CameraStates::kVATS)
	{
		SetData();
	}

	bool CameraDeathCinematic::OnUpdate(std::uint8_t currentCamera, std::uint8_t)
	{
		this->Player = RE::PlayerCharacter::GetSingleton();
		// Stopped CinematicDeath
		if (currentCamera != RE::CameraStates::kVATS && m_DeathCinematicState)
		{
			m_DeathCinematicState = CameraDeathCinematic::State::kStopped;
			return false;
		}
		// Playing CinematicDeath
		if (currentCamera == RE::CameraStates::kVATS && m_DeathCinematicState == CameraDeathCinematic::State::kStopped)
		{
			m_DeathCinematicState = CameraDeathCinematic::State::kPlaying;
			return true;
		}
		return false;
	}

	bool CameraDeathCinematic::HasControl()
	{
		if (m_DeathCinematicState)
		{
			return true;
		}
		return false;
	}

	std::uint8_t CameraDeathCinematic::GetEventID()
	{
		return CameraEvent::kKillmove;
	}

	void CameraDeathCinematic::SetData()
	{
		this->m_Data.EventActive = &m_pluginConfig->Events().bKillmove;
		this->m_Data.FOV = &m_pluginConfig->FOV().fKillmove;
		this->m_Data.NearDistance = &m_pluginConfig->NearDistance().fKillmove;
	}

}
