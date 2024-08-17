#include <iostream>
#include <VGContext.h>
#include <VGShape.h>

int main()
{
	auto context = VGNew<VGContext>();
	auto shape = VGNew<VGShape>();

	for (auto i = 0; i < 1; ++i)
	{
		context->addElement(shape);
		context->animateElement();
		context->renderElement();
	}

	return 0;
}