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

	VGFloat2 getScale() const;
	void setScale(VGFloat2 value);
	void setScale(float sx, float sy) { setScale({ sx, sy }); }

	float getRotate() const;
	void setRotate(float value);

	VGFloat2 getTranslate() const;
	void setTranslate(VGFloat2 value);
	void setTranslate(float tx, float ty) { setTranslate({ tx, ty }); }

protected:
	virtual void clip() = 0;
	virtual void fill() = 0;
	virtual void stroke() = 0;

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