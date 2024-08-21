#pragma once
#include "VGElement.h"

/// @brief 
class VGTessellate
{
public:
	using point_t = VGPoint;
	using index_t = uint32_t;

public:
	static bool Fill(VGElementRaw element, VGVector<point_t>& inPoints, VGVector<index_t>& inIndecies);

	static bool Stroke(VGElementRaw element, VGVector<point_t>& inPoints, VGVector<index_t>& inIndecies);
};