#pragma once
#include "VG.h"

/// @brief 
class OPENVG_API VGElement
{
public:
	virtual ~VGElement() = default;

};
using VGElementRef = VGRef<VGElement>;
using VGElementRaw = VGRaw<VGElement>;