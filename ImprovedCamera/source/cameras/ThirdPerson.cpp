/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

// Precompiled Header
#include "stdafx.h"

#include "cameras/ThirdPerson.h"

#include "skyrimse/Addresses.h"
#include "skyrimse/ImprovedCameraSE.h"
#include "utils/ICMath.h"

namespace ImprovedCamera {

	using namespace Address::Variable;

	CameraThirdPerson::CameraThirdPerson() :
		ICamera("ThirdPerson", RE::CameraStates::kThirdPerson)
	{
		SetData();
	}

	bool CameraThirdPerson::OnUpdate(std::uint8_t currentCamera, std::uint8_t)
	{
		this->Player = RE::PlayerCharacter::GetSingleton();
		auto controlMap = RE::ControlMap::GetSingleton();

		// Exited Third Person
		if (currentCamera != RE::CameraStates::kThirdPerson && m_ThirdPersonState)
		{
			m_ThirdPersonState = CameraThirdPerson::State::kExit;
			return false;
		}
		// Entered Third Person
		if (currentCamera == RE::CameraStates::kThirdPerson && m_ThirdPersonState == CameraThirdPerson::State::kExit)
		{
			m_ThirdPersonState = this->Player->AsActorState()->IsWeaponDrawn() ? CameraThirdPerson::State::kWeaponDrawnEnter : CameraThirdPerson::State::kEnter;
			SetData();
			// Sleeping check
			if (Helper::IsSleeping(this->Player))
			{
				m_ThirdPersonState = CameraThirdPerson::State::kSleepingEnter;
				return true;
			}
			// Crafting check
			auto thirdpersonState = (RE::ThirdPersonState*)RE::PlayerCamera::GetSingleton()->currentState.get();
			if (!thirdpersonState->IsInputEventHandlingEnabled())
			{
				m_ThirdPersonState = CameraThirdPerson::State::kCraftingEnter;
				SetData();
				return true;
			}
			// VampireLord check
			bool vampireLordCheck = Helper::IsTransformed(this->Player, "DLC1VampireBeastRace");
			if (vampireLordCheck && this->Player->IsInKillMove())
			{
				m_ThirdPersonState = CameraThirdPerson::State::kVampireLordKillmove;
				SetData();
				return true;
			}
			if (vampireLordCheck)
			{
				m_ThirdPersonState = CameraThirdPerson::State::kVampireLordEnter;
				SetData();
				return true;
			}
			// Werewolf check
			bool werewolfCheck = Helper::IsTransformed(this->Player, "WerewolfBeastRace");
			if (werewolfCheck)
			{
				m_ThirdPersonState = CameraThirdPerson::State::kWerewolfEnter;
				SetData();
				return true;
			}
			// NecroLich check
			bool necroLichCheck = Helper::IsTransformed(this->Player, "NecroLichRace");
			if (necroLichCheck)
			{
				m_ThirdPersonState = CameraThirdPerson::State::kNecroLichEnter;
				SetData();
				return true;
			}
			// Death Cinematic check
			if (this->Player->IsInKillMove())
			{
				m_ThirdPersonState = CameraThirdPerson::State::kDeathCinematicEnter;
				SetData();
				return true;
			}
			// Third party animations
			if (Helper::IsScripted() || !controlMap->IsMovementControlsEnabled())
			{
				m_ThirdPersonState = CameraThirdPerson::State::kScriptedEnter;
				SetData();
				return true;
			}
			// Animation detection
			if (Helper::IsIdlePlaying(this->Player) && !Helper::IsSittingOrSleeping(this->Player))
			{
				m_ThirdPersonState = CameraThirdPerson::State::kAnimationEnter;
				SetData();
				return true;
			}
			return true;
		}
		if (currentCamera == RE::CameraStates::kThirdPerson)
		{
			// Normal Third Person Entered
			if (m_ThirdPersonState == CameraThirdPerson::State::kEnter)
			{
				m_ThirdPersonState = CameraThirdPerson::State::kIdle;
				return true;
			}
			// Weapon Drawn Entered
			if (m_ThirdPersonState == CameraThirdPerson::State::kWeaponDrawnEnter)
			{
				m_ThirdPersonState = CameraThirdPerson::State::kWeaponDrawnIdle;
				return true;
			}
			// Sleeping Entered
			if (m_ThirdPersonState == CameraThirdPerson::State::kSleepingEnter)
			{
				m_ThirdPersonState = CameraThirdPerson::State::kSleepingIdle;
				controlMap->enabledControls.set(RE::UserEvents::USER_EVENT_FLAG::kLooking);  // Allow Looking
				return true;
			}
			// Crafting Entered
			if (m_ThirdPersonState == CameraThirdPerson::State::kCraftingEnter)
			{
				m_ThirdPersonState = CameraThirdPerson::State::kCraftingIdle;
				return true;
			}
			// Death Cinematic Entered
			if (m_ThirdPersonState == CameraThirdPerson::State::kDeathCinematicEnter)
			{
				m_ThirdPersonState = CameraThirdPerson::State::kDeathCinematicIdle;
				return true;
			}
			// Scripted Entered
			if (m_ThirdPersonState == CameraThirdPerson::State::kScriptedEnter)
			{
				m_ThirdPersonState = CameraThirdPerson::State::kScriptedIdle;
				return true;
			}
			// Animation Entered
			if (m_ThirdPersonState == CameraThirdPerson::State::kAnimationEnter)
			{
				m_ThirdPersonState = CameraThirdPerson::State::kAnimationIdle;
				controlMap->enabledControls.set(RE::UserEvents::USER_EVENT_FLAG::kPOVSwitch);  // Allow POV
				return true;
			}
			// VampireLord Entered
			if (m_ThirdPersonState == CameraThirdPerson::State::kVampireLordEnter)
			{
				m_ThirdPersonState = CameraThirdPerson::State::kVampireLordIdle;
				return true;
			}
			// Werewolf Entered
			if (m_ThirdPersonState == CameraThirdPerson::State::kWerewolfEnter)
			{
				m_ThirdPersonState = CameraThirdPerson::State::kWerewolfIdle;
				return true;
			}
			// NecroLich Entered
			if (m_ThirdPersonState == CameraThirdPerson::State::kNecroLichEnter)
			{
				m_ThirdPersonState = CameraThirdPerson::State::kNecroLichIdle;
				return true;
			}
		}
		return false;
	}

	bool CameraThirdPerson::HasControl()
	{
		auto pluginCamera = DLLMain::Plugin::Get()->SkyrimSE()->Camera();

		auto thirdpersonState = (RE::ThirdPersonState*)RE::PlayerCamera::GetSingleton()->currentState.get();
		auto controlMap = RE::ControlMap::GetSingleton();
		auto playerControls = RE::PlayerControls::GetSingleton();

		float zoom = *fMinCurrentZoom + m_pluginConfig->Fixes().fSwitchPOVDetectDistance;
		bool isFirstPerson = pluginCamera->IsFirstPerson();

		if (m_ThirdPersonState)
		{
			if (m_ThirdPersonState == CameraThirdPerson::State::kIdle || m_ThirdPersonState == CameraThirdPerson::State::kWeaponDrawnIdle)
			{
				// Weapon Drawn detection
				if (m_ThirdPersonState == CameraThirdPerson::State::kIdle && this->Player->AsActorState()->IsWeaponDrawn() ||
					m_ThirdPersonState == CameraThirdPerson::State::kWeaponDrawnIdle && !this->Player->AsActorState()->IsWeaponDrawn())
				{
					m_ThirdPersonState = CameraThirdPerson::State::kExit;
					return true;
				}
				// Crafting detection
				if (!thirdpersonState->IsInputEventHandlingEnabled())
				{
					m_ThirdPersonState = CameraThirdPerson::State::kExit;
					return true;
				}
				// Death Cinematic detection
				if (this->Player->IsInKillMove())
				{
					m_ThirdPersonState = CameraThirdPerson::State::kExit;
					return true;
				}
				// Scripted detection
				if (Helper::IsScripted() || !controlMap->IsMovementControlsEnabled())
				{
					m_ThirdPersonState = CameraThirdPerson::State::kExit;
					return true;
				}
				// VampireLord detection
				bool vampireLordCheck = Helper::IsTransformed(this->Player, "DLC1VampireBeastRace");
				if (vampireLordCheck)
				{
					m_ThirdPersonState = CameraThirdPerson::State::kExit;
					return true;
				}
				// Werewolf detection
				bool werewolfCheck = Helper::IsTransformed(this->Player, "WerewolfBeastRace");
				if (werewolfCheck)
				{
					m_ThirdPersonState = CameraThirdPerson::State::kExit;
					return true;
				}
				// NecroLich detection
				bool necroLichCheck = Helper::IsTransformed(this->Player, "NecroLichRace");
				if (necroLichCheck)
				{
					m_ThirdPersonState = CameraThirdPerson::State::kExit;
					return true;
				}
				// Animation detection
				if (Helper::IsIdlePlaying(this->Player) && !Helper::IsSittingOrSleeping(this->Player))
				{
					m_ThirdPersonState = CameraThirdPerson::State::kExit;
					return true;
				}
				return true;
			}
			// Angle Restriction Scripted
			if (m_pluginConfig->Events().bScripted &&
				(m_ThirdPersonState == CameraThirdPerson::State::kScriptedIdle ||
				m_ThirdPersonState == CameraThirdPerson::State::kAnimationIdle))
			{
				if (isFirstPerson && thirdpersonState->currentZoomOffset <= zoom)
				{
					float scriptedMaxAngle = m_pluginConfig->RestrictAngles().fScripted * (M_PI / 180.0f);  // Degrees to Radians;

					if (thirdpersonState->freeRotation.x >= scriptedMaxAngle)
						thirdpersonState->freeRotation.x = scriptedMaxAngle;

					if (thirdpersonState->freeRotation.x <= -scriptedMaxAngle)
						thirdpersonState->freeRotation.x = -scriptedMaxAngle;

					float scriptedMaxPitch = m_pluginConfig->RestrictAngles().fScriptedPitch * (M_PI / 180.0f);  // Degrees to Radians

					if (thirdpersonState->freeRotation.y >= scriptedMaxPitch)
						thirdpersonState->freeRotation.y = scriptedMaxPitch;

					if (thirdpersonState->freeRotation.y <= -scriptedMaxPitch)
						thirdpersonState->freeRotation.y = -scriptedMaxPitch;
				}
			}
			// Angle Restriction VampireLord
			if (m_pluginConfig->Events().bVampireLord && m_ThirdPersonState == CameraThirdPerson::State::kVampireLordIdle)
			{
				if (this->Player->IsInKillMove())
				{
					m_ThirdPersonState = CameraThirdPerson::State::kExit;
					return true;
				}
				if (isFirstPerson && thirdpersonState->currentZoomOffset <= zoom)
				{
					float vampireLordMaxAngle = m_pluginConfig->RestrictAngles().fVampireLord * (M_PI / 180.0f);  // Degrees to Radians;

					if (thirdpersonState->freeRotation.x >= vampireLordMaxAngle)
						thirdpersonState->freeRotation.x = vampireLordMaxAngle;

					if (thirdpersonState->freeRotation.x <= -vampireLordMaxAngle)
						thirdpersonState->freeRotation.x = -vampireLordMaxAngle;
				}
			}
			// Angle Restriction Werewolf
			if (m_pluginConfig->Events().bWerewolf && m_ThirdPersonState == CameraThirdPerson::State::kWerewolfIdle)
			{
				if (isFirstPerson && thirdpersonState->currentZoomOffset <= zoom)
				{
					float werewolfMaxAngle = m_pluginConfig->RestrictAngles().fWerewolf * (M_PI / 180.0f);  // Degrees to Radians;

					if (thirdpersonState->freeRotation.x >= werewolfMaxAngle)
						thirdpersonState->freeRotation.x = werewolfMaxAngle;

					if (thirdpersonState->freeRotation.x <= -werewolfMaxAngle)
						thirdpersonState->freeRotation.x = -werewolfMaxAngle;
				}
			}
			// Angle Restriction NecroLich
			if (m_pluginConfig->Events().bNecroLich && m_ThirdPersonState == CameraThirdPerson::State::kNecroLichIdle)
			{
				if (isFirstPerson && thirdpersonState->currentZoomOffset <= zoom)
				{
					float necroLichMaxAngle = m_pluginConfig->RestrictAngles().fNecroLich * (M_PI / 180.0f);  // Degrees to Radians;

					if (thirdpersonState->freeRotation.x >= necroLichMaxAngle)
						thirdpersonState->freeRotation.x = necroLichMaxAngle;

					if (thirdpersonState->freeRotation.x <= -necroLichMaxAngle)
						thirdpersonState->freeRotation.x = -necroLichMaxAngle;
				}
			}
			// Exit Crafting detection
			if (m_ThirdPersonState == CameraThirdPerson::State::kCraftingIdle && thirdpersonState->IsInputEventHandlingEnabled())
			{
				m_ThirdPersonState = CameraThirdPerson::State::kExit;
				return true;
			}
			// Exit Scripted detection
			if (m_ThirdPersonState == CameraThirdPerson::State::kScriptedIdle && !Helper::IsScripted() && controlMap->IsMovementControlsEnabled())
			{
				m_ThirdPersonState = CameraThirdPerson::State::kExit;
				return true;
			}
			// Exit Animation detection
			if (m_ThirdPersonState == CameraThirdPerson::State::kAnimationIdle && !Helper::IsIdlePlaying(this->Player))
			{
				m_ThirdPersonState = CameraThirdPerson::State::kExit;
				return true;
			}
			// Exit VampireLord Killmove
			if (m_ThirdPersonState == CameraThirdPerson::State::kVampireLordKillmove && !this->Player->IsInKillMove())
			{
				m_ThirdPersonState = CameraThirdPerson::State::kExit;
				return true;
			}
			// Exit VampireLord/Werewolf/NecroLich detection
			if (m_ThirdPersonState >= CameraThirdPerson::State::kVampireLordEnter && !playerControls->data.povBeastMode)
			{
				m_ThirdPersonState = CameraThirdPerson::State::kExit;
				return true;
			}
			return true;
		}
		return false;
	}

	std::uint8_t CameraThirdPerson::GetEventID()
	{
		switch (m_ThirdPersonState)
		{
			case CameraThirdPerson::State::kCraftingEnter:
			case CameraThirdPerson::State::kCraftingIdle:
				return CameraEvent::kCrafting;

			case CameraThirdPerson::State::kDeathCinematicEnter:
			case CameraThirdPerson::State::kDeathCinematicIdle:
				return CameraEvent::kDeath;

			case CameraThirdPerson::State::kScriptedEnter:
			case CameraThirdPerson::State::kScriptedIdle:
			case CameraThirdPerson::State::kAnimationEnter:
			case CameraThirdPerson::State::kAnimationIdle:
				return CameraEvent::kScripted;

			case CameraThirdPerson::State::kVampireLordKillmove:
				return CameraEvent::kKillmove;

			case CameraThirdPerson::State::kVampireLordEnter:
			case CameraThirdPerson::State::kVampireLordIdle:
				return CameraEvent::kVampireLord;

			case CameraThirdPerson::State::kWerewolfEnter:
			case CameraThirdPerson::State::kWerewolfIdle:
				return CameraEvent::kWerewolf;

			case CameraThirdPerson::State::kNecroLichEnter:
			case CameraThirdPerson::State::kNecroLichIdle:
				return CameraEvent::kNecroLich;
		}
		return CameraEvent::kThirdPerson;
	}

	void CameraThirdPerson::SetData()
	{
		switch (m_ThirdPersonState)
		{
			case CameraThirdPerson::State::kCraftingEnter:
			case CameraThirdPerson::State::kCraftingIdle:
			{
				this->m_Data.EventActive = &m_pluginConfig->Events().bCrafting;
				this->m_Data.FOV = &m_pluginConfig->FOV().fCrafting;
				this->m_Data.NearDistance = &m_pluginConfig->NearDistance().fCrafting;
				break;
			}
			case CameraThirdPerson::State::kDeathCinematicEnter:
			case CameraThirdPerson::State::kDeathCinematicIdle:
			{
				this->m_Data.EventActive = &m_pluginConfig->Events().bDeath;
				this->m_Data.FOV = &m_pluginConfig->FOV().fDeath;
				this->m_Data.NearDistance = &m_pluginConfig->NearDistance().fDeath;
				break;
			}
			case CameraThirdPerson::State::kScriptedEnter:
			case CameraThirdPerson::State::kScriptedIdle:
			case CameraThirdPerson::State::kAnimationEnter:
			case CameraThirdPerson::State::kAnimationIdle:
			{
				this->m_Data.EventActive = &m_pluginConfig->Events().bScripted;
				this->m_Data.FOV = &m_pluginConfig->FOV().fScripted;
				this->m_Data.NearDistance = &m_pluginConfig->NearDistance().fScripted;
				break;
			}
			case CameraThirdPerson::State::kVampireLordKillmove:
			{
				this->m_Data.EventActive = &m_pluginConfig->Events().bKillmove;
				this->m_Data.FOV = &m_pluginConfig->FOV().fKillmove;
				this->m_Data.NearDistance = &m_pluginConfig->NearDistance().fKillmove;
				break;
			}
			case CameraThirdPerson::State::kVampireLordEnter:
			case CameraThirdPerson::State::kVampireLordIdle:
			{
				this->m_Data.EventActive = &m_pluginConfig->Events().bVampireLord;
				this->m_Data.FOV = &m_pluginConfig->FOV().fVampireLord;
				this->m_Data.NearDistance = &m_pluginConfig->NearDistance().fVampireLord;
				break;
			}
			case CameraThirdPerson::State::kWerewolfEnter:
			case CameraThirdPerson::State::kWerewolfIdle:
			{
				this->m_Data.EventActive = &m_pluginConfig->Events().bWerewolf;
				this->m_Data.FOV = &m_pluginConfig->FOV().fWerewolf;
				this->m_Data.NearDistance = &m_pluginConfig->NearDistance().fWerewolf;
				break;
			}
			case CameraThirdPerson::State::kNecroLichEnter:
			case CameraThirdPerson::State::kNecroLichIdle:
			{
				this->m_Data.EventActive = &m_pluginConfig->Events().bNecroLich;
				this->m_Data.FOV = &m_pluginConfig->FOV().fNecroLich;
				this->m_Data.NearDistance = &m_pluginConfig->NearDistance().fNecroLich;
				break;
			}
			default:
			{
				this->m_Data.EventActive = &m_pluginConfig->Events().bThirdPerson;
				this->m_Data.FOV = &m_pluginConfig->FOV().fThirdPerson;
				this->m_Data.NearDistance = &m_pluginConfig->NearDistance().fThirdPerson;
			}
		}
	}

}
