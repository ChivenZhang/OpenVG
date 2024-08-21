#include "../VGPainter.h"

class VGPainterPrivateData : public VGPainterPrivate
{
public:
	VGVector<VGPrimitive> PrimitiveList;
};
#define PRIVATE() ((VGPainterPrivateData*) m_Private)

VGPainter::VGPainter()
{
	m_Private = new VGPainterPrivateData;
}

VGPainter::~VGPainter()
{
	delete m_Private; m_Private = nullptr;
}

void VGPainter::clip(VGElementRaw element)
{
	if (element->getClipCache() == nullptr)
	{
		auto cache = VGNew<VGPrimitive>();
		element->setFillCache(cache);
	}
	PRIVATE()->PrimitiveList.insert(PRIVATE()->PrimitiveList.end(), *element->getFillCache());
}

void VGPainter::fill(VGElementRaw element)
{
	if (element->getFillCache() == nullptr)
	{
		auto cache = VGNew<VGPrimitive>();
		element->setFillCache(cache);
	}
	PRIVATE()->PrimitiveList.insert(PRIVATE()->PrimitiveList.end(), *element->getFillCache());
}

void VGPainter::stroke(VGElementRaw element)
{
	if (element->getStrokeCache() == nullptr)
	{
		auto cache = VGNew<VGPrimitive>();
		element->setStrokeCache(cache);
	}
	PRIVATE()->PrimitiveList.insert(PRIVATE()->PrimitiveList.end(), *element->getStrokeCache());
}

VGArrayView<const VGPrimitive> VGPainter::getPrimitiveList() const
{
	return PRIVATE()->PrimitiveList;
}
