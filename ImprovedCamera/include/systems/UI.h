/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once


namespace Systems {

	class UI {

	public:
		static std::unique_ptr<UI> CreateMenu(HWND windowMenu);
		virtual ~UI() = default;

		virtual bool Initialize() = 0;
		virtual LRESULT WndprocHandler(const HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam) = 0;
		virtual void BeginFrame() = 0;
		virtual void OnUpdate() = 0;
		virtual void EndFrame() = 0;
		virtual void ResizeBuffer(const glm::uvec2 size) = 0;
		// Keyboard input
		virtual void AddKeyEvent(const std::uint32_t key, const bool down) = 0;
		virtual void AddKeyModEvent(const std::uint32_t key, const bool down) = 0;
		virtual void AddCharacterEvent(const std::uint32_t character) = 0;
		// Mouse input
		virtual void AddMousePosEvent(const float x_pos, const float y_pos) = 0;
		virtual void AddMouseButtonEvent(const std::uint32_t button, const bool down) = 0;
		virtual void AddMouseWheelEvent(const float x_wheel, const float y_wheel) = 0;

		const bool IsUIDisplayed() const { return m_DisplayUI; };

	protected:
		bool m_DisplayUI = false;
	};

}
