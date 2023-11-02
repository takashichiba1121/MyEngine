#include "GunEnemy.h"
#include"EnemyManager.h"


void GunEnemy::Update(float attackRange)
{
	if ( isDaed_ == false )
	{
		if ( isMove_ )
		{
			Attack();

			Move();
		}
		else
		{
			Vector3 playerPos,enemyPos,playerScale,enemyScale;

			playerPos = playerObj_->GetPosition();

			enemyPos = obj_->GetPosition();

			playerScale = playerObj_->GetScale();

			enemyScale = obj_->GetScale();

			float distance = static_cast< float >( sqrt(pow(playerPos.x - enemyPos.x,2) + pow(playerPos.z - enemyPos.z,2)) );

			float playerYmax,playerYmin,enemyYmax,enemyYmin;

			playerYmax = playerPos.y + playerScale.y;

			playerYmin = playerPos.y - playerScale.y;

			enemyYmax = enemyPos.y + ( enemyScale.y + 3 );

			enemyYmin = enemyPos.y - ( enemyScale.y + 3 );

			isMove_ = ( distance <= attackRange && ( playerYmax >= enemyYmin && playerYmin <= enemyYmax ) );
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

	attackCircle_->Update();

	obj_->Update();

	attackCircle_->SetScale({ attackRange,0, attackRange });
}

void GunEnemy::Move()
{
	Vector3 frontVec = playerObj_->GetPosition() - obj_->GetPosition();

	obj_->SetRot({ 0, atan2f(frontVec.x, frontVec.z),0 });
}

void GunEnemy::Attack()
{
	if ( IntervalTimer_ > 0 )
	{
		IntervalTimer_--;
	}

	if ( IntervalTimer_ <= 0 )
	{
		IntervalTimer_ = kAttackTime_;

		Vector3 bulletPosition = { obj_->GetPosition().x ,obj_->GetPosition().y + 3 ,obj_->GetPosition().z };

		//弾の速度
		const float kBulletSpeed = 0.5f;
		Vector3 velocity = playerObj_->GetPosition() -bulletPosition;
		velocity.normalize();
		velocity *= kBulletSpeed;

		//弾の生成し、初期化
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		newBullet->Initialize(bulletModel_,velocity,bulletPosition);

		newBullet->SetPhase(EnemyBullet::Phase::Charge);

		//弾の登録する
		EnemyManager::Instance()->AddBullet(std::move(newBullet));
	}
}
