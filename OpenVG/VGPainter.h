#pragma once
#include "VGElement.h"

/// @brief 
class VGPainterPrivate
{
public:
	virtual ~VGPainterPrivate() = default;
};
using VGPainterPrivateRaw = VGRaw<VGPainterPrivate>;

/// @brief 
class OPENVG_API VGPainter
{
public:
	VGPainter();
	virtual ~VGPainter();

	virtual void clip(VGElementRaw element);
	virtual void fill(VGElementRaw element);
	virtual void stroke(VGElementRaw element);

	virtual VGArrayView<const VGPrimitive> getPrimitiveList() const;

private:
	VGPainterPrivateRaw m_Private;
};
using VGPainterRef = VGRef<VGPainter>;
using VGPainterRaw = VGRaw<VGPainter>;