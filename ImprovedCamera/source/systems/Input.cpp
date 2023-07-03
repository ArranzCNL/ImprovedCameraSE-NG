/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

 // Precompiled Header
#include "stdafx.h"

#include "systems/Input.h"

#include "plugin.h"
#include "utils/Log.h"
#include "utils/Utils.h"

#include <MinHook.h>


namespace Systems {

    typedef HRESULT(WINAPI* pDirectInput8Create)(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter);
    static pDirectInput8Create DirectInput8SystemCreate = nullptr;

    HRESULT STDMETHODCALLTYPE Input::Hook_GetDeviceData(IDirectInputDevice8* pDirectInputDevice8, DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags)
    {
        auto pluginInput = DLLMain::Plugin::Get()->Input();
        auto pluginGraphics = DLLMain::Plugin::Get()->Graphics();

        if (!pluginInput->m_InitKeyboard)
        {
            pDirectInputDevice8->Unacquire();
            pDirectInputDevice8->SetCooperativeLevel(pluginGraphics->Window()->Properties().hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
            pDirectInputDevice8->Acquire();

            pluginInput->m_InitKeyboard = true;
        }
        HRESULT result = pluginInput->m_GetDeviceData(pDirectInputDevice8, cbObjectData, rgdod, pdwInOut, dwFlags);

        if (result == DI_OK && pluginInput->m_MenuShown)
            *pdwInOut = 0;

        return result;
    }

    HRESULT STDMETHODCALLTYPE Input::Hook_GetDeviceState(IDirectInputDevice8* pDirectInputDevice8, DWORD cbData, LPVOID lpvData)
    {
        auto pluginInput = DLLMain::Plugin::Get()->Input();
        auto pluginGraphics = DLLMain::Plugin::Get()->Graphics();

        // Mouse State 2 (Most apps should be using this version)
        if ((!pluginInput->m_InitMouse || pluginInput->m_MenuShown) && cbData == sizeof(DIMOUSESTATE2))
        {
            pDirectInputDevice8->Unacquire();
            pDirectInputDevice8->SetCooperativeLevel(pluginGraphics->Window()->Properties().hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
            pDirectInputDevice8->Acquire();

            pluginInput->m_InitMouse = true;
        }
        HRESULT result = pluginInput->m_GetDeviceState(pDirectInputDevice8, cbData, lpvData);

        if (result == DI_OK && cbData == sizeof(DIMOUSESTATE2) && pluginInput->m_MenuShown)
        {
            LPDIMOUSESTATE2 mouseState2 = (LPDIMOUSESTATE2)lpvData;
            memset(mouseState2->rgbButtons, 0, sizeof(mouseState2->rgbButtons) / sizeof(BYTE));
        }
        return result;
    }

    Input::Input()
    {
        LOG_INFO("Checking for input loaders...");
        CheckLoader("dinput8.dll");
        LOG_INFO("Finished checking for input loaders.");

        bool getDirectInput8 = GetSystemDirectInput8();

        if (getDirectInput8)
        {
            auto pluginConfig = DLLMain::Plugin::Get()->Config();

            if (pluginConfig->ModuleData().iMenuMode == Systems::Window::MenuDisplay::kInternal)
            {
                if (Initialize())
                    m_Initialized = true;
            }
        }
        else
            LOG_ERROR("Failed to find system dinput8.dll.");
    }

    bool Input::Initialize()
    {
        if (DirectInput8SystemCreate(GetModuleHandleA(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&m_DirectInput8, nullptr) != DI_OK)
        {
            LOG_ERROR("  DirectInput8Create failed!");
            return false;
        }

        if (m_DirectInput8->CreateDevice(GUID_SysMouse, &m_DirectInputMouse, nullptr) != DI_OK)
        {
            m_DirectInput8->Release();
            m_DirectInput8 = nullptr;
            LOG_ERROR("  CreateDevice Mouse failed!");
            return false;
        }

        if (m_DirectInput8->CreateDevice(GUID_SysKeyboard, &m_DirectInputKeyboard, nullptr) != DI_OK)
        {
            m_DirectInput8->Release();
            m_DirectInput8 = nullptr;
            LOG_ERROR("  CreateDevice Keyboard failed!");
            return false;
        }

        void** ppDirectInput8DeviceMouseVtbl = *reinterpret_cast<void***>(m_DirectInputMouse);
        m_GetDeviceStateMouse = (GetDeviceStateHook)ppDirectInput8DeviceMouseVtbl[9];

        void** ppDirectInput8DeviceKeyboardVtbl = *reinterpret_cast<void***>(m_DirectInputKeyboard);
        m_GetDeviceDataKeyboard = (GetDeviceDataHook)ppDirectInput8DeviceKeyboardVtbl[10];

        m_DirectInputKeyboard->Release();
        m_DirectInputKeyboard = nullptr;
        m_DirectInputMouse->Release();
        m_DirectInputMouse = nullptr;
        m_DirectInput8->Release();
        m_DirectInput8 = nullptr;

        MH_STATUS status = MH_Initialize();
        if (status != MH_OK && status != MH_ERROR_ALREADY_INITIALIZED)
            return false;

        status = MH_CreateHook(reinterpret_cast<LPVOID>(m_GetDeviceStateMouse), reinterpret_cast<LPVOID>(Hook_GetDeviceState), reinterpret_cast<LPVOID*>(&m_GetDeviceState));
        if (status != MH_OK)
        {
            LOG_ERROR("  MinHook:\t\t\t\tFailed to CreateHook [IDirectInputDevice8] GetDeviceState. Error Code: ({})", (std::int32_t)status);
            return false;
        }
        if (MH_EnableHook(reinterpret_cast<LPVOID>(m_GetDeviceStateMouse)) != MH_OK)
        {
            LOG_ERROR("  MinHook:\t\t\t\tFailed to EnableHook for [IDirectInputDevice8] GetDeviceState.");
            return false;
        }
        LOG_TRACE("  MinHook:\t\t\t\tHooked [IDirectInputDevice8] GetDeviceState.");

        status = MH_CreateHook(reinterpret_cast<LPVOID>(m_GetDeviceDataKeyboard), reinterpret_cast<LPVOID>(Hook_GetDeviceData), reinterpret_cast<LPVOID*>(&m_GetDeviceData));
        if (status != MH_OK)
        {
            LOG_ERROR("  MinHook:\t\t\t\tFailed to CreateHook for [IDirectInputDevice8] GetDeviceData. Error Code: ({})", (std::int32_t)status);
            return false;
        }
        if (MH_EnableHook(reinterpret_cast<LPVOID>(m_GetDeviceDataKeyboard)) != MH_OK)
        {
            LOG_ERROR("  MinHook:\t\t\t\tFailed to EnableHook for [IDirectInputDevice8] GetDeviceData.");
            return false;
        }
        LOG_TRACE("  MinHook:\t\t\t\tHooked [IDirectInputDevice8] GetDeviceData.");

        return true;
    }

    void Input::CheckLoader(const std::string& fileName)
    {
        auto pluginSkyrimSE = DLLMain::Plugin::Get()->SkyrimSE();

        std::string fullFilePath = pluginSkyrimSE->Path() + fileName;
        std::string productName = Utils::GetFileInfo(fullFilePath, "ProductName");

        if (!productName.empty())
        {
            std::string fileVersion = Utils::GetFileInfo(fullFilePath, "ProductVersion");
            LOG_TRACE("  Found:\t\t\t\t{} v{} ({})", productName.c_str(), fileVersion.c_str(), fileName.c_str());
        }
    }

    bool Input::GetSystemDirectInput8()
    {
        // Get the pointer to DirectInput8Create via System directory for dinput8.dll
        if (!DirectInput8SystemCreate)
        {
            char dllFile[MAX_PATH]{};
            if (!GetSystemDirectoryA(dllFile, MAX_PATH))
                return false;

            strcat_s(dllFile, MAX_PATH * sizeof(char), "\\dinput8.dll");

            if (HMODULE hDInput8 = LoadLibraryA(dllFile))
                DirectInput8SystemCreate = (pDirectInput8Create)GetProcAddress(hDInput8, "DirectInput8Create");

            if (!DirectInput8SystemCreate)
                return false;

            return true;
        }
        return false;
    }

    void Input::MenuDisplayed(bool showMenu)
    {
        if (!m_Initialized) return;

        // Fix held down left shift
        if (!showMenu)
        {
            INPUT ipKeyboard{};
            // Send KeyUP event for Esc
            ipKeyboard.type = INPUT_KEYBOARD;
            ipKeyboard.ki.wScan = DIK_ESCAPE;
            ipKeyboard.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
            SendInput(1, &ipKeyboard, sizeof(INPUT));
            // Send KeyUP event for Left Shift
            ipKeyboard.type = INPUT_KEYBOARD;
            ipKeyboard.ki.wScan = DIK_LSHIFT;
            ipKeyboard.ki.dwFlags = KEYEVENTF_SCANCODE;
            SendInput(1, &ipKeyboard, sizeof(INPUT));
            Sleep(10);
            ipKeyboard.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
            SendInput(1, &ipKeyboard, sizeof(INPUT));
        }

        m_MenuShown = showMenu;
    }

    void Input::OnUpdate()
    {
        return;
    }

}
