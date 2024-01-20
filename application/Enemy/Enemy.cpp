#include"Enemy.h"
#include"EnemyManager.h"
#include<imgui.h>
#include"TextureManager.h"

void Enemy::Initialize(Model* enemyModel,Model* bulletModel,Player* player,uint32_t number)
{
	model_ = enemyModel;

	bulletModel_ = bulletModel;

	player_ = player;

	enemyType_ =EnemyType::Tutorial;

	number_ = number;

	obj_ = std::make_unique<Object3d>();

	obj_->Initialize();

	obj_->SetModel(model_);

	obj_->SetPolygonExplosion({ 0.0f,1.0f,6.28f,20.0f });
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

void Enemy::OnEnemyCollision(Vector3 reject)
{
	obj_->SetPosition(obj_->GetPosition() + ( reject ));
}

void Enemy::AttackOff()
{
	isAttack_ = false;
}
