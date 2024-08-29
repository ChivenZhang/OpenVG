#include "../VGPainter.h"
#include "VGTessellate.h"
#include "VGTrueType.h"

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
	element->clip();
	if (element->getClipCache() == nullptr) return;
	auto& primitive = PRIVATE()->PrimitiveList.emplace_back(*element->getClipCache());
	auto& matrix = primitive.MatrixList.back();
	matrix = VGFloat3x3::Transform(element->getTranslate().X, element->getTranslate().Y, element->getRotate(), element->getScale().X, element->getScale().Y);
}

void VGPainter::fill(VGElementRaw element)
{
	element->fill();
	if (element->getFillCache() == nullptr) return;
	auto& primitive = PRIVATE()->PrimitiveList.emplace_back(*element->getFillCache());
	auto& matrix = primitive.MatrixList.back();
	matrix = VGFloat3x3::Transform(element->getTranslate().X, element->getTranslate().Y, element->getRotate(), element->getScale().X, element->getScale().Y);
}

void VGPainter::stroke(VGElementRaw element)
{
	element->stroke();
	if (element->getStrokeCache() == nullptr) return;
	auto& primitive = PRIVATE()->PrimitiveList.emplace_back(*element->getStrokeCache());
	auto& matrix = primitive.MatrixList.back();
	matrix = VGFloat3x3::Transform(element->getTranslate().X, element->getTranslate().Y, element->getRotate(), element->getScale().X, element->getScale().Y);
}

VGVector<VGPrimitive>& VGPainter::getPrimitiveList()
{
	return PRIVATE()->PrimitiveList;
}

VGVector<VGPrimitive> const& VGPainter::getPrimitives() const
{
	return PRIVATE()->PrimitiveList;
}