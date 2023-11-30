#include "RunEnemy.h"
#include"EnemyManager.h"


void RunEnemy::Update()
{
	if ( isDaed_ == false )
	{
		Move();

		Vector3 playerPos,enemyPos,playerScale,enemyScale;

		playerPos = playerObj_->GetPosition();

		enemyPos = obj_->GetPosition();

		playerScale = playerObj_->GetScale();

		enemyScale = obj_->GetScale();

		float distance = static_cast< float >( sqrt(pow(playerPos.x - enemyPos.x,2) + pow(playerPos.z - enemyPos.z,2)) );

		float playerYmax,playerYmin,enemyYmax,enemyYmin;

		playerYmax = playerPos.y + playerScale.y;

		playerYmin = playerPos.y - playerScale.y;

		enemyYmax = enemyPos.y + enemyScale.y;

		enemyYmin = enemyPos.y - enemyScale.y;

		isMove_ = ( distance <= attackRange_ && ( playerPos.y == enemyPos.y ) );

		if ( isMove_ == false )
		{
			isAttack_ = false;

			attackTimer_ = kAttackTimer_;
		}
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

void RunEnemy::Move()
{
	if ( isMove_ )
	{
		if ( isAttack_ )
		{
			attackTimer_--;



			obj_->SetPosition(obj_->GetPosition() + attackVec * attackSpeed_);
			if ( attackTimer_ <= 0 )
			{
				isAttack_ = false;

				attackTimer_ = kAttackTimer_;
			}
		}
		else
		{
			IntervalTimer_--;

			if ( IntervalTimer_ <= kIntervalTime_ )
			{
				Vector3 frontVec = playerObj_->GetPosition() - obj_->GetPosition();

				obj_->SetRot({ 0, atan2f(frontVec.x, frontVec.z),0 });
			}

			if ( IntervalTimer_ <= 0 )
			{
				isAttack_ = true;

				IntervalTimer_ = kIntervalTime_;

				attackVec = playerObj_->GetPosition() - obj_->GetPosition();

				attackVec.y = 0;

				attackVec.normalize();
			}
		}
	}
}