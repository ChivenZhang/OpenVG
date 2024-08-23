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
	uint32_t m_NativeFillBuffer;
	uint32_t m_NativeStrokeBuffer;
	uint32_t m_NativeLinearBuffer;
	uint32_t m_NativeRadialBuffer;
	uint32_t m_NativeMatrixBuffer;
	uint32_t m_NativePrimitive;
	size_t m_PrimitiveIndex;
	VGVector<void*> m_TextureList;
	VGVector<VGPrimitive::fill_t> m_FillList;
	VGVector<VGPrimitive::stroke_t> m_StrokeList;
	VGVector<VGPrimitive::primitive_t> m_PrimitiveList;
	VGVector<VGPrimitive::linear_t> m_LinearList;
	VGVector<VGPrimitive::radial_t> m_RadialList;
	VGVector<VGPrimitive::matrix_t> m_MatrixList;
};