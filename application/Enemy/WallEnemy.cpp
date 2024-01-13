#include "WallEnemy.h"

void WallEnemy::Initialize(Model* enemyModel,Model* bulletModel,Player* player,uint32_t number)
{
	model_ = enemyModel;

	bulletModel_ = bulletModel;

	player_ = player;

	enemyType_ = EnemyType::Wall;

	obj_ = std::make_unique<Object3d>();

	obj_->Initialize();

	obj_->SetModel(model_);

	obj_->SetPolygonExplosion({ 0.0f,1.0f,6.28f,20.0f });

	number_ = number;
}

void WallEnemy::Update()
{
	if ( isDaed_)
	{
		ExplosionFrame_++;

		float a = ExplosionFrame_ / ExplosionMaxFrame_;

		obj_->SetDestruction(a);

		obj_->Setalpha(static_cast< float >( ( ExplosionMaxFrame_ - ExplosionFrame_ ) / ExplosionMaxFrame_ ));

		Vector3 pos = obj_->GetPosition();

		pos.y -= 0.5f;

		obj_->SetPosition(pos);
		if ( ExplosionFrame_ >= ExplosionMaxFrame_ )
		{
			isDelete_ = true;
		}
	}
	obj_->Update();
}
