#pragma once

namespace RE
{
	class VRDeviceConnectionChange;
	class VROverlayChange;
	class VRResetHMDHeight;

	class BSVRInterface :
		public BSTEventSource<VROverlayChange>,
		public BSTEventSource<VRDeviceConnectionChange>,
		public BSTEventSource<VRResetHMDHeight>
	{
	public:
#if !defined(ENABLE_SKYRIM_AE) && !defined(ENABLE_SKYRIM_SE)
		constexpr static auto RTTI = RTTI_BSVRInterface;
#endif

		virtual ~BSVRInterface();  // 0

		// add
		virtual void* Shutdown(void) = 0;  // 01
		virtual void  Unk_02(void) = 0;    // 02
		virtual void  Unk_03(void) = 0;    // 03
		virtual void  Unk_04(void) = 0;    // 04
		virtual void  Unk_05(void) = 0;    // 05
		virtual void  Unk_06(void) = 0;    // 06
		virtual void  Unk_07(void) = 0;    // 07
		virtual void  Unk_08(void) = 0;    // 08
		virtual void  Unk_09(void) = 0;    // 09
		virtual void  Unk_0A(void) = 0;    // 0A
		virtual void  Unk_0B(void) = 0;    // 0B
		virtual void  Unk_0C(void) = 0;    // 0C -- { return 0; }
		virtual void  Unk_0D(void) = 0;    // 0D
		virtual void  Unk_0E(void) = 0;    // 0E
		virtual void  Unk_0F(void) = 0;    // 0F
		virtual void  Unk_10(void) = 0;    // 10
		virtual void  Unk_11(void);        // 11 -- { return; }
		virtual void  Unk_12(void) = 0;    // 12
		virtual void  Unk_13(void) = 0;    // 13
		virtual void  Unk_14(void) = 0;    // 14
		virtual void  Unk_15(void);        // 15 -- { return; }
		virtual void  Unk_16(void) = 0;    // 16
		virtual void  Unk_17(void) = 0;    // 17
	};
	static_assert(sizeof(BSVRInterface) == 0x110);
}
