#pragma once
#include "VGElement.h"

/// @brief 
class VGShapePrivate
{
public:
	virtual ~VGShapePrivate() = default;
};
using VGShapePrivateRaw = VGRaw<VGShapePrivate>;

/// @brief 
class OPENVG_API VGShape : public VGElement
{
public:
	VGShape();
	~VGShape();

private:
	VGShapePrivateRaw m_Private;
};
using VGShapeRef = VGRef<VGShape>;
using VGShapeRaw = VGRaw<VGShape>;