#include "../VGPainter.h"
#include "VGTessellate.h"

class VGPainterPrivateData : public VGPainterPrivate
{
public:
	VGVector<VGPrimitive> PrimitiveList;
};
#define PRIVATE() ((VGPainterPrivateData*) m_Private)

VGPainter::VGPainter()
{
	m_Private = new VGPainterPrivateData;
}

VGPainter::~VGPainter()
{
	delete m_Private; m_Private = nullptr;
}

void VGPainter::clip(VGElementRaw element)
{
	if (element->getClipCache() == nullptr)
	{
		auto cache = VGNew<VGPrimitive>();
		VGVector<VGTessellate::point_t> points;
		VGVector<VGTessellate::index_t> indies;
		if (VGTessellate::Fill(element, points, indies))
		{
			auto& primitives = cache->Primitive;
			for (size_t i = 0; i + 3 <= indies.size(); i += 3)
			{
				auto index0 = indies[i + 0];
				auto index1 = indies[i + 1];
				auto index2 = indies[i + 2];
				auto point0 = points[index0];
				auto point1 = points[index1];
				auto point2 = points[index2];
				auto& primitive0 = primitives.emplace_back();
				primitive0 = { point0.X, point0.Y, -1, -1, 0, };
				auto& primitive1 = primitives.emplace_back();
				primitive1 = { point1.X, point1.Y, -1, -1, 0, };
				auto& primitive2 = primitives.emplace_back();
				primitive2 = { point2.X, point2.Y, -1, -1, 0, };
			}
		}
		element->setClipCache(cache);
	}

	auto& result = PRIVATE()->PrimitiveList.emplace_back();
	result.Primitive = element->getClipCache()->Primitive;
	auto& primitives = result.Primitive;
	auto& fills = result.FillStyle;
	auto& strokes = result.StrokeStyle;
	auto& images = result.ImageList;
	auto& linears = result.LinearGradient;
	auto& radials = result.RadialGradient;
	auto& matrixs = result.MatrixList;

	auto& matrix = matrixs.emplace_back();
	matrix = VGFloat3x3::Transform(element->getTranslate().X, element->getTranslate().Y, element->getRotate(), element->getScaling().X, element->getScaling().Y);
}

void VGPainter::fill(VGElementRaw element)
{
	if (element->getFillStyle() == nullptr) return;

	if (element->getFillCache() == nullptr)
	{
		auto cache = VGNew<VGPrimitive>();
		VGVector<VGTessellate::point_t> points;
		VGVector<VGTessellate::index_t> indies;
		if (VGTessellate::Fill(element, points, indies))
		{
			auto& primitives = cache->Primitive;
			for (size_t i = 0; i + 3 <= indies.size(); i += 3)
			{
				auto index0 = indies[i + 0];
				auto index1 = indies[i + 1];
				auto index2 = indies[i + 2];
				auto point0 = points[index0];
				auto point1 = points[index1];
				auto point2 = points[index2];
				auto& primitive0 = primitives.emplace_back();
				primitive0 = { point0.X, point0.Y, -1, -1, 0, };
				auto& primitive1 = primitives.emplace_back();
				primitive1 = { point1.X, point1.Y, -1, -1, 0, };
				auto& primitive2 = primitives.emplace_back();
				primitive2 = { point2.X, point2.Y, -1, -1, 0, };
			}
		}
		element->setFillCache(cache);
	}

	auto& result = PRIVATE()->PrimitiveList.emplace_back();
	result.Primitive = element->getFillCache()->Primitive;
	auto& primitives = result.Primitive;
	auto& fills = result.FillStyle;
	auto& strokes = result.StrokeStyle;
	auto& images = result.ImageList;
	auto& linears = result.LinearGradient;
	auto& radials = result.RadialGradient;
	auto& matrixs = result.MatrixList;

	auto& matrix = matrixs.emplace_back();
	matrix = VGFloat3x3::Transform(element->getTranslate().X, element->getTranslate().Y, element->getRotate(), element->getScaling().X, element->getScaling().Y);

	for (size_t i = 0; i < primitives.size(); ++i)
	{
		primitives[i].Fill = 0;
		primitives[i].Stroke = -1;
	}

	auto style = element->getFillStyle();
	auto& fill = fills.emplace_back();
	if (style) fill.Color = style->Color;
	if (style && style->Image.Data.size())
	{
		auto& image = images.emplace_back();
		image = style->Image;
		fill.Image = 0;
	}
	if (style && VGCast<VGLinearGradient>(style->Gradient))
	{
		auto gradient = VGCast<VGLinearGradient>(style->Gradient).get();
		auto& linear = linears.emplace_back();
		auto stops = gradient->getColorStop();
		if (stops.size())
		{
			linear.GradStartPos.X = gradient->getStartPos().X;
			linear.GradStartPos.Y = gradient->getStartPos().Y;
			linear.GradEndPos.X = gradient->getEndPos().X;
			linear.GradEndPos.Y = gradient->getEndPos().Y;
			linear.NumStops.X = (uint32_t)stops.size();
			for (size_t i = 0; i < stops.size(); ++i)
			{
				linear.StopColors[i].R = stops[i].R;
				linear.StopColors[i].G = stops[i].G;
				linear.StopColors[i].B = stops[i].B;
				linear.StopColors[i].A = stops[i].A;
				linear.StopPoints[i].X = stops[i].Offset;
			}
		}
	}
	if (style && VGCast<VGRadialGradient>(style->Gradient))
	{
		auto gradient = VGCast<VGRadialGradient>(style->Gradient).get();
		auto& radial = radials.emplace_back();
		auto stops = gradient->getColorStop();
		if (stops.size())
		{
			radial.Radius.X = gradient->getRadius();
			radial.CenterPos.X = gradient->getCenterPos().X;
			radial.CenterPos.Y = gradient->getCenterPos().Y;
			radial.NumStops.X = (uint32_t)stops.size();
			for (size_t i = 0; i < stops.size(); ++i)
			{
				radial.StopColors[i].R = stops[i].R;
				radial.StopColors[i].G = stops[i].G;
				radial.StopColors[i].B = stops[i].B;
				radial.StopColors[i].A = stops[i].A;
				radial.StopPoints[i].X = stops[i].Offset;
			}
		}
	}
}

void VGPainter::stroke(VGElementRaw element)
{
	if (element->getStrokeStyle() == nullptr) return;

	// if (element->getStrokeCache() == nullptr)
	{
		auto cache = VGNew<VGPrimitive>();
		VGVector<VGTessellate::point_t> points;
		VGVector<VGTessellate::index_t> indies;
		if (VGTessellate::Stroke(element, points, indies))
		{
			auto& primitives = cache->Primitive;
			for (size_t i = 0; i + 3 <= indies.size(); i += 3)
			{
				auto index0 = indies[i + 0];
				auto index1 = indies[i + 1];
				auto index2 = indies[i + 2];
				auto point0 = points[index0];
				auto point1 = points[index1];
				auto point2 = points[index2];
				auto& primitive0 = primitives.emplace_back();
				primitive0 = { point0.X, point0.Y, -1, -1, 0, };
				auto& primitive1 = primitives.emplace_back();
				primitive1 = { point1.X, point1.Y, -1, -1, 0, };
				auto& primitive2 = primitives.emplace_back();
				primitive2 = { point2.X, point2.Y, -1, -1, 0, };
			}
		}
		element->setStrokeCache(cache);
	}

	auto& result = PRIVATE()->PrimitiveList.emplace_back();
	result.Primitive = element->getStrokeCache()->Primitive;
	auto& primitives = result.Primitive;
	auto& fills = result.FillStyle;
	auto& strokes = result.StrokeStyle;
	auto& images = result.ImageList;
	auto& linears = result.LinearGradient;
	auto& radials = result.RadialGradient;
	auto& matrixs = result.MatrixList;

	auto& matrix = matrixs.emplace_back();
	matrix = VGFloat3x3::Transform(element->getTranslate().X, element->getTranslate().Y, element->getRotate(), element->getScaling().X, element->getScaling().Y);

	for (size_t i = 0; i < primitives.size(); ++i)
	{
		primitives[i].Fill = -1;
		primitives[i].Stroke = 0;
	}

	auto style = element->getStrokeStyle();
	auto& stroke = strokes.emplace_back();
	if (style) stroke.Color = style->Color;
	if (style && style->Image.Data.size())
	{
		auto& image = images.emplace_back();
		image = style->Image;
		stroke.Image = 0;
	}
	if (style && VGCast<VGLinearGradient>(style->Gradient))
	{
		auto gradient = VGCast<VGLinearGradient>(style->Gradient).get();
		auto& linear = linears.emplace_back();
		auto stops = gradient->getColorStop();
		if (stops.size())
		{
			linear.GradStartPos.X = gradient->getStartPos().X;
			linear.GradStartPos.Y = gradient->getStartPos().Y;
			linear.GradEndPos.X = gradient->getEndPos().X;
			linear.GradEndPos.Y = gradient->getEndPos().Y;
			linear.NumStops.X = (uint32_t)stops.size();
			for (size_t i = 0; i < stops.size(); ++i)
			{
				linear.StopColors[i].R = stops[i].R;
				linear.StopColors[i].G = stops[i].G;
				linear.StopColors[i].B = stops[i].B;
				linear.StopColors[i].A = stops[i].A;
				linear.StopPoints[i].X = stops[i].Offset;
			}
		}
	}
	if (style && VGCast<VGRadialGradient>(style->Gradient))
	{
		auto gradient = VGCast<VGRadialGradient>(style->Gradient).get();
		auto& radial = radials.emplace_back();
		auto stops = gradient->getColorStop();
		if (stops.size())
		{
			radial.Radius.X = gradient->getRadius();
			radial.CenterPos.X = gradient->getCenterPos().X;
			radial.CenterPos.Y = gradient->getCenterPos().Y;
			radial.NumStops.X = (uint32_t)stops.size();
			for (size_t i = 0; i < stops.size(); ++i)
			{
				radial.StopColors[i].R = stops[i].R;
				radial.StopColors[i].G = stops[i].G;
				radial.StopColors[i].B = stops[i].B;
				radial.StopColors[i].A = stops[i].A;
				radial.StopPoints[i].X = stops[i].Offset;
			}
		}
	}
}

VGVector<VGPrimitive>& VGPainter::getPrimitiveList()
{
	return PRIVATE()->PrimitiveList;
}

VGVector<VGPrimitive> const& VGPainter::getPrimitiveList() const
{
	return PRIVATE()->PrimitiveList;
}