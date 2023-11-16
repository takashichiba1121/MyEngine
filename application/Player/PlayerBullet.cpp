#include"PlayerBullet.h"
#include"PlayerBulletManager.h"

void PlayerBullet::Initialize(Model* model, Vector2 velocity, Vector3 position,uint32_t life)
{
	obj_ = std::make_unique<Object3d>();

	obj_->Initialize();
	
	velocity_ = velocity;

	obj_->SetModel(model);

	obj_->SetPosition(position);

	obj_->SetColor({3,3,3});

	life_ = life;
}

void PlayerBullet::Update()
{
	Vector3 move;

	float f;
	switch ( phase_ )
	{
	case Phase::Charge:
		chageTimer_++;
		f = ( float ) chageTimer_ / chageTime_;

		obj_->SetScale({ f,f,f });

		if ( chageTimer_ >= chageTime_ )
		{
			phase_ = Phase::Attack;
		}

		break;
	case Phase::Attack:
		if ( --life_ <= 0 )
		{
			isDead_ = true;
		}

		move = obj_->GetPosition();

		move += {velocity_.x,0,velocity_.y};

		obj_->SetPosition(move);
		break;
	default:
		break;
	}

	obj_->Update();
}

void PlayerBullet::Draw()
{
	obj_->Draw();
}

void PlayerBullet::OnCollision()
{
	isDead_ = true;

	for ( int i = 0; i < 50; i++ )
	{
		//消えるまでの時間
		const uint32_t rnd_life = 10;
		//最低限のライフ
		const uint32_t constlife = 30;
		uint32_t life = ( rand() / RAND_MAX * rnd_life ) + constlife;

		//XYZの広がる距離
		const float rnd_pos = 0.3f;
		Vector3 pos{};
		pos.x = ( float ) rand() / RAND_MAX * rnd_pos - rnd_pos / 2;
		pos.y = ( ( float ) rand() / RAND_MAX * rnd_pos - rnd_pos / 2 );
		pos.z = ( float ) rand() / RAND_MAX * rnd_pos - rnd_pos / 2;

		//pos.normalize();

		//追加
		PlayerBulletManager::Instance()->GetParticle()->Add(life,obj_->GetPosition(),pos,{0,0,0},1.0f,1.0f,{1,1,1,1},{1,0.7f,0,0});
	}
}

void PlayerBullet::SetPhase(Phase phase)
{
	phase_ = phase;

	if ( phase == Phase::Charge )
	{
		obj_->SetScale({ 0,0,0 });

		obj_->Update();
	}
}
