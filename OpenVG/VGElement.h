#pragma once
#include "VGGradient.h"
class VGContext;
class VGTessellate;

struct VGFillStyle
{
	VGColor Color = { 1,1,1,1 };
	VGImage Image;
	VGGradientRef Gradient;
};
using VGFillStyleRef = VGRef<VGFillStyle>;
using VGFillStyleRaw = VGRaw<VGFillStyle>;

struct VGStrokeStyle
{
	VGColor Color = { 0,0,0,1 };
	VGImage Image;
	VGGradientRef Gradient;
	VGStrokeCap LineCap;
	VGStrokeJoin LineJoin;
	float LineWidth = 1;
	float MiterLimit = 4;
	float DashOffset = 0;
	VGVector<float> DashControl;
};
using VGStrokeStyleRef = VGRef<VGStrokeStyle>;
using VGStrokeStyleRaw = VGRaw<VGStrokeStyle>;

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
	void moveTo(VGPoint p) { moveTo(p.X, p.Y); }
	void lineTo(float x, float y);
	void lineTo(VGPoint p) { lineTo(p.X, p.Y); }
	void curveTo(float cx1, float cy1, float x, float y);
	void curveTo(VGPoint p1, VGPoint p2) { curveTo(p1.X, p1.Y, p2.X, p2.Y); }
	void cubicTo(float cx1, float cy1, float cx2, float cy2, float x, float y);
	void cubicTo(VGPoint p1, VGPoint p2, VGPoint p3) { cubicTo(p1.X, p1.Y, p2.X, p2.Y, p3.X, p3.Y); }
	void arcTo(float cx1, float cy1, float rx, float ry, float r, float a1, float a2);
	void arcTo(VGPoint p, float rx, float ry, float r, float a1, float a2) { arcTo(p.X, p.Y, rx, ry, r, a1, a2); }
	void pieTo(float cx1, float cy1, float rx, float ry, float r, float a1, float a2);
	void pieTo(VGPoint p, float rx, float ry, float r, float a1, float a2) { pieTo(p.X, p.Y, rx, ry, r, a1, a2); }
	void chordTo(float cx1, float cy1, float rx, float ry, float r, float a1, float a2);
	void chordTo(VGPoint p, float rx, float ry, float r, float a1, float a2) { chordTo(p.X, p.Y, rx, ry, r, a1, a2); }
	void close();
	void reset();

	VGFloat2 getScale() const;
	void setScale(VGFloat2 value);
	void setScale(float sx, float sy) { setScale({ sx, sy }); }

	float getRotate() const;
	void setRotate(float value);

	VGFloat2 getTranslate() const;
	void setTranslate(VGFloat2 value);
	void setTranslate(float tx, float ty) { setTranslate({ tx, ty }); }

	VGColor getFillColor() const;
	void setFillColor(VGColor value);

	VGColor getStrokeColor() const;
	void setStrokeColor(VGColor value);

	float getLineWidth() const;
	void setLineWidth(float value);

	float getMiterLimit() const;
	void setMiterLimit(float value);

	VGStrokeCap getLineCap() const;
	void setLineCap(VGStrokeCap value);

	VGStrokeJoin getLineJoin() const;
	void setLineJoin(VGStrokeJoin value);

	float getDashOffset() const;
	VGArrayView<const float> getLineDash() const;
	void setLineDash(VGVector<float> value, float offset = 0);

private:
	VGFillStyleRaw getFillStyle() const;
	VGStrokeStyleRaw getStrokeStyle() const;
	VGArrayView<VGPoint> getPointList() const;
	VGArrayView<VGPointType> getPointTypeList() const;

	VGPrimitiveRaw getClipCache() const;
	void setClipCache(VGPrimitiveRef value);
	VGPrimitiveRaw getFillCache() const;
	void setFillCache(VGPrimitiveRef value);
	VGPrimitiveRaw getStrokeCache() const;
	void setStrokeCache(VGPrimitiveRef value);

protected:
	friend class VGContext;
	friend class VGPainter;
	friend class VGTessellate;

	VGElementPrivateRaw m_Private;
};
using VGElementRef = VGRef<VGElement>;
using VGElementRaw = VGRaw<VGElement>;