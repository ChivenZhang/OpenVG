#pragma once
#include "../VG.h"

class VGPath
{
public:
	enum class CommandType { MoveTo, LineTo, QuadraticCurveTo, CubicCurveTo, Arc, ClosePath };

	struct Command
	{
		CommandType type;
		union
		{
			struct
			{
				VGPoint p1;
				VGPoint p2; // Optional second point for curves
				VGPoint p3; // Optional third point for cubic curves
			};
			struct
			{
				VGPoint center;
				float radius;
				float startAngle;
				float endAngle;
				bool clockwise;
			};
		};

		Command(CommandType t, VGPoint p1_val, VGPoint p2_val = VGPoint(0, 0), VGPoint p3_val = VGPoint(0, 0))
			: type(t), p1(p1_val), p2(p2_val), p3(p3_val) {}

		Command(CommandType t, VGPoint c, float r, float start, float end, bool cw)
			: type(t), center(c), radius(r), startAngle(start), endAngle(end), clockwise(cw) {}
	};

	void moveTo(float x, float y) {
		currentPoint = VGPoint(x, y);
		path.push_back(Command(CommandType::MoveTo, VGPoint(x, y)));
	}

	void lineTo(float x, float y) {
		path.push_back(Command(CommandType::LineTo, VGPoint(x, y)));
		currentPoint = VGPoint(x, y);
	}

	void quadraticCurveTo(float cx, float cy, float x, float y) {
		path.push_back(Command(CommandType::QuadraticCurveTo, VGPoint(cx, cy), VGPoint(x, y)));
		currentPoint = VGPoint(x, y);
	}

	void cubicBezierCurveTo(float cx1, float cy1, float cx2, float cy2, float x, float y) {
		path.push_back(Command(CommandType::CubicCurveTo, VGPoint(cx1, cy1), VGPoint(cx2, cy2), VGPoint(x, y)));
		currentPoint = VGPoint(x, y);
	}

	void arc(float cx, float cy, float radius, float startAngle, float endAngle, bool clockwise) {
		path.push_back(Command(CommandType::Arc, VGPoint(cx, cy), radius, startAngle, endAngle, clockwise));

		// Update currentPoint to the end of the arc
		currentPoint.X = cx + radius * std::cos(endAngle);
		currentPoint.Y = cy + radius * std::sin(endAngle);
	}

	void closePath() {
		path.push_back(Command(CommandType::ClosePath, startVGPoint));
	}

	const VGVector<Command>& getPath() const {
		return path;
	}

	void clearPath() {
		path.clear();
	}

	VGVector<VGPoint> tessellatePath(float tolerance = 1.0) {
		VGVector<VGPoint> tessellatedPath;
		for (const auto& cmd : path) {
			switch (cmd.type) {
			case CommandType::MoveTo:
				tessellatedPath.push_back(cmd.p1);
				break;
			case CommandType::LineTo:
				tessellatedPath.push_back(cmd.p1);
				break;
			case CommandType::QuadraticCurveTo:
				tessellateQuadraticCurve(tessellatedPath, currentPoint, cmd.p1, cmd.p2, tolerance);
				break;
			case CommandType::CubicCurveTo:
				tessellateCubicCurve(tessellatedPath, currentPoint, cmd.p1, cmd.p2, cmd.p3, tolerance);
				break;
			case CommandType::Arc:
				tessellateArc(tessellatedPath, cmd.center, cmd.radius, cmd.startAngle, cmd.endAngle, cmd.clockwise, tolerance);
				break;
			case CommandType::ClosePath:
				tessellatedPath.push_back(cmd.p1);
				break;
			}
			currentPoint = cmd.p1; // Update the current point after processing each command
			currentPoint = cmd.p1; // Update the current point after processing each command
		}
		return tessellatedPath;
	}

private:
	VGVector<Command> path;
	VGPoint startVGPoint = VGPoint(0, 0);
	VGPoint currentPoint = VGPoint(0, 0);

	void tessellateQuadraticCurve(VGVector<VGPoint>& tessellatedPath, const VGPoint& p0, const VGPoint& p1, const VGPoint& p2, float tolerance) const {
		tessellateQuadraticRecursive(tessellatedPath, p0, p1, p2, tolerance);
	}

	void tessellateQuadraticRecursive(VGVector<VGPoint>& tessellatedPath, const VGPoint& p0, const VGPoint& p1, const VGPoint& p2, float tolerance) const {
		VGPoint mid1 = VGPoint((p0.X + p1.X) / 2.0, (p0.Y + p1.Y) / 2.0);
		VGPoint mid2 = VGPoint((p1.X + p2.X) / 2.0, (p1.Y + p2.Y) / 2.0);
		VGPoint mid = VGPoint((mid1.X + mid2.X) / 2.0, (mid1.Y + mid2.Y) / 2.0);

		float dx = p2.X - p0.X;
		float dy = p2.Y - p0.Y;
		float d = std::abs((p1.X - p2.X) * dy - (p1.Y - p2.Y) * dx);

		if (d * d <= tolerance * (dx * dx + dy * dy)) {
			tessellatedPath.push_back(p2);
		}
		else {
			tessellateQuadraticRecursive(tessellatedPath, p0, mid1, mid, tolerance);
			tessellateQuadraticRecursive(tessellatedPath, mid, mid2, p2, tolerance);
		}
	}

	void tessellateCubicCurve(VGVector<VGPoint>& tessellatedPath, const VGPoint& p0, const VGPoint& p1, const VGPoint& p2, const VGPoint& p3, float tolerance) const {
		tessellateCubicRecursive(tessellatedPath, p0, p1, p2, p3, tolerance);
	}

	void tessellateCubicRecursive(VGVector<VGPoint>& tessellatedPath, const VGPoint& p0, const VGPoint& p1, const VGPoint& p2, const VGPoint& p3, float tolerance) const {
		VGPoint mid1 = VGPoint((p0.X + p1.X) / 2.0, (p0.Y + p1.Y) / 2.0);
		VGPoint mid2 = VGPoint((p1.X + p2.X) / 2.0, (p1.Y + p2.Y) / 2.0);
		VGPoint mid3 = VGPoint((p2.X + p3.X) / 2.0, (p2.Y + p3.Y) / 2.0);

		VGPoint mid12 = VGPoint((mid1.X + mid2.X) / 2.0, (mid1.Y + mid2.Y) / 2.0);
		VGPoint mid23 = VGPoint((mid2.X + mid3.X) / 2.0, (mid2.Y + mid3.Y) / 2.0);

		VGPoint mid = VGPoint((mid12.X + mid23.X) / 2.0, (mid12.Y + mid23.Y) / 2.0);

		float dx = p3.X - p0.X;
		float dy = p3.Y - p0.Y;
		float d = std::abs((p2.X - p3.X) * dy - (p2.Y - p3.Y) * dx);

		if (d * d <= tolerance * (dx * dx + dy * dy)) {
			tessellatedPath.push_back(p3);
		}
		else {
			tessellateCubicRecursive(tessellatedPath, p0, mid1, mid12, mid, tolerance);
			tessellateCubicRecursive(tessellatedPath, mid, mid23, mid3, p3, tolerance);
		}
	}

	void tessellateArc(std::vector<VGPoint>& tessellatedPath, const VGPoint& center, float radius, float startAngle, float endAngle, bool clockwise, float tolerance) const {
		float angleDiff = endAngle - startAngle;
		if (clockwise && angleDiff > 0) {
			angleDiff -= 2 * M_PI;
		}
		else if (!clockwise && angleDiff < 0) {
			angleDiff += 2 * M_PI;
		}

		int segments = std::max(2, static_cast<int>(std::abs(angleDiff) / std::sqrt(tolerance / radius)));
		float angleStep = angleDiff / segments;

		for (int i = 0; i <= segments; ++i) {
			float angle = startAngle + i * angleStep;
			float x = center.X + radius * std::cos(angle);
			float y = center.Y + radius * std::sin(angle);
			tessellatedPath.push_back(VGPoint(x, y));
		}
	}
};