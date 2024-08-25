#pragma once
#include "VG.h"
#include "../VGText.h"

class VGTrueType
{
public:
	static bool Path(VGTextRaw element, VGRect rect, VGString text, VGVector<VGPoint>& points, VGVector<VGPointType>& types);
};