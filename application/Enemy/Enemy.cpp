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

	obj_->SetRot({0,135,0});

	obj_->SetPolygonExplosion({ 0.0f,1.0f,6.28f,20.0f });

	bulletModel_ = bulletModel;

	playerObj_ = playerObj;


}

void Enemy::Update()
{
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
}

void Enemy::Draw()
{
	obj_->Draw();

#ifdef _DEBUG
	if ( isDaed_ == false )
	{
		//attackCircle_->Draw();
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
