#include "RE/E/ExtraAliasInstanceArray.h"

#include "RE/M/MemoryManager.h"

namespace RE
{
	ExtraAliasInstanceArray::ExtraAliasInstanceArray() :
		BSExtraData(),
		aliases(),
		lock()
	{
		stl::emplace_vtable(this);
	}

	ExtraAliasInstanceArray::~ExtraAliasInstanceArray()
	{
		for (auto& alias : aliases) {
			free(alias);
		}
	}

	ExtraDataType ExtraAliasInstanceArray::GetType() const
	{
		return ExtraDataType::kAliasInstanceArray;
	}
}
