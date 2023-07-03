#pragma once

#include "RE/B/BSAtomic.h"
#include "RE/I/IMemoryPagePolicy.h"

namespace RE
{
	namespace BSScript
	{
		class SimpleAllocMemoryPagePolicy : public IMemoryPagePolicy
		{
		public:
			inline static constexpr auto RTTI = RTTI_BSScript__SimpleAllocMemoryPagePolicy;
			inline static constexpr auto VTABLE = VTABLE_BSScript__SimpleAllocMemoryPagePolicy;

			~SimpleAllocMemoryPagePolicy() override;  // 00

			// override (IMemoryPagePolicy)
			std::uint32_t    MaximumPageSize() override;                                                                                         // 01 - { return maxPageSize; }
			AllocationStatus AllocatePage(std::uint32_t a_pageSize, NewPageStrategy a_stategy, BSTAutoPointer<MemoryPage>& a_newPage) override;  // 02
			AllocationStatus GetLargestAvailablePage(BSTAutoPointer<MemoryPage>& a_newPage) override;                                            // 03
			void             DisposePage(BSTAutoPointer<MemoryPage>& a_newPage) override;                                                        // 04

			// members
			const std::uint32_t minPageSize{ 0 };          // 08
			const std::uint32_t maxPageSize{ 0 };          // 0C
			const std::uint32_t maxAllocatedMemory{ 0 };   // 10
			const bool          ignoreMemoryLimit{ 0 };    // 14 - Set each update by overstress status
			BSSpinLock          dataLock;                  // 18
			std::uint32_t       currentMemorySize;         // 20
			std::uint32_t       maxAdditionalAllocations;  // 20
		};
		static_assert(sizeof(SimpleAllocMemoryPagePolicy) == 0x28);
	}
}
