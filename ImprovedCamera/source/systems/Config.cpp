/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

// Precompiled Header
#include "PCH.h"

#include "systems/Config.h"

#include "plugin.h"
#include "utils/Utils.h"

#define MINI_CASE_SENSITIVE
#include "mini/ini.h"

namespace Systems {

	Config::Config()
	{
		auto plugin = DLLMain::Plugin::Get();

		m_Name = plugin->Name() + ".ini";
		m_Path = plugin->Path() + plugin->Name() + "\\";

		m_FontPath = m_Path + "Fonts\\";
		m_ProfilePath = m_Path + "Profiles\\";
		m_FileName = m_Path + m_Name;

		ReadIni(m_FileName);

		if (m_PreInitialized)
			ReadIni(m_ModuleData.sProfileName);
	}

	bool Config::ReadIni(std::string& name)
	{
		try
		{
			if (!m_PreInitialized && m_FileName.compare(name) == 0)
			{
				mINI::INIFile ModuleData(name.c_str());
				mINI::INIStructure ini;
				ModuleData.read(ini);

				m_ModuleData.sFileName = ini.get("MODULE DATA").get("FileName");
				m_ModuleData.sFileVersionMin = ini.get("MODULE DATA").get("FileVersionMin");
				m_ModuleData.sFileVersionMax = ini.get("MODULE DATA").get("FileVersionMax");
				m_ModuleData.sWindowName = ini.get("MODULE DATA").get("WindowName");
				m_ModuleData.iCheckCompatibility = std::stoi(ini.get("MODULE DATA").get("CheckCompatibility"));
				m_ModuleData.iMenuMode = std::stoi(ini.get("MODULE DATA").get("MenuMode"));
				m_ModuleData.iMenuTimeout = std::stoi(ini.get("MODULE DATA").get("MenuTimeout"));
				m_ModuleData.iMenuKey = std::stoi(ini.get("MODULE DATA").get("MenuKey"), NULL, 16);
				m_ModuleData.sMenuFont = ini.get("MODULE DATA").get("MenuFont");
				m_ModuleData.fMenuFontSize = std::stof(ini.get("MODULE DATA").get("MenuFontSize"));
				m_ModuleData.sProfileName = ini.get("MODULE DATA").get("ProfileName");

				m_PreInitialized = true;
			}
			else
			{
				std::string fileName = m_ProfilePath + name.c_str();

				mINI::INIFile ProfileData(fileName);
				mINI::INIStructure ini;
				ProfileData.read(ini);

				m_General.bEnableBody = std::stoi(ini.get("GENERAL").get("bEnableBody"));
				m_General.bEnableBodyConsole = std::stoi(ini.get("GENERAL").get("bEnableBodyConsole"));
				m_General.bEnableShadows = std::stoi(ini.get("GENERAL").get("bEnableShadows"));
				m_General.bAdjustPlayerScale = std::stoi(ini.get("GENERAL").get("bAdjustPlayerScale"));
				m_General.fBodyHeightOffset = std::stof(ini.get("GENERAL").get("fBodyHeightOffset"));
				m_General.bEnableHead = std::stoi(ini.get("GENERAL").get("bEnableHead"));
				m_General.bEnableHeadCombat = std::stoi(ini.get("GENERAL").get("bEnableHeadCombat"));
				m_General.bEnableHeadHorse = std::stoi(ini.get("GENERAL").get("bEnableHeadHorse"));
				m_General.bEnableHeadDragon = std::stoi(ini.get("GENERAL").get("bEnableHeadDragon"));
				m_General.bEnableHeadVampireLord = std::stoi(ini.get("GENERAL").get("bEnableHeadVampireLord"));
				m_General.bEnableHeadWerewolf = std::stoi(ini.get("GENERAL").get("bEnableHeadWerewolf"));
				m_General.bEnableHeadScripted = std::stoi(ini.get("GENERAL").get("bEnableHeadScripted"));
				m_General.bEnableThirdPersonArms = std::stoi(ini.get("GENERAL").get("bEnableThirdPersonArms"));
				m_General.bEnableThirdPersonTorch = std::stoi(ini.get("GENERAL").get("bEnableThirdPersonTorch"));
				m_General.bEnableThirdPersonTorchBlock = std::stoi(ini.get("GENERAL").get("bEnableThirdPersonTorchBlock"));
				m_General.bEnableThirdPersonShield = std::stoi(ini.get("GENERAL").get("bEnableThirdPersonShield"));
				m_General.bEnableThirdPersonShieldBlock = std::stoi(ini.get("GENERAL").get("bEnableThirdPersonShieldBlock"));
				m_General.bEnableThirdPersonBow = std::stoi(ini.get("GENERAL").get("bEnableThirdPersonBow"));
				m_General.bEnableThirdPersonBowAim = std::stoi(ini.get("GENERAL").get("bEnableThirdPersonBowAim"));
				m_General.bEnableThirdPersonCrossbow = std::stoi(ini.get("GENERAL").get("bEnableThirdPersonCrossbow"));
				m_General.bEnableThirdPersonCrossbowAim = std::stoi(ini.get("GENERAL").get("bEnableThirdPersonCrossbowAim"));

				m_Hide.b2HWeapon = std::stoi(ini.get("HIDE").get("b2HWeapon"));
				m_Hide.bBow = std::stoi(ini.get("HIDE").get("bBow"));
				m_Hide.bQuiver = std::stoi(ini.get("HIDE").get("bQuiver"));

				m_Hide.bSitting = std::stoi(ini.get("HIDE").get("bSitting"));
				m_Hide.bSleeping = std::stoi(ini.get("HIDE").get("bSleeping"));
				m_Hide.bJumping = std::stoi(ini.get("HIDE").get("bJumping"));
				m_Hide.bSwimming = std::stoi(ini.get("HIDE").get("bSwimming"));
				m_Hide.bSneakRoll = std::stoi(ini.get("HIDE").get("bSneakRoll"));
				m_Hide.bAttack = std::stoi(ini.get("HIDE").get("bAttack"));
				m_Hide.bPowerAttack = std::stoi(ini.get("HIDE").get("bPowerAttack"));
				m_Hide.bAttackBow = std::stoi(ini.get("HIDE").get("bAttackBow"));
				m_Hide.bKillmove = std::stoi(ini.get("HIDE").get("bKillmove"));

				m_Fixes.bFirstPersonOverhaul = std::stoi(ini.get("FIXES").get("bFirstPersonOverhaul"));
				m_Fixes.bOverrideVanillaArmsOnMovement = std::stoi(ini.get("FIXES").get("bOverrideVanillaArmsOnMovement"));
				m_Fixes.bArcheryGameplayOverhaul = std::stoi(ini.get("FIXES").get("bArcheryGameplayOverhaul"));
				m_Fixes.bSmoothAnimationTransitions = std::stoi(ini.get("FIXES").get("bSmoothAnimationTransitions"));
				m_Fixes.fControllerBufferDepth1st = std::stof(ini.get("FIXES").get("fControllerBufferDepth1st"));

				m_RestrictAngles.fSitting = std::stof(ini.get("RESTRICT ANGLES").get("fSitting"));
				m_RestrictAngles.fSittingMaxLookingUp = std::stof(ini.get("RESTRICT ANGLES").get("fSittingMaxLookingUp"));
				m_RestrictAngles.fSittingMaxLookingDown = std::stof(ini.get("RESTRICT ANGLES").get("fSittingMaxLookingDown"));
				m_RestrictAngles.fMounted = std::stof(ini.get("RESTRICT ANGLES").get("fMounted"));
				m_RestrictAngles.fMountedMaxLookingUp = std::stof(ini.get("RESTRICT ANGLES").get("fMountedMaxLookingUp"));
				m_RestrictAngles.fMountedMaxLookingDown = std::stof(ini.get("RESTRICT ANGLES").get("fMountedMaxLookingDown"));
				m_RestrictAngles.fFlying = std::stof(ini.get("RESTRICT ANGLES").get("fFlying"));
				m_RestrictAngles.fFlyingMaxLookingUp = std::stof(ini.get("RESTRICT ANGLES").get("fFlyingMaxLookingUp"));
				m_RestrictAngles.fFlyingMaxLookingDown = std::stof(ini.get("RESTRICT ANGLES").get("fFlyingMaxLookingDown"));
				m_RestrictAngles.fVampireLord = std::stof(ini.get("RESTRICT ANGLES").get("fVampireLord"));
				m_RestrictAngles.fWerewolf = std::stof(ini.get("RESTRICT ANGLES").get("fWerewolf"));
				m_RestrictAngles.fNecroLich = std::stof(ini.get("RESTRICT ANGLES").get("fNecroLich"));
				m_RestrictAngles.fScripted = std::stof(ini.get("RESTRICT ANGLES").get("fScripted"));
				m_RestrictAngles.fScriptedPitch = std::stof(ini.get("RESTRICT ANGLES").get("fScriptedPitch"));

				m_Events.bFirstPerson = std::stoi(ini.get("EVENTS").get("bFirstPerson"));
				m_Events.bFirstPersonCombat = std::stoi(ini.get("EVENTS").get("bFirstPersonCombat"));
				m_Events.bFurniture = std::stoi(ini.get("EVENTS").get("bFurniture"));
				m_Events.bCrafting = std::stoi(ini.get("EVENTS").get("bCrafting"));
				m_Events.bKillmove = std::stoi(ini.get("EVENTS").get("bKillmove"));
				m_Events.bRagdoll = std::stoi(ini.get("EVENTS").get("bRagdoll"));
				m_Events.bDeath = std::stoi(ini.get("EVENTS").get("bDeath"));
				m_Events.bCannibal = std::stoi(ini.get("EVENTS").get("bCannibal"));
				m_Events.bHorse = std::stoi(ini.get("EVENTS").get("bHorse"));
				m_Events.bHorseCombat = std::stoi(ini.get("EVENTS").get("bHorseCombat"));
				m_Events.bHorseTransition = std::stoi(ini.get("EVENTS").get("bHorseTransition"));
				m_Events.bDragon = std::stoi(ini.get("EVENTS").get("bDragon"));
				m_Events.bDragonCombat = std::stoi(ini.get("EVENTS").get("bDragonCombat"));
				m_Events.bDragonTransition = std::stoi(ini.get("EVENTS").get("bDragonTransition"));
				m_Events.bVampireLord = std::stoi(ini.get("EVENTS").get("bVampireLord"));
				m_Events.bWerewolf = std::stoi(ini.get("EVENTS").get("bWerewolf"));
				m_Events.bTransform = std::stoi(ini.get("EVENTS").get("bTransform"));
				m_Events.bNecroLich = std::stoi(ini.get("EVENTS").get("bNecroLich"));
				m_Events.bScripted = std::stoi(ini.get("EVENTS").get("bScripted"));
				m_Events.bThirdPerson = std::stoi(ini.get("EVENTS").get("bThirdPerson"));

				m_FOV.bEnableOverride = std::stoi(ini.get("FOV").get("bEnableOverride"));
				m_FOV.fFirstPerson = std::stof(ini.get("FOV").get("fFirstPerson"));
				m_FOV.fFirstPersonHands = std::stof(ini.get("FOV").get("fFirstPersonHands"));
				m_FOV.fFirstPersonCombat = std::stof(ini.get("FOV").get("fFirstPersonCombat"));
				m_FOV.fFurniture = std::stof(ini.get("FOV").get("fFurniture"));
				m_FOV.fCrafting = std::stof(ini.get("FOV").get("fCrafting"));
				m_FOV.fKillmove = std::stof(ini.get("FOV").get("fKillmove"));
				m_FOV.fRagdoll = std::stof(ini.get("FOV").get("fRagdoll"));
				m_FOV.fDeath = std::stof(ini.get("FOV").get("fDeath"));
				m_FOV.fCannibal = std::stof(ini.get("FOV").get("fCannibal"));
				m_FOV.fHorse = std::stof(ini.get("FOV").get("fHorse"));
				m_FOV.fHorseCombat = std::stof(ini.get("FOV").get("fHorseCombat"));
				m_FOV.fHorseTransition = std::stof(ini.get("FOV").get("fHorseTransition"));
				m_FOV.fDragon = std::stof(ini.get("FOV").get("fDragon"));
				m_FOV.fDragonCombat = std::stof(ini.get("FOV").get("fDragonCombat"));
				m_FOV.fDragonTransition = std::stof(ini.get("FOV").get("fDragonTransition"));
				m_FOV.fVampireLord = std::stof(ini.get("FOV").get("fVampireLord"));
				m_FOV.fWerewolf = std::stof(ini.get("FOV").get("fWerewolf"));
				m_FOV.fNecroLich = std::stof(ini.get("FOV").get("fNecroLich"));
				m_FOV.fScripted = std::stof(ini.get("FOV").get("fScripted"));
				m_FOV.fThirdPerson = std::stof(ini.get("FOV").get("fThirdPerson"));

				m_NearDistance.bEnableOverride = std::stoi(ini.get("NEARDISTANCE").get("bEnableOverride"));
				m_NearDistance.fPitchThreshold = std::stof(ini.get("NEARDISTANCE").get("fPitchThreshold"));
				m_NearDistance.fFirstPerson = std::stof(ini.get("NEARDISTANCE").get("fFirstPerson"));
				m_NearDistance.fFirstPersonCombat = std::stof(ini.get("NEARDISTANCE").get("fFirstPersonCombat"));
				m_NearDistance.fSitting = std::stof(ini.get("NEARDISTANCE").get("fSitting"));
				m_NearDistance.fFurniture = std::stof(ini.get("NEARDISTANCE").get("fFurniture"));
				m_NearDistance.fCrafting = std::stof(ini.get("NEARDISTANCE").get("fCrafting"));
				m_NearDistance.fKillmove = std::stof(ini.get("NEARDISTANCE").get("fKillmove"));
				m_NearDistance.fRagdoll = std::stof(ini.get("NEARDISTANCE").get("fRagdoll"));
				m_NearDistance.fDeath = std::stof(ini.get("NEARDISTANCE").get("fDeath"));
				m_NearDistance.fCannibal = std::stof(ini.get("NEARDISTANCE").get("fCannibal"));
				m_NearDistance.fHorse = std::stof(ini.get("NEARDISTANCE").get("fHorse"));
				m_NearDistance.fHorseCombat = std::stof(ini.get("NEARDISTANCE").get("fHorseCombat"));
				m_NearDistance.fHorseTransition = std::stof(ini.get("NEARDISTANCE").get("fHorseTransition"));
				m_NearDistance.fDragon = std::stof(ini.get("NEARDISTANCE").get("fDragon"));
				m_NearDistance.fDragonCombat = std::stof(ini.get("NEARDISTANCE").get("fDragonCombat"));
				m_NearDistance.fDragonTransition = std::stof(ini.get("NEARDISTANCE").get("fDragonTransition"));
				m_NearDistance.fVampireLord = std::stof(ini.get("NEARDISTANCE").get("fVampireLord"));
				m_NearDistance.fWerewolf = std::stof(ini.get("NEARDISTANCE").get("fWerewolf"));
				m_NearDistance.fNecroLich = std::stof(ini.get("NEARDISTANCE").get("fNecroLich"));
				m_NearDistance.fScripted = std::stof(ini.get("NEARDISTANCE").get("fScripted"));
				m_NearDistance.fThirdPerson = std::stof(ini.get("NEARDISTANCE").get("fThirdPerson"));

				m_Headbob.bIdle = std::stoi(ini.get("HEADBOB").get("bIdle"));
				m_Headbob.bWalk = std::stoi(ini.get("HEADBOB").get("bWalk"));
				m_Headbob.bRun = std::stoi(ini.get("HEADBOB").get("bRun"));
				m_Headbob.bSprint = std::stoi(ini.get("HEADBOB").get("bSprint"));
				m_Headbob.bCombat = std::stoi(ini.get("HEADBOB").get("bCombat"));
				m_Headbob.bSneak = std::stoi(ini.get("HEADBOB").get("bSneak"));
				m_Headbob.bSneakRoll = std::stoi(ini.get("HEADBOB").get("bSneakRoll"));
				m_Headbob.fRotationIdle = std::stof(ini.get("HEADBOB").get("fRotationIdle"));
				m_Headbob.fRotationWalk = std::stof(ini.get("HEADBOB").get("fRotationWalk"));
				m_Headbob.fRotationRun = std::stof(ini.get("HEADBOB").get("fRotationRun"));
				m_Headbob.fRotationSprint = std::stof(ini.get("HEADBOB").get("fRotationSprint"));
				m_Headbob.fRotationCombat = std::stof(ini.get("HEADBOB").get("fRotationCombat"));
				m_Headbob.fRotationSneak = std::stof(ini.get("HEADBOB").get("fRotationSneak"));
				m_Headbob.fRotationSneakRoll = std::stof(ini.get("HEADBOB").get("fRotationSneakRoll"));

				m_Camera.fFirstPersonPosX = std::stof(ini.get("CAMERA").get("fFirstPersonPosX"));
				m_Camera.fFirstPersonPosY = std::stof(ini.get("CAMERA").get("fFirstPersonPosY"));
				m_Camera.fFirstPersonPosZ = std::stof(ini.get("CAMERA").get("fFirstPersonPosZ"));
				m_Camera.fFirstPersonCombatPosX = std::stof(ini.get("CAMERA").get("fFirstPersonCombatPosX"));
				m_Camera.fFirstPersonCombatPosY = std::stof(ini.get("CAMERA").get("fFirstPersonCombatPosY"));
				m_Camera.fFirstPersonCombatPosZ = std::stof(ini.get("CAMERA").get("fFirstPersonCombatPosZ"));
				m_Camera.fHorsePosX = std::stof(ini.get("CAMERA").get("fHorsePosX"));
				m_Camera.fHorsePosY = std::stof(ini.get("CAMERA").get("fHorsePosY"));
				m_Camera.fHorsePosZ = std::stof(ini.get("CAMERA").get("fHorsePosZ"));
				m_Camera.fHorseCombatPosX = std::stof(ini.get("CAMERA").get("fHorseCombatPosX"));
				m_Camera.fHorseCombatPosY = std::stof(ini.get("CAMERA").get("fHorseCombatPosY"));
				m_Camera.fHorseCombatPosZ = std::stof(ini.get("CAMERA").get("fHorseCombatPosZ"));
				m_Camera.fDragonPosX = std::stof(ini.get("CAMERA").get("fDragonPosX"));
				m_Camera.fDragonPosY = std::stof(ini.get("CAMERA").get("fDragonPosY"));
				m_Camera.fDragonPosZ = std::stof(ini.get("CAMERA").get("fDragonPosZ"));
				m_Camera.fVampireLordPosX = std::stof(ini.get("CAMERA").get("fVampireLordPosX"));
				m_Camera.fVampireLordPosY = std::stof(ini.get("CAMERA").get("fVampireLordPosY"));
				m_Camera.fVampireLordPosZ = std::stof(ini.get("CAMERA").get("fVampireLordPosZ"));
				m_Camera.fWerewolfPosX = std::stof(ini.get("CAMERA").get("fWerewolfPosX"));
				m_Camera.fWerewolfPosY = std::stof(ini.get("CAMERA").get("fWerewolfPosY"));
				m_Camera.fWerewolfPosZ = std::stof(ini.get("CAMERA").get("fWerewolfPosZ"));
				m_Camera.fNecroLichPosX = std::stof(ini.get("CAMERA").get("fNecroLichPosX"));
				m_Camera.fNecroLichPosY = std::stof(ini.get("CAMERA").get("fNecroLichPosY"));
				m_Camera.fNecroLichPosZ = std::stof(ini.get("CAMERA").get("fNecroLichPosZ"));
				m_Camera.fScriptedPosX = std::stof(ini.get("CAMERA").get("fScriptedPosX"));
				m_Camera.fScriptedPosY = std::stof(ini.get("CAMERA").get("fScriptedPosY"));
				m_Camera.fScriptedPosZ = std::stof(ini.get("CAMERA").get("fScriptedPosZ"));
			}
		}
		catch (...)
		{
			auto plugin = DLLMain::Plugin::Get();
			std::string errorInfo{};

			if (m_PreInitialized && !m_Initialized)
			{
				m_PreInitialized = false;
				errorInfo = "Invalid Profile: " + m_ModuleData.sProfileName;
				MessageBox(NULL, errorInfo.c_str(), plugin->Description().c_str(), MB_ICONERROR);
			}
			else if (!m_PreInitialized)
			{
				errorInfo = "Invalid: " + m_Name;
				MessageBox(NULL, errorInfo.c_str(), plugin->Description().c_str(), MB_ICONERROR);
			}
			return false;
		}
		return true;
	}

	void Config::WriteIni(std::string& name, bool updateMain)
	{
		if (updateMain)
		{
			/* External Editing Only
			ini["MODULE DATA"]["FileName"] = m_ModuleData.sFileName;
			ini["MODULE DATA"]["FileVersionMin"] = m_ModuleData.sFileVersionMin;
			ini["MODULE DATA"]["FileVersionMax"] = m_ModuleData.sFileVersionMax;
			ini["MODULE DATA"]["WindowName"] = m_ModuleData.sWindowName;
			ini["MODULE DATA"]["CheckCompatibility"] = std::to_string(m_ModuleData.iCheckCompatibility);
			ini["MODULE DATA"]["MenuMode"] = std::to_string(m_ModuleData.iMenuMode);
			ini["MODULE DATA"]["MenuTimeout"] = std::to_string(m_ModuleData.iMenuTimeout);
			ini["MODULE DATA"]["MenuKey"] = std::to_string(m_ModuleData.iMenuKey);
			ini["MODULE DATA"]["MenuFont"] = m_ModuleData.sMenuFont;
			ini["MODULE DATA"]["MenuFontSize"] = std::to_string(m_ModuleData.fMenuFontSize);*/

			mINI::INIFile file(m_FileName.c_str());
			mINI::INIStructure ini;
			file.read(ini);
			ini["MODULE DATA"]["ProfileName"] = m_ModuleData.sProfileName;
			file.write(ini);
		}
		else
		{
			std::string fileName = m_ProfilePath + name.c_str();

			mINI::INIFile file(fileName.c_str());
			mINI::INIStructure ini;
			file.read(ini);

			ini["GENERAL"]["bEnableBody"] = std::to_string(m_General.bEnableBody);
			ini["GENERAL"]["bEnableBodyConsole"] = std::to_string(m_General.bEnableBodyConsole);
			ini["GENERAL"]["bEnableShadows"] = std::to_string(m_General.bEnableShadows);
			ini["GENERAL"]["bAdjustPlayerScale"] = std::to_string(m_General.bAdjustPlayerScale);
			ini["GENERAL"]["fBodyHeightOffset"] = std::to_string(m_General.fBodyHeightOffset);
			ini["GENERAL"]["bEnableHead"] = std::to_string(m_General.bEnableHead);
			ini["GENERAL"]["bEnableHeadCombat"] = std::to_string(m_General.bEnableHeadCombat);
			ini["GENERAL"]["bEnableHeadHorse"] = std::to_string(m_General.bEnableHeadHorse);
			ini["GENERAL"]["bEnableHeadDragon"] = std::to_string(m_General.bEnableHeadDragon);
			ini["GENERAL"]["bEnableHeadVampireLord"] = std::to_string(m_General.bEnableHeadVampireLord);
			ini["GENERAL"]["bEnableHeadWerewolf"] = std::to_string(m_General.bEnableHeadWerewolf);
			ini["GENERAL"]["bEnableHeadScripted"] = std::to_string(m_General.bEnableHeadScripted);
			ini["GENERAL"]["bEnableThirdPersonArms"] = std::to_string(m_General.bEnableThirdPersonArms);
			ini["GENERAL"]["bEnableThirdPersonTorch"] = std::to_string(m_General.bEnableThirdPersonTorch);
			ini["GENERAL"]["bEnableThirdPersonTorchBlock"] = std::to_string(m_General.bEnableThirdPersonTorchBlock);
			ini["GENERAL"]["bEnableThirdPersonShield"] = std::to_string(m_General.bEnableThirdPersonShield);
			ini["GENERAL"]["bEnableThirdPersonShieldBlock"] = std::to_string(m_General.bEnableThirdPersonShieldBlock);
			ini["GENERAL"]["bEnableThirdPersonBow"] = std::to_string(m_General.bEnableThirdPersonBow);
			ini["GENERAL"]["bEnableThirdPersonBowAim"] = std::to_string(m_General.bEnableThirdPersonBowAim);
			ini["GENERAL"]["bEnableThirdPersonCrossbow"] = std::to_string(m_General.bEnableThirdPersonCrossbow);
			ini["GENERAL"]["bEnableThirdPersonCrossbowAim"] = std::to_string(m_General.bEnableThirdPersonCrossbowAim);

			ini["HIDE"]["b2HWeapon"] = std::to_string(m_Hide.b2HWeapon);
			ini["HIDE"]["bBow"] = std::to_string(m_Hide.bBow);
			ini["HIDE"]["bQuiver"] = std::to_string(m_Hide.bQuiver);
			ini["HIDE"]["bSitting"] = std::to_string(m_Hide.bSitting);
			ini["HIDE"]["bSleeping"] = std::to_string(m_Hide.bSleeping);
			ini["HIDE"]["bJumping"] = std::to_string(m_Hide.bJumping);
			ini["HIDE"]["bSwimming"] = std::to_string(m_Hide.bSwimming);
			ini["HIDE"]["bSneakRoll"] = std::to_string(m_Hide.bSneakRoll);
			ini["HIDE"]["bAttack"] = std::to_string(m_Hide.bAttack);
			ini["HIDE"]["bPowerAttack"] = std::to_string(m_Hide.bPowerAttack);
			ini["HIDE"]["bAttackBow"] = std::to_string(m_Hide.bAttackBow);
			ini["HIDE"]["bKillmove"] = std::to_string(m_Hide.bKillmove);

			ini["FIXES"]["bFirstPersonOverhaul"] = std::to_string(m_Fixes.bFirstPersonOverhaul);
			ini["FIXES"]["bOverrideVanillaArmsOnMovement"] = std::to_string(m_Fixes.bOverrideVanillaArmsOnMovement);
			ini["FIXES"]["bArcheryGameplayOverhaul"] = std::to_string(m_Fixes.bArcheryGameplayOverhaul);
			ini["FIXES"]["bSmoothAnimationTransitions"] = std::to_string(m_Fixes.bSmoothAnimationTransitions);
			ini["FIXES"]["fControllerBufferDepth1st"] = std::to_string(m_Fixes.fControllerBufferDepth1st);

			ini["RESTRICT ANGLES"]["fSitting"] = std::to_string(m_RestrictAngles.fSitting);
			ini["RESTRICT ANGLES"]["fSittingMaxLookingUp"] = std::to_string(m_RestrictAngles.fSittingMaxLookingUp);
			ini["RESTRICT ANGLES"]["fSittingMaxLookingDown"] = std::to_string(m_RestrictAngles.fSittingMaxLookingDown);
			ini["RESTRICT ANGLES"]["fMounted"] = std::to_string(m_RestrictAngles.fMounted);
			ini["RESTRICT ANGLES"]["fMountedMaxLookingUp"] = std::to_string(m_RestrictAngles.fMountedMaxLookingUp);
			ini["RESTRICT ANGLES"]["fMountedMaxLookingDown"] = std::to_string(m_RestrictAngles.fMountedMaxLookingDown);
			ini["RESTRICT ANGLES"]["fFlying"] = std::to_string(m_RestrictAngles.fFlying);
			ini["RESTRICT ANGLES"]["fFlyingMaxLookingUp"] = std::to_string(m_RestrictAngles.fFlyingMaxLookingUp);
			ini["RESTRICT ANGLES"]["fFlyingMaxLookingDown"] = std::to_string(m_RestrictAngles.fFlyingMaxLookingDown);
			ini["RESTRICT ANGLES"]["fVampireLord"] = std::to_string(m_RestrictAngles.fVampireLord);
			ini["RESTRICT ANGLES"]["fWerewolf"] = std::to_string(m_RestrictAngles.fWerewolf);
			ini["RESTRICT ANGLES"]["fNecroLich"] = std::to_string(m_RestrictAngles.fNecroLich);
			ini["RESTRICT ANGLES"]["fScripted"] = std::to_string(m_RestrictAngles.fScripted);
			ini["RESTRICT ANGLES"]["fScriptedPitch"] = std::to_string(m_RestrictAngles.fScriptedPitch);

			ini["EVENTS"]["bFirstPerson"] = std::to_string(m_Events.bFirstPerson);
			ini["EVENTS"]["bFirstPersonCombat"] = std::to_string(m_Events.bFirstPersonCombat);
			ini["EVENTS"]["bFurniture"] = std::to_string(m_Events.bFurniture);
			ini["EVENTS"]["bCrafting"] = std::to_string(m_Events.bCrafting);
			ini["EVENTS"]["bKillmove"] = std::to_string(m_Events.bKillmove);
			ini["EVENTS"]["bRagdoll"] = std::to_string(m_Events.bRagdoll);
			ini["EVENTS"]["bDeath"] = std::to_string(m_Events.bDeath);
			ini["EVENTS"]["bCannibal"] = std::to_string(m_Events.bCannibal);
			ini["EVENTS"]["bHorse"] = std::to_string(m_Events.bHorse);
			ini["EVENTS"]["bHorseCombat"] = std::to_string(m_Events.bHorseCombat);
			ini["EVENTS"]["bHorseTransition"] = std::to_string(m_Events.bHorseTransition);
			ini["EVENTS"]["bDragon"] = std::to_string(m_Events.bDragon);
			ini["EVENTS"]["bDragonCombat"] = std::to_string(m_Events.bDragonCombat);
			ini["EVENTS"]["bDragonTransition"] = std::to_string(m_Events.bDragonTransition);
			ini["EVENTS"]["bVampireLord"] = std::to_string(m_Events.bVampireLord);
			ini["EVENTS"]["bWerewolf"] = std::to_string(m_Events.bWerewolf);
			ini["EVENTS"]["bTransform"] = std::to_string(m_Events.bTransform);
			ini["EVENTS"]["bNecroLich"] = std::to_string(m_Events.bNecroLich);
			ini["EVENTS"]["bScripted"] = std::to_string(m_Events.bScripted);
			ini["EVENTS"]["bThirdPerson"] = std::to_string(m_Events.bThirdPerson);

			ini["FOV"]["bEnableOverride"] = std::to_string(m_FOV.bEnableOverride);
			ini["FOV"]["fFirstPerson"] = std::to_string(m_FOV.fFirstPerson);
			ini["FOV"]["fFirstPersonHands"] = std::to_string(m_FOV.fFirstPersonHands);
			ini["FOV"]["fFirstPersonCombat"] = std::to_string(m_FOV.fFirstPersonCombat);
			ini["FOV"]["fFurniture"] = std::to_string(m_FOV.fFurniture);
			ini["FOV"]["fCrafting"] = std::to_string(m_FOV.fCrafting);
			ini["FOV"]["fKillmove"] = std::to_string(m_FOV.fKillmove);
			ini["FOV"]["fRagdoll"] = std::to_string(m_FOV.fRagdoll);
			ini["FOV"]["fDeath"] = std::to_string(m_FOV.fDeath);
			ini["FOV"]["fCannibal"] = std::to_string(m_FOV.fCannibal);
			ini["FOV"]["fHorse"] = std::to_string(m_FOV.fHorse);
			ini["FOV"]["fHorseCombat"] = std::to_string(m_FOV.fHorseCombat);
			ini["FOV"]["fHorseTransition"] = std::to_string(m_FOV.fHorseTransition);
			ini["FOV"]["fDragon"] = std::to_string(m_FOV.fDragon);
			ini["FOV"]["fDragonCombat"] = std::to_string(m_FOV.fDragonCombat);
			ini["FOV"]["fDragonTransition"] = std::to_string(m_FOV.fDragonTransition);
			ini["FOV"]["fVampireLord"] = std::to_string(m_FOV.fVampireLord);
			ini["FOV"]["fWerewolf"] = std::to_string(m_FOV.fWerewolf);
			ini["FOV"]["fNecroLich"] = std::to_string(m_FOV.fNecroLich);
			ini["FOV"]["fScripted"] = std::to_string(m_FOV.fScripted);
			ini["FOV"]["fThirdPerson"] = std::to_string(m_FOV.fThirdPerson);

			ini["NEARDISTANCE"]["bEnableOverride"] = std::to_string(m_NearDistance.bEnableOverride);
			ini["NEARDISTANCE"]["fPitchThreshold"] = std::to_string(m_NearDistance.fPitchThreshold);
			ini["NEARDISTANCE"]["fFirstPerson"] = std::to_string(m_NearDistance.fFirstPerson);
			ini["NEARDISTANCE"]["fFirstPersonCombat"] = std::to_string(m_NearDistance.fFirstPersonCombat);
			ini["NEARDISTANCE"]["fSitting"] = std::to_string(m_NearDistance.fSitting);
			ini["NEARDISTANCE"]["fFurniture"] = std::to_string(m_NearDistance.fFurniture);
			ini["NEARDISTANCE"]["fCrafting"] = std::to_string(m_NearDistance.fCrafting);
			ini["NEARDISTANCE"]["fKillmove"] = std::to_string(m_NearDistance.fKillmove);
			ini["NEARDISTANCE"]["fRagdoll"] = std::to_string(m_NearDistance.fRagdoll);
			ini["NEARDISTANCE"]["fDeath"] = std::to_string(m_NearDistance.fDeath);
			ini["NEARDISTANCE"]["fCannibal"] = std::to_string(m_NearDistance.fCannibal);
			ini["NEARDISTANCE"]["fHorse"] = std::to_string(m_NearDistance.fHorse);
			ini["NEARDISTANCE"]["fHorseCombat"] = std::to_string(m_NearDistance.fHorseCombat);
			ini["NEARDISTANCE"]["fHorseTransition"] = std::to_string(m_NearDistance.fHorseTransition);
			ini["NEARDISTANCE"]["fDragon"] = std::to_string(m_NearDistance.fDragon);
			ini["NEARDISTANCE"]["fDragonCombat"] = std::to_string(m_NearDistance.fDragonCombat);
			ini["NEARDISTANCE"]["fDragonTransition"] = std::to_string(m_NearDistance.fDragonTransition);
			ini["NEARDISTANCE"]["fVampireLord"] = std::to_string(m_NearDistance.fVampireLord);
			ini["NEARDISTANCE"]["fWerewolf"] = std::to_string(m_NearDistance.fWerewolf);
			ini["NEARDISTANCE"]["fNecroLich"] = std::to_string(m_NearDistance.fNecroLich);
			ini["NEARDISTANCE"]["fScripted"] = std::to_string(m_NearDistance.fScripted);
			ini["NEARDISTANCE"]["fThirdPerson"] = std::to_string(m_NearDistance.fThirdPerson);

			ini["HEADBOB"]["bIdle"] = std::to_string(m_Headbob.bIdle);
			ini["HEADBOB"]["bWalk"] = std::to_string(m_Headbob.bWalk);
			ini["HEADBOB"]["bRun"] = std::to_string(m_Headbob.bRun);
			ini["HEADBOB"]["bSprint"] = std::to_string(m_Headbob.bSprint);
			ini["HEADBOB"]["bCombat"] = std::to_string(m_Headbob.bCombat);
			ini["HEADBOB"]["bSneak"] = std::to_string(m_Headbob.bSneak);
			ini["HEADBOB"]["bSneakRoll"] = std::to_string(m_Headbob.bSneakRoll);
			ini["HEADBOB"]["fRotationIdle"] = std::to_string(m_Headbob.fRotationIdle);
			ini["HEADBOB"]["fRotationWalk"] = std::to_string(m_Headbob.fRotationWalk);
			ini["HEADBOB"]["fRotationRun"] = std::to_string(m_Headbob.fRotationRun);
			ini["HEADBOB"]["fRotationSprint"] = std::to_string(m_Headbob.fRotationSprint);
			ini["HEADBOB"]["fRotationCombat"] = std::to_string(m_Headbob.fRotationCombat);
			ini["HEADBOB"]["fRotationSneak"] = std::to_string(m_Headbob.fRotationSneak);
			ini["HEADBOB"]["fRotationSneakRoll"] = std::to_string(m_Headbob.fRotationSneakRoll);

			ini["CAMERA"]["fFirstPersonPosX"] = std::to_string(m_Camera.fFirstPersonPosX);
			ini["CAMERA"]["fFirstPersonPosY"] = std::to_string(m_Camera.fFirstPersonPosY);
			ini["CAMERA"]["fFirstPersonPosZ"] = std::to_string(m_Camera.fFirstPersonPosZ);
			ini["CAMERA"]["fFirstPersonCombatPosX"] = std::to_string(m_Camera.fFirstPersonCombatPosX);
			ini["CAMERA"]["fFirstPersonCombatPosY"] = std::to_string(m_Camera.fFirstPersonCombatPosY);
			ini["CAMERA"]["fFirstPersonCombatPosZ"] = std::to_string(m_Camera.fFirstPersonCombatPosZ);
			ini["CAMERA"]["fHorsePosX"] = std::to_string(m_Camera.fHorsePosX);
			ini["CAMERA"]["fHorsePosY"] = std::to_string(m_Camera.fHorsePosY);
			ini["CAMERA"]["fHorsePosZ"] = std::to_string(m_Camera.fHorsePosZ);
			ini["CAMERA"]["fHorseCombatPosX"] = std::to_string(m_Camera.fHorseCombatPosX);
			ini["CAMERA"]["fHorseCombatPosY"] = std::to_string(m_Camera.fHorseCombatPosY);
			ini["CAMERA"]["fHorseCombatPosZ"] = std::to_string(m_Camera.fHorseCombatPosZ);
			ini["CAMERA"]["fDragonPosX"] = std::to_string(m_Camera.fDragonPosX);
			ini["CAMERA"]["fDragonPosY"] = std::to_string(m_Camera.fDragonPosY);
			ini["CAMERA"]["fDragonPosZ"] = std::to_string(m_Camera.fDragonPosZ);
			ini["CAMERA"]["fVampireLordPosX"] = std::to_string(m_Camera.fVampireLordPosX);
			ini["CAMERA"]["fVampireLordPosY"] = std::to_string(m_Camera.fVampireLordPosY);
			ini["CAMERA"]["fVampireLordPosZ"] = std::to_string(m_Camera.fVampireLordPosZ);
			ini["CAMERA"]["fWerewolfPosX"] = std::to_string(m_Camera.fWerewolfPosX);
			ini["CAMERA"]["fWerewolfPosY"] = std::to_string(m_Camera.fWerewolfPosY);
			ini["CAMERA"]["fWerewolfPosZ"] = std::to_string(m_Camera.fWerewolfPosZ);
			ini["CAMERA"]["fNecroLichPosX"] = std::to_string(m_Camera.fNecroLichPosX);
			ini["CAMERA"]["fNecroLichPosY"] = std::to_string(m_Camera.fNecroLichPosY);
			ini["CAMERA"]["fNecroLichPosZ"] = std::to_string(m_Camera.fNecroLichPosZ);
			ini["CAMERA"]["fScriptedPosX"] = std::to_string(m_Camera.fScriptedPosX);
			ini["CAMERA"]["fScriptedPosY"] = std::to_string(m_Camera.fScriptedPosY);
			ini["CAMERA"]["fScriptedPosZ"] = std::to_string(m_Camera.fScriptedPosZ);

			file.write(ini);
		}
	}

}
