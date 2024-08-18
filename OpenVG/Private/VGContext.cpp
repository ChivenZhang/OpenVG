#include "VGContext.h"
#include "VGContext.h"
#include "VGContext.h"

class VGContextPrivateData : public VGContextPrivate
{
public:
	VGRenderRef Render;
};
#define PRIVATE() ((VGContextPrivateData*)m_Private)

VGContext::VGContext()
{
	m_Private = new VGContextPrivateData;
}

VGContext::~VGContext()
{
	delete m_Private; m_Private = nullptr;
}

VGRenderRaw VGContext::getRender() const
{
	return PRIVATE()->Render.get();
}

void VGContext::setRender(VGRenderRef value)
{
	PRIVATE()->Render = value;
}

void VGContext::addElement(VGElementRef value)
{
}

void VGContext::animateElement(float time)
{
}

void VGContext::renderElement()
{
}
