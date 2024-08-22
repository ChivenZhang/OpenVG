#include "VGElement.h"
#include "VGElement.h"
#include "VGElement.h"
#include "VGElement.h"
#include "VGElement.h"
#include "VGElement.h"
#include "VGElement.h"
#include "../VGElement.h"

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
	PRIVATE()->ClipCache = nullptr;
	PRIVATE()->FillCache = nullptr;
	PRIVATE()->StrokeCache = nullptr;
	PRIVATE()->PointList.push_back({ x, y });
	PRIVATE()->PointTypeList.push_back(VGPointType::MoveTo);
}

void VGElement::lineTo(float x, float y)
{
	PRIVATE()->ClipCache = nullptr;
	PRIVATE()->FillCache = nullptr;
	PRIVATE()->StrokeCache = nullptr;
	PRIVATE()->PointList.push_back({ x, y });
	PRIVATE()->PointTypeList.push_back(VGPointType::LineTo);
}

void VGElement::cubicTo(float cx1, float cy1, float cx2, float cy2, float x, float y)
{
	PRIVATE()->ClipCache = nullptr;
	PRIVATE()->FillCache = nullptr;
	PRIVATE()->StrokeCache = nullptr;
	PRIVATE()->PointList.push_back({ cx1, cy1 });
	PRIVATE()->PointList.push_back({ cx2, cy2 });
	PRIVATE()->PointList.push_back({ x, y });
	PRIVATE()->PointTypeList.push_back(VGPointType::CubicTo);
}

void VGElement::close()
{
	if (PRIVATE()->PointTypeList.empty()) return;
	if (PRIVATE()->PointTypeList.back() == VGPointType::Close) return;
	PRIVATE()->ClipCache = nullptr;
	PRIVATE()->FillCache = nullptr;
	PRIVATE()->StrokeCache = nullptr;
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

VGColor VGElement::getFillColor() const
{
	if (PRIVATE()->FillStyle == nullptr) return VGColor();
	return PRIVATE()->FillStyle->Color;
}

void VGElement::setFillColor(VGColor value)
{
	if (PRIVATE()->FillStyle == nullptr) PRIVATE()->FillStyle = VGNew<VGFillStyle>();
	PRIVATE()->FillStyle->Color = value;
}

VGColor VGElement::getStrokeColor() const
{
	if (PRIVATE()->StrokeStyle == nullptr) return VGColor();
	return PRIVATE()->StrokeStyle->Color;
}

void VGElement::setStrokeColor(VGColor value)
{
	if (PRIVATE()->StrokeStyle == nullptr) PRIVATE()->StrokeStyle = VGNew<VGStrokeStyle>();
	PRIVATE()->StrokeStyle->Color = value;
}

float VGElement::getLineWidth() const
{
	if (PRIVATE()->StrokeStyle == nullptr) return float();
	return PRIVATE()->StrokeStyle->LineWidth;
}

void VGElement::setLineWidth(float value)
{
	if (PRIVATE()->StrokeStyle == nullptr) PRIVATE()->StrokeStyle = VGNew<VGStrokeStyle>();
	PRIVATE()->StrokeStyle->LineWidth = value;
}

float VGElement::getDashOffset() const
{
	if (PRIVATE()->StrokeStyle == nullptr) return float();
	return PRIVATE()->StrokeStyle->DashOffset;
}

void VGElement::setDashOffset(float value)
{
	if (PRIVATE()->StrokeStyle == nullptr) PRIVATE()->StrokeStyle = VGNew<VGStrokeStyle>();
	PRIVATE()->StrokeStyle->DashOffset = value;
}

float VGElement::getMiterLimit() const
{
	if (PRIVATE()->StrokeStyle == nullptr) return float();
	return PRIVATE()->StrokeStyle->MiterLimit;
}

void VGElement::setMiterLimit(float value)
{
	if (PRIVATE()->StrokeStyle == nullptr) PRIVATE()->StrokeStyle = VGNew<VGStrokeStyle>();
	PRIVATE()->StrokeStyle->MiterLimit = value;
}

VGStrokeCap VGElement::getLineCap() const
{
	if (PRIVATE()->StrokeStyle == nullptr) return VGStrokeCap::Square;
	return PRIVATE()->StrokeStyle->LineCap;
}

void VGElement::setLineCap(VGStrokeCap value)
{
	if (PRIVATE()->StrokeStyle == nullptr) PRIVATE()->StrokeStyle = VGNew<VGStrokeStyle>();
	PRIVATE()->StrokeStyle->LineCap = value;
}

VGStrokeJoin VGElement::getLineJoin() const
{
	if (PRIVATE()->StrokeStyle == nullptr) return VGStrokeJoin::Bevel;
	return PRIVATE()->StrokeStyle->LineJoin;
}

void VGElement::setLineJoin(VGStrokeJoin value)
{
	if (PRIVATE()->StrokeStyle == nullptr) PRIVATE()->StrokeStyle = VGNew<VGStrokeStyle>();
	PRIVATE()->StrokeStyle->LineJoin = value;
}

VGArrayView<const float> VGElement::getDashControl() const
{
	if (PRIVATE()->StrokeStyle == nullptr) return VGArrayView<const float>();
	return PRIVATE()->StrokeStyle->DashControl;
}

void VGElement::setDashControl(VGVector<float> value)
{
	if (PRIVATE()->StrokeStyle == nullptr) PRIVATE()->StrokeStyle = VGNew<VGStrokeStyle>();
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

void VGElement::setClipCache(VGPrimitiveRef value)
{
	PRIVATE()->ClipCache = value;
}

VGPrimitiveRaw VGElement::getFillCache() const
{
	return PRIVATE()->FillCache.get();
}

void VGElement::setFillCache(VGPrimitiveRef value)
{
	PRIVATE()->FillCache = value;
}

VGPrimitiveRaw VGElement::getStrokeCache() const
{
	return PRIVATE()->StrokeCache.get();
}

void VGElement::setStrokeCache(VGPrimitiveRef value)
{
	PRIVATE()->StrokeCache = value;
}
