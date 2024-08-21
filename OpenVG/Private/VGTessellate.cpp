#include "VGTessellate.h"
#include <tesselator.h>

bool VGTessellate::Fill(VGElementRaw element, VGVector<point_t>& inPoints, VGVector<index_t>& inIndecies)
{
	auto points = element->getPointList();
	auto types = element->getPointTypeList();
	TESStesselator* tess = tessNewTess(nullptr);
	VGVector<float> contour;

	tessAddContour(tess, 2, contour.data(), sizeof(float) * 2, contour.size() >> 1);
	tessTesselate(tess, TESS_WINDING_ODD, TESS_POLYGONS, 3, 2, nullptr);
	const float* vertices = tessGetVertices(tess);
	const int* elements = tessGetElements(tess);
	const int vertexCount = tessGetVertexCount(tess);
	const int elementCount = tessGetElementCount(tess);

	tessDeleteTess(tess);
	return false;
}

bool VGTessellate::Stroke(VGElementRaw element, VGVector<point_t>& inPoints, VGVector<index_t>& inIndecies)
{
	auto points = element->getPointList();
	auto types = element->getPointTypeList();

	return false;
}