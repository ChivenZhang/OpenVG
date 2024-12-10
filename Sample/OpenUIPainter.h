#pragma once
/*=================================================
* Copyright © 2020-2025 ChivenZhang.
* All Rights Reserved.
* =====================Note=========================
*
*
* ====================History=======================
* Created by ChivenZhang@gmail.com.
*
* =================================================*/
#include "OpenVGPainter.h"

class OpenUIPainter : public OpenVGPainter
{
public:
	OpenUIPainter(uint32_t width, uint32_t height, uint32_t density = 96);
	~OpenUIPainter();
	virtual void resize(uint32_t width, uint32_t height) override;
	uint32_t getTexture() const;

private:
	uint32_t m_NativeTexture;
	uint32_t m_NativeFrameBuffer;
};