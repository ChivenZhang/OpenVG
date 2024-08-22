#include "OpenUIPainter.h"
#include "OpenVGRender.h"
#include <GL/glew.h>
#include <OpenVG/VGContext.h>
#include <OpenVG/VGShape.h>

class OpenUIPainterPrivate : public UIPainterPrivate
{
public:
	VGRect Client;
	VGContextRef Context;
	uint32_t NativeFrame;
	uint32_t NativeTexture;
	UIPen Pen;
	UIFont Font;
	UIBrush Brush;
	UIRect ClipRect, Viewport;
	bool EnableCilp = false;

	VGElementRef RectShape;
};
#define PRIVATE() ((OpenUIPainterPrivate*) m_Private)
#define CONTEXT() (PRIVATE()->Context)

OpenUIPainter::OpenUIPainter(uint32_t width, uint32_t height)
{
	m_Private = new OpenUIPainterPrivate;
	PRIVATE()->Context = VGNew<VGContext>();
	CONTEXT()->setPainter(VGNew<VGPainter>());
	auto render = VGNew<OpenVGRender>();
	CONTEXT()->setRender(render);

	PRIVATE()->NativeTexture = 0;
	glGenTextures(1, &PRIVATE()->NativeTexture);
	resize(width, height);

	PRIVATE()->NativeFrame = 0;
	glGenFramebuffers(1, &PRIVATE()->NativeFrame);
	glBindFramebuffer(GL_FRAMEBUFFER, PRIVATE()->NativeFrame);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, PRIVATE()->NativeTexture, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "FBO is not complete!" << std::endl;
		::exit(-1);
	}
}

OpenUIPainter::~OpenUIPainter()
{
	glDeleteFramebuffers(1, &PRIVATE()->NativeFrame); PRIVATE()->NativeFrame = 0;
	glDeleteTextures(1, &PRIVATE()->NativeTexture); PRIVATE()->NativeTexture = 0;
	delete m_Private; m_Private = nullptr;
}

UIRect OpenUIPainter::boundingRect(float x, float y, float width, float height, UIString const& text, float cursor, int* row, int* column, UIRectRaw cursorRect)
{
	return UIRect();
}

UIRect OpenUIPainter::boundingRect(float x, float y, float width, float height, UIString const& text, float row, float column, int* cursor, UIRectRaw cursorRect)
{
	return UIRect();
}

UIRect OpenUIPainter::boundingRect(float x, float y, float width, float height, UIString const& text, float posX, float posY, int* row, int* column, int* cursor, UIRectRaw cursorRect)
{
	return UIRect();
}

void OpenUIPainter::drawArc(float x, float y, float width, float height, float startAngle, float spanAngle)
{
}

void OpenUIPainter::drawChord(float x, float y, float width, float height, float startAngle, float spanAngle)
{
}

void OpenUIPainter::drawEllipse(float x, float y, float width, float height)
{
}

void OpenUIPainter::drawImage(float x, float y, UIImage image, float sx, float sy, float sw, float sh)
{
}

void OpenUIPainter::drawLine(float x1, float y1, float x2, float y2)
{
}

void OpenUIPainter::drawLines(UIArrayView<UILine> lines)
{
}

void OpenUIPainter::drawPie(float x, float y, float width, float height, float startAngle, float spanAngle)
{
}

void OpenUIPainter::drawPoint(float x, float y)
{
}

void OpenUIPainter::drawPoints(UIArrayView<UIPoint> points)
{
}

void OpenUIPainter::drawPolygon(UIArrayView<UIPoint> points)
{
}

void OpenUIPainter::drawPolyline(UIArrayView<UIPoint> points)
{
}

void OpenUIPainter::drawRect(float x, float y, float width, float height)
{
	if (PRIVATE()->RectShape == nullptr)
	{
		PRIVATE()->RectShape = VGNew<VGElement>();
		PRIVATE()->RectShape->moveTo(x, y);
		PRIVATE()->RectShape->cubicTo(x + width, y, x + width, y + height, x, y + height);
		PRIVATE()->RectShape->close();
	}

	/*shape->moveTo(x, y);
	shape->lineTo(x + width, y);
	shape->lineTo(x + width, y + height);
	shape->lineTo(x, y + height);
	shape->close();*/
	if (PRIVATE()->Brush.Style != UIBrush::NoBrush)
	{
		auto color = getBrush().Color;
		PRIVATE()->RectShape->setFillColor({ color.R, color.G, color.B, color.A });
		CONTEXT()->fillElement(PRIVATE()->RectShape);
	}
	if (PRIVATE()->Pen.Style != UIPen::NoPen)
	{
		auto color = getPen().Color;
		PRIVATE()->RectShape->setStrokeColor({ color.R, color.G, color.B, color.A });
		CONTEXT()->strokeElement(PRIVATE()->RectShape);
	}
}

void OpenUIPainter::drawRects(UIArrayView<UIRect> rects)
{
	for (size_t i = 0; i < rects.size(); ++i)
	{
		drawRect(rects[i].X, rects[i].Y, rects[i].W, rects[i].H);
	}
}

void OpenUIPainter::drawRoundedRect(float x, float y, float width, float height, float xRadius, float yRadius)
{
}

void OpenUIPainter::drawText(float x, float y, float width, float height, const UIString& text, UIRectRaw boundingRect, float cursor, UIRectRaw cursorRect)
{
}

UIPen const& OpenUIPainter::getPen() const
{
	return PRIVATE()->Pen;
}

void OpenUIPainter::setPen(const UIPen& pen)
{
	PRIVATE()->Pen = pen;
}

UIBrush const& OpenUIPainter::getBrush() const
{
	return PRIVATE()->Brush;
}

void OpenUIPainter::setBrush(const UIBrush& brush)
{
	PRIVATE()->Brush = brush;
}

UIFont const& OpenUIPainter::getFont() const
{
	return PRIVATE()->Font;
}

void OpenUIPainter::setFont(const UIFont& font)
{
	PRIVATE()->Font = font;
}

void OpenUIPainter::setClipping(bool enable)
{
}

void OpenUIPainter::setClipRect(float x, float y, float width, float height)
{
	PRIVATE()->ClipRect = { x, y, width, height };
}

void OpenUIPainter::setViewport(float x, float y, float width, float height)
{
	PRIVATE()->Viewport = { x, y, width, height };
}

void OpenUIPainter::shear(float sh, float sv)
{
}

void OpenUIPainter::rotate(float angle)
{
}

void OpenUIPainter::scale(float dx, float dy)
{
}

void OpenUIPainter::translate(float dx, float dy)
{
}

uint32_t OpenUIPainter::getWidth() const
{
	return PRIVATE()->Client.W;
}

uint32_t OpenUIPainter::getHeight() const
{
	return PRIVATE()->Client.H;
}

uint32_t OpenUIPainter::getStride() const
{
	return getWidth() * 4;
}

UIArrayView<const uint8_t> OpenUIPainter::getPixelData() const
{
	return UIArrayView<const uint8_t>();
}

void OpenUIPainter::resize(uint32_t width, uint32_t height)
{
	PRIVATE()->Client = VGRect{ 0,0,(float)width,(float)height };
	glBindTexture(GL_TEXTURE_2D, PRIVATE()->NativeTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);
}

uint32_t OpenUIPainter::getTexture() const
{
	auto client = PRIVATE()->Client;
	glBindFramebuffer(GL_FRAMEBUFFER, PRIVATE()->NativeFrame);
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glViewport((int32_t)client.X, (int32_t)client.Y, (int32_t)client.W, (int32_t)client.H);
	PRIVATE()->Context->renderElement(PRIVATE()->Client);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return PRIVATE()->NativeTexture;
}

void OpenUIPainter::setTexture(uint32_t value)
{
	// Nothing
}
