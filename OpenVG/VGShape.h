#pragma once
#include "VGElement.h"

/// @brief 
class OPENVG_API VGShape : public VGElement
{
public:
	VGShape();
	~VGShape();

	void arc(float x, float y, float width, float height, float startAngle, float spanAngle);
	void pie(float x, float y, float width, float height, float startAngle, float spanAngle);
	void chord(float x, float y, float width, float height, float startAngle, float spanAngle);
	void ellipse(float x, float y, float width, float height);
	void rect(float x, float y, float width, float height, float roundX = 0, float roundY = 0);
	void line(float x1, float y1, float x2, float y2);
	void point(float x, float y);

	template<class... Args>
	void points(float x, float y, Args... args)
	{
		point(x, y);
		points(args...);
	}
	void points(float x, float y)
	{
		point(x, y);
	}

	template<class... Args>
	void points(VGPoint p, Args... args)
	{
		point(p.X, p.Y);
		points(args...);
	}
	void points(VGPoint p)
	{
		point(p.X, p.Y);
	}

	template<class... Args>
	void lines(VGPoint p1, VGPoint p2, Args... args)
	{
		line(p1.X, p1.Y, p2.X, p2.Y);
		lines(args...);
	}
	void lines(VGPoint p1, VGPoint p2)
	{
		line(p1.X, p1.Y, p2.X, p2.Y);
	}

	template<class... Args>
	void lines(float x1, float y1, float x2, float y2, Args... args)
	{
		line(x1, y1, x2, y2);
		lines(args...);
	}
	void lines(float x1, float y1, float x2, float y2)
	{
		line(x1, y1, x2, y2);
	}

private:
	VGElementPrivateRaw m_Private;
};
using VGShapeRef = VGRef<VGShape>;
using VGShapeRaw = VGRaw<VGShape>;