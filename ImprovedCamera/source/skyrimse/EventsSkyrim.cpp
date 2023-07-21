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
#include "utils/Log.h"

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
		if (!a_event)
			return EventResult::kContinue;

		if (strcmp(a_event->menuName.c_str(), "Console") == 0)
		{
			auto camera = RE::PlayerCamera::GetSingleton();

			if (camera->IsInFirstPerson())
			{
				auto pluginConfig = DLLMain::Plugin::Get()->Config();
				auto bodyConsole = pluginConfig->General().bEnableBodyConsole;

				auto thirdperson3D = RE::PlayerCharacter::GetSingleton()->Get3D(0);
				if (!thirdperson3D)
					return RE::BSEventNotifyControl::kContinue;

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
		if (!a_event)
			return EventResult::kContinue;

		RE::BSAnimationGraphManagerPtr graphMgr;

		if (a_event->holder->GetAnimationGraphManager(graphMgr) && graphMgr)
		{
			std::int32_t activeGraph = 0;  //graphMgr->GetRuntimeData().activeGraph; // 0 is third person, 1 is first person.

			RE::BShkbAnimationGraphPtr project = graphMgr->graphs[activeGraph];
			auto behaviourGraph = project ? graphMgr->graphs[activeGraph]->behaviorGraph : nullptr;

			if (!behaviourGraph)
				return EventResult::kContinue;

			RE::NodeList activeNodes = *behaviourGraph->activeNodes;
			std::int32_t index = 0;

			if (!activeNodes.empty())
			{
#ifdef _DEBUG
				LOG_DEBUG("Behavior Project: {}", project->projectName);
#endif
				for (auto nodeInfo : activeNodes)
				{
					auto nodeClone = nodeInfo.nodeClone;

					if (nodeClone && nodeClone->GetClassType())
					{
						auto clipGenerator = ClipGeneratorNode(nodeClone);

						if (clipGenerator)
						{
							std::string animationFile = clipGenerator->animationName.c_str();
#ifdef _DEBUG
							LOG_DEBUG("Animation[{}] Name: {}\n\tFile: {}", index, nodeClone->name.c_str(), animationFile);
#endif
							if (index == 0)
							{
								auto pluginCamera = DLLMain::Plugin::Get()->SkyrimSE()->Camera();
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
					index++;
				}
			}
		}
		return EventResult::kContinue;
	}

}
