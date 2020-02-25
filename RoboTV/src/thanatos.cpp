#include "thanatos.h"

namespace gamespace
{

	thanatos::thanatos()
	{
	}

	thanatos::thanatos(float xPosition, float yPosition, const cursor* gameCursor)
		:animatedSprite(xPosition, yPosition, 24.f, 24.f, "../res/assets/thanatos_spritesheet.png", 2, 3, 8, 8)
	{
		cursorInstance = gameCursor;

		AABB.x = actualRectangle.x + AABBxOffset;
		AABB.y = actualRectangle.y + AABByOffset;
		AABB.width = actualRectangle.width + AABBWidthOffset;
		AABB.height = actualRectangle.height + AABBHeightOffset;

		moveDirection.x = 0.f;
		moveDirection.y = 0.f;

		idleAnim.animationTime = 1.f;
		idleAnim.addFrame({ 0,0 });
		idleAnim.addFrame({ 1,0 });

		walkAnim.animationTime = 0.6f;
		walkAnim.addFrame({ 0,1 });
		walkAnim.addFrame({ 1,1 });

		attackAnim.animationTime = 0.3f;
		attackAnim.addFrame({ 0,2 });
		attackAnim.addFrame({ 0,0 });
		attackAnim.addFrame({ 0,2 });
		attackAnim.addFrame({ 0,0 });

		dashAnim.animationTime = 0.2f;
		dashAnim.addFrame({ 0,2 });
		dashAnim.addFrame({ 0,0 });

		damageAnim.animationTime = 0.5f;
		damageAnim.addFrame({ 1,2 });

		NewAnimation(idleAnim);

		state = idle;
		stateTimer = 0.f;
	}



	thanatos::~thanatos()
	{

	}

	void thanatos::Draw()
	{
		//DrawRectangleRec(AABB, RED);  //see hitbox compared to the character
		if(cursorInstance->actualRectangle.x > AABB.x)
			animatedSprite::Draw();
		else
			DrawTexturePro(spriteTexture, {sourceRec.x,sourceRec.y, -sourceRec.width, sourceRec.height}, actualRectangle, origin, rotation, WHITE);
	}

	void thanatos::Update(float frameTime)
	{

		moveDirection.x = 0.f;
		moveDirection.y = 0.f;

		if (IsKeyDown(KEY_D))
			moveDirection.x += 1;
		if (IsKeyDown(KEY_A))
			moveDirection.x -= 1;

		if (IsKeyDown(KEY_W))
			moveDirection.y -= 1;

		if (IsKeyDown(KEY_S))
			moveDirection.y += 1;

		switch (state)
		{
		case idle:
			if (IsKeyDown(KEY_SPACE))
				ChangeState(dashing);
			else
				if (IsMouseButtonPressed(0))
					ChangeState(attacking);
				else
					if (moveDirection.x != 0.f || moveDirection.y != 0.f)
						ChangeState(walking);

			break;
		case walking:

			if (moveDirection.x != 0.f)
			{
				moveDirection.x = moveDirection.x / sqrtf(moveDirection.x * moveDirection.x + moveDirection.y * moveDirection.y);
				AABB.x += moveDirection.x * moveSpeed * frameTime;

			}
			if (moveDirection.y != 0.f)
			{
				moveDirection.y = moveDirection.y / sqrtf(moveDirection.x * moveDirection.x + moveDirection.y * moveDirection.y);
				AABB.y += moveDirection.y * moveSpeed * frameTime;
			}

			if (IsKeyDown(KEY_SPACE))
				ChangeState(dashing);
			else
				if (IsMouseButtonPressed(0))
					ChangeState(attacking);
				else
					if (moveDirection.x == 0 && moveDirection.y == 0)
						ChangeState(idle);
			break;

			break;
		case dashing:
			if (stateTimer >= dashTime)
				ChangeState(idle);
			break;
		case attacking:
			if (stateTimer >= attackTime)
				ChangeState(idle);
			else
				if (IsKeyPressed(KEY_SPACE))
					ChangeState(dashing);
			break;
		case damaged:

			break;
		}

		animatedSprite::Update(frameTime);
		stateTimer += frameTime;

		actualRectangle.x = AABB.x - AABBxOffset;
		actualRectangle.y = AABB.y - AABByOffset;

	}

	void thanatos::UpdateSafePosition()
	{
		safePosition = { AABB.x, AABB.y };
	}

	bool thanatos::CoolideWithWall(const rectangle* wall)
	{
		if (CheckCollisionRecs(AABB, wall->actualRectangle))
		{
			if (safePosition.x + AABB.width <= wall->actualRectangle.x)
			{
				actualRectangle.x = wall->actualRectangle.x - AABB.width / 2;
			}
			if (safePosition.x >= wall->actualRectangle.x + wall->actualRectangle.width)
			{
				actualRectangle.x = wall->actualRectangle.x + wall->actualRectangle.width + AABB.width / 2;
			}

			if (safePosition.y + AABB.height <= wall->actualRectangle.y)
			{
				actualRectangle.y = wall->actualRectangle.y - AABB.height + wallCollisionOffset;
			}
			if (safePosition.y >= wall->actualRectangle.y + wall->actualRectangle.height)
			{
				actualRectangle.y = wall->actualRectangle.y + wall->actualRectangle.height + wallCollisionOffset;
			}

			AABB.x = actualRectangle.x + AABBxOffset;
			AABB.y = actualRectangle.y + AABByOffset;
			return true;
		}
		else
		{
			return false;
		}



	}

	void thanatos::ChangeState(thanatosStates newState)
	{
		if (state != newState)
		{
			state = newState;
			stateTimer = 0.f;
			switch (state)
			{
			case idle:
				NewAnimation(idleAnim);
				break;
			case walking:
				NewAnimation(walkAnim);
				break;
			case attacking:
				NewAnimation(attackAnim);
				break;
			case dashing:
				NewAnimation(dashAnim);
				break;
			case damaged:
				NewAnimation(damageAnim);
				break;
			}
		}
	}
}