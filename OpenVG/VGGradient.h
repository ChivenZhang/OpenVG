#pragma once
#include "VG.h"

/// @brief 
class OPENVG_API VGGradient
{
public:
	virtual ~VGGradient() = default;

	virtual VGArrayView<VGColorStop> getColorStop() const = 0;

	virtual void setColorStop(VGVector<VGColorStop> value) = 0;
};
using VGGradientRaw = VGRaw<VGGradient>;
using VGGradientRef = VGRef<VGGradient>;

class OPENVG_API VGLinearGradient : public VGGradient
{
public:
	VGLinearGradient(float x0, float y0, float x1, float y1);

	VGPoint getStartPos() const;

	VGPoint getEndPos() const;
};

class OPENVG_API VGRadialGradient : public VGGradient
{
public:
	VGRadialGradient(float x0, float y0, float r0);

	VGPoint getCenterPos() const;

	float getRadius() const;
};