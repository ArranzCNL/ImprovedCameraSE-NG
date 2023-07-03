#pragma once

namespace RE
{
	class BSMemStorage;
	class SaveStorageWrapper;

	namespace BSScript
	{
		class IHandleReaderWriter
		{
		public:
			inline static constexpr auto RTTI = RTTI_BSScript__IHandleReaderWriter;
			inline static constexpr auto VTABLE = VTABLE_BSScript__IHandleReaderWriter;

			virtual ~IHandleReaderWriter();  // 0

			// add
			virtual void Unk_08() = 0;
			virtual bool SaveHandle(SaveStorageWrapper* saveWrapper, VMHandle handle) = 0;
			virtual bool LoadHandle(BSMemStorage* loadWrapper, VMHandle* out) = 0;
			virtual bool SaveRefID(SaveStorageWrapper* saveWrapper, uint32_t handleLow);
			virtual bool ParseScriptInstance(BSMemStorage* loadWrapper, uint32_t* scriptInstanceValues);
		};
		static_assert(sizeof(IHandleReaderWriter) == 0x8);
	}
}
