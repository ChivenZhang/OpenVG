#pragma once
#include "VG.h"

/// @brief 
class OPENVG_API VGRender
{
public:
	virtual ~VGRender() = default;

	virtual void render(VGRect client, VGArrayView<const VGPrimitive> data) = 0;
};
using VGRenderRef = VGRef<VGRender>;
using VGRenderRaw = VGRaw<VGRender>;