/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <InitGuid.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

 // Forward declare UIMenu
namespace Menu {
	class UIMenu;
}

namespace Systems {
	class Graphics;
	class Window;
}


namespace Systems {

	typedef HRESULT(STDMETHODCALLTYPE* GetDeviceDataHook)(IDirectInputDevice8* pInputDevice8, DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags);
	typedef HRESULT(STDMETHODCALLTYPE* GetDeviceStateHook)(IDirectInputDevice8* pInputDevice8, DWORD cbData, LPVOID lpvData);

	// DirectInput doesn't have this
	typedef struct _DIKEYBOARDSTATE {
		BYTE rgbKeys[256];
	} DIKEYBOARDSTATE, *LPDIKEYBOARDSTATE;

	class Input {

	public:
		Input();
		~Input() = default;

	private:
		bool m_Initialized = false;
		bool m_MenuShown = false;
		bool m_InitMouse = false;
		bool m_InitKeyboard = false;

		IDirectInput8* m_DirectInput8 = nullptr;
		LPDIRECTINPUTDEVICE8 m_DirectInputMouse = nullptr;
		LPDIRECTINPUTDEVICE8 m_DirectInputKeyboard = nullptr;

		GetDeviceStateHook m_GetDeviceState = nullptr;
		GetDeviceStateHook m_GetDeviceStateMouse = nullptr;
		GetDeviceDataHook m_GetDeviceData = nullptr;
		GetDeviceDataHook m_GetDeviceDataKeyboard = nullptr;

	private:
		void CheckLoader(const std::string& fileName);
		bool GetSystemDirectInput8();
		bool Initialize();
		void MenuDisplayed(bool showMenu);
		void OnUpdate();

		static HRESULT STDMETHODCALLTYPE Hook_GetDeviceState(IDirectInputDevice8* pDirectInputDevice8, DWORD cbData, LPVOID lpvData);
		static HRESULT STDMETHODCALLTYPE Hook_GetDeviceData(IDirectInputDevice8* pDirectInputDevice8, DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags);

		friend Menu::UIMenu;
		friend Graphics;
		friend Window;
	};

}
