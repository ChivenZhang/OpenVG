#pragma once
/*=================================================
* Copyright © 2020-2025 ChivenZhang.
* All Rights Reserved.
* =====================Note=========================
*
*
* ====================History=======================
* Created by ChivenZhang@gmail.com.
*
* =================================================*/
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