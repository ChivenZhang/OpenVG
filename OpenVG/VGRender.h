#pragma once
#include "VG.h"

/// @brief 
class OPENVG_API VGRender
{
public:
	using primitive_t = VGPrimitive::primitive_t;

public:
	virtual ~VGRender() = default;

	virtual void render(VGRect client, VGArrayView<const VGPrimitive> data) = 0;
};
using VGRenderRef = VGRef<VGRender>;
using VGRenderRaw = VGRaw<VGRender>;