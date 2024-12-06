#pragma once
#include "VGElement.h"

/// @brief 
class OPENVG_API VGPicture : public VGElement
{
public:
	VGPicture();
	~VGPicture();

	VGColor getFillColor() const;
	void setFillColor(VGColor value);

	VGImage getImage() const;
	void setImage(float x, float y, float width, float height, VGImage value);

protected:
	virtual void clip() override;
	virtual void fill() override;
	virtual void stroke() override;

private:
	VGElementPrivateRaw m_Private;
};
using VGPictureRef = VGRef<VGPicture>;
using VGPictureRaw = VGRaw<VGPicture>;