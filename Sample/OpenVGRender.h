#pragma once
#include <OpenVG/VGRender.h>

class OpenVGRender : public VGRender
{
public:
	virtual void renderElement(VGArrayView<VGPrimitive> primitive) override;

	void setTexture(uint32_t value);
	uint32_t getTexture() const;
};