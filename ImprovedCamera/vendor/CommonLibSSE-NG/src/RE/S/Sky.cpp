#include "RE/S/Sky.h"

#include "RE/T/TESWeather.h"

namespace RE
{
	Sky* Sky::GetSingleton()
	{
		using func_t = decltype(&Sky::GetSingleton);
		REL::Relocation<func_t> func{ Offset::Sky::GetSingleton };
		return func();
	}

	bool Sky::IsRaining() const
	{
		return (currentWeather && currentWeather->data.flags.any(TESWeather::WeatherDataFlag::kRainy) && (currentWeather->data.precipitationBeginFadeIn * (1.0f / 255.0f)) < currentWeatherPct) ||
		       (lastWeather && lastWeather->data.flags.any(TESWeather::WeatherDataFlag::kRainy) && ((lastWeather->data.precipitationEndFadeOut * (1.0f / 255.0f)) + 0.001f) > currentWeatherPct);
	}

	bool Sky::IsSnowing() const
	{
		return (currentWeather && currentWeather->data.flags.any(TESWeather::WeatherDataFlag::kSnow) && (currentWeather->data.precipitationBeginFadeIn * (1.0f / 255.0f)) < currentWeatherPct) ||
		       (lastWeather && lastWeather->data.flags.any(TESWeather::WeatherDataFlag::kSnow) && ((lastWeather->data.precipitationEndFadeOut * (1.0f / 255.0f)) + 0.001f) > currentWeatherPct);
	}

	void Sky::SetWeather(TESWeather* a_weather, bool a_override, bool a_accelerate)
	{
		using func_t = decltype(&Sky::SetWeather);
		REL::Relocation<func_t> func{ Offset::Sky::SetWeather };
		func(this, a_weather, a_override, a_accelerate);
	}

	void Sky::ForceWeather(TESWeather* a_weather, bool a_override)
	{
		using func_t = decltype(&Sky::ForceWeather);
		REL::Relocation<func_t> func{ Offset::Sky::ForceWeather };
		func(this, a_weather, a_override);
	}

	void Sky::ResetWeather()
	{
		using func_t = decltype(&Sky::ResetWeather);
		REL::Relocation<func_t> func{ Offset::Sky::ResetWeather };
		func(this);
	}
}
