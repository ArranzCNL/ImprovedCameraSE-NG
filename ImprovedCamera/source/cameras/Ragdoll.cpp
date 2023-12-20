/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

// Precompiled Header
#include "PCH.h"

#include "cameras/Ragdoll.h"

namespace ImprovedCamera {

	CameraRagdoll::CameraRagdoll() :
		ICamera("Ragdoll", RE::CameraStates::kBleedout)
	{
		SetData();
	}

	bool CameraRagdoll::OnUpdate(std::uint8_t currentCamera, std::uint8_t)
	{
		this->Player = RE::PlayerCharacter::GetSingleton();
		// Stopped Ragdolling
		if (currentCamera != RE::CameraStates::kBleedout && m_RagdollState)
		{
			m_RagdollState = CameraRagdoll::State::kStopped;
			return false;
		}
		// Playing Ragdoll
		if (currentCamera == RE::CameraStates::kBleedout && m_RagdollState == CameraRagdoll::State::kStopped)
		{
			m_RagdollState = this->Player->IsDead() ? CameraRagdoll::State::kPlayingDeath : CameraRagdoll::State::kPlaying;
			SetData();
			return true;
		}
		return false;
	}

	bool CameraRagdoll::HasControl()
	{
		if (m_RagdollState)
		{
			return true;
		}
		return false;
	}

	std::uint8_t CameraRagdoll::GetEventID()
	{
		return this->Player->IsDead(1) ? CameraEvent::kDeath : CameraEvent::kRagdoll;
	}

	void CameraRagdoll::SetData()
	{
		if (m_RagdollState == CameraRagdoll::State::kPlayingDeath)
		{
			this->m_Data.EventActive = &m_pluginConfig->Events().bDeath;
			this->m_Data.FOV = &m_pluginConfig->FOV().fDeath;
			this->m_Data.NearDistance = &m_pluginConfig->NearDistance().fDeath;
		}
		else
		{
			this->m_Data.EventActive = &m_pluginConfig->Events().bRagdoll;
			this->m_Data.FOV = &m_pluginConfig->FOV().fRagdoll;
			this->m_Data.NearDistance = &m_pluginConfig->NearDistance().fRagdoll;
		}
	}

}
