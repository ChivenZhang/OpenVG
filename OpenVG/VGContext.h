#pragma once
#include "VGElement.h"
#include "VGRender.h"
#include "VGPainter.h"

class VGContextPrivate
{
public:
	virtual ~VGContextPrivate() = default;
};
using VGContextPrivateRaw = VGRaw<VGContextPrivate>;

/// @brief 
class OPENVG_API VGContext
{
public:
	VGContext();
	~VGContext();
	virtual VGPainterRaw getPainter() const;
	virtual void setPainter(VGPainterRef value);
	virtual VGRenderRaw getRender() const;
	virtual void setRender(VGRenderRef value);
	virtual void clipElement(VGElementRef value);
	virtual void fillElement(VGElementRef value);
	virtual void strokeElement(VGElementRef value);
	virtual void animateElement(float time);
	virtual void renderElement(VGRect client);

private:
	VGContextPrivateRaw m_Private;
};
using VGContextRef = VGRef<VGContext>;
using VGContextRaw = VGRaw<VGContext>;