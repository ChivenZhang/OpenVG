#pragma once
#include "VG.h"

/// @brief 
class OPENVG_API VGPainter
{
public:
	virtual ~VGPainter() = default;

};
using VGPainterRef = VGRef<VGPainter>;
using VGPainterRaw = VGRaw<VGPainter>;