#pragma once
#include "VG.h"

/// @brief 
class OPENVG_API VGPainter
{
public:
	virtual ~VGPainter() = default;

	virtual void paint(VGArrayView<VGPrimitive> primitive);

};
using VGPainterRef = VGRef<VGPainter>;
using VGPainterRaw = VGRaw<VGPainter>;