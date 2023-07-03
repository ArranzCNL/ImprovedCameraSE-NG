#include "RE/N/NiTransform.h"

namespace RE
{
	NiTransform NiTransform::Invert() const
	{
		NiTransform ret;
		ret.rotate = rotate.Transpose();
		ret.scale = 1.f / scale;
		ret.translate = (ret.rotate * -translate) * ret.scale;
		return ret;
	}

	bool NiTransform::operator==(const NiTransform& a_rhs) const
	{
		return (translate == a_rhs.translate && rotate == a_rhs.rotate && scale == a_rhs.scale);
	}

	bool NiTransform::operator!=(const NiTransform& a_rhs) const
	{
		return !operator==(a_rhs);
	}

	NiTransform NiTransform::operator*(const NiTransform& a_rhs) const
	{
		NiTransform ret;
		ret.scale = scale * a_rhs.scale;
		ret.rotate = rotate * a_rhs.rotate;
		ret.translate = translate + (rotate * a_rhs.translate) * scale;
		return ret;
	}

	NiPoint3 NiTransform::operator*(const NiPoint3& a_point) const
	{
		return (((rotate * a_point) * scale) + translate);
	}
}
