#include "drone.h"

namespace gamespace
{

	drone::drone()
	{
	}

	drone::drone(thanatos* thanatosInstance) : animatedSprite(0.f,0.f,12.f,12.f,"../res/assets/drone_spritesheet.png",2,4,4,4)
	{
		parentThanatos = thanatosInstance;
		actualRectangle.y = parentThanatos->actualRectangle.y;
		actualRectangle.x = parentThanatos->actualRectangle.x + droneOffset;

		idleAnim.animationTime = 1.9f;
		idleAnim.addFrame({ 0,0 });
		idleAnim.addFrame({ 1,0 });

		NewAnimation(idleAnim);
	}

	drone::~drone()
	{

	}

	void drone::Update(float frameTime) 
	{
		animatedSprite::Update(frameTime);
		actualRectangle.y = parentThanatos->actualRectangle.y;
		actualRectangle.x = parentThanatos->actualRectangle.x + droneOffset;
	}

	void drone::UpdateDrone(cursor* cursorInstance) 
	{
		float resultantX = cursorInstance->actualRectangle.x - parentThanatos->actualRectangle.x;
		float resultantY = cursorInstance->actualRectangle.y - parentThanatos->actualRectangle.y;

		float magnitude = sqrtf(resultantX * resultantX + resultantY * resultantY);

		actualRectangle.x = parentThanatos->actualRectangle.x + (resultantX / magnitude * droneOffset);
		actualRectangle.y = parentThanatos->actualRectangle.y + (resultantY / magnitude * droneOffset);
	}

}