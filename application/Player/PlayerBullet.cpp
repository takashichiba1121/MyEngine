#include"PlayerBullet.h"

void PlayerBullet::Initialize(Model* model, Vector2 velocity, Vector3 position,uint32_t life)
{
	obj_ = std::make_unique<Object3d>();

	obj_->Initialize();
	
	velocity_ = velocity;

	obj_->SetModel(model);

	obj_->SetPosition(position);

	life_ = life;
}

void PlayerBullet::Update()
{

	if (--life_ <= 0) {
		isDead_ = true;
	}

	Vector3 move = obj_->GetPosition();

	move += {velocity_.x,0,velocity_.y};

	obj_->SetPosition(move);

	obj_->Update();
}

void PlayerBullet::Draw()
{
	obj_->Draw();
}

void PlayerBullet::OnCollision()
{
	isDead_ = true;
}
