#include "VGShape.h"

class VGShapePrivateData : public VGShapePrivate
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

void VGShape::moveTo(float x, float y)
{
	PRIVATE()->PointList.push_back({ x,y });
	PRIVATE()->PointTypes.push_back(VGPointType::MoveTo);
}

void VGShape::lineTo(float x, float y)
{
	PRIVATE()->PointList.push_back({ x,y });
	PRIVATE()->PointTypes.push_back(VGPointType::LineTo);
}

void VGShape::cubicTo(float cx1, float cy1, float cx2, float cy2, float x, float y)
{
	PRIVATE()->PointList.push_back({ cx1,cy1 });
	PRIVATE()->PointList.push_back({ cx2,cy2 });
	PRIVATE()->PointList.push_back({ x,y });
	PRIVATE()->PointTypes.push_back(VGPointType::LineTo);
}

void VGShape::close()
{
	if (PRIVATE()->PointTypes.empty()) return;
	if (PRIVATE()->PointTypes.back() == VGPointType::Close) return;
	PRIVATE()->PointTypes.push_back(VGPointType::Close);
}

VGFillStyleRaw VGShape::getFillStyle() const
{
	return &PRIVATE()->FillStyle;
}

VGStrokeStyleRaw VGShape::getStrokeStyle() const
{
	return &PRIVATE()->StrokeStyle;
}

float VGShape::getLineWidth() const
{
	return PRIVATE()->StrokeStyle.LineWidth;
}

void VGShape::setLineWidth(float value)
{
	PRIVATE()->StrokeStyle.LineWidth = value;
}

float VGShape::getDashOffset() const
{
	return PRIVATE()->StrokeStyle.DashOffset;
}

void VGShape::setDashOffset(float value)
{
	PRIVATE()->StrokeStyle.DashOffset = value;
}

float VGShape::getMiterLimit() const
{
	return PRIVATE()->StrokeStyle.MiterLimit;
}

void VGShape::setMiterLimit(float value)
{
	PRIVATE()->StrokeStyle.MiterLimit = value;
}

VGStrokeCap VGShape::getLineCap() const
{
	return PRIVATE()->StrokeStyle.LineCap;
}

void VGShape::setLineCap(VGStrokeCap value)
{
	PRIVATE()->StrokeStyle.LineCap = value;
}

VGStrokeJoin VGShape::getLineJoin() const
{
	return PRIVATE()->StrokeStyle.LineJoin;
}

void VGShape::setLineJoin(VGStrokeJoin value)
{
	PRIVATE()->StrokeStyle.LineJoin = value;
}

VGArrayView<const float> VGShape::getDashControl() const
{
	return PRIVATE()->StrokeStyle.DashControl;
}

void VGShape::setDashControl(VGVector<float> value)
{
	PRIVATE()->StrokeStyle.DashControl = value;
}
