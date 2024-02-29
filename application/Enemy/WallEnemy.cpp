#include "WallEnemy.h"

void WallEnemy::Initialize(Model* enemyModel,Model* bulletModel,Player* player,uint32_t number)
{
	model_ = enemyModel;

	for ( uint32_t i = 0; i < cPartiacleNum_; i++ )
	{
		particle_[i].reset(new Object3d);

		particle_[ i ]->Initialize();

		particle_[ i ]->SetModel(model_);

		particle_[ i ]->SetIsDraw(false);
	}
	bulletModel_ = bulletModel;

	player_ = player;

	enemyType_ = EnemyType::Wall;

	obj_ = std::make_unique<Object3d>();

	obj_->Initialize();

	obj_->SetModel(model_);

	number_ = number;
}

void WallEnemy::Update()
{
	if ( isDaed_ )
	{
		obj_->SetIsDraw(false);

		ExplosionFrame_++;

		for ( uint32_t i = 0; i < cPartiacleNum_; i++ )
		{
			Vector3 pos = particle_[ i ]->GetPosition();

			pos += cVec_[ i ]/particleSpeed_;

			particle_[ i ]->SetPosition(pos);

			Vector3 rot = particle_[ i ]->GetRot();

			rot +={0.5f,0.5f,0.5f};

			particle_[ i ]->SetRot(rot);

			particle_[ i ]->Update();

			particle_[ i ]->SetIsDraw(true);
		}

		if ( ExplosionFrame_ >= cEndExplosionFrame_ )
		{
			isDelete_ = true;
		}
	}
	obj_->Update();
}

void WallEnemy::Draw()
{
	obj_->Draw();

	for ( uint32_t i = 0; i < cPartiacleNum_; i++ )
	{
		particle_[ i ]->Draw();
	}
}

void WallEnemy::OnCollision()
{
	isDaed_ = true;

	for ( uint32_t i = 0; i < cPartiacleNum_; i++ )
	{
		particle_[ i ]->SetPosition(obj_->GetPosition());
	}
}
