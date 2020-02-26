#include "sfaira.h"

namespace gamespace
{
	sfaira::sfaira()
	{

	}

	sfaira::sfaira(float xPosition, float yPosition) 
		:enemy(xPosition, yPosition, 24.f, 24.f, "../res/assets/sfaira.png", 2, 4, 8, 8)
	{
		targetWalkDistance = sfairaTargetWalkDistance;
		targetAttackDistance = sfairaTargetAttackDistance;
		windupTime = sfairaWindupTime;
		attackTime = sfairaAttackTime;
		recoveryTime = sfairaRecoveryTime;
		attackCooldown = sfairaAttackCooldown;
		damagedTime = sfairaDamagedTime;
		moveSpeed = sfairaMoveSpeed;
		attackSpeed = sfairaAttackSpeed;
		damagedSpeed = sfairaDamagedSpeed;
		attackDamage = sfairaAttackDamage;

		idleAnim.animationTime = 1.1f;
		idleAnim.addFrame({ 0,0 });
		idleAnim.addFrame({ 1,0 });

		walkAnim.animationTime = 0.5f;
		walkAnim.addFrame({ 0,0 });
		walkAnim.addFrame({ 1,0 });
		walkAnim.addFrame({ 0,1 });
		walkAnim.addFrame({ 1,1 });

		windupAnim.animationTime = 0.1f;
		windupAnim.addFrame({ 0,2 });
		windupAnim.addFrame({ 1,2 });

		attackAnim.animationTime = 0.2f;
		attackAnim.addFrame({ 0,3 });
		windupAnim.addFrame({ 1,2 });
		windupAnim.addFrame({ 0,2 });
		attackAnim.addFrame({ 0,3 });


		damageAnim.animationTime = 0.2f;
		damageAnim.addFrame({ 0,3 });

		NewAnimation(idleAnim);

		ChangeState(idle);
		currentHP = sfairaMaxHP;

		for (int i = 0; i < maxVelos; i++)
		{
			velosList.push_back(new velos(false));
		}
	}

	sfaira::~sfaira()
	{
		for (int i = 0; i < maxVelos; i++)
		{
			delete velosList[i];
		}
	}

	void sfaira::Draw() 
	{
		enemy::Draw();

		for (int i = 0; i < maxVelos; i++)
		{
			if (velosList[i]->visible)
			{
				velosList[i]->Draw();
			}
		}
	}

	void sfaira::Update(float frameTime)
	{
		enemy::Update(frameTime);

		for (int i = 0; i < maxVelos; i++)
		{
			if (velosList[i]->active)
			{
				velosList[i]->Update(frameTime);
			}
		}
	}

	void sfaira::UpdateEnemy(Vector2 targetPosition) 
	{
		lastKnownTargetPosition.x = targetPosition.x - actualRectangle.x;
		lastKnownTargetPosition.y = targetPosition.y - actualRectangle.y;

		distanceToTarget = sqrtf(lastKnownTargetPosition.x * lastKnownTargetPosition.x + lastKnownTargetPosition.y * lastKnownTargetPosition.y);

		if (state != damaged)
		{
			/*moveDirection.x = lastKnownTargetPosition.x / distanceToTarget;
			moveDirection.y = lastKnownTargetPosition.y / distanceToTarget;*/
		}

		switch (state)
		{
		case idle:
			if (stateTimer >= recoveryTime && distanceToTarget <= targetWalkDistance)
				ChangeState(walking);
			break;
		case walking:
			if (stateTimer >= attackCooldown && distanceToTarget <= targetAttackDistance)
				ChangeState(windup);
			else
				if (distanceToTarget > targetWalkDistance)
					ChangeState(idle);
			break;
		case windup:
			if (stateTimer >= windupTime)
				ChangeState(attacking);
			break;
		case attacking:
			if (stateTimer >= attackTime)
			{
				for (int i = 0; i < maxVelos; i++)
				{
					if (!velosList[i]->active)
					{
						Fire(velosList[i], targetPosition);
						i = maxVelos;
					}
				}
				ChangeState(idle);
				isAttacking = false;
			}
			break;
		case damaged:
			if (stateTimer >= damagedTime && currentHP > 0.f)
				ChangeState(idle);
			break;
		default:
			break;
		}
	}

	void sfaira::Fire(velos* attackToFire, Vector2 targetPosition) 
	{
		float resultantX = targetPosition.x - actualRectangle.x;
		float resultantY = targetPosition.y - actualRectangle.y;

		float magnitude = sqrtf(resultantX * resultantX + resultantY * resultantY);

		float normalizedX = resultantX / magnitude;
		float normalizedY = resultantY / magnitude;

		float attackAngle = acosf(normalizedX) * 180.f / PI + 90.f;
		if (normalizedY < 0)
			attackAngle = -attackAngle;

		attackToFire->Activate({ actualRectangle.x, actualRectangle.y}, attackAngle, normalizedY < 0);
		attackToFire->UpdateTarget(targetPosition);
	}

}