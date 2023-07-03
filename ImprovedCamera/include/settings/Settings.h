/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once


namespace Settings {

	// 11 Settings
	struct ModuleData {
		std::string sFileName;
		std::string sFileVersionMin;
		std::string sFileVersionMax;
		std::string sWindowName;
		std::string sProfileName;
		std::int32_t iCheckCompatibility;
		std::int32_t iMenuMode;
		std::int32_t iMenuKey;
		std::int32_t iMenuTimeout;
		std::string sMenuFont;
		float fMenuFontSize;
	};
// Data for ImprovedCameraSE.ini
	// 19 Settings
	struct General {
		bool bEnableBody;
		bool bEnableBodyConsole;
		bool bEnableShadows;
		bool bAdjustPlayerScale;
		float fBodyHeightOffset;
		bool bEnableHead;
		bool bEnableHeadCombat;
		bool bEnableHeadHorse;
		bool bEnableHeadDragon;
		bool bEnableHeadVampireLord;
		bool bEnableHeadWerewolf;
		bool bEnableHeadScripted;
		bool bEnableThirdPersonArms;
		bool bEnableThirdPersonShield;
		bool bEnableThirdPersonShieldBlock;
		bool bEnableThirdPersonBow;
		bool bEnableThirdPersonBowAim;
		bool bEnableThirdPersonCrossbow;
		bool bEnableThirdPersonCrossbowAim;
	};
	// 12 Settings
	struct Hide {
		bool b2HWeapon;
		bool bBow;
		bool bQuiver;

		bool bSitting;
		bool bSleeping;
		bool bJumping;
		bool bSwimming;
		bool bSneakRoll;
		bool bAttack;
		bool bPowerAttack;
		bool bAttackBow;
		bool bKillmove;
	};
	// 7 Settings
	struct Fixes {
		bool bQuickLightLighting;
		bool bFirstPersonOverhaul;
		bool bArcheryGameplayOverhaul;
		float fSwitchPOVDetectDistance;
		bool bSmoothAnimationTransitions;
		float fControllerBufferDepth1st;
		float fControllerBufferDepth3rd;
	};
	// 14 Settings
	struct RestrictAngles {
		float fSitting;
		float fSittingMaxLookingUp;
		float fSittingMaxLookingDown;
		float fMounted;
		float fMountedMaxLookingUp;
		float fMountedMaxLookingDown;
		float fFlying;
		float fFlyingMaxLookingUp;
		float fFlyingMaxLookingDown;
		float fVampireLord;
		float fWerewolf;
		float fNecroLich;
		float fScripted;
		float fScriptedPitch;
	};
	// 20 Settings
	struct Events {
		bool bFirstPerson;
		bool bFirstPersonCombat;
		bool bFurniture;
		bool bCrafting;
		bool bKillmove;
		bool bRagdoll;
		bool bDeath;
		bool bCannibal;
		bool bHorse;
		bool bHorseCombat;
		bool bHorseTransition;
		bool bDragon;
		bool bDragonCombat;
		bool bDragonTransition;
		bool bVampireLord;
		bool bWerewolf;
		bool bTransform;
		bool bNecroLich;
		bool bScripted;
		bool bThirdPerson;
	};
	// 21 Settings
	struct FOV {
		bool bEnableOverride;
		float fFirstPerson;
		float fFirstPersonHands;
		float fFirstPersonCombat;
		float fFurniture;
		float fCrafting;
		float fKillmove;
		float fRagdoll;
		float fDeath;
		float fCannibal;
		float fHorse;
		float fHorseCombat;
		float fHorseTransition;
		float fDragon;
		float fDragonCombat;
		float fDragonTransition;
		float fVampireLord;
		float fWerewolf;
		float fNecroLich;
		float fScripted;
		float fThirdPerson;
	};
	// 22 Settings
	struct NearDistance {
		bool bEnableOverride;
		float fPitchThreshold;
		float fFirstPerson;
		float fFirstPersonCombat;
		float fSitting;
		float fFurniture;
		float fCrafting;
		float fKillmove;
		float fRagdoll;
		float fDeath;
		float fCannibal;
		float fHorse;
		float fHorseCombat;
		float fHorseTransition;
		float fDragon;
		float fDragonCombat;
		float fDragonTransition;
		float fVampireLord;
		float fWerewolf;
		float fNecroLich;
		float fScripted;
		float fThirdPerson;
	};
	// 14 Settings
	struct Headbob {
		bool bIdle;
		bool bWalk;
		bool bRun;
		bool bSprint;
		bool bCombat;
		bool bSneak;
		bool bSneakRoll;
		float fRotationIdle;
		float fRotationWalk;
		float fRotationRun;
		float fRotationSprint;
		float fRotationCombat;
		float fRotationSneak;
		float fRotationSneakRoll;
	};
	// 27 Settings
	struct Camera {
		float fFirstPersonPosX;
		float fFirstPersonPosY;
		float fFirstPersonPosZ;
		float fFirstPersonCombatPosX;
		float fFirstPersonCombatPosY;
		float fFirstPersonCombatPosZ;
		float fHorsePosX;
		float fHorsePosY;
		float fHorsePosZ;
		float fHorseCombatPosX;
		float fHorseCombatPosY;
		float fHorseCombatPosZ;
		float fDragonPosX;
		float fDragonPosY;
		float fDragonPosZ;
		float fVampireLordPosX;
		float fVampireLordPosY;
		float fVampireLordPosZ;
		float fWerewolfPosX;
		float fWerewolfPosY;
		float fWerewolfPosZ;
		float fNecroLichPosX;
		float fNecroLichPosY;
		float fNecroLichPosZ;
		float fScriptedPosX;
		float fScriptedPosY;
		float fScriptedPosZ;
	};

}
