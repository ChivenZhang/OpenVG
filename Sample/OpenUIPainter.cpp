#include "OpenUIPainter.h"
#include "OpenVGRender.h"
#include <OpenVG/VGContext.h>
#include <OpenVG/VGShape.h>

class OpenUIPainterPrivate : public UIPainterPrivate
{
public:
	VGContextRef Context;
	uint32_t Texture = 0;
};
#define PRIVATE() ((OpenUIPainterPrivate*) m_Private)

OpenUIPainter::OpenUIPainter(uint32_t width, uint32_t height)
{
	m_Private = new OpenUIPainterPrivate;
	PRIVATE()->Context = VGNew<VGContext>();
	PRIVATE()->Context->setRender(VGNew<OpenVGRender>());
}

OpenUIPainter::~OpenUIPainter()
{
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
	auto shape = VGNew<VGShape>();
	// TODO
	PRIVATE()->Context->addElement(shape);
}

void OpenUIPainter::drawRects(UIArrayView<UIRect> rects)
{
}

void OpenUIPainter::drawRoundedRect(float x, float y, float width, float height, float xRadius, float yRadius)
{
}

void OpenUIPainter::drawText(float x, float y, float width, float height, const UIString& text, UIRectRaw boundingRect, float cursor, UIRectRaw cursorRect)
{
}

UIPen const& OpenUIPainter::getPen() const
{
	return UIPen();
}

void OpenUIPainter::setPen(const UIPen& pen)
{
}

UIBrush const& OpenUIPainter::getBrush() const
{
	return UIBrush();
}

void OpenUIPainter::setBrush(const UIBrush& brush)
{
}

UIFont const& OpenUIPainter::getFont() const
{
	return UIFont();
}

void OpenUIPainter::setFont(const UIFont& font)
{
}

void OpenUIPainter::setClipping(bool enable)
{
}

void OpenUIPainter::setClipRect(float x, float y, float width, float height)
{
}

void OpenUIPainter::setViewport(float x, float y, float width, float height)
{
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
	return 0;
}

uint32_t OpenUIPainter::getHeight() const
{
	return 0;
}

uint32_t OpenUIPainter::getStride() const
{
	return 0;
}

UIArrayView<const uint8_t> OpenUIPainter::getPixelData() const
{
	return UIArrayView<const uint8_t>();
}

void OpenUIPainter::resize(uint32_t width, uint32_t height)
{
}

void OpenUIPainter::setTexture(uint32_t value)
{
	PRIVATE()->Texture = value;
}

uint32_t OpenUIPainter::getTexture() const
{
	return PRIVATE()->Texture;
}
