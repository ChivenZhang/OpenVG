#pragma once
#include "VGShape.h"

/// @brief 
class VGTessellate
{
public:
	using point_t = VGPoint;
	using index_t = uint32_t;

public:
	static bool Fill(VGShapeRaw element, VGVector<point_t>& outPoints, VGVector<index_t>& outIndecies);

	static bool Stroke(VGShapeRaw element, VGVector<point_t>& outPoints, VGVector<index_t>& outIndecies);
};