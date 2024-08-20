#pragma once
#include <OpenVG/VGRender.h>


class OpenVGRender : public VGRender
{
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