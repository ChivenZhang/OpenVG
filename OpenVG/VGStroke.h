#pragma once
#include "VG.h"

/// @brief 
class OPENVG_API VGStroke
{
public:
	virtual ~VGStroke() = default;

};
using VGStrokeRaw = VGRaw<VGStroke>;
using VGStrokeRef = VGRef<VGStroke>;