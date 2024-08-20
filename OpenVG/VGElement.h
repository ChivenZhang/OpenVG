#pragma once
#include "VGPainter.h"

/// @brief 
class OPENVG_API VGElement
{
public:
	virtual ~VGElement() = default;

	virtual void paint(VGPainterRaw painter) = 0;
};
using VGElementRef = VGRef<VGElement>;
using VGElementRaw = VGRaw<VGElement>;