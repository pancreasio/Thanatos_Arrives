#include "rectangle.h"

namespace gamespace
{
	rectangle::rectangle()
	{
		width = 10.f;
		height = 5.f;
		color = RED;
	}

	rectangle::rectangle(float xPosition, float yPosition, float recWidth, float recHeight, Color recColor) :gameObject(xPosition, yPosition)
	{
		width = recWidth;
		height = recHeight;
		color = recColor;
	}


	rectangle::~rectangle()
	{
	}

	void rectangle::Draw() 
	{
		DrawRectangle(static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height), color);
	}
}