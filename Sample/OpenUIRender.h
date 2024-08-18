#pragma once
#include <OpenUI/UIRender.h>

class OpenUIRender : public UIRender
{
public:
	using primitive_t = struct primitive_t
	{
		float X = 0, Y = 0;
		uint32_t Index = 0;
	};

public:
	OpenUIRender();
	~OpenUIRender();
	virtual void render(UIRect client, UIArrayView<UIPrimitive> data) override;

protected:
	uint32_t m_NativeProgram;
	uint32_t m_NativeBuffer;
	uint32_t m_NativePrimitive;
	UIVector<primitive_t> m_PrimitiveList;
};