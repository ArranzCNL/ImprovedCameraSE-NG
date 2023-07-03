/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "skyrimse/Addresses.h"
#include "plugin.h"
#include <RE/Skyrim.h>


namespace Helper {

	static inline void* GetVTableAddress(void* object, std::uint32_t address)
	{
		std::uint64_t* vtbl = (std::uint64_t*)*(std::uint64_t*)object;
		return (void*)(vtbl[address / 8]);
	}

	static inline void UpdateNode(RE::NiNode* node, SKSE::stl::enumeration<RE::NiUpdateData::Flag, std::uint32_t>flags = RE::NiUpdateData::Flag::kNone, float updateTime = 0.f)
	{
		auto updateData = RE::NiUpdateData();
		updateData.flags = flags;
		updateData.time = updateTime;
		node->Update(updateData);
	}

	static RE::NiNode* GetHeadNode(RE::NiNode* node)
	{
		const char* headNames[] =
		{
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
			"Head",
		};

		for (const char* headName : headNames)
		{
			auto headObject = node->GetObjectByName(headName);
			if (!headObject)
				continue;

			auto headNode = headObject->AsNode();
			if (headNode)
				return headNode;
		}
		return nullptr;
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

	static inline bool IsAiming(RE::PlayerCharacter* player, bool crossbow = false)
	{
		using namespace Address::Function;

		if (!player)
			return false;

		std::int32_t equippedItemID = GetEquippedItemTypeID(player);

		if ((crossbow && equippedItemID != RE::EQUIPPED_ITEMTYPE_ID::kCrossbow) || (!crossbow && equippedItemID != RE::EQUIPPED_ITEMTYPE_ID::kBow))
			return false;

		auto attackState = player->AsActorState()->GetAttackState();
		if (attackState == RE::ATTACK_STATE_ENUM::kBowAttached && DLLMain::Plugin::Get()->Config()->Fixes().bArcheryGameplayOverhaul)
			return false;

		return (attackState >= RE::ATTACK_STATE_ENUM::kBowDraw && attackState <= RE::ATTACK_STATE_ENUM::kBowNextAttack);
	}

	static inline bool IsRighthandWeaponEquipped(RE::PlayerCharacter* player)
	{
		using namespace Address::Function;

		if (!player)
			return false;

		std::int32_t equippedLeftItemID = GetEquippedItemTypeID(player);
		std::int32_t equippedRightItemID = GetEquippedItemTypeID(player, true);

		return (equippedLeftItemID == RE::EQUIPPED_ITEMTYPE_ID::kFist && (equippedRightItemID == RE::EQUIPPED_ITEMTYPE_ID::kFist || equippedRightItemID == RE::EQUIPPED_ITEMTYPE_ID::kSword
			|| equippedRightItemID == RE::EQUIPPED_ITEMTYPE_ID::kDagger || equippedRightItemID == RE::EQUIPPED_ITEMTYPE_ID::kAxe || equippedRightItemID == RE::EQUIPPED_ITEMTYPE_ID::kMace
			|| equippedRightItemID == RE::EQUIPPED_ITEMTYPE_ID::kStaff));
	}

	static inline bool IsRangedWeaponEquipped(RE::PlayerCharacter* player, bool crossbow = false)
	{
		using namespace Address::Function;

		if (!player)
			return false;

		std::int32_t equippedItemID = GetEquippedItemTypeID(player);

		if ((crossbow && equippedItemID == RE::EQUIPPED_ITEMTYPE_ID::kCrossbow) || (!crossbow && equippedItemID == RE::EQUIPPED_ITEMTYPE_ID::kBow))
			return true;

		return false;
	}
	
	static inline bool IsTorchOut(RE::Actor* actor)
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
		if (camera->currentState->id == RE::CameraStates::kFirstPerson && playerControls->data.unk2A && playerControls->data.povScriptMode
			&& CanLook() && !CanMove())
			return true;

		return false;
	}

	static inline bool IsBeastMode()
	{
		auto playerControls = RE::PlayerControls::GetSingleton();
		return (playerControls && playerControls->data.povBeastMode);
	}

}
