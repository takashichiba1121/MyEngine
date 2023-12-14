#include "RunEnemy.h"
#include"EnemyManager.h"



void RunEnemy::Update()
{
	if ( isDaed_ == false )
	{
		Vector3 playerPos,enemyPos,playerScale,enemyScale;

		playerPos = player_->GetObj()->GetPosition();

		enemyPos = obj_->GetPosition();

		playerScale = player_->GetObj()->GetScale();

		enemyScale = obj_->GetScale();

		float distance = static_cast< float >( sqrt(pow(playerPos.x - enemyPos.x,2) + pow(playerPos.z - enemyPos.z,2)) );

		isMove_ = ( distance <= attackRange_ && ( playerPos.y == enemyPos.y ) );

		if ( distance <= 10 && ( playerPos.y == enemyPos.y ) && isAttack_ == false )
		{
			isAttack_ = true;

			attackTimer_ = 0;

			Vector3 frontVec = player_->GetObj()->GetPosition() - obj_->GetPosition();

			attackEndRot = atan2f(frontVec.x,frontVec.z);

			attackStartRot = obj_->GetRot().y;

			attackVec = frontVec;

			attackVec.normalize();
		}

		if ( isMove_ == false )
		{
			attackTimer_ = 0;
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

void RunEnemy::Move()
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

void RunEnemy::Attack()
{
	if ( isAttack_ )
	{
		attackTimer_++;
		if ( attackTimer_ <= kAttackTimer_ - 60 )
		{
			float timeRate = ( float(attackTimer_) / float(kAttackTimer_ - 60) );

			float a = attackStartRot * ( 1.0f - timeRate ) + attackEndRot * timeRate;

			obj_->SetRot({ 0,a,0 });
		}
		else if ( attackTimer_ <= kAttackTimer_ - 40 )
		{
			obj_->SetPosition(obj_->GetPosition() - attackVec * 0.05f);
		}
		else if ( attackTimer_ >= kAttackTimer_ - 30 )
		{
			Vector3 vec = Matrix4Math::transform({ 1,0,0 },obj_->GetMatWorld());

			vec.y = 0;

			vec.normalize();

			const uint32_t constlife = 10;
			uint32_t life = constlife;

			//XYZの広がる距離
			Vector3 verocity{};
			verocity.x = 0;
			verocity.y = -0;
			verocity.z = 0;

			Vector3 pos = obj_->GetPosition();

			pos.y -= obj_->GetScale().y;

			pos += vec;

			//追加
			EnemyManager::Instance()->GetParticle()->Add(life,pos,verocity,{ 0,0,0 },0.5f,0.5f,{ 1,0.8f,0,1 },{ 1,0.8f,0,1 });

			pos -= vec * 2;
							//追加
			EnemyManager::Instance()->GetParticle()->Add(life,pos,verocity,{ 0,0,0 },0.5f,0.5f,{ 1,0.8f,0,1 },{ 1,0.8f,0,1 });
			obj_->SetPosition(obj_->GetPosition() + attackVec * attackSpeed_);
			if ( attackTimer_ >= kAttackTimer_ )
			{
				isAttack_ = false;

				attackTimer_ = 0;
			}
		}
	}
}

void RunEnemy::AttackOff()
{
	isAttack_ = false;
}
