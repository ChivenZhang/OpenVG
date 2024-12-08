#include "OpenUIPainter.h"
#include "OpenVGRender.h"
#include <GL/glew.h>
#include <OpenVG/VGContext.h>
#include <OpenVG/VGShape.h>
#include <OpenVG/VGText.h>
#include <OpenVG/VGPicture.h>
#include <OpenVG/VGTrueType.h>

OpenUIPainter::OpenUIPainter(uint32_t width, uint32_t height)
	:
	OpenVGPainter(width, height)
{
	uint32_t texture = 0;
	glGenTextures(1, &texture);
	m_NativeTexture = texture;

	resize(width, height);

	uint32_t frameBuffer = 0;
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) VG_FATAL("FBO is not complete!");
	m_NativeFrameBuffer = frameBuffer;
}

OpenUIPainter::~OpenUIPainter()
{
	glDeleteFramebuffers(1, &m_NativeFrameBuffer);
	m_NativeFrameBuffer = 0;

	glDeleteTextures(1, &m_NativeTexture);
	m_NativeTexture = 0;
}

void OpenUIPainter::resize(uint32_t width, uint32_t height)
{
	OpenVGPainter::resize(width, height);

	glBindTexture(GL_TEXTURE_2D, m_NativeTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);
}

uint32_t OpenUIPainter::getTexture() const
{
	auto& primitives = getVGContext()->getPainter()->getPrimitives();
	if (primitives.size())
	{
		auto width = getWidth(), height = getHeight();
		glBindFramebuffer(GL_FRAMEBUFFER, m_NativeFrameBuffer);
		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glViewport(0, 0, width, height);
		getVGContext()->renderElement({ 0, 0, (float)width, (float)height });
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	return m_NativeTexture;
}