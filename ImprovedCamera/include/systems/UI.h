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
		virtual LRESULT WndprocHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
		virtual void BeginFrame() = 0;
		virtual void OnUpdate() = 0;
		virtual void EndFrame() = 0;
		virtual void ResizeBuffer(const glm::uvec2 size) = 0;

		const bool IsMenuDisplayed() const { return m_DisplayMenu; };

	protected:
		bool m_DisplayMenu = false;
	};

}
