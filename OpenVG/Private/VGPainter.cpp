#include "VGPainter.h"
#include "VGPainter.h"
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
			auto fillIndex = (int32_t)cache->FillStyle.size();
			auto strokeIndex = (int32_t)-1;
			auto imageIndex = (int32_t)cache->ImageList.size();
			auto& primitives = cache->Primitive;
			auto& fills = cache->FillStyle;
			auto& strokes = cache->StrokeStyle;
			auto& images = cache->ImageList;
			auto& linears = cache->LinearGradient;
			auto& radials = cache->RadialGradient;
			for (size_t i = 0; i + 3 <= indies.size(); i += 3)
			{
				auto index0 = indies[i + 0];
				auto index1 = indies[i + 1];
				auto index2 = indies[i + 2];
				auto point0 = points[index0];
				auto point1 = points[index1];
				auto point2 = points[index2];
				auto& primitive0 = primitives.emplace_back();
				primitive0 = { point0.X, point0.Y, fillIndex, strokeIndex };
				auto& primitive1 = primitives.emplace_back();
				primitive1 = { point1.X, point1.Y, fillIndex, strokeIndex };
				auto& primitive2 = primitives.emplace_back();
				primitive2 = { point2.X, point2.Y, fillIndex, strokeIndex };
			}

			auto style = element->getFillStyle();
			auto& fill = fills.emplace_back();
			if (style) fill.Color = style->Color;
			if (style && style->Image.Data.size())
			{
				auto& image = images.emplace_back();
				image = style->Image;
				fill.Image = imageIndex;
			}
			else
			{
				fill.Image = -1;
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
				else
				{
					linear.NumStops.X = 0;
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
				else
				{
					radial.NumStops.X = 0;
				}
			}
		}
		element->setClipCache(cache);
	}
	PRIVATE()->PrimitiveList.insert(PRIVATE()->PrimitiveList.end(), *element->getFillCache());
}

void VGPainter::fill(VGElementRaw element)
{
	if (element->getFillCache() == nullptr)
	{
		auto cache = VGNew<VGPrimitive>();
		VGVector<VGTessellate::point_t> points;
		VGVector<VGTessellate::index_t> indies;
		if (VGTessellate::Fill(element, points, indies))
		{
			auto& fills = cache->FillStyle;
			auto& strokes = cache->StrokeStyle;
			auto& images = cache->ImageList;
			auto& linears = cache->LinearGradient;
			auto& radials = cache->RadialGradient;
			auto& primitives = cache->Primitive;
			auto& matrixs = cache->MatrixList;

			auto fillIndex = (int32_t)fills.size();
			auto strokeIndex = (int32_t)-1;
			auto imageIndex = (int32_t)images.size();
			auto matrixIndex = (int32_t)matrixs.size();

			for (size_t i = 0; i + 3 <= indies.size(); i += 3)
			{
				auto index0 = indies[i + 0];
				auto index1 = indies[i + 1];
				auto index2 = indies[i + 2];
				auto point0 = points[index0];
				auto point1 = points[index1];
				auto point2 = points[index2];
				auto& primitive0 = primitives.emplace_back();
				primitive0 = { point0.X, point0.Y, fillIndex, strokeIndex, matrixIndex, };
				auto& primitive1 = primitives.emplace_back();
				primitive1 = { point1.X, point1.Y, fillIndex, strokeIndex, matrixIndex, };
				auto& primitive2 = primitives.emplace_back();
				primitive2 = { point2.X, point2.Y, fillIndex, strokeIndex, matrixIndex, };
			}

			auto& matrix = matrixs.emplace_back();
			matrix.rotate(VGDeg2Rad(0));
			matrix.print();

			auto style = element->getFillStyle();
			auto& fill = fills.emplace_back();
			if (style) fill.Color = style->Color;
			if (style && style->Image.Data.size())
			{
				auto& image = images.emplace_back();
				image = style->Image;
				fill.Image = imageIndex;
			}
			else
			{
				fill.Image = -1;
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
				else
				{
					linear.NumStops.X = 0;
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
				else
				{
					radial.NumStops.X = 0;
				}
			}
		}
		element->setFillCache(cache);
	}
	PRIVATE()->PrimitiveList.push_back(*element->getFillCache());
}

void VGPainter::stroke(VGElementRaw element)
{
	if (element->getStrokeCache() == nullptr)
	{
		auto cache = VGNew<VGPrimitive>();
		VGVector<VGTessellate::point_t> points;
		VGVector<VGTessellate::index_t> indies;
		if (VGTessellate::Stroke(element, points, indies))
		{
			auto fillIndex = (int32_t)-1;
			auto strokeIndex = (int32_t)cache->StrokeStyle.size();
			auto imageIndex = (int32_t)cache->ImageList.size();
			auto& primitives = cache->Primitive;
			auto& fills = cache->FillStyle;
			auto& strokes = cache->StrokeStyle;
			auto& images = cache->ImageList;
			auto& linears = cache->LinearGradient;
			auto& radials = cache->RadialGradient;
			for (size_t i = 0; i + 3 <= indies.size(); i += 3)
			{
				auto index0 = indies[i + 0];
				auto index1 = indies[i + 1];
				auto index2 = indies[i + 2];
				auto point0 = points[index0];
				auto point1 = points[index1];
				auto point2 = points[index2];
				auto& primitive0 = primitives.emplace_back();
				primitive0 = { point0.X, point0.Y, fillIndex, strokeIndex };
				auto& primitive1 = primitives.emplace_back();
				primitive1 = { point1.X, point1.Y, fillIndex, strokeIndex };
				auto& primitive2 = primitives.emplace_back();
				primitive2 = { point2.X, point2.Y, fillIndex, strokeIndex };
			}

			auto style = element->getStrokeStyle();
			auto& stroke = strokes.emplace_back();
			if (style) stroke.Color = style->Color;
			if (style && style->Image.Data.size())
			{
				auto& image = images.emplace_back();
				image = style->Image;
				stroke.Image = imageIndex;
			}
			else
			{
				stroke.Image = -1;
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
				else
				{
					linear.NumStops.X = 0;
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
				else
				{
					radial.NumStops.X = 0;
				}
			}
		}
		element->setStrokeCache(cache);
	}
	PRIVATE()->PrimitiveList.insert(PRIVATE()->PrimitiveList.end(), *element->getStrokeCache());
}

VGVector<VGPrimitive>& VGPainter::getPrimitiveList()
{
	return PRIVATE()->PrimitiveList;
}

VGVector<VGPrimitive> const& VGPainter::getPrimitiveList() const
{
	return PRIVATE()->PrimitiveList;
}