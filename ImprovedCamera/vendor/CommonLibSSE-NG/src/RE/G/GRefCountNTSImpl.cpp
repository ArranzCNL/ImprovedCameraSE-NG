#include "RE/G/GRefCountNTSImpl.h"

namespace RE
{
	void GRefCountNTSImpl::AddRef()
	{
		_refCount = _refCount + 1;
	}

	void GRefCountNTSImpl::Release()
	{
		_refCount = _refCount - 1;
		if (_refCount == 0) {
			delete this;
		}
	}
}
