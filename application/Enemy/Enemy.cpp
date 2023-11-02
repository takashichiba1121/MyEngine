#include"Enemy.h"
#include"EnemyManager.h"
#include<imgui.h>
#include"TextureManager.h"

void Enemy::Initialize(Model* enemyModel,Model* bulletModel,Vector3 position,Object3d* playerObj)
{
	model_ = enemyModel;

	obj_ = std::make_unique<Object3d>();

	obj_->Initialize();

	obj_->SetModel(model_);

	obj_->SetPosition(position);

	obj_->SetPolygonExplosion({ 0.0f,1.0f,6.28f,20.0f });

	bulletModel_ = bulletModel;

	playerObj_ = playerObj;

	circle_.reset(Model::CreatePlaneModel(TextureManager::Instance()->LoadTexture("Resources/Circle/enemyTex.png")));

	attackCircle_ = std::make_unique<Object3d>();

	attackCircle_->Initialize();

	attackCircle_->SetModel(circle_.get());

	attackCircle_->SetPosition({ position.x,position.y - obj_->GetScale().y + 0.1f,position.z });

	attackCircle_->SetScale({ 25,0,25 });

	//attackCircle_->SetRot({ 3.14f,0,0 });

	attackCircle_->SetPolygonExplosion({ 0.0f,1.0f,6.28f,20.0f });

	attackCircle_->SetColor({ 1,0,0 });

	attackCircle_->Setalpha(0.5f);

	attackCircle_->Update();


}

void Enemy::Update(float attackRange)
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

		isMove_ = ( distance <= attackRange && ( playerPos.y == enemyPos.y ) );

		if (isMove_==false )
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

	attackCircle_->Update();

	obj_->Update();

	attackCircle_->SetScale({ attackRange,0, attackRange });
}

void Enemy::Move()
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

void Enemy::ObjectUpdate()
{
	obj_->Update();

	attackCircle_->Update();
}

void Enemy::Draw()
{
	obj_->Draw();

#ifdef _DEBUG
	if ( isDaed_ == false )
	{
		attackCircle_->Draw();
	}
#endif
}

void Enemy::OnCollision()
{
	isDaed_ = true;
}

void Enemy::AttackOff()
{
	isAttack_ = false;

	attackTimer_ = kAttackTimer_;
}
