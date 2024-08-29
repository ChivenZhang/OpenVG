#include "../VGText.h"
#include "VGTrueType.h"

class VGTextPrivateData : public VGElementPrivate
{
public:
	uint32_t Size = 10;
	uint32_t Spacing = 0;
	uint32_t LineSpacing = 0;
	bool LineWrap = false;
	VGString Family = "sans-serif";
	VGTextStyle Style = VGTextStyle::StyleNormal;
	VGTextWeight Weight = VGTextWeight::WeightNormal;
	VGTextAlgins Aligns = VGTextAlgin::AlignLeft | VGTextAlgin::AlignTop;
	VGTextDirection Direction = VGTextDirection::DirectionAutoLayout;
	VGTextEllipsize Ellipsize = VGTextEllipsize::EllipsizeNone;
};
#define PRIVATE() ((VGTextPrivateData*)m_Private)

VGText::VGText()
{
	m_Private = new VGTextPrivateData;
}

VGText::~VGText()
{
	delete m_Private; m_Private = nullptr;
}

VGString VGText::getFamily() const
{
	return PRIVATE()->Family;
}

void VGText::setFamily(VGString value)
{
	PRIVATE()->Family = value;
}

uint32_t VGText::getSize() const
{
	return PRIVATE()->Size;
}

void VGText::setSize(uint32_t value)
{
	PRIVATE()->Size = value;
}

uint32_t VGText::getSpacing() const
{
	return PRIVATE()->Spacing;
}

void VGText::setSpacing(uint32_t value)
{
	PRIVATE()->Size = value;
}

uint32_t VGText::getLineSpacing() const
{
	return PRIVATE()->LineSpacing;
}

void VGText::getLineSpacing(uint32_t value)
{
	PRIVATE()->LineSpacing = value;
}

bool VGText::getLineWrap() const
{
	return PRIVATE()->LineWrap;
}

void VGText::setLineWrap(bool value)
{
	PRIVATE()->LineWrap = value;
}

VGTextStyle VGText::getStyle() const
{
	return PRIVATE()->Style;
}

void VGText::setStyle(VGTextStyle value)
{
	PRIVATE()->Style = value;
}

VGTextWeight VGText::getWeight() const
{
	return PRIVATE()->Weight;
}

void VGText::setWeight(VGTextWeight value)
{
	PRIVATE()->Weight = value;
}

VGTextAlgins VGText::getAlignment() const
{
	return PRIVATE()->Aligns;
}

void VGText::setAlignment(VGTextAlgins value)
{
	PRIVATE()->Aligns = value;
}

VGTextDirection VGText::getDirection() const
{
	return PRIVATE()->Direction;
}

void VGText::setDirection(VGTextDirection value)
{
	PRIVATE()->Direction = value;
}

VGTextEllipsize VGText::getEllipsize() const
{
	return PRIVATE()->Ellipsize;
}

void VGText::setEllipsize(VGTextEllipsize value)
{
	PRIVATE()->Ellipsize = value;
}

void VGText::setText(float x, float y, float width, float height, VGString text)
{
	/*VGVector<VGPoint> points;
	VGVector<VGPointType> types;
	if (VGTrueType::Path(this, { x,y,width,height }, text, points, types) && points.size() && types.size())
	{
		for (size_t i = 0, k = 0; i < types.size(); ++i)
		{
			switch (types[i])
			{
			case VGPointType::MoveTo: moveTo(points[k++]); break;
			case VGPointType::LineTo: lineTo(points[k++]); break;
			case VGPointType::CurveTo: curveTo(points[k], points[k + 1]); k += 2; break;
			case VGPointType::CubicTo: cubicTo(points[k], points[k + 1], points[k + 2]); k += 3; break;
			case VGPointType::Close: close(); break;
			}
		}
	}*/
}
