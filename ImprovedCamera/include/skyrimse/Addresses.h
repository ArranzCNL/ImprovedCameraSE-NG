/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

namespace RE {

	class BipedModel {

	public:
		enum
		{
			kWeightModel_Small = 0,
			kWeightModel_Large = 1
		};
		BipedAnim* bipedData;
	};
}

namespace Address {

	namespace Hook {

		static std::uintptr_t ProcessInput;
		static std::uintptr_t UpdateSwitchPOV;
		static std::uintptr_t UpdateCamera;
		static std::uintptr_t UpdateFirstPerson;
		static std::uintptr_t TESObjectCell_Get3D;
		static std::uintptr_t SmoothAnimationTransitions;
		static std::uintptr_t ModelReferenceEffect_UpdatePosition;
		static std::uintptr_t ModelReferenceEffect_Update;
		static std::uintptr_t ShaderReferenceEffect1;
		static std::uintptr_t ShaderReferenceEffect2;
		static std::uintptr_t GetEffectNode_IsThirdPerson;
		static std::uintptr_t ForceFirstPerson;
		static std::uintptr_t ForceThirdPerson;
		static std::uintptr_t Ragdoll;
		static std::uintptr_t Ragdoll_IsTaskPoolRequired;
		static std::uintptr_t Ragdoll_UpdateObjectUpwards;
		static std::uintptr_t RagdollDeath;
		static std::uintptr_t KillActor;
		static std::uintptr_t ThirdpersonFade;
		static std::uintptr_t HorseLookingDownFix1;
		static std::uintptr_t HorseLookingDownFix2;
		static std::uintptr_t HorseLookingDownFix3;
	}

	namespace Function {

		extern void ModelReferenceEffect_Attach(void* arg);
		extern const bool ModelReferenceEffect_Sub_14057BCC0(void* arg);   // 1.6.640
		extern const bool ShaderReferenceEffect_Sub_140584680(void* arg);  // 1.6.640
		extern const bool BipedAnim_GetTorchObject(void* bipedData, std::int32_t value);
		extern const bool Ragdoll_IsTaskPoolRequired();
		extern RE::NiNode* Get3D(RE::TESObjectREFR* objectREFR);
		extern void ResetNodes(RE::ActorMagicCaster* magicCaster);
	}

	namespace Variable {

		extern float* NiNodeGlobalTime;
		extern float* fControllerBufferDepth;
		extern float* fDefaultWorldFOV;
		extern float* fNearDistance;
		extern float* fMinCurrentZoom;
		extern float* fSittingMaxLookingDown;
		extern float* fMountedMaxLookingUp;
		extern float* fMountedMaxLookingDown;
		extern float* fFlyingMaxLookingUp;
		extern float* fFlyingMaxLookingDown;
	}

}
