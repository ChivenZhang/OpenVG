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
	uint32_t m_NativeVertexBuffer;
	uint32_t m_NativeStyleBuffer;
	uint32_t m_NativeLinearBuffer;
	uint32_t m_NativeRadialBuffer;
	uint32_t m_NativeMatrixBuffer;
	uint32_t m_NativePrimitive;
	size_t m_PrimitiveIndex;
	VGVector<void*> m_TextureList;
	VGVector<VGPrimitive::point_t> m_PointList;
	VGVector<VGPrimitive::style_t> m_StyleList;
	VGVector<VGPrimitive::linear_t> m_LinearList;
	VGVector<VGPrimitive::radial_t> m_RadialList;
	VGVector<VGPrimitive::matrix_t> m_MatrixList;
};