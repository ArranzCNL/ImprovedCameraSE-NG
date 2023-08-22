/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "plugin.h"
#include "skyrimse/Addresses.h"
#include <RE/Skyrim.h>

namespace RE {

	struct WEAPON_IDS {

		enum WEAPON_ID : std::int8_t
		{
			kFist = 0,
			kSword,
			kDagger,
			kAxe,
			kMace,
			kGreatsword,
			kWarhammer,
			kBattleaxe = kWarhammer,
			kBow,
			kStaff,
			kCrossbow,

			kTotal = 10
		};
	};
	using WEAPON_ID = WEAPON_IDS::WEAPON_ID;

}

namespace Helper {

	static inline void* GetVTableAddress(void* object, std::uint32_t address)
	{
		std::uint64_t* vtbl = (std::uint64_t*)*(std::uint64_t*)object;
		return (void*)(vtbl[address / 8]);
	}
	// Easier to work with than GetObjectByName
	static inline RE::NiNode* FindNode(RE::NiNode* node, const char* name)
	{
		if (!node)
			return nullptr;

		if (strcmp(node->name.c_str(), name) == 0)
			return node;

		for (auto it = node->children.begin(); it != node->children.end(); ++it)
		{
			// Need this check as some containers can be null.
			if (it->get() == nullptr)
				continue;

			auto findNode = FindNode(it->get()->AsNode(), name);
			if (findNode)
				return findNode;
		}
		return nullptr;
	}

	static inline void UpdateNode(RE::NiNode* node, SKSE::stl::enumeration<RE::NiUpdateData::Flag, std::uint32_t> flags = RE::NiUpdateData::Flag::kNone, float updateTime = 0.f)
	{
		auto updateData = RE::NiUpdateData();
		updateData.flags = flags;
		updateData.time = updateTime;
		node->Update(updateData);
	}

	static inline RE::NiNode* GetHeadNode(RE::NiNode* node)
	{
		const char* headNames[] = {
			"NPC Head [Head]",
			"NPC Head",
			"Bip01 Head",
			"Boar_Reikling_Head",
			"Boar_Head",
			"Canine_Head",
			"ChaurusFlyerHead",
			"DragPriestNPC Head [Head]",
			"DwarvenSpiderHead_XYZ",
			"ElkScull",
			"FireAtronach_Head [Head]",
			"Goat_Head",
			"HEAD",
			"Head [Head]",
			"Horker_Head01",
			"HorseScull",
			"IW Head",
			"MainBody",
			"Mammoth Head",
			"Mcrab_Body",
			"NetchPelvis [Pelv]",
			"NPC M HeadNoodle01",
			"RabbitHead",
			"Sabrecat_Head [Head]",
			"Scull",
			"SlaughterfishHead",
			"Torso Main",
			"Wisp Head",
			"NPC UpperLid",
		};

		for (const char* headName : headNames)
		{
			auto headNode = FindNode(node, headName);
			if (headNode)
				return headNode;
		}
		return nullptr;
	}

	static inline bool CorrectFurnitureIdle()
	{
		auto player = RE::PlayerCharacter::GetSingleton();
		auto furnitureIdle = player->GetActorRuntimeData().currentProcess->middleHigh->furnitureIdle;

		if (furnitureIdle)
		{
			const char* parentIdle = furnitureIdle->parentIdle->formEditorID.c_str();
			// Common furniture idles which should never be used in first person
			const char* idleNames[] = {
				"EnterBed",
				"IdleJarlChair",
				"IdleGreybeardMeditate",
				"RailLeanRoot",
				"WallLeanRoot",
				"IdleSoldierWall",
				"CounterRoot",
				"LeanTableRoot",
				"SitLedgeRoot",
				"SitCrossLeggedRoot",
				"IdleKneeling",
				"LayDownRoot",
				"IdleCartTravelDriver",
			};

			for (const char* idleName : idleNames)
			{
				if (strcmp(idleName, parentIdle) == 0)
					return true;
			}
		}
		return false;
	}

	static inline bool CanLook()
	{
		auto controlMap = RE::ControlMap::GetSingleton();
		return controlMap->IsLookingControlsEnabled();
	}

	static inline bool CanMove()
	{
		auto controlMap = RE::ControlMap::GetSingleton();
		return controlMap->IsMovementControlsEnabled();
	}

	static inline bool CannotMoveAndLook()
	{
		return !CanMove() && !CanLook();
	}

	static inline bool IsOnMount(RE::PlayerCharacter* player)
	{
		return player->IsOnMount();
	}

	static inline bool IsScripted()
	{
		auto playerControls = RE::PlayerControls::GetSingleton();
		return playerControls->data.povScriptMode;
	}

	static inline bool IsTransformed(RE::PlayerCharacter* player, const char* raceName)
	{
		if (!player || !raceName)
			return false;

		const char* race = player->GetRace()->formEditorID.c_str();
		return (race && strcmp(race, raceName) == 0);
	}

	static inline bool IsSitting(RE::PlayerCharacter* player)
	{
		auto actorState = player->AsActorState();
		if (actorState->GetSitSleepState() == RE::SIT_SLEEP_STATE::kIsSitting)
			return true;

		return false;
	}

	static inline bool IsSleeping(RE::PlayerCharacter* player)
	{
		auto actorState = player->AsActorState();
		if (actorState->GetSitSleepState() == RE::SIT_SLEEP_STATE::kIsSleeping)
			return true;

		return false;
	}

	static inline bool IsSittingOrSleeping(RE::PlayerCharacter* player)
	{
		return IsSitting(player) || IsSleeping(player);
	}

	static inline std::int8_t GetWeaponID(RE::PlayerCharacter* player, bool rightHand = false)
	{
		auto playerState = player->AsActorState();

		if (playerState->IsWeaponDrawn())
		{
			std::int8_t weaponID = 0;

			if (rightHand)
			{
				auto equippedRightHand = player->GetActorRuntimeData().currentProcess->GetEquippedRightHand();
				auto rightWeapon = equippedRightHand ? equippedRightHand->As<RE::TESObjectWEAP>() : nullptr;

				if (rightWeapon)
					weaponID = static_cast<std::int8_t>(rightWeapon->GetWeaponType());

				return weaponID;
			}

			auto equippedLeftHand = player->GetActorRuntimeData().currentProcess->GetEquippedLeftHand();
			auto leftWeapon = equippedLeftHand ? equippedLeftHand->As<RE::TESObjectWEAP>() : nullptr;

			if (leftWeapon)
				weaponID = static_cast<std::int8_t>(leftWeapon->GetWeaponType());

			return weaponID;
		}
		return -1;
	}

	static inline bool IsAiming(RE::PlayerCharacter* player, bool crossbow = false)
	{
		if (!player)
			return false;

		std::int8_t weaponID = GetWeaponID(player);

		if ((crossbow && weaponID != RE::WEAPON_ID::kCrossbow) || (!crossbow && weaponID != RE::WEAPON_ID::kBow))
			return false;

		auto attackState = player->AsActorState()->GetAttackState();

		if (attackState == RE::ATTACK_STATE_ENUM::kBowAttached && DLLMain::Plugin::Get()->Config()->Fixes().bArcheryGameplayOverhaul)
			return false;

		return (attackState >= RE::ATTACK_STATE_ENUM::kBowDraw && attackState <= RE::ATTACK_STATE_ENUM::kBowNextAttack);
	}

	static inline bool IsRighthandWeaponEquipped(RE::PlayerCharacter* player)
	{
		if (!player)
			return false;

		std::int8_t leftWeaponID = GetWeaponID(player);
		std::int8_t rightWeaponID = GetWeaponID(player, true);

		return (leftWeaponID == RE::WEAPON_ID::kFist && (rightWeaponID == RE::WEAPON_ID::kFist || rightWeaponID == RE::WEAPON_ID::kSword ||
			rightWeaponID == RE::WEAPON_ID::kDagger || rightWeaponID == RE::WEAPON_ID::kAxe || rightWeaponID == RE::WEAPON_ID::kMace ||
			rightWeaponID == RE::WEAPON_ID::kStaff));
	}

	static inline bool IsRangedWeaponEquipped(RE::PlayerCharacter* player, bool crossbow = false)
	{
		if (!player)
			return false;

		std::int8_t weaponID = GetWeaponID(player);

		if ((crossbow && weaponID == RE::WEAPON_ID::kCrossbow) || (!crossbow && weaponID == RE::WEAPON_ID::kBow))
			return true;

		return false;
	}

	static inline bool IsShieldEquipped(RE::PlayerCharacter* player)
	{
		auto thirdperson3D = player->Get3D(0);
		if (!thirdperson3D)
			return false;

		auto shieldNode = FindNode(thirdperson3D->AsNode(), "Shield");
		if (shieldNode && shieldNode->children.size() == 1)
			return true;

		return false;
	}

	static inline bool IsTorchEquipped(RE::Actor* actor)
	{
		using namespace Address::Function;

		typedef RE::BipedModel* (*_GetBipedModel)(RE::Actor*);
		_GetBipedModel GetBipedModel = (_GetBipedModel)GetVTableAddress(actor, 0x3F8);
		RE::BipedModel* model = GetBipedModel(actor);
		RE::BipedAnim* bipedData = model->bipedData;

		bool torch_out = false;

		if (bipedData)
		{
			bipedData->IncRef();

			torch_out = BipedAnim_GetTorchObject(bipedData, 1);
			if (!torch_out)
				torch_out = BipedAnim_GetTorchObject(bipedData, 0);

			if (bipedData->DecRef() == 0)
			{
				bipedData->~BipedAnim();
				RE::MemoryManager::GetSingleton()->Deallocate(bipedData, false);
			}
		}
		return torch_out;
	}

	// Needs testing.
	static inline bool IsIdlePlaying(RE::PlayerCharacter* player)
	{
		return !player->GetActorRuntimeData().movementController.get()->unk1C0;
	}

	static inline bool IsInteractionEnter(RE::PlayerCharacter* player)
	{
		auto actorState = player->AsActorState();
		if (actorState->GetSitSleepState() == RE::SIT_SLEEP_STATE::kWaitingForSitAnim)
			return true;

		return false;
	}

	static inline bool IsInteracting(RE::PlayerCharacter* player)
	{
		auto actorState = player->AsActorState();
		if (actorState->GetSitSleepState() == RE::SIT_SLEEP_STATE::kIsSitting)
			return true;

		return false;
	}

	static inline bool IsInteractionLeave(RE::PlayerCharacter* player)
	{
		auto actorState = player->AsActorState();
		if (actorState->GetSitSleepState() == RE::SIT_SLEEP_STATE::kWantToStand)
			return true;

		return false;
	}

	static inline bool IsOnCart()
	{
		auto camera = RE::PlayerCamera::GetSingleton();
		auto playerControls = RE::PlayerControls::GetSingleton();

		// unk2A seems to be LoadingData, called just after loading screen and switched off a few moments later.
		if (camera->currentState->id == RE::CameraStates::kFirstPerson && playerControls->data.unk2A && playerControls->data.povScriptMode && CanLook() && !CanMove())
			return true;

		return false;
	}

	static inline bool IsBeastMode()
	{
		auto playerControls = RE::PlayerControls::GetSingleton();
		return (playerControls && playerControls->data.povBeastMode);
	}

}
