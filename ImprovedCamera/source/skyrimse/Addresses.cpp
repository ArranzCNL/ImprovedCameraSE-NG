/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

// Precompiled Header
#include "stdafx.h"

#include "skyrimse/Addresses.h"

namespace Address {

	namespace Function {

		void ModelReferenceEffect_Attach(void* arg)
		{
			using func_t = decltype(&ModelReferenceEffect_Attach);
			REL::Relocation<func_t> func{ REL::RelocationID(33872, 34668) };
			return func(arg);
		}

		const bool ModelReferenceEffect_Sub_14057BCC0(void* arg)
		{
			using func_t = decltype(&ModelReferenceEffect_Sub_14057BCC0);
			REL::Relocation<func_t> func{ REL::RelocationID(33873, 34669) };
			return func(arg);
		}

		const bool ShaderReferenceEffect_Sub_140584680(void* arg)
		{
			using func_t = decltype(&ShaderReferenceEffect_Sub_140584680);
			REL::Relocation<func_t> func{ REL::RelocationID(34131, 34933) };
			return func(arg);
		}

		const bool BipedAnim_GetTorchObject(void* bipedData, std::int32_t value)
		{
			using func_t = decltype(&BipedAnim_GetTorchObject);
			REL::Relocation<func_t> func{ REL::RelocationID(15517, 15694) };
			return func(bipedData, value);
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
