#pragma once
#include "VGShape.h"

/// @brief 
class OPENVG_API VGTessellate
{
public:
	static bool Fill(VGShapeRaw element, VGPrimitiveRaw result);

	static bool Stroke(VGShapeRaw element, VGPrimitiveRaw result);
};