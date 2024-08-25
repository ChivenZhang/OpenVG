#include "VGShape.h"
#include "VGShape.h"
#include "VGShape.h"
#include "VGShape.h"
#include "VGShape.h"
#include "VGShape.h"
#include "VGShape.h"
#include "VGShape.h"

class VGShapePrivateData : public VGElementPrivate
{
public:
	VGFillStyle FillStyle;
	VGStrokeStyle StrokeStyle;
	VGVector<VGPoint> PointList;
	VGVector<VGPointType> PointTypes;
};
#define PRIVATE() ((VGShapePrivateData*) m_Private)

VGShape::VGShape()
{
	m_Private = new VGShapePrivateData;
}

VGShape::~VGShape()
{
	delete m_Private; m_Private = nullptr;
}

void VGShape::arc(float x, float y, float width, float height, float startAngle, float spanAngle)
{
	arcTo(x + width * 0.5f, y + height * 0.5f, width * 0.5f, height * 0.5f, 0, startAngle, spanAngle);
}

void VGShape::pie(float x, float y, float width, float height, float startAngle, float spanAngle)
{
	pieTo(x + width * 0.5f, y + height * 0.5f, width * 0.5f, height * 0.5f, 0, startAngle, spanAngle);
}

void VGShape::chord(float x, float y, float width, float height, float startAngle, float spanAngle)
{
	chordTo(x + width * 0.5f, y + height * 0.5f, width * 0.5f, height * 0.5f, 0, startAngle, spanAngle);
}

void VGShape::ellipse(float x, float y, float width, float height)
{
	arcTo(x + width * 0.5f, y + height * 0.5f, width * 0.5f, height * 0.5f, 0, 0, 360);
}

void VGShape::rect(float x, float y, float width, float height, float roundX, float roundY)
{
	moveTo(x, y);
	lineTo(x, y + height);
	lineTo(x + width, y + height);
	lineTo(x + width, y);
}

void VGShape::line(float x1, float y1, float x2, float y2)
{
	moveTo(x1, y1);
	lineTo(x2, y2);
}

void VGShape::point(float x, float y)
{
	rect(x, y, 2, 2);
}
