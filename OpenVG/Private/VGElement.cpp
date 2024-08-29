#include "VGElement.h"
#include "VGElement.h"
#include "../VGElement.h"

class VGElementPrivateData : public VGElementPrivate
{
public:
	float Rotation = 0;
	VGFloat2 Translate = { 0,0 }, Scaling = { 1,1 };
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

VGFloat2 VGElement::getScale() const
{
	return PRIVATE()->Scaling;
}

void VGElement::setScale(VGFloat2 value)
{
	PRIVATE()->Scaling = value;
}

float VGElement::getRotate() const
{
	return PRIVATE()->Rotation;
}

void VGElement::setRotate(float value)
{
	PRIVATE()->Rotation = value;
}

VGFloat2 VGElement::getTranslate() const
{
	return PRIVATE()->Translate;
}

void VGElement::setTranslate(VGFloat2 value)
{
	PRIVATE()->Translate = value;
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
