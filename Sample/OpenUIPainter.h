#pragma once
#include "OpenVGPainter.h"

class OpenUIPainter : public OpenVGPainter
{
public:
	OpenUIPainter(uint32_t width, uint32_t height);
	~OpenUIPainter();
	virtual void resize(uint32_t width, uint32_t height) override;
	uint32_t getTexture() const;

private:
	uint32_t m_NativeTexture;
	uint32_t m_NativeFrameBuffer;
};