#pragma once
#include <OpenVG/VGRender.h>


class OpenVGRender : public VGRender
{
public:
	OpenVGRender();
	~OpenVGRender();

	virtual void render(VGRect client, VGArrayView<const VGPrimitive> data) override;

protected:
	uint32_t m_NativeProgram;
	uint32_t m_NativeBuffer;
	uint32_t m_NativePrimitive;
	VGVector<void*> m_TextureList;
	VGVector<VGPrimitive::fill_t> m_FillList;
	VGVector<VGPrimitive::stroke_t> m_StrokeList;
	VGVector<VGPrimitive::primitive_t> m_PrimitiveList;
};