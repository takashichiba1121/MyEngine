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

	obj_->SetColor({ cBulletColor_ });

	life_ = life;

	type_ = Type::Bomb;
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
		f = ( float ) chageTimer_ / 10;

		f *= 10;

		obj_->SetScale({ f,f,f });

		if ( chageTimer_ >= 10 )
		{
			isDead_ = true;
			if ( lightIndex_ >= 0 )
			{
				light_->SetPointActive(lightIndex_,false);
			}
			for ( int i = 0; i < 100; i++ )
			{
				//消えるまでの時間
				const uint32_t rnd_life = 10;
				//最低限のライフ
				const uint32_t constlife = 30;
				uint32_t life = ( rand() / RAND_MAX * rnd_life ) + constlife;

				//XYZの広がる距離
				const float rnd_velocity = 0.6f;
				Vector3 velocity{};
				velocity.x = ( float ) rand() / RAND_MAX * rnd_velocity - rnd_velocity / 2;
				velocity.y = ( ( float ) rand() / RAND_MAX * rnd_velocity - rnd_velocity / 2 );
				velocity.z = ( ( float ) rand() / RAND_MAX * rnd_velocity - rnd_velocity / 2 );

						//XYZの広がる距離
				//const float rnd_accel = 0.05f;
				Vector3 accel = { 0,0,0 };
				//accel.x = ( float ) rand() / RAND_MAX * rnd_accel - rnd_accel / 2;
				//accel.y = ( ( float ) rand() / RAND_MAX * rnd_accel - rnd_accel / 2 );
				//accel.z = ( ( float ) rand() / RAND_MAX * rnd_accel - rnd_accel / 2 );

				//pos.normalize();

				const Vector4 startColor = { cBulletColor_.x,cBulletColor_.y,cBulletColor_.z,1 };

				const Vector4 endColor = { cBulletColor_.x,cBulletColor_.y,cBulletColor_.z,0 };

				//追加
				PlayerBulletManager::Instance()->GetParticle()->Add(life,obj_->GetPosition(),velocity,accel,3.0f,1.0f,startColor,endColor);
			}
		}
		break;
	default:
		break;
	}

	Vector2 objUV = obj_->GetUVShift();

	objUV += {0.01f,-0.01f};

	obj_->SetUVShift(objUV);

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

		light_->SetPointColor(lightIndex_,{ cBulletColor_/2 });

		light_->SetPointAtten(lightIndex_,{ 0.015f,0.005f,0.005f });
	}
}
