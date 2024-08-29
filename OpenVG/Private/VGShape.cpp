#include "VGShape.h"
#include "VGTessellate.h"

class VGShapePrivateData : public VGElementPrivate
{
public:
	VGFillStyleRef FillStyle;
	VGStrokeStyleRef StrokeStyle;
	VGVector<VGPoint> PointList;
	VGVector<VGPointType> PointTypeList;
};
#define PRIVATE() ((VGShapePrivateData*) m_Private)

VGShape::VGShape()
{
	m_Private = new VGShapePrivateData;
}

VGShape::~VGShape()
{
	delete m_Private; m_Private = nullptr;
}

VGColor VGShape::getFillColor() const
{
	if (PRIVATE()->FillStyle == nullptr) return VGColor();
	return PRIVATE()->FillStyle->Color;
}

void VGShape::setFillColor(VGColor value)
{
	if (PRIVATE()->FillStyle == nullptr) PRIVATE()->FillStyle = VGNew<VGFillStyle>();
	PRIVATE()->FillStyle->Color = value;
}

VGColor VGShape::getStrokeColor() const
{
	if (PRIVATE()->StrokeStyle == nullptr) return VGColor();
	return PRIVATE()->StrokeStyle->Color;
}

void VGShape::setStrokeColor(VGColor value)
{
	if (PRIVATE()->StrokeStyle == nullptr) PRIVATE()->StrokeStyle = VGNew<VGStrokeStyle>();
	PRIVATE()->StrokeStyle->Color = value;
}

float VGShape::getLineWidth() const
{
	if (PRIVATE()->StrokeStyle == nullptr) return float();
	return PRIVATE()->StrokeStyle->LineWidth;
}

void VGShape::setLineWidth(float value)
{
	if (PRIVATE()->StrokeStyle == nullptr) PRIVATE()->StrokeStyle = VGNew<VGStrokeStyle>();
	PRIVATE()->StrokeStyle->LineWidth = value;
}

float VGShape::getMiterLimit() const
{
	if (PRIVATE()->StrokeStyle == nullptr) return float();
	return PRIVATE()->StrokeStyle->MiterLimit;
}

void VGShape::setMiterLimit(float value)
{
	if (PRIVATE()->StrokeStyle == nullptr) PRIVATE()->StrokeStyle = VGNew<VGStrokeStyle>();
	PRIVATE()->StrokeStyle->MiterLimit = value;
}

VGStrokeCap VGShape::getLineCap() const
{
	if (PRIVATE()->StrokeStyle == nullptr) return VGStrokeCap::Square;
	return PRIVATE()->StrokeStyle->LineCap;
}

void VGShape::setLineCap(VGStrokeCap value)
{
	if (PRIVATE()->StrokeStyle == nullptr) PRIVATE()->StrokeStyle = VGNew<VGStrokeStyle>();
	PRIVATE()->StrokeStyle->LineCap = value;
}

VGStrokeJoin VGShape::getLineJoin() const
{
	if (PRIVATE()->StrokeStyle == nullptr) return VGStrokeJoin::Bevel;
	return PRIVATE()->StrokeStyle->LineJoin;
}

void VGShape::setLineJoin(VGStrokeJoin value)
{
	if (PRIVATE()->StrokeStyle == nullptr) PRIVATE()->StrokeStyle = VGNew<VGStrokeStyle>();
	PRIVATE()->StrokeStyle->LineJoin = value;
}

float VGShape::getDashOffset() const
{
	if (PRIVATE()->StrokeStyle == nullptr) return float();
	return PRIVATE()->StrokeStyle->DashOffset;
}

VGArrayView<const float> VGShape::getLineDash() const
{
	if (PRIVATE()->StrokeStyle == nullptr) return VGArrayView<const float>();
	return PRIVATE()->StrokeStyle->DashControl;
}

void VGShape::setLineDash(VGVector<float> value, float offset)
{
	if (PRIVATE()->StrokeStyle == nullptr) PRIVATE()->StrokeStyle = VGNew<VGStrokeStyle>();
	PRIVATE()->StrokeStyle->DashControl = value;
	PRIVATE()->StrokeStyle->DashOffset = offset;
}

VGArrayView<const VGPoint> VGShape::getPointList() const
{
	return PRIVATE()->PointList;
}

VGArrayView<const VGPointType> VGShape::getTypeList() const
{
	return PRIVATE()->PointTypeList;
}

void VGShape::moveTo(float x, float y)
{
	setClipCache(nullptr);
	setFillCache(nullptr);
	setStrokeCache(nullptr);
	PRIVATE()->PointList.push_back({ x, y });
	PRIVATE()->PointTypeList.push_back(VGPointType::MoveTo);
}

void VGShape::lineTo(float x, float y)
{
	setClipCache(nullptr);
	setFillCache(nullptr);
	setStrokeCache(nullptr);
	PRIVATE()->PointList.push_back({ x, y });
	PRIVATE()->PointTypeList.push_back(VGPointType::LineTo);
}

void VGShape::curveTo(float cx1, float cy1, float x, float y)
{
	setClipCache(nullptr);
	setFillCache(nullptr);
	setStrokeCache(nullptr);
	PRIVATE()->PointList.push_back({ cx1, cy1 });
	PRIVATE()->PointList.push_back({ x, y });
	PRIVATE()->PointTypeList.push_back(VGPointType::CurveTo);
}

void VGShape::cubicTo(float cx1, float cy1, float cx2, float cy2, float x, float y)
{
	setClipCache(nullptr);
	setFillCache(nullptr);
	setStrokeCache(nullptr);
	PRIVATE()->PointList.push_back({ cx1, cy1 });
	PRIVATE()->PointList.push_back({ cx2, cy2 });
	PRIVATE()->PointList.push_back({ x, y });
	PRIVATE()->PointTypeList.push_back(VGPointType::CubicTo);
}

void VGShape::arcTo(float cx1, float cy1, float rx, float ry, float r, float a1, float a2)
{
	setClipCache(nullptr);
	setFillCache(nullptr);
	setStrokeCache(nullptr);
	PRIVATE()->PointList.push_back({ cx1, cy1 });
	PRIVATE()->PointList.push_back({ rx, ry });
	PRIVATE()->PointList.push_back({ VGDeg2Rad(a1), VGDeg2Rad(a2) });
	PRIVATE()->PointTypeList.push_back(VGPointType::ArcTo);
}

void VGShape::pieTo(float cx1, float cy1, float rx, float ry, float r, float a1, float a2)
{
	setClipCache(nullptr);
	setFillCache(nullptr);
	setStrokeCache(nullptr);
	PRIVATE()->PointList.push_back({ cx1, cy1 });
	PRIVATE()->PointList.push_back({ rx, ry });
	PRIVATE()->PointList.push_back({ VGDeg2Rad(a1), VGDeg2Rad(a2) });
	PRIVATE()->PointTypeList.push_back(VGPointType::PieTo);
}

void VGShape::chordTo(float cx1, float cy1, float rx, float ry, float r, float a1, float a2)
{
	setClipCache(nullptr);
	setFillCache(nullptr);
	setStrokeCache(nullptr);
	PRIVATE()->PointList.push_back({ cx1, cy1 });
	PRIVATE()->PointList.push_back({ rx, ry });
	PRIVATE()->PointList.push_back({ VGDeg2Rad(a1), VGDeg2Rad(a2) });
	PRIVATE()->PointTypeList.push_back(VGPointType::ChordTo);
}

void VGShape::close()
{
	if (PRIVATE()->PointTypeList.empty()) return;
	if (PRIVATE()->PointTypeList.back() == VGPointType::Close) return;
	setClipCache(nullptr);
	setFillCache(nullptr);
	setStrokeCache(nullptr);
	PRIVATE()->PointTypeList.push_back(VGPointType::Close);
}

void VGShape::reset()
{
	PRIVATE()->FillStyle = nullptr;
	PRIVATE()->StrokeStyle = nullptr;
	setClipCache(nullptr);
	setFillCache(nullptr);
	setStrokeCache(nullptr);
	PRIVATE()->PointList.clear();
	PRIVATE()->PointTypeList.clear();
}

void VGShape::arc(float x, float y, float width, float height, float startAngle, float spanAngle)
{
	arcTo(x + width * 0.5f, y + height * 0.5f, width * 0.5f, height * 0.5f, 0, startAngle, spanAngle);
}

void VGShape::pie(float x, float y, float width, float height, float startAngle, float spanAngle)
{
	pieTo(x + width * 0.5f, y + height * 0.5f, width * 0.5f, height * 0.5f, 0, startAngle, spanAngle);
}

void VGShape::chord(float x, float y, float width, float height, float startAngle, float spanAngle)
{
	chordTo(x + width * 0.5f, y + height * 0.5f, width * 0.5f, height * 0.5f, 0, startAngle, spanAngle);
}

void VGShape::ellipse(float x, float y, float width, float height)
{
	arcTo(x + width * 0.5f, y + height * 0.5f, width * 0.5f, height * 0.5f, 0, 0, 360);
}

void VGShape::rect(float x, float y, float width, float height, float roundX, float roundY)
{
	moveTo(x, y);
	lineTo(x, y + height);
	lineTo(x + width, y + height);
	lineTo(x + width, y);
}

void VGShape::line(float x1, float y1, float x2, float y2)
{
	moveTo(x1, y1);
	lineTo(x2, y2);
}

void VGShape::point(float x, float y)
{
	rect(x, y, 2, 2);
}

void VGShape::clip()
{
	if (getClipCache() == nullptr)
	{
		auto cache = VGNew<VGPrimitive>();
		VGVector<VGTessellate::point_t> _points;
		VGVector<VGTessellate::index_t> _indies;
		if (VGTessellate::Fill(this, _points, _indies))
		{
			auto& points = cache->PointList;
			for (size_t i = 0; i + 3 <= _indies.size(); i += 3)
			{
				auto point0 = _points[_indies[i + 0]];
				auto point1 = _points[_indies[i + 1]];
				auto point2 = _points[_indies[i + 2]];
				points.emplace_back(VGPrimitive::point_t{ point0.X, point0.Y, -1, -1, 0, 0, });
				points.emplace_back(VGPrimitive::point_t{ point1.X, point1.Y, -1, -1, 0, 0, });
				points.emplace_back(VGPrimitive::point_t{ point2.X, point2.Y, -1, -1, 0, 0, });
			}
		}
		setClipCache(cache);
	}
}

void VGShape::fill()
{
	if (PRIVATE()->PointTypeList.empty()) return;
	if (PRIVATE()->FillStyle == nullptr) return;

	if (getFillCache() == nullptr)
	{
		auto cache = VGNew<VGPrimitive>();
		VGVector<VGTessellate::point_t> _points;
		VGVector<VGTessellate::index_t> _indies;
		if (VGTessellate::Fill(this, _points, _indies))
		{
			auto& points = cache->PointList;
			auto& styles = cache->StyleList;
			auto& images = cache->ImageList;
			auto& linears = cache->LinearList;
			auto& radials = cache->RadialList;
			auto& matrixs = cache->MatrixList;
			for (size_t i = 0; i + 3 <= _indies.size(); i += 3)
			{
				auto point0 = _points[_indies[i + 0]];
				auto point1 = _points[_indies[i + 1]];
				auto point2 = _points[_indies[i + 2]];
				points.emplace_back(VGPrimitive::point_t{ point0.X, point0.Y, -1, -1, 0, 0, });
				points.emplace_back(VGPrimitive::point_t{ point1.X, point1.Y, -1, -1, 0, 0, });
				points.emplace_back(VGPrimitive::point_t{ point2.X, point2.Y, -1, -1, 0, 0, });
			}

			auto _style = PRIVATE()->FillStyle;
			auto& style = styles.emplace_back();

			auto& matrix = matrixs.emplace_back();
			matrix = VGFloat3x3::Transform(getTranslate().X, getTranslate().Y, getRotate(), getScale().X, getScale().Y);

			if (_style) style.Color = _style->Color;
			if (_style && _style->Image.Data.size())
			{
				style.Image = 0;
				style.Flags |= (VG_FLAGS_COLOR_IMAGE);
				images.emplace_back(_style->Image);
			}
			if (_style && VGCast<VGLinearGradient>(_style->Gradient))
			{
				style.Flags |= (VG_FLAGS_STYLE_LINEAR);

				auto gradient = VGCast<VGLinearGradient>(_style->Gradient).get();
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
			if (_style && VGCast<VGRadialGradient>(_style->Gradient))
			{
				style.Flags |= (VG_FLAGS_STYLE_RADIAL);

				auto gradient = VGCast<VGRadialGradient>(_style->Gradient).get();
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
		setFillCache(cache);
	}
}

void VGShape::stroke()
{
	if (PRIVATE()->PointTypeList.empty()) return;
	if (PRIVATE()->StrokeStyle == nullptr) return;

	if (getStrokeCache() == nullptr)
	{
		auto cache = VGNew<VGPrimitive>();
		VGVector<VGTessellate::point_t> _points;
		VGVector<VGTessellate::index_t> _indies;
		if (VGTessellate::Stroke(this, _points, _indies))
		{
			auto& points = cache->PointList;
			auto& styles = cache->StyleList;
			auto& images = cache->ImageList;
			auto& linears = cache->LinearList;
			auto& radials = cache->RadialList;
			auto& matrixs = cache->MatrixList;
			for (size_t i = 0; i + 3 <= _indies.size(); i += 3)
			{
				auto point0 = _points[_indies[i + 0]];
				auto point1 = _points[_indies[i + 1]];
				auto point2 = _points[_indies[i + 2]];
				points.emplace_back(VGPrimitive::point_t{ point0.X, point0.Y, -1, -1, 0, 0, });
				points.emplace_back(VGPrimitive::point_t{ point1.X, point1.Y, -1, -1, 0, 0, });
				points.emplace_back(VGPrimitive::point_t{ point2.X, point2.Y, -1, -1, 0, 0, });
			}

			auto _style = PRIVATE()->StrokeStyle;
			auto& style = styles.emplace_back();

			auto& matrix = matrixs.emplace_back();
			matrix = VGFloat3x3::Transform(getTranslate().X, getTranslate().Y, getRotate(), getScale().X, getScale().Y);

			style.Flags |= (VG_FLAGS_FILL_STROKE);

			if (_style) style.Color = _style->Color;
			if (_style && _style->Image.Data.size())
			{
				style.Image = 0;
				style.Flags |= (VG_FLAGS_COLOR_IMAGE);
				images.emplace_back(_style->Image);
			}
			if (_style && VGCast<VGLinearGradient>(_style->Gradient))
			{
				style.Flags |= (VG_FLAGS_STYLE_LINEAR);

				auto gradient = VGCast<VGLinearGradient>(_style->Gradient).get();
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
			if (_style && VGCast<VGRadialGradient>(_style->Gradient))
			{
				style.Flags |= (VG_FLAGS_STYLE_RADIAL);

				auto gradient = VGCast<VGRadialGradient>(_style->Gradient).get();
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
		setStrokeCache(cache);
	}
}
