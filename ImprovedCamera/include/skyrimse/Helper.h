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

	struct EQUIPPED_ITEMTYPE_IDS {

		enum EQUIPPED_ITEMTYPE_ID : std::int32_t
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
			kMagic,
			kShield,
			kTorch,
			kCrossbow,

			kTotal = 13
		};
	};
	using EQUIPPED_ITEMTYPE_ID = EQUIPPED_ITEMTYPE_IDS::EQUIPPED_ITEMTYPE_ID;

}

namespace Helper {

	static bool archeryGameplayOverhaul = false;

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

		for (auto headName : headNames)
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
				"EnterBedRoll",
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
				"IdleLaydown",
				"IdleCartTravelDriver",
			};

			for (auto idleName : idleNames)
			{
				if (strcmp(idleName, parentIdle) == 0)
					return true;
			}
		}
		return false;
	}

	static inline float GetHighHeelsOffset(RE::PlayerCharacter* player)
	{
		auto& biped = player->GetBiped(0);
		float heelsOffset = 0.0f;

		if (biped)
		{
			auto& bipedData = biped->objects[RE::BIPED_OBJECT::kFeet];

			if (bipedData.partClone)
			{
				auto feetObject = bipedData.partClone.get();
				if (!feetObject)
					return heelsOffset;

				auto feetNode = feetObject->AsNode();
				if (feetNode)
				{
					for (auto& childNode : feetNode->GetChildren())
					{
						if (childNode.get())
						{
							if (childNode->HasExtraData("HH_OFFSET"))
							{
								auto hhOffset = childNode->GetExtraData<RE::NiFloatExtraData>("HH_OFFSET");
								if (hhOffset)
									return heelsOffset = hhOffset->value;
							}
						}
					}
				}
			}
		}
		return heelsOffset;
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

		if (attackState == RE::ATTACK_STATE_ENUM::kBowAttached && archeryGameplayOverhaul)
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

		return (equippedLeftItemID == RE::EQUIPPED_ITEMTYPE_ID::kFist && (equippedRightItemID == RE::EQUIPPED_ITEMTYPE_ID::kFist ||
			equippedRightItemID == RE::EQUIPPED_ITEMTYPE_ID::kSword || equippedRightItemID == RE::EQUIPPED_ITEMTYPE_ID::kDagger ||
			equippedRightItemID == RE::EQUIPPED_ITEMTYPE_ID::kAxe || equippedRightItemID == RE::EQUIPPED_ITEMTYPE_ID::kMace ||
			equippedRightItemID == RE::EQUIPPED_ITEMTYPE_ID::kStaff));
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

	static inline bool IsCastingMagic(RE::PlayerCharacter* player)
	{
		auto leftSpell = player->GetActorRuntimeData().selectedSpells[RE::Actor::SlotTypes::kLeftHand];
		auto rightSpell = player->GetActorRuntimeData().selectedSpells[RE::Actor::SlotTypes::kRightHand];

		if ((leftSpell && player->IsCasting(leftSpell)) || (rightSpell && player->IsCasting(rightSpell)))
			return true;

		return false;
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
