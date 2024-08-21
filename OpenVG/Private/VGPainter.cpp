#include "../VGPainter.h"
#include <tesselator.h>

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
		// TODO:
		element->setFillCache(cache);
	}
	PRIVATE()->PrimitiveList.insert(PRIVATE()->PrimitiveList.end(), *element->getFillCache());
}

void VGPainter::fill(VGElementRaw element)
{
	if (element->getFillCache() == nullptr)
	{
		auto points = element->getPointList();
		auto types = element->getPointTypeList();

		auto cache = VGNew<VGPrimitive>();
		TESStesselator* tess = tessNewTess(nullptr);
		VGVector<float> contour;

		tessAddContour(tess, 2, contour.data(), sizeof(float) * 2, contour.size() >> 1);
		tessTesselate(tess, TESS_WINDING_ODD, TESS_POLYGONS, 3, 2, nullptr);
		const float* vertices = tessGetVertices(tess);
		const int* elements = tessGetElements(tess);
		const int vertexCount = tessGetVertexCount(tess);
		const int elementCount = tessGetElementCount(tess);



		tessDeleteTess(tess);

		element->setFillCache(cache);
	}
	PRIVATE()->PrimitiveList.insert(PRIVATE()->PrimitiveList.end(), *element->getFillCache());
}

void VGPainter::stroke(VGElementRaw element)
{
	if (element->getStrokeCache() == nullptr)
	{
		auto cache = VGNew<VGPrimitive>();
		// TODO:
		element->setStrokeCache(cache);
	}
	PRIVATE()->PrimitiveList.insert(PRIVATE()->PrimitiveList.end(), *element->getStrokeCache());
}

VGArrayView<const VGPrimitive> VGPainter::getPrimitiveList() const
{
	return PRIVATE()->PrimitiveList;
}
