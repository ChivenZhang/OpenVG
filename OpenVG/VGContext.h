#pragma once
#include "VGElement.h"

/// @brief 
class OPENVG_API VGContext
{
public:
	virtual ~VGContext() = default;
	virtual void addElement(VGElementRef value);
	virtual void animateElement();
	virtual void renderElement();
};
using VGContextRef = VGRef<VGContext>;
using VGContextRaw = VGRaw<VGContext>;