#pragma once
#include "VGElement.h"
#include "VGStroke.h"
#include "VGGradient.h"

struct VGFillStyle
{
	VGColor Color;
	VGGradientRef Gradient;
};
using VGFillStyleRaw = VGRaw<VGFillStyle>;

struct VGStrokeStyle
{
	VGColor Color;
	VGGradientRef Gradient;
	VGStrokeCap LineCap;
	VGStrokeJoin LineJoin;
	float LineWidth = 0;
	float MiterLimit = 4;
	float DashOffset = 0;
	VGVector<float> DashControl;
};
using VGStrokeStyleRaw = VGRaw<VGStrokeStyle>;

/// @brief 
class VGShapePrivate
{
public:
	virtual ~VGShapePrivate() = default;
};
using VGShapePrivateRaw = VGRaw<VGShapePrivate>;

/// @brief 
class OPENVG_API VGShape : public VGElement
{
public:
	VGShape();
	~VGShape();

	VGFillStyleRaw getFillStyle() const;
	VGStrokeStyleRaw getStrokeStyle() const;

	float getLineWidth() const;
	void setLineWidth(float value);

	float getDashOffset() const;
	void setDashOffset(float value);

	float getMiterLimit() const;
	void setMiterLimit(float value);

	VGStrokeCap getLineCap() const;
	void setLineCap(VGStrokeCap value);

	VGStrokeJoin getLineJoin() const;
	void setLineJoin(VGStrokeJoin value);

	VGArrayView<const float> getDashControl() const;
	void setDashControl(VGVector<float> value);

private:
	VGShapePrivateRaw m_Private;
};
using VGShapeRef = VGRef<VGShape>;
using VGShapeRaw = VGRaw<VGShape>;