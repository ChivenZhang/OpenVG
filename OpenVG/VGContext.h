#pragma once
#include "VGElement.h"
#include "VGRender.h"

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
	virtual VGRenderRaw getRender() const;
	virtual void setRender(VGRenderRef value);
	virtual void addElement(VGElementRef value);
	virtual void animateElement(float time);
	virtual void renderElement();

private:
	VGContextPrivateRaw m_Private;
};
using VGContextRef = VGRef<VGContext>;
using VGContextRaw = VGRaw<VGContext>;