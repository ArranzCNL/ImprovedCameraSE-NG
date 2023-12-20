/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

// Precompiled Header
#include "PCH.h"

#include "skyrimse/Addresses.h"

namespace Address {

	namespace Function {

		const bool ShaderReferenceEffect_Sub_140584680(void* arg)
		{
			using func_t = decltype(&ShaderReferenceEffect_Sub_140584680);
			REL::Relocation<func_t> func{ REL::RelocationID(34131, 34933) };
			return func(arg);
		}

		__declspec(noinline) const std::int32_t GetEquippedItemTypeID(RE::Actor* actor, bool rightHand, void* arg1, std::int32_t arg2)
		{
			// Make sure arg1 and arg2 are not changed.
			arg1 = nullptr; arg2 = 0;

			using func_t = const std::int32_t(*)(void* arg1, std::int32_t arg2, RE::Actor* actor, bool rightHand);
			REL::Relocation<func_t> func{ REL::RelocationID(53876, 54685) };
			return func(arg1, arg2, actor, rightHand);
		}

		const bool Ragdoll_IsTaskPoolRequired()
		{
			using func_t = decltype(&Ragdoll_IsTaskPoolRequired);
			REL::Relocation<func_t> func{ REL::RelocationID(38079, 39033) };
			return func();
		}

		RE::NiNode* Get3D(RE::TESObjectREFR* objectREFR)
		{
			using func_t = decltype(&Get3D);
			REL::Relocation<func_t> func{ REL::RelocationID(19308, 19735) };
			return func(objectREFR);
		}

		void ResetNodes(RE::ActorMagicCaster* magicCaster)
		{
			using func_t = decltype(&ResetNodes);
			REL::Relocation<func_t> func{ REL::RelocationID(33375, 34156) };
			return func(magicCaster);
		}
	}

	namespace Variable {

		float* NiNodeGlobalTime;
		float* fControllerBufferDepth;
		float* fDefaultWorldFOV;
		float* fNearDistance;
		float* fMinCurrentZoom;
		float* fSittingMaxLookingDown;
		float* fMountedMaxLookingUp;
		float* fMountedMaxLookingDown;
		float* fFlyingMaxLookingUp;
		float* fFlyingMaxLookingDown;
	}
}
