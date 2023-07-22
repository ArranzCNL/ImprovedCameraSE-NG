/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

namespace Systems {

	class Window {

	public:
		Window();
		~Window();

	public:
		struct UIDisplay {
			enum
			{
				kNone = 0,
				kOverlay,
				kInternal,

				kTotal
			};
		};

	private:
		struct Prop {
			HWND hWnd = nullptr;
			WNDPROC Wndproc = nullptr;
			std::string name{};

			glm::ivec2 size = { 0, 0 };
			glm::ivec2 position = { 0, 0 };

		} properties;

	public:
		const Prop& Properties() const { return *m_Properties; };
		void ClipMouseCursor(HWND window);

	private:
		bool m_Initialized = false;
		bool m_WindowSystemRunning = true;
		bool m_CaptureMsgs = false;

		Prop* m_Properties = nullptr;
		HWND m_MenuHwnd = nullptr;

		std::int32_t m_MenuMode = UIDisplay::kNone;

	private:
		bool CreateOverlay();
		void RunOverlay();

	private:
		static LRESULT CALLBACK ApplicationMessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		friend class Graphics;
	};

}
