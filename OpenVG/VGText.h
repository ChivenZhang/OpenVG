#pragma once
#include "VGElement.h"

/// @brief 
class OPENVG_API VGText : public VGElement
{
public:
	VGText();
	~VGText();

	VGString getFamily() const;
	void setFamily(VGString value);

	uint32_t getSize() const;
	void setSize(uint32_t value);

	uint32_t getSpacing() const;
	void setSpacing(uint32_t value);

	uint32_t getLineSpacing() const;
	void getLineSpacing(uint32_t value);

	bool getLineWrap() const;
	void setLineWrap(bool value);

	VGTextStyle getStyle() const;
	void setStyle(VGTextStyle value);

	VGTextWeight getWeight() const;
	void setWeight(VGTextWeight value);

	VGTextAlgins getAlignment() const;
	void setAlignment(VGTextAlgins value);

	VGTextDirection getDirection() const;
	void setDirection(VGTextDirection value);

	VGTextEllipsize getEllipsize() const;
	void setEllipsize(VGTextEllipsize value);

	void setText(float x, float y, float width, float height, VGString text);

private:
	VGElementPrivateRaw m_Private;
};
using VGTextRef = VGRef<VGText>;
using VGTextRaw = VGRaw<VGText>;