#pragma once
#include "VGElement.h"
class VGContext;

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
	~VGPainter();

	void clip(VGElementRaw element);
	void fill(VGElementRaw element);
	void stroke(VGElementRaw element);
	VGVector<VGPrimitive> const& getPrimitives() const;

private:
	VGVector<VGPrimitive>& getPrimitiveList();

private:
	VGPainterPrivateRaw m_Private;

	friend class VGContext;
};
using VGPainterRef = VGRef<VGPainter>;
using VGPainterRaw = VGRaw<VGPainter>;