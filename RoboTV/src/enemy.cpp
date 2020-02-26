#include "enemy.h"

namespace gamespace
{
	enemy::enemy()
	{

	}

	enemy::enemy(float xPosition, float yPosition, float recWidth, float recHeight,
		const char* textureFile, int columns, int rows, int frameWidth, int frameHeight)
		: animatedSprite(xPosition, yPosition, recWidth, recHeight, textureFile, columns, rows, frameWidth, frameHeight)
	{
		isAttacking = false;
		explosionInstance = new explosion();
	}

	enemy::~enemy()
	{
		delete explosionInstance;
	}

	void enemy::Draw()
	{
		if (!explosionInstance->active)
		{
			if (moveDirection.x <= 0.f)
				animatedSprite::Draw();
			else
				DrawTexturePro(spriteTexture, { sourceRec.x,sourceRec.y, -sourceRec.width, sourceRec.height }, actualRectangle, origin, rotation, WHITE);
		}
		else
			explosionInstance->Draw();
	}

	void enemy::Update(float frameTime)
	{
		animatedSprite::Update(frameTime);

		switch (state)
		{
		case idle:
			break;
		case walking:
			Move(moveDirection.x * frameTime * moveSpeed, moveDirection.y * frameTime * moveSpeed);
			break;
		case windup:
			break;
		case attacking:
			Move(moveDirection.x * frameTime * attackSpeed, moveDirection.y * frameTime * attackSpeed);
			break;
		case damaged:
			Move(moveDirection.x * frameTime * damagedSpeed, moveDirection.y * frameTime * damagedSpeed);
			if (currentHP <= 0.f)
			{
				if (explosionInstance->active)
				{
					explosionInstance->Update(frameTime);
				}
				else
				{
					active = false;
					visible = false;
				}
			}
			break;
		default:
			break;
		}
		stateTimer += frameTime;
	}

	void enemy::UpdateEnemy(Vector2 targetPosition)
	{
		
	}

	void enemy::RecieveDamage(Vector2 damageSource, float damageRecieved) 
	{
		if (state != damaged)
		{
			ChangeState(damaged);
			currentHP -= damageRecieved;
			if (currentHP >= 0.f)
			{
				float resultantX = actualRectangle.x - damageSource.x;
				float resultantY = actualRectangle.y - damageSource.y;

				float magnitude = sqrtf(resultantX * resultantX + resultantY * resultantY);

				moveDirection.x = resultantX / magnitude;
				moveDirection.y = resultantY / magnitude;
			}
			else
			{
				explosionInstance->Activate({ actualRectangle.x, actualRectangle.y });
			}
		}

	}

	void enemy::ChangeState(enemyStates newState)
	{
		if (newState != state)
		{
			state = newState;
			switch (state)
			{
			case idle:
				NewAnimation(idleAnim);
				break;
			case walking:
				NewAnimation(walkAnim);
				break;
			case windup:
				NewAnimation(windupAnim);
				break;
			case attacking:
				isAttacking = true;
				NewAnimation(attackAnim);
				break;
			case damaged:
				NewAnimation(damageAnim);
				break;
			}
			stateTimer = 0.f;
		}
	}
}