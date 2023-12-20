#include "jumpEnemy.h"

void jumpEnemy::Initialize(Model* enemyModel,Model* bulletModel,const Vector3& position,Player* player,EnemyType enemyType,uint32_t number)
{
	model_ = enemyModel;

	bulletModel_ = bulletModel;

	player_ = player;

	enemyType_ = enemyType;

	obj_ = std::make_unique<Object3d>();

	obj_->Initialize();

	obj_->SetModel(model_);

	obj_->SetPosition(position);

	obj_->SetRot({ 0,135,0 });

	obj_->SetPolygonExplosion({ 0.0f,1.0f,6.28f,20.0f });

	number_ = number;

	attackSE_.Load("Resources/Sound/Jamp.wav");
}

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

		isMove_ = ( distance <= attackRange_&&(playerPos.y == enemyPos.y));

		if ( distance <= 20&& isAttack_ == false&&player_->GetOnGround()==false)
		{
			isAttack_ = true;

			attackTimer_ =0;

			attackEndPoint_ = playerPos;

			attackMiddlePoint_ = ( playerPos + enemyPos ) / 2;

			attackMiddlePoint_.y += 30;

			attackStartPoint_ = enemyPos;

			Vector3 frontVec = player_->GetObj()->GetPosition() - obj_->GetPosition();

			obj_->SetRot({ 0, atan2f(frontVec.x, frontVec.z),0 });

			attackSE_.Play(false,0.3f);
		}

		Move();

		Attack();
	}
	else
	{
		ExplosionFrame_++;

		float a = ExplosionFrame_ / ExplosionMaxFrame_;

		obj_->SetDestruction(a);

		obj_->Setalpha(static_cast< float >( ( ExplosionMaxFrame_ - ExplosionFrame_ ) / ExplosionMaxFrame_ ));
		if ( ExplosionFrame_ >= ExplosionMaxFrame_ )
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

		attackVec_ = frontVec;

		attackVec_.y = 0;

		attackVec_.normalize();

		obj_->SetPosition(obj_->GetPosition() + attackVec_ * moveSpeed_);
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

			Vector3 a = attackStartPoint_ * ( 1.0f - timeRate ) +attackMiddlePoint_* timeRate;
			Vector3 b = attackMiddlePoint_ * ( 1.0f - timeRate ) + attackEndPoint_ * timeRate;

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

void jumpEnemy::OnEnemyCollision(Vector3 reject)
{
	reject.y = 0;

	obj_->SetPosition(obj_->GetPosition() + ( reject ));

	attackEndPoint_ += reject;
	attackMiddlePoint_ += reject;
	attackStartPoint_ += reject;
}
