#include "VGTessellate.h"
#include <micro-tess/path.h>
#include <micro-tess/static_array.h>
#include <micro-tess/dynamic_array.h>
#include <micro-tess/std_rebind_allocator.h>

bool VGTessellate::Fill(VGElementRaw element, VGVector<point_t>& outPoints, VGVector<index_t>& outIndices)
{
	auto points = element->getPointList();
	auto types = element->getPointTypeList();
	microtess::path<float, dynamic_array, microtess::std_rebind_allocator<>> path{};
	for (size_t i = 0, k = 0; i < types.size(); ++i)
	{
		switch (types[i])
		{
		case VGPointType::MoveTo:
		{
			path.moveTo({ points[k].X, points[k].Y });
			k += 1;
		} break;
		case VGPointType::LineTo:
		{
			path.lineTo({ points[k].X, points[k].Y });
			k += 1;
		} break;
		case VGPointType::CurveTo:
		{
			auto c1 = points[k + 0];
			auto last = points[k + 1];
			path.quadraticCurveTo({ c1.X, c1.Y }, { last.X, last.Y });
			k += 2;
		} break;
		case VGPointType::CubicTo:
		{
			auto c1 = points[k + 0];
			auto c2 = points[k + 1];
			auto last = points[k + 2];
			path.cubicBezierCurveTo({ c1.X, c1.Y }, { c2.X, c2.Y }, { last.X, last.Y });
			k += 3;
		} break;
		case VGPointType::ArcTo:
		{
			auto c1 = points[k + 0];
			auto r1 = points[k + 1];
			auto r = points[k + 2];
			auto a1 = points[k + 3];
			path.ellipse({ c1.X, c1.Y }, r1.X, r1.Y, r.X, a1.X, a1.Y, false);
			k += 4;
		} break;
		case VGPointType::PieTo:
		{
			auto c1 = points[k + 0];
			auto r1 = points[k + 1];
			auto r = points[k + 2];
			auto a1 = points[k + 3];
			path.ellipse({ c1.X, c1.Y }, r1.X, r1.Y, r.X, a1.X, a1.Y, false);
			k += 4;
		} break;
		case VGPointType::ChordTo:
		{
			auto c1 = points[k + 0];
			auto r1 = points[k + 1];
			auto r = points[k + 2];
			auto a1 = points[k + 3];
			path.ellipse({ c1.X, c1.Y }, r1.X, r1.Y, r.X, a1.X, a1.Y, false);
			k += 4;
		} break;
		case VGPointType::Close:
		{
			path.closePath();
		} break;
		}
	}
	auto& buffers = path.tessellateFill<>(microtess::fill_rule::even_odd, microtess::tess_quality::fine, false, false);
	auto& vertices = buffers.output_vertices;
	auto& indices = buffers.output_indices;
	if (vertices.size() && indices.size())
	{
		outPoints.resize(vertices.size());
		::memcpy(outPoints.data(), vertices.data(), sizeof(VGFloat2) * vertices.size());
		outIndices.resize(indices.size());
		::memcpy(outIndices.data(), indices.data(), sizeof(uint32_t) * indices.size());
		return true;
	}
	return false;
}

bool VGTessellate::Stroke(VGElementRaw element, VGVector<point_t>& outPoints, VGVector<index_t>& outIndices)
{
	auto points = element->getPointList();
	auto types = element->getPointTypeList();
	microtess::path<float, dynamic_array, microtess::std_rebind_allocator<>> path{};
	for (size_t i = 0, k = 0; i < types.size(); ++i)
	{
		switch (types[i])
		{
		case VGPointType::MoveTo:
		{
			path.moveTo({ points[k].X, points[k].Y });
			k += 1;
		} break;
		case VGPointType::LineTo:
		{
			path.lineTo({ points[k].X, points[k].Y });
			k += 1;
		} break;
		case VGPointType::CurveTo:
		{
			auto c1 = points[k + 0];
			auto last = points[k + 1];
			path.quadraticCurveTo({ c1.X, c1.Y }, { last.X, last.Y });
			k += 2;
		} break;
		case VGPointType::CubicTo:
		{
			auto c1 = points[k + 0];
			auto c2 = points[k + 1];
			auto end = points[k + 2];
			path.cubicBezierCurveTo({ c1.X, c1.Y }, { c2.X, c2.Y }, { end.X, end.Y });
			k += 3;
		} break;
		case VGPointType::ArcTo:
		{
			auto c1 = points[k + 0];
			auto r1 = points[k + 1];
			auto r = points[k + 2];
			auto a1 = points[k + 3];
			path.ellipse({ c1.X, c1.Y }, r1.X, r1.Y, r.X, a1.X, a1.Y, false);
			k += 4;
		} break;
		case VGPointType::Close:
		{
			path.closePath();
		} break;
		}
	}

	auto strokeWidth = element->getLineWidth();
	auto miterLimit = element->getMiterLimit();
	auto dashOffset = (int32_t)element->getDashOffset();
	auto dashControl = element->getLineDash();
	auto strokeCap = microtess::stroke_cap::butt;
	switch (element->getStrokeStyle()->LineCap)
	{
	case VGStrokeCap::Butt: strokeCap = microtess::stroke_cap::butt; break;
	case VGStrokeCap::Round: strokeCap = microtess::stroke_cap::round; break;
	case VGStrokeCap::Square: strokeCap = microtess::stroke_cap::square; break;
	default: strokeCap = microtess::stroke_cap::butt; break;
	}
	auto strokeJoin = microtess::stroke_line_join::bevel;
	switch (element->getStrokeStyle()->LineJoin)
	{
	case VGStrokeJoin::Bevel: strokeJoin = microtess::stroke_line_join::bevel; break;
	case VGStrokeJoin::Miter: strokeJoin = microtess::stroke_line_join::miter; break;
	case VGStrokeJoin::Round: strokeJoin = microtess::stroke_line_join::round; break;
	default: strokeJoin = microtess::stroke_line_join::bevel; break;
	}
	auto& buffers = path.tessellateStroke<>(strokeWidth, strokeCap, strokeJoin, miterLimit, dashControl, dashOffset, false);
	auto& vertices = buffers.output_vertices;
	auto& indices = buffers.output_indices;
	if (vertices.size() && indices.size())
	{
		// 遍历三角带的每个顶点（除了最后两个，因为每个三角形需要三个顶点）  
		for (size_t i = 0; i < indices.size() - 2; ++i)
		{
			// 偶数索引：使用当前顶点、前一个顶点、下一个顶点  
			outIndices.push_back(indices[i + 0]);
			outIndices.push_back(indices[i + 1]);
			outIndices.push_back(indices[i + 2]);
		}
		outPoints.resize(vertices.size());
		::memcpy(outPoints.data(), vertices.data(), sizeof(VGFloat2) * vertices.size());
		return true;
	}
	return false;
}