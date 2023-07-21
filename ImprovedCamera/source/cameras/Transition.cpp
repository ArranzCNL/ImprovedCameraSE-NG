/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

// Precompiled Header
#include "stdafx.h"

#include "cameras/Transition.h"

namespace ImprovedCamera {

	CameraTransition::CameraTransition() :
		ICamera("Transition", RE::CameraStates::kPCTransition)
	{
		SetData();
	}

	bool CameraTransition::OnUpdate(std::uint8_t currentCamera, std::uint8_t previousCamera)
	{
		this->Player = RE::PlayerCharacter::GetSingleton();
		// Stopped Transition
		if (currentCamera != RE::CameraStates::kPCTransition && m_TransitionState)
		{
			m_TransitionState = CameraTransition::State::kStopped;
			return false;
		}
		// Playing Transition
		if (currentCamera == RE::CameraStates::kPCTransition && m_TransitionState == CameraTransition::State::kStopped)
		{
			auto pluginConfig = DLLMain::Plugin::Get()->Config();
			auto playerControls = RE::PlayerControls::GetSingleton();
			m_TransitionState = CameraTransition::State::kPlaying;
			SetData();

			// Transition for Werewolf transformation
			if (playerControls->data.povBeastMode && !this->Player->IsInKillMove() && pluginConfig->Events().bTransform)
			{
				m_TransitionState = CameraTransition::State::kTransformation;
				SetData();
				// Set fade level
				auto thirdpersonNode = this->Player->Get3D(0)->AsNode();
				thirdpersonNode->fadeAmount = 0.0;
				return true;
			}
			// Werewolf Killmove
			if (playerControls->data.povBeastMode && this->Player->IsInKillMove() && pluginConfig->Events().bKillmove)
			{
				m_TransitionState = CameraTransition::State::kWerewolfKillmove;
				SetData();
				return true;
			}
			// Transition from Furniture (Sawmill)
			if (previousCamera == RE::CameraStates::kFurniture && pluginConfig->Events().bFurniture)
			{
				m_TransitionState = CameraTransition::State::kFurniture;
				SetData();
				return true;
			}
			// Transition from crafting
			if (previousCamera == RE::CameraStates::kThirdPerson)
			{
				auto thirdpersonState = (RE::ThirdPersonState*)RE::PlayerCamera::GetSingleton()->cameraStates[previousCamera].get();

				if (!thirdpersonState->IsInputEventHandlingEnabled() && pluginConfig->Events().bCrafting)
				{
					m_TransitionState = CameraTransition::State::kCrafting;
					SetData();
					return true;
				}
			}
			// Transition from ragdoll after getting back up
			if (previousCamera == RE::CameraStates::kBleedout && pluginConfig->Events().bRagdoll)
			{
				m_TransitionState = CameraTransition::State::kRagdoll;
				SetData();
				return true;
			}
			// Transition from dismounted horse to first/third person
			if (previousCamera == RE::CameraStates::kMount && pluginConfig->Events().bHorse)
			{
				m_TransitionState = CameraTransition::State::kHorse;
				SetData();
				return true;
			}
			// Transition from dismounted dragon to first/third person
			if (previousCamera == RE::CameraStates::kDragon && pluginConfig->Events().bDragon)
			{
				m_TransitionState = CameraTransition::State::kDragon;
				SetData();
				return true;
			}
			// Use this for feeding detection
			if (previousCamera == RE::CameraStates::kFirstPerson && this->Player->GetPlayerRuntimeData().unkBDB.all(RE::PlayerCharacter::FlagBDB::kIsInThirdPersonMode) && pluginConfig->Events().bCannibal)
			{
				m_TransitionState = CameraTransition::State::kCannibal;
				SetData();
				return true;
			}
			return true;
		}
		return false;
	}

	bool CameraTransition::HasControl()
	{
		if (m_TransitionState)
		{
			return true;
		}
		return false;
	}

	std::uint8_t CameraTransition::GetEventID()
	{
		switch (m_TransitionState)
		{
			case CameraTransition::State::kCrafting:
				return CameraEvent::kCrafting;

			case CameraTransition::State::kDragon:
				return CameraEvent::kDragonTransition;

			case CameraTransition::State::kFurniture:
				return CameraEvent::kFurniture;

			case CameraTransition::State::kHorse:
				return CameraEvent::kHorseTransition;

			case CameraTransition::State::kRagdoll:
				return CameraEvent::kRagdoll;

			case CameraTransition::State::kTransformation:
				return CameraEvent::kTransform;

			case CameraTransition::State::kWerewolfKillmove:
				return CameraEvent::kKillmove;

			case CameraTransition::State::kCannibal:
				return CameraEvent::kCannibal;
		}
		return CameraEvent::kScripted;  // Unknown state
	}

	void CameraTransition::SetData()
	{
		switch (m_TransitionState)
		{
			case CameraTransition::State::kCrafting:
			{
				this->m_Data.EventActive = &m_pluginConfig->Events().bCrafting;
				this->m_Data.FOV = &m_pluginConfig->FOV().fCrafting;
				this->m_Data.NearDistance = &m_pluginConfig->NearDistance().fCrafting;
				break;
			}
			case CameraTransition::State::kDragon:
			{
				this->m_Data.EventActive = &m_pluginConfig->Events().bDragonTransition;
				this->m_Data.FOV = &m_pluginConfig->FOV().fDragonTransition;
				this->m_Data.NearDistance = &m_pluginConfig->NearDistance().fDragonTransition;
				break;
			}
			case CameraTransition::State::kFurniture:
			{
				this->m_Data.EventActive = &m_pluginConfig->Events().bFurniture;
				this->m_Data.FOV = &m_pluginConfig->FOV().fFurniture;
				this->m_Data.NearDistance = &m_pluginConfig->NearDistance().fFurniture;
				break;
			}
			case CameraTransition::State::kHorse:
			{
				this->m_Data.EventActive = &m_pluginConfig->Events().bHorseTransition;
				this->m_Data.FOV = &m_pluginConfig->FOV().fHorseTransition;
				this->m_Data.NearDistance = &m_pluginConfig->NearDistance().fHorseTransition;
				break;
			}
			case CameraTransition::State::kRagdoll:
			{
				this->m_Data.EventActive = &m_pluginConfig->Events().bRagdoll;
				this->m_Data.FOV = &m_pluginConfig->FOV().fRagdoll;
				this->m_Data.NearDistance = &m_pluginConfig->NearDistance().fRagdoll;
				break;
			}
			case CameraTransition::State::kTransformation:
			{
				this->m_Data.EventActive = &m_pluginConfig->Events().bTransform;
				this->m_Data.FOV = &m_pluginConfig->FOV().fScripted;
				this->m_Data.NearDistance = &m_pluginConfig->NearDistance().fScripted;
				break;
			}
			case CameraTransition::State::kWerewolfKillmove:
			{
				this->m_Data.EventActive = &m_pluginConfig->Events().bKillmove;
				this->m_Data.FOV = &m_pluginConfig->FOV().fKillmove;
				this->m_Data.NearDistance = &m_pluginConfig->NearDistance().fKillmove;
				break;
			}
			case CameraTransition::State::kCannibal:
			{
				this->m_Data.EventActive = &m_pluginConfig->Events().bCannibal;
				this->m_Data.FOV = &m_pluginConfig->FOV().fCannibal;
				this->m_Data.NearDistance = &m_pluginConfig->NearDistance().fCannibal;
				break;
			}
			default:
			{
				this->m_Data.EventActive = &m_pluginConfig->Events().bScripted;
				this->m_Data.FOV = &m_pluginConfig->FOV().fScripted;
				this->m_Data.NearDistance = &m_pluginConfig->NearDistance().fScripted;
			}
		}
	}

}
