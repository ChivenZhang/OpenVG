#include "OpenUIPainter.h"
#include "OpenVGRender.h"
#include <GL/glew.h>
#include <OpenVG/VGContext.h>
#include <OpenVG/VGShape.h>
#include <OpenVG/VGText.h>

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
	UIRect Scissor, Viewport;
	bool EnableCilp = false;
	VGShapeRef RectFillShape;
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

UIRect OpenUIPainter::boundingRect(float x, float y, float width, float height, UIString const& text, float cursor, UIRectRaw cursorRect)
{
	return UIRect();
}

UIRect OpenUIPainter::boundingRect(float x, float y, float width, float height, UIString const& text, float posX, float posY, int* cursor, UIRectRaw cursorRect)
{
	return UIRect();
}

void OpenUIPainter::drawArc(float x, float y, float width, float height, float startAngle, float spanAngle)
{
	if (PRIVATE()->Brush.Style != UIBrush::NoBrush)
	{
		VGShape shape;
		shape.moveTo(x, y);
		shape.arcTo(width * 0.5f, height * 0.5f, width * 0.5f, height * 0.5f, startAngle, spanAngle);
		shape.close();
		auto color = getBrush().Color;
		shape.setFillColor({ color.R, color.G, color.B, color.A });
		auto scissor = PRIVATE()->Scissor;
		shape.setScissor({ scissor.X, scissor.Y, scissor.W, scissor.H });
		CONTEXT()->fillElement(&shape);
	}
	if (PRIVATE()->Pen.Style != UIPen::NoPen)
	{
		VGShape shape;
		shape.moveTo(x, y);
		shape.arcTo(width * 0.5f, height * 0.5f, width * 0.5f, height * 0.5f, startAngle, spanAngle);
		shape.close();
		auto color = getPen().Color;
		shape.setStrokeColor({ color.R, color.G, color.B, color.A });
		auto scissor = PRIVATE()->Scissor;
		shape.setScissor({ scissor.X, scissor.Y, scissor.W, scissor.H });
		CONTEXT()->strokeElement(&shape);
	}
}

void OpenUIPainter::drawChord(float x, float y, float width, float height, float startAngle, float spanAngle)
{
}

void OpenUIPainter::drawEllipse(float x, float y, float width, float height)
{
	if (width <= 0 || height <= 0) return;
	if (PRIVATE()->Brush.Style != UIBrush::NoBrush)
	{
		VGShape shape;
		shape.arcTo(x + width * 0.5f, y + height * 0.5f, width * 0.5f, height * 0.5f, 0, 360);
		shape.close();
		auto color = getBrush().Color;
		shape.setFillColor({ color.R, color.G, color.B, color.A });
		auto scissor = PRIVATE()->Scissor;
		shape.setScissor({ scissor.X, scissor.Y, scissor.W, scissor.H });
		CONTEXT()->fillElement(&shape);
	}
	if (PRIVATE()->Pen.Style != UIPen::NoPen)
	{
		VGShape shape;
		shape.arcTo(x + width * 0.5f, y + height * 0.5f, width * 0.5f, height * 0.5f, 0, 360);
		shape.close();
		shape.setLineWidth(getPen().Width);
		auto color = getPen().Color;
		shape.setStrokeColor({ color.R, color.G, color.B, color.A });
		auto scissor = PRIVATE()->Scissor;
		shape.setScissor({ scissor.X, scissor.Y, scissor.W, scissor.H });
		CONTEXT()->strokeElement(&shape);
	}
}

void OpenUIPainter::drawImage(float x, float y, UIImage image, float sx, float sy, float sw, float sh)
{
}

void OpenUIPainter::drawLine(float x1, float y1, float x2, float y2)
{
	if (PRIVATE()->Pen.Style != UIPen::NoPen)
	{
		VGShape shape;
		shape.moveTo(x1, y1);
		shape.lineTo(x2, y2);
		shape.close();
		shape.setLineWidth(getPen().Width);
		auto color = getPen().Color;
		shape.setStrokeColor({ color.R, color.G, color.B, color.A });
		auto scissor = PRIVATE()->Scissor;
		shape.setScissor({ scissor.X, scissor.Y, scissor.W, scissor.H });
		CONTEXT()->strokeElement(&shape);
	}
}

void OpenUIPainter::drawLines(UIArrayView<UILine> lines)
{
	if (PRIVATE()->Pen.Style != UIPen::NoPen)
	{
		VGShape shape;
		for (size_t i = 0; i < lines.size(); ++i)
		{
			shape.moveTo(lines[i].P0.X, lines[i].P0.Y);
			shape.lineTo(lines[i].P1.X, lines[i].P1.Y);
			shape.close();
		}
		shape.setLineWidth(getPen().Width);
		auto color = getPen().Color;
		shape.setStrokeColor({ color.R, color.G, color.B, color.A });
		auto scissor = PRIVATE()->Scissor;
		shape.setScissor({ scissor.X, scissor.Y, scissor.W, scissor.H });
		CONTEXT()->strokeElement(&shape);
	}
}

void OpenUIPainter::drawPie(float x, float y, float width, float height, float startAngle, float spanAngle)
{
	if (width <= 0 || height <= 0) return;
	if (PRIVATE()->Brush.Style != UIBrush::NoBrush)
	{
		VGShape shape;
		shape.moveTo(0, 0);
		shape.arcTo(0, 0, width * 0.5f, height * 0.5f, startAngle, spanAngle);
		shape.close();
		auto color = getBrush().Color;
		shape.setFillColor({ color.R, color.G, color.B, color.A });
		auto scissor = PRIVATE()->Scissor;
		shape.setScissor({ scissor.X, scissor.Y, scissor.W, scissor.H });
		CONTEXT()->fillElement(&shape);
	}
	if (PRIVATE()->Pen.Style != UIPen::NoPen)
	{
		VGShape shape;
		shape.moveTo(0, 0);
		shape.arcTo(0, 0, width * 0.5f, height * 0.5f, startAngle, spanAngle);
		shape.close();
		shape.setLineWidth(getPen().Width);
		auto color = getPen().Color;
		shape.setStrokeColor({ color.R, color.G, color.B, color.A });
		auto scissor = PRIVATE()->Scissor;
		shape.setScissor({ scissor.X, scissor.Y, scissor.W, scissor.H });
		CONTEXT()->strokeElement(&shape);
	}
}

void OpenUIPainter::drawPoint(float x, float y)
{
	drawRect(x, y, 1, 1);
}

void OpenUIPainter::drawPoints(UIArrayView<UIPoint> points)
{
	for (size_t i = 0; i < points.size(); ++i)
	{
		drawRect(points[i].X, points[i].Y, 1, 1);
	}
}

void OpenUIPainter::drawPolygon(UIArrayView<UIPoint> points)
{
	if (points.size() < 3) return;
	if (PRIVATE()->Brush.Style != UIBrush::NoBrush)
	{
		VGShape shape;
		shape.moveTo(points.front().X, points.front().Y);
		for (size_t i = 1; i < points.size(); ++i)
			shape.lineTo(points[i].X, points[i].Y);
		shape.lineTo(points.front().X, points.front().Y);
		shape.close();
		auto color = getBrush().Color;
		shape.setFillColor({ color.R, color.G, color.B, color.A });
		auto scissor = PRIVATE()->Scissor;
		shape.setScissor({ scissor.X, scissor.Y, scissor.W, scissor.H });
		CONTEXT()->fillElement(&shape);
	}
	if (PRIVATE()->Pen.Style != UIPen::NoPen)
	{
		VGShape shape;
		shape.moveTo(points.front().X, points.front().Y);
		for (size_t i = 1; i < points.size(); ++i)
			shape.lineTo(points[i].X, points[i].Y);
		shape.lineTo(points.front().X, points.front().Y);
		shape.close();
		shape.setLineWidth(getPen().Width);
		auto color = getPen().Color;
		shape.setStrokeColor({ color.R, color.G, color.B, color.A });
		auto scissor = PRIVATE()->Scissor;
		shape.setScissor({ scissor.X, scissor.Y, scissor.W, scissor.H });
		CONTEXT()->strokeElement(&shape);
	}
}

void OpenUIPainter::drawPolyline(UIArrayView<UIPoint> points)
{
	if (points.size() < 2) return;
	if (PRIVATE()->Pen.Style != UIPen::NoPen)
	{
		VGShape shape;
		shape.moveTo(points.front().X, points.front().Y);
		for (size_t i = 1; i < points.size(); ++i)
			shape.lineTo(points[i].X, points[i].Y);
		shape.close();
		shape.setLineWidth(getPen().Width);
		auto color = getPen().Color;
		shape.setStrokeColor({ color.R, color.G, color.B, color.A });
		auto scissor = PRIVATE()->Scissor;
		shape.setScissor({ scissor.X, scissor.Y, scissor.W, scissor.H });
		CONTEXT()->strokeElement(&shape);
	}
}

void OpenUIPainter::drawRect(float x, float y, float width, float height)
{
	if (PRIVATE()->Brush.Style != UIBrush::NoBrush)
	{
		if (PRIVATE()->RectFillShape == nullptr)
		{
			PRIVATE()->RectFillShape = VGNew<VGShape>();
			PRIVATE()->RectFillShape->moveTo(0, 0);
			PRIVATE()->RectFillShape->lineTo(0, 0 + 100);
			PRIVATE()->RectFillShape->lineTo(0 + 100, 0 + 100);
			PRIVATE()->RectFillShape->lineTo(0 + 100, 0);
			PRIVATE()->RectFillShape->close();
		}
		auto& shape = *PRIVATE()->RectFillShape.get();
		shape.setRotate(0);
		shape.setTranslate({ x, y });
		shape.setScale({ width * 0.01f , height * 0.01f });
		auto color = getBrush().Color;
		shape.setFillColor({ color.R, color.G, color.B, color.A });
		auto scissor = PRIVATE()->Scissor;
		shape.setScissor({ scissor.X, scissor.Y, scissor.W, scissor.H });
		CONTEXT()->fillElement(&shape);
	}
	if (PRIVATE()->Pen.Style != UIPen::NoPen)
	{
		VGShape shape;
		shape.reset();
		shape.moveTo(x, y);
		shape.lineTo(x, y + height);
		shape.lineTo(x + width, y + height);
		shape.lineTo(x + width, y);
		shape.close();
		shape.setRotate(0);
		shape.setScale(1, 1);
		shape.setTranslate(0, 0);
		shape.setLineWidth(getPen().Width);
		auto color = getPen().Color;
		shape.setStrokeColor({ color.R, color.G, color.B, color.A });
		auto scissor = PRIVATE()->Scissor;
		shape.setScissor({ scissor.X, scissor.Y, scissor.W, scissor.H });
		CONTEXT()->strokeElement(&shape);
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
	if (PRIVATE()->Brush.Style != UIBrush::NoBrush)
	{
		VGShape shape;
		shape.reset();
		shape.arcTo(0 + xRadius, 0 + yRadius, xRadius, yRadius, 180, 90);
		shape.arcTo(0 + width - xRadius, 0 + yRadius, xRadius, yRadius, -90, 90);
		shape.arcTo(0 + width - xRadius, 0 + height - yRadius, xRadius, yRadius, 0, 90);
		shape.arcTo(0 + xRadius, 0 + height - yRadius, xRadius, yRadius, 90, 90);
		shape.close();
		shape.setTranslate({ x, y });
		auto color = getBrush().Color;
		shape.setFillColor({ color.R, color.G, color.B, color.A });
		auto scissor = PRIVATE()->Scissor;
		shape.setScissor({ scissor.X, scissor.Y, scissor.W, scissor.H });
		CONTEXT()->fillElement(&shape);
	}
	if (PRIVATE()->Pen.Style != UIPen::NoPen)
	{
		VGShape shape;
		shape.reset();
		shape.arcTo(0 + xRadius, 0 + yRadius, xRadius, yRadius, 180, 90);
		shape.arcTo(0 + width - xRadius, 0 + yRadius, xRadius, yRadius, -90, 90);
		shape.arcTo(0 + width - xRadius, 0 + height - yRadius, xRadius, yRadius, 0, 90);
		shape.arcTo(0 + xRadius, 0 + height - yRadius, xRadius, yRadius, 90, 90);
		shape.close();
		shape.setTranslate({ x, y });
		shape.setLineWidth(getPen().Width);
		auto color = getPen().Color;
		shape.setStrokeColor({ color.R, color.G, color.B, color.A });
		auto scissor = PRIVATE()->Scissor;
		shape.setScissor({ scissor.X, scissor.Y, scissor.W, scissor.H });
		CONTEXT()->strokeElement(&shape);
	}
}

void OpenUIPainter::drawText(float x, float y, float width, float height, const UIString& text, UIRectRaw boundingRect, float cursor, UIRectRaw cursorRect)
{
	//if (PRIVATE()->Brush.Style != UIBrush::NoBrush)
	//{
	//	if (PRIVATE()->TextFillShape == nullptr)
	//	{
	//		PRIVATE()->TextFillShape = VGNew<VGText>();
	//		PRIVATE()->TextFillShape->setFamily(getFont().Family);
	//		PRIVATE()->TextFillShape->setText(0, 0, width, height, text);
	//	}
	//	PRIVATE()->TextFillShape->setRotate(0);
	//	PRIVATE()->TextFillShape->setTranslate(x, y);
	//	//PRIVATE()->TextFillShape->setScale(getFont().Size * 0.01f, getFont().Size * 0.01f);
	//	PRIVATE()->TextFillShape->setScale(25 * 0.01f, 25 * 0.01f);
	//	PRIVATE()->TextFillShape->setFillColor({ getBrush().Color.R, getBrush().Color.G, getBrush().Color.B, getBrush().Color.A });
	//	PRIVATE()->Context->fillElement(PRIVATE()->TextFillShape);
	//}
	//if (PRIVATE()->Pen.Style != UIPen::NoPen)
	//{
	//	auto shape = VGNew<VGText>();
	//	shape->setText(x, y, width, height, text);
	//	shape->setLineWidth(getPen().Width);
	//	shape->setStrokeColor({ getPen().Color.R, getPen().Color.G, getPen().Color.B, getPen().Color.A });
	//	PRIVATE()->Context->strokeElement(shape);
	//}
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
	PRIVATE()->Scissor = { x, y, width, height };
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
	auto& primitives = PRIVATE()->Context->getPainter()->getPrimitives();
	if (primitives.size())
	{
		auto client = PRIVATE()->Client;
		glBindFramebuffer(GL_FRAMEBUFFER, PRIVATE()->NativeFrame);
		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glViewport((int32_t)client.X, (int32_t)client.Y, (int32_t)client.W, (int32_t)client.H);
		PRIVATE()->Context->renderElement(PRIVATE()->Client);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	return PRIVATE()->NativeTexture;
}

void OpenUIPainter::setTexture(uint32_t value)
{
	PRIVATE()->NativeTexture = value;
	glBindFramebuffer(GL_FRAMEBUFFER, PRIVATE()->NativeFrame);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, PRIVATE()->NativeTexture, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "FBO is not complete!" << std::endl;
		::exit(-1);
	}
}
