#include "RE/M/Main.h"

#include "RE/N/NiCamera.h"

namespace RE
{
	Main* Main::GetSingleton()
	{
		REL::Relocation<Main**> singleton{ Offset::Main::Singleton };
		return *singleton;
	}

	float Main::QFrameAnimTime()
	{
		REL::Relocation<float*> data{ RELOCATION_ID(516940, 403447) };
		return *data;
	}

	NiCamera* Main::WorldRootCamera()
	{
		using func_t = decltype(&Main::WorldRootCamera);
		REL::Relocation<func_t> func{ RELOCATION_ID(35601, 36609) };
		return func();
	}
}
