#include "jumpEnemy.h"

void jumpEnemy::Update()
{
	if ( isDaed_ == false )
	{
		Vector3 playerPos,enemyPos,playerScale,enemyScale;

		playerPos = player_->GetObj()->GetPosition();

		enemyPos = obj_->GetPosition();

		playerScale = player_->GetObj()->GetScale();

		enemyScale = obj_->GetScale();

		float distance = static_cast< float >( sqrt(pow(playerPos.x - enemyPos.x,2) + pow(playerPos.z - enemyPos.z,2)) );

		isMove_ = ( distance <= attackRange_);

		if ( distance <= 20&& isAttack_ == false )
		{
			isAttack_ = true;

			attackTimer_ =0;

			attackEndPoint = playerPos;

			attackMiddlePoint = ( playerPos + enemyPos ) / 2;

			attackMiddlePoint.y += 30;

			attackStartPoint = enemyPos;

			Vector3 frontVec = player_->GetObj()->GetPosition() - obj_->GetPosition();

			obj_->SetRot({ 0, atan2f(frontVec.x, frontVec.z),0 });
		}

		Move();

		Attack();
	}
	else
	{
		ExplosionFrame++;

		float a = ExplosionFrame / ExplosionMaxFrame;

		obj_->SetDestruction(a);

		obj_->Setalpha(static_cast< float >( ( ExplosionMaxFrame - ExplosionFrame ) / ExplosionMaxFrame ));
		if ( ExplosionFrame >= ExplosionMaxFrame )
		{
			isDelete_ = true;
		}
	}

	obj_->Update();
}

void jumpEnemy::Move()
{
	if ( isMove_ && isAttack_ == false )
	{
		Vector3 frontVec = player_->GetObj()->GetPosition() - obj_->GetPosition();

		obj_->SetRot({ 0, atan2f(frontVec.x, frontVec.z),0 });

		attackVec = frontVec;

		attackVec.y = 0;

		attackVec.normalize();

		obj_->SetPosition(obj_->GetPosition() + attackVec * moveSpeed_);
	}
}

void jumpEnemy::Attack()
{
	if ( isAttack_ )
	{
		attackTimer_++;
		if ( attackTimer_ <= 20 )
		{

		}
		else
		{
			float timeRate =( float(attackTimer_-20) / float( kAttackTimer_ - 20 ));

			Vector3 a = attackStartPoint * ( 1.0f - timeRate ) +attackMiddlePoint* timeRate;
			Vector3 b = attackMiddlePoint * ( 1.0f - timeRate ) + attackEndPoint * timeRate;

			Vector3 c = a * ( 1.0f - timeRate ) + b * timeRate;

			obj_->SetPosition(c);
			if (attackTimer_>=kAttackTimer_ )
			{
				isAttack_ = false;
			}
		}
	}
}

void jumpEnemy::AttackOff()
{
	isAttack_ = false;
}
