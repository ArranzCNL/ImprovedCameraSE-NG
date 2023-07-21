/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

// CommonLibSSE-NG
// 4005 - macro redefinition
// 4200 - zero-sized array
#pragma warning(disable: 4005 4200)

// Minimum supported Windows version
#define _WIN32_WINNT 0x0601  // Windows 7

// CommonLibSSE-NG configuration
#define ENABLE_SKYRIM_AE
#define ENABLE_SKYRIM_SE
#define SKSE_SUPPORT_XBYAK

// Windows configuration
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

// CommonLibSSE headers
#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>

// Xbyak Support
#ifdef SKSE_SUPPORT_XBYAK
	#include <xbyak/xbyak.h>
#endif

// Additional headers
#include <Psapi.h>
#include <glm/glm.hpp>

using namespace std::literals;

namespace stl {

	using namespace SKSE::stl;

	void asm_replace(std::uintptr_t a_from, std::size_t a_size, std::uintptr_t a_to);

	template <class T>
	void asm_replace(std::uintptr_t a_from)
	{
		asm_replace(a_from, T::size, reinterpret_cast<std::uintptr_t>(T::func));
	}

	template <class T>
	void write_thunk_call(std::uintptr_t a_src)
	{
		auto& trampoline = SKSE::GetTrampoline();
		SKSE::AllocTrampoline(16);

		T::func = trampoline.write_call<T::size>(a_src, T::thunk);
	}

	template <class F, class T>
	void write_vfunc()
	{
		REL::Relocation<std::uintptr_t> vtbl{ F::VTABLE[T::index] };
		T::func = vtbl.write_vfunc(T::offset, T::thunk);
	}

}
