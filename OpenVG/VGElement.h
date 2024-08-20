#pragma once
#include "VGPainter.h"
#include "VGGradient.h"
class VGContext;

/// @brief 
struct VGFillStyle
{
	VGColor Color;
	VGImage Image;
	VGGradientRef Gradient;
};
using VGFillStyleRef = VGRef<VGFillStyle>;
using VGFillStyleRaw = VGRaw<VGFillStyle>;

/// @brief 
struct VGStrokeStyle
{
	VGColor Color;
	VGImage Image;
	VGGradientRef Gradient;
	VGStrokeCap LineCap;
	VGStrokeJoin LineJoin;
	float LineWidth = 0;
	float MiterLimit = 4;
	float DashOffset = 0;
	VGVector<float> DashControl;
};
using VGStrokeStyleRef = VGRef<VGStrokeStyle>;
using VGStrokeStyleRaw = VGRaw<VGStrokeStyle>;

/// @brief 
class VGElementPrivate
{
public:
	virtual ~VGElementPrivate() = default;
};
using VGElementPrivateRaw = VGRaw<VGElementPrivate>;

/// @brief 
class OPENVG_API VGElement
{
public:
	VGElement();
	virtual ~VGElement();

	void moveTo(float x, float y);
	void lineTo(float x, float y);
	void cubicTo(float cx1, float cy1, float cx2, float cy2, float x, float y);
	void close();
	void reset();

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
	VGFillStyleRaw getFillStyle() const;
	VGStrokeStyleRaw getStrokeStyle() const;

	VGArrayView<VGPoint> getPointList() const;
	VGArrayView<VGPointType> getPointTypeList() const;

	VGPrimitiveRaw getClipCache() const;
	VGPrimitiveRaw getFillCache() const;
	VGPrimitiveRaw getStrokeCache() const;

protected:
	VGElementPrivateRaw m_Private;

	friend class VGContext;
};
using VGElementRef = VGRef<VGElement>;
using VGElementRaw = VGRaw<VGElement>;