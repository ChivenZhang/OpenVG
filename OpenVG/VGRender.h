#pragma once
#include "VG.h"

/// @brief 
class OPENVG_API VGRender
{
public:
	virtual ~VGRender() = default;

	virtual void renderElement(VGArrayView<VGPrimitive> primitive) = 0;
};
using VGRenderRef = VGRef<VGRender>;
using VGRenderRaw = VGRaw<VGRender>;