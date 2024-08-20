#include "VGElement.h"

class VGElementPrivateData : public VGElementPrivate
{
public:
	VGFillStyleRef FillStyle;
	VGStrokeStyleRef StrokeStyle;
	VGVector<VGPoint> PointList;
	VGVector<VGPointType> PointTypeList;
	VGPrimitiveRef ClipCache, FillCache, StrokeCache;
};
#define PRIVATE() ((VGElementPrivateData*) m_Private)

VGElement::VGElement()
{
	m_Private = new VGElementPrivateData;
}

VGElement::~VGElement()
{
	delete m_Private; m_Private = nullptr;
}

void VGElement::moveTo(float x, float y)
{
	PRIVATE()->PointList.push_back({ x, y });
	PRIVATE()->PointTypeList.push_back(VGPointType::MoveTo);
}

void VGElement::lineTo(float x, float y)
{
	PRIVATE()->PointList.push_back({ x, y });
	PRIVATE()->PointTypeList.push_back(VGPointType::LineTo);
}

void VGElement::cubicTo(float cx1, float cy1, float cx2, float cy2, float x, float y)
{
	PRIVATE()->PointList.push_back({ cx1, cy1 });
	PRIVATE()->PointList.push_back({ cx2, cy2 });
	PRIVATE()->PointList.push_back({ x, y });
	PRIVATE()->PointTypeList.push_back(VGPointType::LineTo);
}

void VGElement::close()
{
	if (PRIVATE()->PointTypeList.empty()) return;
	if (PRIVATE()->PointTypeList.back() == VGPointType::Close) return;
	PRIVATE()->PointTypeList.push_back(VGPointType::Close);
}

void VGElement::reset()
{
	PRIVATE()->FillStyle = nullptr;
	PRIVATE()->StrokeStyle = nullptr;
	PRIVATE()->ClipCache = nullptr;
	PRIVATE()->FillCache = nullptr;
	PRIVATE()->StrokeCache = nullptr;
	PRIVATE()->PointList.clear();
	PRIVATE()->PointTypeList.clear();
}

float VGElement::getLineWidth() const
{
	return PRIVATE()->StrokeStyle->LineWidth;
}

void VGElement::setLineWidth(float value)
{
	PRIVATE()->StrokeStyle->LineWidth = value;
}

float VGElement::getDashOffset() const
{
	return PRIVATE()->StrokeStyle->DashOffset;
}

void VGElement::setDashOffset(float value)
{
	PRIVATE()->StrokeStyle->DashOffset = value;
}

float VGElement::getMiterLimit() const
{
	return PRIVATE()->StrokeStyle->MiterLimit;
}

void VGElement::setMiterLimit(float value)
{
	PRIVATE()->StrokeStyle->MiterLimit = value;
}

VGStrokeCap VGElement::getLineCap() const
{
	return PRIVATE()->StrokeStyle->LineCap;
}

void VGElement::setLineCap(VGStrokeCap value)
{
	PRIVATE()->StrokeStyle->LineCap = value;
}

VGStrokeJoin VGElement::getLineJoin() const
{
	return PRIVATE()->StrokeStyle->LineJoin;
}

void VGElement::setLineJoin(VGStrokeJoin value)
{
	PRIVATE()->StrokeStyle->LineJoin = value;
}

VGArrayView<const float> VGElement::getDashControl() const
{
	return PRIVATE()->StrokeStyle->DashControl;
}

void VGElement::setDashControl(VGVector<float> value)
{
	PRIVATE()->StrokeStyle->DashControl = value;
}

VGFillStyleRaw VGElement::getFillStyle() const
{
	return PRIVATE()->FillStyle.get();
}

VGStrokeStyleRaw VGElement::getStrokeStyle() const
{
	return PRIVATE()->StrokeStyle.get();
}

VGArrayView<VGPoint> VGElement::getPointList() const
{
	return PRIVATE()->PointList;
}

VGArrayView<VGPointType> VGElement::getPointTypeList() const
{
	return PRIVATE()->PointTypeList;
}

VGPrimitiveRaw VGElement::getClipCache() const
{
	return PRIVATE()->ClipCache.get();
}

VGPrimitiveRaw VGElement::getFillCache() const
{
	return PRIVATE()->FillCache.get();
}

VGPrimitiveRaw VGElement::getStrokeCache() const
{
	return PRIVATE()->StrokeCache.get();
}
