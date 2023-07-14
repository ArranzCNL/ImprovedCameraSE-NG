/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

 // Precompiled Header
#include "stdafx.h"

#include "skyrimse/EventsSkyrim.h"

#include "plugin.h"
#include "skyrimse/ImprovedCameraSE.h"


namespace Events {

	static inline RE::hkbClipGenerator* ClipGeneratorNode(RE::hkbNode* a_node)
	{
		if (a_node && a_node->GetClassType())
		{
			if (strcmp(a_node->GetClassType()->name, "hkbClipGenerator") == 0)
				return skyrim_cast<RE::hkbClipGenerator*>(a_node);
		}
		return nullptr;
	}

	void Observer::Register()
	{
		auto player = RE::PlayerCharacter::GetSingleton();
		player->AddAnimationGraphEventSink(Observer::Get());

		RE::UI::GetSingleton()->AddEventSink<RE::MenuOpenCloseEvent>(Observer::Get());
	}

	EventResult Observer::ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*)
	{
		if (strcmp(a_event->menuName.c_str(), "Console") == 0)
		{
			auto camera = RE::PlayerCamera::GetSingleton();

			if (camera->IsInFirstPerson())
			{
				auto pluginConfig = DLLMain::Plugin::Get()->Config();
				auto bodyConsole = pluginConfig->General().bEnableBodyConsole;

				auto thirdperson3D = RE::PlayerCharacter::GetSingleton()->Get3D(0);
				if (!thirdperson3D)
					return RE::BSEventNotifyControl::kContinue;;

				auto thirdpersonNode = thirdperson3D->AsNode();

				if (a_event->opening && !bodyConsole)
					thirdpersonNode->GetFlags().set(RE::NiAVObject::Flag::kHidden);
				else
					thirdpersonNode->GetFlags().reset(RE::NiAVObject::Flag::kHidden);
			}
		}
		return RE::BSEventNotifyControl::kContinue;
	}

	EventResult Observer::ProcessEvent(const RE::BSAnimationGraphEvent* a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>*)
	{
		if (a_event)
		{
			auto player = RE::PlayerCharacter::GetSingleton();
			RE::BSAnimationGraphManagerPtr graphMgr;

			if (player->GetAnimationGraphManager(graphMgr) && graphMgr)
			{
				auto project = &graphMgr->graphs[0];
				auto behaviourGraph = project ? graphMgr->graphs[0]->behaviorGraph : nullptr;
				auto activeNodes = behaviourGraph ? behaviourGraph->activeNodes : nullptr;

				if (activeNodes)
				{
					auto nodeInfo = activeNodes->data();
					auto nodeClone = nodeInfo->nodeClone;

					if (nodeClone && nodeClone->GetClassType())
					{
						auto clipGenerator = ClipGeneratorNode(nodeClone);

						if (clipGenerator)
						{
							auto pluginCamera = DLLMain::Plugin::Get()->SkyrimSE()->Camera();
							std::string animationFile = clipGenerator->animationName.c_str();
							std::string elderscroll = "IdleReadElderScroll";
							std::string cartRiding = "CartPrisonerCSway";

							if (animationFile.find(elderscroll) != std::string::npos)
								pluginCamera->SetElderScrollReading(true);
							else
								pluginCamera->SetElderScrollReading(false);

							if (animationFile.find(cartRiding) != std::string::npos)
								pluginCamera->SetCartRiding(true);
							else
								pluginCamera->SetCartRiding(false);
						}
					}
				}
			}
		}
		return EventResult::kContinue;
	}

}
