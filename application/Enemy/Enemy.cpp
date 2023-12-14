#include"Enemy.h"
#include"EnemyManager.h"
#include<imgui.h>
#include"TextureManager.h"

void Enemy::Initialize(Model* enemyModel,Model* bulletModel,const Vector3& position,Player* player,EnemyType enemyType)
{
	model_ = enemyModel;

	obj_ = std::make_unique<Object3d>();

	obj_->Initialize();

	obj_->SetModel(model_);

	obj_->SetPosition(position);

	obj_->SetRot({0,135,0});

	obj_->SetPolygonExplosion({ 0.0f,1.0f,6.28f,20.0f });

	bulletModel_ = bulletModel;

	player_ = player;

	enemyType_ = enemyType;
}

void Enemy::Update()
{
}

void Enemy::Move()
{
}

void Enemy::ObjectUpdate()
{
	obj_->Update();
}

void Enemy::Draw()
{
	obj_->Draw();
}

void Enemy::OnCollision()
{
	isDaed_ = true;
}

void Enemy::AttackOff()
{
	isAttack_ = false;
}
