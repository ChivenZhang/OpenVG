#include "VGTessellate.h"
#include <tesselator.h>

struct Bezier
{
	VGPoint start;
	VGPoint ctrl1;
	VGPoint ctrl2;
	VGPoint end;

	void split(float t, Bezier& left);
	void split(Bezier& left, Bezier& right) const;
	void split(float at, Bezier& left, Bezier& right) const;
	float length() const;
	float lengthApprox() const;
	float at(float at, float length) const;
	float atApprox(float at, float length) const;
	VGPoint at(float t) const;
	float angle(float t) const;
};

#define BEZIER_EPSILON 1e-2f
#define MATH_PI  3.14159265358979323846f

static inline float _deg2rad(float degree)
{
	return degree * (MATH_PI / 180.0f);
}

static inline float _rad2deg(float radian)
{
	return radian * (180.0f / MATH_PI);
}

float _atan2(float y, float x)
{
	if (y == 0.0f && x == 0.0f) return 0.0f;
	auto a = std::min(fabsf(x), fabsf(y)) / std::max(fabsf(x), fabsf(y));
	auto s = a * a;
	auto r = ((-0.0464964749f * s + 0.15931422f) * s - 0.327622764f) * s * a + a;
	if (fabsf(y) > fabsf(x)) r = 1.57079637f - r;
	if (x < 0) r = 3.14159274f - r;
	if (y < 0) return -r;
	return r;
}

static float _lineLength(const VGPoint& pt1, const VGPoint& pt2)
{
	VGPoint diff = { pt2.X - pt1.X, pt2.Y - pt1.Y };
	return sqrtf(diff.X * diff.X + diff.Y * diff.Y);
}

template<typename LengthFunc>
float _bezLength(const Bezier& cur, LengthFunc lineLengthFunc)
{
	Bezier left, right;
	auto len = lineLengthFunc(cur.start, cur.ctrl1) + lineLengthFunc(cur.ctrl1, cur.ctrl2) + lineLengthFunc(cur.ctrl2, cur.end);
	auto chord = lineLengthFunc(cur.start, cur.end);

	if (fabsf(len - chord) > BEZIER_EPSILON) {
		cur.split(left, right);
		return _bezLength(left, lineLengthFunc) + _bezLength(right, lineLengthFunc);
	}
	return len;
}

static float _lineLengthApprox(const VGPoint& pt1, const VGPoint& pt2)
{
	/* approximate sqrt(x*x + y*y) using alpha max plus beta min algorithm.
	   With alpha = 1, beta = 3/8, giving results with the largest error less
	   than 7% compared to the exact value. */
	VGPoint diff = { pt2.X - pt1.X, pt2.Y - pt1.Y };
	if (diff.X < 0) diff.X = -diff.X;
	if (diff.Y < 0) diff.Y = -diff.Y;
	return (diff.X > diff.Y) ? (diff.X + diff.Y * 0.375f) : (diff.Y + diff.X * 0.375f);
}

void Bezier::split(Bezier& left, Bezier& right) const
{
	auto c = (ctrl1.X + ctrl2.X) * 0.5f;
	left.ctrl1.X = (start.X + ctrl1.X) * 0.5f;
	right.ctrl2.X = (ctrl2.X + end.X) * 0.5f;
	left.start.X = start.X;
	right.end.X = end.X;
	left.ctrl2.X = (left.ctrl1.X + c) * 0.5f;
	right.ctrl1.X = (right.ctrl2.X + c) * 0.5f;
	left.end.X = right.start.X = (left.ctrl2.X + right.ctrl1.X) * 0.5f;

	c = (ctrl1.Y + ctrl2.Y) * 0.5f;
	left.ctrl1.Y = (start.Y + ctrl1.Y) * 0.5f;
	right.ctrl2.Y = (ctrl2.Y + end.Y) * 0.5f;
	left.start.Y = start.Y;
	right.end.Y = end.Y;
	left.ctrl2.Y = (left.ctrl1.Y + c) * 0.5f;
	right.ctrl1.Y = (right.ctrl2.Y + c) * 0.5f;
	left.end.Y = right.start.Y = (left.ctrl2.Y + right.ctrl1.Y) * 0.5f;
}


void Bezier::split(float at, Bezier& left, Bezier& right) const
{
	right = *this;
	auto t = right.at(at, right.length());
	right.split(t, left);
}


float Bezier::length() const
{
	return _bezLength(*this, _lineLength);
}


float Bezier::lengthApprox() const
{
	return _bezLength(*this, _lineLengthApprox);
}

template<typename LengthFunc>
float _bezAt(const Bezier& bz, float at, float length, LengthFunc lineLengthFunc)
{
	auto biggest = 1.0f;
	auto smallest = 0.0f;
	auto t = 0.5f;

	//just in case to prevent an infinite loop
	if (at <= 0) return 0.0f;
	if (at >= length) return 1.0f;

	while (true) {
		auto right = bz;
		Bezier left;
		right.split(t, left);
		length = _bezLength(left, lineLengthFunc);
		if (fabsf(length - at) < BEZIER_EPSILON || fabsf(smallest - biggest) < 1e-3f) {
			break;
		}
		if (length < at) {
			smallest = t;
			t = (t + biggest) * 0.5f;
		}
		else {
			biggest = t;
			t = (smallest + t) * 0.5f;
		}
	}
	return t;
}


void Bezier::split(float t, Bezier& left)
{
	left.start = start;

	left.ctrl1.X = start.X + t * (ctrl1.X - start.X);
	left.ctrl1.Y = start.Y + t * (ctrl1.Y - start.Y);

	left.ctrl2.X = ctrl1.X + t * (ctrl2.X - ctrl1.X); //temporary holding spot
	left.ctrl2.Y = ctrl1.Y + t * (ctrl2.Y - ctrl1.Y); //temporary holding spot

	ctrl2.X = ctrl2.X + t * (end.X - ctrl2.X);
	ctrl2.Y = ctrl2.Y + t * (end.Y - ctrl2.Y);

	ctrl1.X = left.ctrl2.X + t * (ctrl2.X - left.ctrl2.X);
	ctrl1.Y = left.ctrl2.Y + t * (ctrl2.Y - left.ctrl2.Y);

	left.ctrl2.X = left.ctrl1.X + t * (left.ctrl2.X - left.ctrl1.X);
	left.ctrl2.Y = left.ctrl1.Y + t * (left.ctrl2.Y - left.ctrl1.Y);

	left.end.X = start.X = left.ctrl2.X + t * (ctrl1.X - left.ctrl2.X);
	left.end.Y = start.Y = left.ctrl2.Y + t * (ctrl1.Y - left.ctrl2.Y);
}


float Bezier::at(float at, float length) const
{
	return _bezAt(*this, at, length, _lineLength);
}


float Bezier::atApprox(float at, float length) const
{
	return _bezAt(*this, at, length, _lineLengthApprox);
}


VGPoint Bezier::at(float t) const
{
	VGPoint cur;
	auto it = 1.0f - t;

	auto ax = start.X * it + ctrl1.X * t;
	auto bx = ctrl1.X * it + ctrl2.X * t;
	auto cx = ctrl2.X * it + end.X * t;
	ax = ax * it + bx * t;
	bx = bx * it + cx * t;
	cur.X = ax * it + bx * t;

	float ay = start.Y * it + ctrl1.Y * t;
	float by = ctrl1.Y * it + ctrl2.Y * t;
	float cy = ctrl2.Y * it + end.Y * t;
	ay = ay * it + by * t;
	by = by * it + cy * t;
	cur.Y = ay * it + by * t;

	return cur;
}


float Bezier::angle(float t) const
{
	if (t < 0 || t > 1) return 0;

	//derivate
	// p'(t) = 3 * (-(1-2t+t^2) * p0 + (1 - 4 * t + 3 * t^2) * p1 + (2 * t - 3 *
	// t^2) * p2 + t^2 * p3)
	float mt = 1.0f - t;
	float d = t * t;
	float a = -mt * mt;
	float b = 1 - 4 * t + 3 * d;
	float c = 2 * t - 3 * d;

	VGPoint pt = { a * start.X + b * ctrl1.X + c * ctrl2.X + d * end.X, a * start.Y + b * ctrl1.Y + c * ctrl2.Y + d * end.Y };
	pt.X *= 3;
	pt.Y *= 3;

	return _rad2deg(_atan2(pt.Y, pt.X));
}

static bool _bezIsFlatten(const Bezier& bz)
{
	float diff1_x = fabs((bz.ctrl1.X * 3.f) - (bz.start.X * 2.f) - bz.end.X);
	float diff1_y = fabs((bz.ctrl1.Y * 3.f) - (bz.start.Y * 2.f) - bz.end.Y);
	float diff2_x = fabs((bz.ctrl2.X * 3.f) - (bz.end.X * 2.f) - bz.start.X);
	float diff2_y = fabs((bz.ctrl2.Y * 3.f) - (bz.end.Y * 2.f) - bz.start.Y);

	if (diff1_x < diff2_x) diff1_x = diff2_x;
	if (diff1_y < diff2_y) diff1_y = diff2_y;

	if (diff1_x + diff1_y <= 0.5f) return true;

	return false;
}

static int32_t _bezierCurveCount(const Bezier& curve)
{

	if (_bezIsFlatten(curve)) {
		return 1;
	}

	Bezier left{};
	Bezier right{};

	curve.split(left, right);

	return _bezierCurveCount(left) + _bezierCurveCount(right);
}

static Bezier _bezFromArc(const VGPoint& start, const VGPoint& end, float radius) {
	// Calculate the angle between the start and end points
	float angle = _atan2(end.Y - start.Y, end.X - start.X);

	// Calculate the control points of the cubic bezier curve
	float c = radius * 0.552284749831;  // c = radius * (4/3) * tan(pi/8)

	Bezier bz;

	bz.start = VGPoint{ start.X, start.Y };
	bz.ctrl1 = VGPoint{ start.X + radius * cos(angle), start.Y + radius * sin(angle) };
	bz.ctrl2 = VGPoint{ end.X - c * cos(angle), end.Y - c * sin(angle) };
	bz.end = VGPoint{ end.X, end.Y };

	return bz;
}

bool VGTessellate::Path(VGElementRaw element, VGVector<VGPoint>& outPoints)
{
	auto points = element->getPointList();
	auto types = element->getPointTypeList();
	size_t startIndex = -1;
	for (size_t i = 0, k = 0; i < types.size(); ++i)
	{
		switch (types[i])
		{
		case VGPointType::MoveTo:
		{
			if (startIndex != -1) continue;
			outPoints.push_back(points[k]);
			startIndex = k;
			k += 1;
		} break;
		case VGPointType::LineTo:
		{
			if (startIndex == -1) continue;
			outPoints.push_back(points[k]);
			k += 1;
		} break;
		case VGPointType::CubicTo:
		{
			if (startIndex == -1) continue;
			auto start = VGPoint{ outPoints.back().X, outPoints.back().Y };
			auto c1 = points[k + 0];
			auto c2 = points[k + 1];
			auto end = points[k + 2];

			Bezier curve{ start, c1, c2, end };
			auto stepCount = _bezierCurveCount(curve);
			if (stepCount <= 1) stepCount = 2;
			float step = 1.f / stepCount;
			for (uint32_t s = 1; s < static_cast<uint32_t>(stepCount); s++)
			{
				outPoints.push_back(curve.at(step * s));
			}
			outPoints.push_back(end);
			k += 3;
		} break;
		case VGPointType::Close:
		{
			if (startIndex != -1)
			{
				outPoints.push_back(VGNonePoint);
				startIndex = -1;
			}
		} break;
		}
	}
	return true;
}

bool VGTessellate::Fill(VGElementRaw element, VGVector<point_t>& outPoints, VGVector<index_t>& outIndices)
{
	auto points = element->getPointList();
	auto types = element->getPointTypeList();
	auto result = true;
	VGVector<VGPoint> contour;
	Path(element, contour);
	for (size_t i = 0, k = 0; i < contour.size(); ++i)
	{
		if (contour[i].X == VGNonePoint.X && contour[i].Y == VGNonePoint.Y)
		{
			TESStesselator* tess = tessNewTess(nullptr);
			tessAddContour(tess, 2, contour.data() + k, sizeof(VGPoint), (i - k));
			result &= tessTesselate(tess, TESS_WINDING_ODD, TESS_POLYGONS, 3, 2, nullptr);
			auto vertices = tessGetVertices(tess);
			auto indices = tessGetElements(tess);
			auto vertexCount = tessGetVertexCount(tess);
			auto elementCount = tessGetElementCount(tess);
			auto vertexOffset = outPoints.size();
			for (size_t k = 0; k < vertexCount; ++k)
			{
				outPoints.push_back({ vertices[2 * k + 0], vertices[2 * k + 1] });
			}
			for (size_t k = 0; k < elementCount; ++k)
			{
				outIndices.push_back(vertexOffset + indices[3 * k + 0]);
				outIndices.push_back(vertexOffset + indices[3 * k + 1]);
				outIndices.push_back(vertexOffset + indices[3 * k + 2]);
			}
			tessDeleteTess(tess);
			k = i + 1;
		}
	}
	return result;
}

bool VGTessellate::Stroke(VGElementRaw element, VGVector<point_t>& outPoints, VGVector<index_t>& outIndices)
{
	auto points = element->getPointList();
	auto types = element->getPointTypeList();
	auto result = false;
	auto width = element->getLineWidth();
	auto miter = element->getMiterLimit();
	auto cap = element->getLineCap();
	auto join = element->getLineJoin();
	auto offset = element->getDashOffset();
	auto control = element->getDashControl();
	auto oldPoints = VGArrayView<VGPoint>();
	auto oldTypes = VGArrayView<VGPointType>();
	auto newPoints = VGArrayView<VGPoint>();
	auto newTypes = VGArrayView<VGPointType>();
	oldPoints = newPoints = points;
	oldTypes = newTypes = types;

	struct State
	{
		VGPoint firstPt = {};
		VGPoint firstPtDir = {};
		VGPoint prevPt = {};
		VGPoint prevPtDir = {};
		bool hasMove = false;
	} state;

	auto stroke_cap_func = [&]() {

		};

	auto stroke_line_func = [&](VGPoint const& curr) {

		};

	auto stroke_cubic_func = [&](VGPoint const& cnt1, VGPoint const& cnt2, VGPoint const& end) {

		};

	auto stroke_close_func = [&]() {

		};

	auto stroke_join_func = [&](VGPoint const& dir) {

		};

	auto stroke_round_func = [&](VGPoint const& prev, VGPoint const& curr, VGPoint const& center) {

		};

	auto stroke_miter_func = [&](VGPoint const& prev, VGPoint const& curr, VGPoint const& center) {

		};

	auto stroke_bevel_func = [&](VGPoint const& prev, VGPoint const& curr, VGPoint const& center) {

		};

	auto stroke_square_func = [&](VGPoint const& p, VGPoint const& outDir) {

		};

	auto stroke_round2_func = [&](VGPoint const& p, VGPoint const& outDir) {

		};

	auto stroke_func = [&]() {

		for (size_t i = 0, k = 0; i < types.size(); ++i)
		{
			switch (types[i])
			{
			case VGPointType::MoveTo:
			{
				if (state.hasMove)
				{
					stroke_cap_func();
					state.hasMove = false;
				}
				state.hasMove = true;
				state.firstPt = points[k];
				state.firstPtDir = VGPoint();
				state.prevPt = points[k];
				state.prevPtDir = VGPoint{};
				k += 1;
			} break;
			case VGPointType::LineTo:
			{
				stroke_line_func(points[k]);
				k += 1;
			} break;
			case VGPointType::CubicTo:
			{
				stroke_cubic_func(points[k + 0], points[k + 1], points[k + 2]);
				k += 3;
			} break;
			case VGPointType::Close:
			{
				stroke_close_func();
				state.hasMove = false;
			} break;
			}
		}
		};

	// ====================================================

	auto dash_stroke_line_func = [&]() {

		};

	auto dash_stroke_cubic_func = [&]() {

		};

	auto dash_stroke_move_func = [&]() {

		};

	auto dash_stroke_line2_func = [&]() {

		};

	auto dash_stroke_cubic2_func = [&]() {

		};

	auto dash_stroke_func = [&]() {

		VGVector<VGPoint> dashPoints;
		VGVector<VGPointType> dashTypes;

		oldPoints = dashPoints;
		oldTypes = dashTypes;

		};

	if (control.size()) dash_stroke_func();
	else stroke_func();

	return result;
}