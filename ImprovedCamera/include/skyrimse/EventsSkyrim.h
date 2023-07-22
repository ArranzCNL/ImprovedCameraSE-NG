/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

namespace Events {

	using EventResult = RE::BSEventNotifyControl;

	class Observer : public RE::BSTEventSink<RE::MenuOpenCloseEvent>, public RE::BSTEventSink<RE::BSAnimationGraphEvent> {

	public:
		Observer() = default;
		virtual ~Observer() = default;

		Observer(const Observer&) = delete;
		Observer& operator=(const Observer&) = delete;
		Observer(Observer&&) = delete;
		Observer& operator=(Observer&&) = delete;

		static Observer* Get()
		{
			static Observer s_Instance;
			return &s_Instance;
		};

	public:
		static void Register();
		void CheckSPIM();

		virtual EventResult ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) override;
		virtual EventResult ProcessEvent(const RE::BSAnimationGraphEvent* a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_eventSource) override;

	private:
		void ResetArms();

		std::uint32_t* m_SPIMInventoryMenu = nullptr;
		std::uint32_t* m_SPIMMagicMenu = nullptr;
	};

}
