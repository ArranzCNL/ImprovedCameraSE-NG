/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

// Precompiled Header
#include "PCH.h"

/* Based upon learn_more's findPattern_v2 */
#include "utils/PatternScan.h"

#include <tlhelp32.h>

namespace Utils {

	#define INRANGE(x, a, b)    (x >= a && x <= b) 
	#define getBits(x)          (INRANGE(x, '0', '9') ? (x - '0') : ((x&(~0x20)) - 'A' + 0xa))
	#define getByte(x)          (getBits(x[0]) << 4 | getBits(x[1]))

	static inline bool CompareBytes(const PBYTE buffer, const PBYTE pattern, const PBYTE mask)
	{
		std::size_t i = 0;

		while (buffer[i] == pattern[i] || mask[i] == (BYTE)'?')
		{
			if (!mask[++i])
				return true;
		}
		return false;
	}

	static PBYTE SearchPattern(const PBYTE buffer, std::size_t length, const char* patternString)
	{
		if (!buffer || !length || !patternString)
			return nullptr;

		std::size_t len = strlen(patternString);
		PBYTE patternBase = (PBYTE)_malloca(len >> 1);
		PBYTE maskBase = (PBYTE)_malloca(len >> 1);
		PBYTE pattern = patternBase;
		PBYTE mask = maskBase;
		len = 0;

		if (pattern == nullptr || mask == nullptr)
			return nullptr;

		while (*patternString)
		{
			if (*patternString == ' ')
				patternString++;

			if (!*patternString)
				break;

			if (*(PBYTE)patternString == (BYTE)'\?')
			{
				*pattern++ = 0;
				*mask++ = '?';
				patternString += ((*(PWORD)patternString == (WORD)'\?\?') ? 2 : 1);
			}
			else
			{
				*pattern++ = getByte(patternString);
				*mask++ = 'x';
				patternString += 2;
			}
			len++;
		}
		*mask = 0;
		pattern = patternBase;
		mask = maskBase;

		for (DWORD i = 0; i < (length - len); ++i)
		{
			if (CompareBytes(buffer + i, patternBase, maskBase))
			{
				_freea(patternBase);
				_freea(maskBase);
				return buffer + i;
			}
		}
		_freea(patternBase);
		_freea(maskBase);

		return nullptr;
	}

	static PBYTE FindPatternInternal(const char* moduleName, const char* patternString)
	{
		if (!moduleName)
			return nullptr;

		MODULEINFO moduleInfo{};
		HMODULE hModule = GetModuleHandleA(moduleName);

		if (!hModule)
			return nullptr;

		GetModuleInformation(GetCurrentProcess(), hModule, &moduleInfo, sizeof(MODULEINFO));
		PBYTE baseAddress = reinterpret_cast<PBYTE>(moduleInfo.lpBaseOfDll);
		std::uint64_t imageSize = (std::uint64_t)moduleInfo.SizeOfImage;

		PBYTE address{ nullptr };
		MEMORY_BASIC_INFORMATION mbi{};

		VirtualQuery(baseAddress, &mbi, sizeof(mbi));

		for (PBYTE currentAddress = baseAddress; currentAddress < baseAddress + imageSize; currentAddress += mbi.RegionSize)
		{
			if (!VirtualQuery(currentAddress, &mbi, sizeof(mbi)))
				continue;

			if (mbi.State != MEM_COMMIT || mbi.Protect == PAGE_NOACCESS)
				continue;

			address = SearchPattern(currentAddress, mbi.RegionSize, patternString);

			if (address != nullptr)
				break;
		}
		return address;
	}

	static PBYTE FindPatternExternal(const char* moduleName, const char* patternString)
	{
		if (!moduleName)
			return nullptr;

		MODULEINFO moduleInfo{};
		HMODULE hModule = GetModuleHandleA(moduleName);

		if (!hModule)
			return nullptr;

		// Create toolhelp snapshot for modules
		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, 0);
		MODULEENTRY32 process{};
		process.dwSize = sizeof(process);
		DWORD pid = 0;

		if (Module32First(snapshot, &process))
		{
			do
			{
				if (_stricmp(process.szModule, moduleName) == 0)
				{
					pid = process.th32ProcessID;
					break;
				}
			} while (Module32Next(snapshot, &process));
		}

		CloseHandle(snapshot);

		if (!pid)
			return nullptr;

		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		GetModuleInformation(hProcess, hModule, &moduleInfo, sizeof(MODULEINFO));
		PBYTE baseAddress = reinterpret_cast<PBYTE>(moduleInfo.lpBaseOfDll);
		std::uint64_t imageSize = (std::uint64_t)moduleInfo.SizeOfImage;

		MEMORY_BASIC_INFORMATION mbi{};
		mbi.RegionSize = 4096;

		VirtualQueryEx(hProcess, baseAddress, &mbi, sizeof(mbi));

		DWORD oldprotect = 0;
		size_t bytesRead = 0;
		PBYTE buffer{ nullptr };
		PBYTE address{ nullptr };

		for (PBYTE currentAddress = baseAddress; currentAddress < baseAddress + imageSize; currentAddress += mbi.RegionSize)
		{
			if (!VirtualQueryEx(hProcess, currentAddress, &mbi, sizeof(mbi)))
				continue;

			if (mbi.State != MEM_COMMIT || mbi.Protect == PAGE_NOACCESS)
				continue;

			delete[] buffer;
			buffer = new BYTE[mbi.RegionSize];

			if (VirtualProtectEx(hProcess, mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &oldprotect))
			{
				ReadProcessMemory(hProcess, mbi.BaseAddress, buffer, mbi.RegionSize, &bytesRead);
				VirtualProtectEx(hProcess, mbi.BaseAddress, mbi.RegionSize, oldprotect, &oldprotect);

				PBYTE patternAddress = SearchPattern(buffer, bytesRead, patternString);

				if (patternAddress != nullptr)
				{
					address = currentAddress + (patternAddress - buffer);
					break;
				}
			}
		}
		delete[] buffer;
		CloseHandle(hProcess);

		return address;
	}

	PBYTE FindPattern(const char* moduleName, const char* patternString, bool externalScan)
	{
		PBYTE match{ nullptr };

		if (externalScan)
			match = FindPatternExternal(moduleName, patternString);
		else
			match = FindPatternInternal(moduleName, patternString);

		return match;
	}

}
