/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

// Precompiled Header
#include "stdafx.h"

#include "cameras/FirstPerson.h"

#include "skyrimse/Addresses.h"
#include "utils/ICMath.h"

namespace ImprovedCamera {

	using namespace Address::Variable;

	CameraFirstPerson::CameraFirstPerson() :
		ICamera("FirstPerson", RE::CameraStates::kFirstPerson)
	{
		SetData();
	}

	bool CameraFirstPerson::OnUpdate(std::uint8_t currentCamera, std::uint8_t)
	{
		this->Player = RE::PlayerCharacter::GetSingleton();

		// Exited First Person
		if (currentCamera != RE::CameraStates::kFirstPerson && m_FirstPersonState)
		{
			m_FirstPersonState = CameraFirstPerson::State::kExit;
			return false;
		}
		// First Person
		if (currentCamera == RE::CameraStates::kFirstPerson && m_FirstPersonState == CameraFirstPerson::State::kExit)
		{
			m_FirstPersonState = this->Player->AsActorState()->IsWeaponDrawn() ? CameraFirstPerson::State::kWeaponDrawnEnter : CameraFirstPerson::State::kEnter;
			SetData();

			// Check for sitting
			if (Helper::IsSitting(this->Player) && !Helper::IsScripted() && Helper::CanLook() && Helper::CanMove())
			{
				m_FirstPersonState = CameraFirstPerson::State::kSittingEnter;
				SetData();
				return true;
			}
			// Check for Cartride
			if (Helper::IsOnCart())
			{
				m_FirstPersonState = CameraFirstPerson::State::kCartRideEnter;
				SetData();
				return true;
			}
			// Check for looking around only
			if (Helper::IsScripted() && Helper::CanLook() && !Helper::CanMove())
			{
				m_FirstPersonState = CameraFirstPerson::State::kLookOnlyEnter;
				SetData();
				return true;
			}
			// Check for animation
			if (Helper::IsScripted() && Helper::CannotMoveAndLook())
			{
				auto leftHand = this->Player->GetEquippedObject(true);
				auto rightHand = this->Player->GetEquippedObject(false);
				// Check for Fishing
				if ((leftHand && strcmp(leftHand->GetName(), "Fishing Rod") == 0) || (rightHand && strcmp(rightHand->GetName(), "Fishing Rod") == 0))
				{
					m_FirstPersonState = CameraFirstPerson::State::kFishingEnter;
					SetData();
					return true;
				}
				m_FirstPersonState = CameraFirstPerson::State::kAnimationEnter;
				SetData();
				return true;
			}
			// Check for scripted
			if (Helper::IsScripted())
			{
				m_FirstPersonState = CameraFirstPerson::State::kScriptedEnter;
				SetData();
				return true;
			}
			return true;
		}
		if (currentCamera == RE::CameraStates::kFirstPerson)
		{
			// Normal First Person Entered
			if (m_FirstPersonState == CameraFirstPerson::State::kEnter)
			{
				m_FirstPersonState = CameraFirstPerson::State::kIdle;
				return true;
			}
			// Weapon Drawn Entered
			if (m_FirstPersonState == CameraFirstPerson::State::kWeaponDrawnEnter)
			{
				m_FirstPersonState = CameraFirstPerson::State::kWeaponDrawnIdle;
				return true;
			}
			// Sitting Entered
			if (m_FirstPersonState == CameraFirstPerson::State::kSittingEnter)
			{
				m_FirstPersonState = CameraFirstPerson::State::kSittingIdle;
				return true;
			}
			// Cartride Entered
			if (m_FirstPersonState == CameraFirstPerson::State::kCartRideEnter)
			{
				m_FirstPersonState = CameraFirstPerson::State::kCartRideIdle;
				auto thirdpersonNode = this->Player->Get3D(0)->AsNode();
				thirdpersonNode->GetFlags().reset(RE::NiAVObject::Flag::kHidden);
				return true;
			}
			// Cartride Dismounting
			if (m_FirstPersonState == CameraFirstPerson::State::kCartRideIdle && Helper::CannotMoveAndLook())
			{
				m_FirstPersonState = CameraFirstPerson::State::kCartRideDismounting;
				return true;
			}
			// Cartride Dismounted
			if (m_FirstPersonState == CameraFirstPerson::State::kCartRideDismounting && !Helper::CannotMoveAndLook())
			{
				m_FirstPersonState = CameraFirstPerson::State::kExit;
				return false;
			}
			// Looking around only Entered
			if (m_FirstPersonState == CameraFirstPerson::State::kLookOnlyEnter)
			{
				m_FirstPersonState = CameraFirstPerson::State::kLookOnlyIdle;
				return true;
			}
			// Fishing Entered
			if (m_FirstPersonState == CameraFirstPerson::State::kFishingEnter)
			{
				m_FirstPersonState = CameraFirstPerson::State::kFishingIdle;
				return true;
			}
			// Animation Entered
			if (m_FirstPersonState == CameraFirstPerson::State::kAnimationEnter)
			{
				m_FirstPersonState = CameraFirstPerson::State::kAnimationIdle;
				return true;
			}
			// Scripted Entered
			if (m_FirstPersonState == CameraFirstPerson::State::kScriptedEnter)
			{
				m_FirstPersonState = CameraFirstPerson::State::kScriptedIdle;
				return true;
			}
		}
		return false;
	}

	bool CameraFirstPerson::HasControl()
	{
		if (m_FirstPersonState)
		{
			// Sitting and Weapon Drawn detection
			if (m_FirstPersonState == CameraFirstPerson::State::kIdle || m_FirstPersonState == CameraFirstPerson::State::kWeaponDrawnIdle)
			{
				if (m_FirstPersonState == CameraFirstPerson::State::kIdle && this->Player->AsActorState()->IsWeaponDrawn() ||
					m_FirstPersonState == CameraFirstPerson::State::kWeaponDrawnIdle && !this->Player->AsActorState()->IsWeaponDrawn())
				{
					m_FirstPersonState = CameraFirstPerson::State::kExit;
					return true;
				}
				// Force a reset we are sitting
				if (Helper::IsSitting(this->Player) && !Helper::IsScripted() && Helper::CanLook() && Helper::CanMove())
				{
					m_FirstPersonState = CameraFirstPerson::State::kExit;
					return true;
				}
				// Force a reset can only look around
				if (Helper::IsScripted() && Helper::CanLook() && !Helper::CanMove())
				{
					m_FirstPersonState = CameraFirstPerson::State::kExit;
					return true;
				}
				// Force a reset animation is running
				if (Helper::IsScripted() && Helper::CannotMoveAndLook())
				{
					m_FirstPersonState = CameraFirstPerson::State::kExit;
					return true;
				}
				// Force a reset scripted is running
				if (Helper::IsScripted())
				{
					m_FirstPersonState = CameraFirstPerson::State::kExit;
					return true;
				}
				return true;
			}
			// Change sitting angles
			if (m_FirstPersonState == CameraFirstPerson::State::kSittingIdle ||
				m_FirstPersonState == CameraFirstPerson::State::kCartRideIdle ||
				m_FirstPersonState == CameraFirstPerson::State::kLookOnlyIdle ||
				m_FirstPersonState == CameraFirstPerson::State::kScriptedIdle)
			{
				// Sanity check getting off the cartride still triggers IsSitting for a moment
				if (Helper::IsSitting(this->Player) && (!Helper::CanLook() || !Helper::CanMove()) &&
					m_FirstPersonState != CameraFirstPerson::State::kScriptedIdle &&
					m_FirstPersonState != CameraFirstPerson::State::kCartRideIdle)
				{
					m_FirstPersonState = CameraFirstPerson::State::kExit;
				}
				auto firstpersonState = (RE::FirstPersonState*)RE::PlayerCamera::GetSingleton()->currentState.get();
				float sittingMaxAngle = m_pluginConfig->RestrictAngles().fSitting * (M_PI / 180.0f);  // Degrees to Radians

				if (firstpersonState->sittingRotation >= sittingMaxAngle)
					firstpersonState->sittingRotation = sittingMaxAngle;

				if (firstpersonState->sittingRotation <= -sittingMaxAngle)
					firstpersonState->sittingRotation = -sittingMaxAngle;

				float sittingMaxLookingUp = m_pluginConfig->RestrictAngles().fSittingMaxLookingUp * (M_PI / 180.0f);  // Degrees to Radians
				if (this->Player->data.angle.x <= -sittingMaxLookingUp)
					this->Player->data.angle.x = -sittingMaxLookingUp;

				*fSittingMaxLookingDown = m_pluginConfig->RestrictAngles().fSittingMaxLookingDown;
			}
			// Looking only change detection
			if (m_FirstPersonState == CameraFirstPerson::State::kLookOnlyIdle && (!Helper::CanLook() || Helper::CanMove() || Helper::IsOnCart()))
			{
				m_FirstPersonState = CameraFirstPerson::State::kExit;
				return true;
			}
			// Fishing change detection
			if (m_FirstPersonState == CameraFirstPerson::State::kFishingIdle && !Helper::CannotMoveAndLook())
			{
				m_FirstPersonState = CameraFirstPerson::State::kExit;
				return true;
			}
			// Animation change detection
			if (m_FirstPersonState == CameraFirstPerson::State::kAnimationIdle && !Helper::CannotMoveAndLook())
			{
				m_FirstPersonState = CameraFirstPerson::State::kExit;
				return true;
			}
			// Scripted change detection
			if (m_FirstPersonState == CameraFirstPerson::State::kScriptedIdle && (!Helper::IsScripted() || Helper::CannotMoveAndLook()))
			{
				m_FirstPersonState = CameraFirstPerson::State::kExit;
				return true;
			}
			return true;
		}
		return false;
	}

	std::uint8_t CameraFirstPerson::GetEventID()
	{
		switch (m_FirstPersonState)
		{
			case CameraFirstPerson::State::kWeaponDrawnEnter:
			case CameraFirstPerson::State::kWeaponDrawnIdle:
				return CameraEvent::kFirstPersonCombat;
		}
		return CameraEvent::kFirstPerson;
	}

	void CameraFirstPerson::SetData()
	{
		switch (m_FirstPersonState)
		{
			case CameraFirstPerson::State::kWeaponDrawnEnter:
			case CameraFirstPerson::State::kWeaponDrawnIdle:
			{
				this->m_Data.EventActive = &m_pluginConfig->Events().bFirstPersonCombat;
				this->m_Data.FOV = &m_pluginConfig->FOV().fFirstPersonCombat;
				this->m_Data.NearDistance = &m_pluginConfig->NearDistance().fFirstPersonCombat;
				break;
			}
			default:
			{
				this->m_Data.EventActive = &m_pluginConfig->Events().bFirstPerson;
				this->m_Data.FOV = &m_pluginConfig->FOV().fFirstPerson;
				this->m_Data.NearDistance = &m_pluginConfig->NearDistance().fFirstPerson;
			}
		}
	}

}
