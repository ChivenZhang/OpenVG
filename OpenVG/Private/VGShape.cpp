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