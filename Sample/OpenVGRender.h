#pragma once
#include <OpenVG/VGRender.h>


class OpenVGRender : public VGRender
{
public:
	static constexpr int MAX_STOP_COUNT = 16;
	struct primitive_t
	{
		float X = 0, Y = 0;
		uint32_t Fill = -1, Stroke = -1;
	};
	struct fill_t
	{
		float Color[4];
		uint32_t Flags;
		uint32_t Image;
		uint32_t Linear;
		uint32_t Radial;
	};
	struct stroke_t
	{
		float Color[4];
		uint32_t Flags;
		uint32_t Image;
		uint32_t Linear;
		uint32_t Radial;
	};
	struct linear_t
	{
		VGFloat4  NumStops;
		VGFloat2  GradStartPos;
		VGFloat2  GradEndPos;
		VGFloat4  StopPoints[MAX_STOP_COUNT / 4];
		VGFloat4  StopColors[MAX_STOP_COUNT];
	};
	struct radial_t
	{
		VGFloat4  NumStops;
		VGFloat2  CenterPos;
		VGFloat2  Radius;
		VGFloat4  StopPoints[MAX_STOP_COUNT / 4];
		VGFloat4  StopColors[MAX_STOP_COUNT];
	};

public:
	OpenVGRender();
	~OpenVGRender();

	virtual void render(VGRect client, VGArrayView<VGPrimitive> data) override;

	uint32_t getTexture() const;
	void setTexture(uint32_t value);

protected:
	uint32_t m_NativeProgram;
	uint32_t m_NativeBuffer;
	uint32_t m_NativePrimitive;
	VGVector<fill_t> m_FillList;
	VGVector<stroke_t> m_StrokeList;
	VGVector<uint32_t> m_TextureList;
	VGVector<primitive_t> m_PrimitiveList;
};