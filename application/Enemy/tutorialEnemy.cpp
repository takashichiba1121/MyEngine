#include "tutorialEnemy.h"

void TutorialEnemy::Initialize(Model* enemyModel,Model* bulletModel,Player* player,uint32_t number)
{
	model_ = enemyModel;

	bulletModel_ = bulletModel;

	player_ = player;

	enemyType_ = EnemyType::Tutorial;

	obj_ = std::make_unique<Object3d>();

	obj_->Initialize();

	obj_->SetModel(model_);

	obj_->SetRot({ 0,135,0 });

	obj_->SetPolygonExplosion({ 0.0f,1.0f,6.28f,20.0f });

	number_ = number;
}

void TutorialEnemy::Update()
{
	if ( isDaed_&&isRevival_==false)
	{
		ExplosionFrame_++;

		float a = ExplosionFrame_ / ExplosionMaxFrame_;

		obj_->SetDestruction(a);

		obj_->Setalpha(static_cast< float >( ( ExplosionMaxFrame_ - ExplosionFrame_ ) / ExplosionMaxFrame_ ));
		if ( ExplosionFrame_ >= ExplosionMaxFrame_ )
		{
			isRevival_ = true;
		}
	}
	else if(isRevival_)
	{
		ExplosionFrame_--;

		float a = ExplosionFrame_ / ExplosionMaxFrame_;

		obj_->SetDestruction(a);

		obj_->Setalpha(static_cast< float >( ( ExplosionMaxFrame_ - ExplosionFrame_ ) / ExplosionMaxFrame_ ));
		if ( ExplosionFrame_ <=0 )
		{
			isRevival_ = false;
			isDaed_ = false;
		}
	}

	obj_->Update();
}
