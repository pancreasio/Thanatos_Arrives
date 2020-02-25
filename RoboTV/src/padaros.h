#ifndef PADAROS_H
#define PADAROS_H

#include "animatedSprite.h"
namespace gamespace
{

	class padaros : public animatedSprite
	{

	public:
		padaros();
		padaros(float xPosition, float yPosition);
		~padaros();
		void Update(float frameTime);
		void UpdatePadaros(Vector2 targetPosition);

	private:

		const float targetWalkDistance = 180.f;
		const float targetAttackDistance = 70.f;
		const float windupTime = 0.7f;
		const float attackTime = 0.2f;
		const float recoveryTime = 1.0f;
		const float damagedTime = 0.3f;
		const float moveSpeed = 140.f;
		const float attackSpeed = 200.f;

		enum padarosStates
		{
			idle, walking, windup, attacking, damaged
		};

		void ChangeState(padarosStates newState);

		float stateTimer;
		float distanceToTarget;
		Vector2 lastKnownTargetPosition;
		padarosStates state;
		animationData idleAnim;
		animationData walkAnim;
		animationData windupAnim;
		animationData attackAnim;
		animationData damageAnim;

	};
}
#endif