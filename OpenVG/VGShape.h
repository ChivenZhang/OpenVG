#pragma once
#include "VGElement.h"

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

	void arc(float x, float y, float width, float height, float startAngle, float spanAngle);
	void chord(float x, float y, float width, float height, float startAngle, float spanAngle);
	void ellipse(float x, float y, float width, float height);
	void ellipse(float x, float y, float width, float height, float startAngle, float spanAngle);
	void line(float x1, float y1, float x2, float y2);

	template<typename... Args>
	void linesTo(float x, float y, Args... args)
	{
		lineTo(x, y); linesTo(args...);
	}

	template<typename... Args>
	void linesTo(VGPoint p, Args... args)
	{
		linesTo(p); linesTo(args...); return *this;
	}
	void linesTo3(VGPoint p)
	{
		lineTo(p.X, p.Y);
	}

private:
	VGShapePrivateRaw m_Private;
};
using VGShapeRef = VGRef<VGShape>;
using VGShapeRaw = VGRaw<VGShape>;