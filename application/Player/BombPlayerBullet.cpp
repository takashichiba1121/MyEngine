#include "BombPlayerBullet.h"
#include"PlayerBulletManager.h"

void BombPlayerBullet::Initialize(Model* model,Vector2 velocity,Vector3 position,uint32_t life)
{
	obj_ = std::make_unique<Object3d>();

	obj_->Initialize();

	velocity_ = velocity;

	model_ = model;

	obj_->SetModel(model_);

	obj_->SetPosition(position);

	obj_->SetColor({ bulletColor_ });

	life_ = life;
}

void BombPlayerBullet::Update()
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
			chageTimer_ = 1;
			phase_ = Phase::Attack;
		}

		break;
	case Phase::Attack:
		if ( --life_ <= 0 )
		{
			if ( lightIndex_ >= 0 )
			{
				light_->SetPointActive(lightIndex_,false);
			}
			isDead_ = true;
		}

		move = obj_->GetPosition();

		move += {velocity_.x,0,velocity_.y};

		obj_->SetPosition(move);

		if ( lightIndex_ >= 0 )
		{
			light_->SetPointPos(lightIndex_,obj_->GetPosition());
		}
		break;
	case Phase::Delete:
		chageTimer_++;
		f = ( float ) chageTimer_ / 5;

		f *= 5;

		obj_->SetScale({ f,f,f });

		if ( chageTimer_ >= 5 )
		{
			isDead_ = true;
			if ( lightIndex_ >= 0 )
			{
				light_->SetPointActive(lightIndex_,false);
			}
			for ( int i = 0; i < 50; i++ )
			{
				//消えるまでの時間
				const uint32_t rnd_life = 10;
				//最低限のライフ
				const uint32_t constlife = 30;
				uint32_t life = ( rand() / RAND_MAX * rnd_life ) + constlife;

				//XYZの広がる距離
				const float rnd_velocity = 0.3f;
				Vector3 velocity{};
				velocity.x = ( float ) rand() / RAND_MAX * rnd_velocity - rnd_velocity / 2;
				velocity.y = ( ( float ) rand() / RAND_MAX * rnd_velocity - rnd_velocity / 2 );
				velocity.z = -abs(( float ) rand() / RAND_MAX * rnd_velocity - rnd_velocity / 2);

				//pos.normalize();

				const Vector4 startColor = { bulletColor_.x,bulletColor_.y,bulletColor_.z,1 };

				const Vector4 endColor = { bulletColor_.x,bulletColor_.y,bulletColor_.z,0.5f };

				//追加
				PlayerBulletManager::Instance()->GetParticle()->Add(life,obj_->GetPosition(),velocity,{ 0,0,0 },1.5f,1.5f,{ startColor },{ endColor });
			}
		}
		break;
	default:
		break;
	}

	obj_->Update();
}

void BombPlayerBullet::OnCollision()
{
	phase_ = Phase::Delete;
}

void BombPlayerBullet::SetLight(LightGroup* light,int32_t lightIndex)
{
	light_ = light;

	lightIndex_ = lightIndex;

	if ( lightIndex_ >= 0 )
	{

		light_->SetPointActive(lightIndex_,true);

		light_->SetPointPos(lightIndex_,obj_->GetPosition());

		light_->SetPointColor(lightIndex_,{ bulletColor_ });

		light_->SetPointAtten(lightIndex_,{ 0.03f,0.01f,0.01f });
	}
}
