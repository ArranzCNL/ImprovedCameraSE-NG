/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

// Windows configuration
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

// CommonLibSSE-NG headers
// 4005 - macro redefinition
// 4200 - zero-sized array
#pragma warning(push)
#pragma warning(disable: 4005 4200)
#include <RE/Skyrim.h>
#pragma warning(pop)
#include <SKSE/SKSE.h>
#include <xbyak/xbyak.h>

// Additional headers
#include <Psapi.h>

// OpenGL Mathematics 
// 4201 - nameless struct/union
#pragma warning(push)
#pragma warning(disable: 4201)
#include <glm/glm.hpp>
#pragma warning(pop)

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
		SKSE::AllocTrampoline(14);

		T::func = trampoline.write_call<T::size>(a_src, T::thunk);
	}

	template <class F, class T>
	void write_vfunc()
	{
		REL::Relocation<std::uintptr_t> vtbl{ F::VTABLE[0] };
		T::func = vtbl.write_vfunc(T::index, T::thunk);
	}

}
