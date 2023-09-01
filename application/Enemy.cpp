#include"Enemy.h"
#include"EnemyManager.h"
#include<imgui.h>

void Enemy::Initialize(Model* bulletModel,Vector3 position, Object3d* playerObj)
{
	model.reset(Model::LoadFormOBJ("enemy", true));

	obj_ = std::make_unique<Object3d>();

	obj_->Initialize();

	obj_->SetModel(model.get());

	obj_->SetPosition(position);

	obj_->SetPolygonExplosion({ 0.0f,1.0f,6.28f,100.0f });

	bulletModel_ = bulletModel;

	playerObj_ = playerObj;
}

void Enemy::Update()
{
	Attack();

	Vector3 frontVec = playerObj_->GetPosition() - obj_->GetPosition();;

	obj_->SetRot({ 0, atan2f(frontVec.x, frontVec.z),0 });

	obj_->Update();
}

void Enemy::Attack()
{
	if (attackTimer_ > 0)
	{
		attackTimer_--;
	}

	if (attackTimer_<=0&&isAttack_==true)
	{
		attackTimer_ = kAttackTime;

		//弾の速度
		const float kBulletSpeed = 0.5f;
		Vector3 velocity(0, 0, kBulletSpeed);
		velocity = Matrix4Math::transform(velocity, obj_->GetMatWorld());
		float len = sqrt(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);
		if (len != 0)
		{
			velocity /= len;
		}
		velocity *= kBulletSpeed;

		//弾の生成し、初期化
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		newBullet->Initialize(bulletModel_, { velocity.x,velocity.z }, obj_->GetPosition());

		//弾の登録する
		EnemyManager::AddBullet(std::move(newBullet));
	}

	ImGui::Text("%d",attackTimer_);
}

void Enemy::Draw()
{
	obj_->Draw();
}

void Enemy::OnCollision()
{
	isDaed_ = true;
}
