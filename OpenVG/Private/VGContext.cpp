#include "../VGContext.h"

class VGContextPrivateData : public VGContextPrivate
{
public:
	VGRenderRef Render;
	VGPainterRef Painter;
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

VGPainterRaw VGContext::getPainter() const
{
	return PRIVATE()->Painter.get();
}

void VGContext::setPainter(VGPainterRef value)
{
	PRIVATE()->Painter = value;
}

VGRenderRaw VGContext::getRender() const
{
	return PRIVATE()->Render.get();
}

void VGContext::setRender(VGRenderRef value)
{
	PRIVATE()->Render = value;
}

void VGContext::clipElement(VGElementRef value)
{
	PRIVATE()->Painter->clip(value.get());
}

void VGContext::fillElement(VGElementRef value)
{
	PRIVATE()->Painter->fill(value.get());
}

void VGContext::strokeElement(VGElementRef value)
{
	PRIVATE()->Painter->stroke(value.get());
}

void VGContext::animateElement(float time)
{
}

void VGContext::renderElement(VGRect client)
{
	if (getPainter() && getRender())
	{
		getRender()->render(client, getPainter()->getPrimitiveList());
	}
}
