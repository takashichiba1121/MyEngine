#include"PlayerBullet.h"
#include"PlayerBulletManager.h"
#include"TextureManager.h"

void PlayerBullet::Initialize(Model* model,Vector2 velocity,Vector3 position,uint32_t life)
{
	obj_ = std::make_unique<Object3d>();

	obj_->Initialize();

	velocity_ = velocity;

	obj_->SetModel(model);

	obj_->SetPosition(position);

	obj_->SetColor(cBulletColor_);

	life_ = life;

	type_ = Type::Nolmal;

	chageTimer_ = 5;
}

void PlayerBullet::Update()
{
	switch ( phase_ )
	{
	case Phase::Charge:
		Charge();
		break;
	case Phase::Attack:
		Attack();
		break;
	case Phase::Delete:
		Delete();
		break;
	default:
		break;
	}

	Vector2 objUV = obj_->GetUVShift();

	objUV += {0.01f,-0.01f};

	obj_->SetUVShift(objUV);

	obj_->Update();
}

void PlayerBullet::Charge()
{
	float f;

	chageTimer_++;
	f = ( float ) chageTimer_ / chageTime_;

	obj_->SetScale({ f,f,f });

	if ( chageTimer_ >= chageTime_ )
	{
		chageTimer_ = 5;
		phase_ = Phase::Attack;
	}
}

void PlayerBullet::Attack()
{
	Vector3 move;

	if ( --life_ <= 0 )
	{
		phase_ = Phase::Delete;
	}

	if ( enemyPos_ !=nullptr )
	{
		Vector3 vec = enemyPos_->GetPosition() - obj_->GetPosition();
		vec.normalize();

		Vector2 vec2 = { vec.x,vec.z };

		velocity_ = velocity_ + ( ( vec2 - velocity_ ) * 0.1f );
	}

	move = obj_->GetPosition();

	move += {velocity_.x,0,velocity_.y};

	obj_->SetPosition(move);

	for ( int i = 0; i < 10; i++ )
	{
		//最低限のライフ
		const uint32_t constlife = 5;
		uint32_t life = constlife;

		//XYZの広がる距離
		const float rnd_velocityX = 0.05f;
		const float rnd_velocityZ = 0.05f;
		Vector3 velocity{};
		velocity.x = ( float ) rand() / RAND_MAX * rnd_velocityX - rnd_velocityX / 2;
		velocity.y = 0;
		velocity.z = ( ( float ) rand() / RAND_MAX * rnd_velocityZ - rnd_velocityZ / 2 );

				//XYZの広がる距離
		Vector3 accel = { 0,0,0 };

		//pos.normalize();

		const Vector4 startColor = { cParticleColor_.x,cParticleColor_.y,cParticleColor_.z,0.5f };

		const Vector4 endColor = { cParticleColor_.x,cParticleColor_.y,cParticleColor_.z,0 };

		//追加
		PlayerBulletManager::Instance()->GetParticle()->Add(life,obj_->GetPosition(),velocity,accel,3.0f,1.0f,startColor,endColor);
	}

	if ( lightIndex_ >= 0 )
	{
		light_->SetPointPos(lightIndex_,obj_->GetPosition());
	}

	circlePartFrame_++;
	if ( circlePartFrame_ >= cMaxCirclePartFrame_ )
	{
		circlePartFrame_ = 0;

		Vector3 vec[ 16 ] =
		{
		{1.0f,0.0f,0.0f},
		{0.75f,0.25f,0.0f},
		{0.5f,0.5f,0.0f},
		{0.25f,0.75f,0.0f},

		{-0.0f,1.0f,0.0f},
		{-0.25f,0.75f,0.0f},
		{-0.5f,0.5f,0.0f},
		{-0.75f,0.25f,0.0f},

		{-1.0f,0.0f,0.0f},
		{-0.75f,-0.25f,0.0f},
		{-0.5f,-0.5f,0.0f},
		{-0.25f,-0.75f,0.0f},

		{0.0f,-1.0f,0.0f},
		{0.25f,-0.75f,0.0f},
		{0.5f,-0.5f,0.0f},
		{0.75f,-0.25f,0.0f},
		};

		for ( int i = 0; i < 16; i++ )
		{
			//最低限のライフ
			const uint32_t constlife = 20;
			uint32_t life = constlife;

			//XYZの広がる距離
			Vector3 velocity = Matrix4Math::transform(vec[i],obj_->GetMatWorld());

			velocity = velocity.normalize() / 10;

					//XYZの広がる距離
			Vector3 accel = { 0,0,0 };

			//pos.normalize();

			const Vector4 startColor = { cParticleColor_.x,cParticleColor_.y,cParticleColor_.z,0.5f };

			const Vector4 endColor = { cParticleColor_.x,cParticleColor_.y,cParticleColor_.z,0 };

			//追加
			PlayerBulletManager::Instance()->GetParticle()->Add(life,obj_->GetPosition(),velocity,accel,1.0f,0.5f,startColor,endColor);
		}
	}
}

void PlayerBullet::Delete()
{
	float f;

	chageTimer_--;
	f = ( float ) chageTimer_ / 5;

	obj_->SetScale({ f,f,f });

	if ( chageTimer_ <= 0 )
	{
		isDead_ = true;
		if ( lightIndex_ >= 0 )
		{
			light_->SetPointActive(lightIndex_,false);
		}
	}
}

void PlayerBullet::Draw()
{
	obj_->Draw();

	for ( uint32_t i = 0; i < part_.size(); i++ )
	{
		part_[ i ]->Draw();
	}
}

void PlayerBullet::OnCollision()
{
	if ( phase_ == Phase::Attack )
	{
		phase_ = Phase::Delete;
		for ( int i = 0; i < 70; i++ )
		{
			//消えるまでの時間
			const uint32_t rnd_life = 10;
			//最低限のライフ
			const uint32_t constlife = 60;
			uint32_t life = ( rand() / RAND_MAX * rnd_life ) + constlife;

			//XYZの広がる距離
			const float rnd_velocityY = 0.3f;
			const float rnd_velocityX = 0.15f;
			const float rnd_velocityZ = 0.05f;
			Vector3 velocity{};
			velocity.x = ( float ) rand() / RAND_MAX * rnd_velocityX - rnd_velocityX / 2;
			velocity.y = abs(( float ) rand() / RAND_MAX * rnd_velocityY);
			velocity.z = ( ( float ) rand() / RAND_MAX * rnd_velocityZ - rnd_velocityZ / 2 );

					//XYZの広がる距離
			//const float rnd_accel = 0.05f;
			Vector3 accel = { 0,0,0 };
			//accel.x = ( float ) rand() / RAND_MAX * rnd_accel - rnd_accel / 2;
			//accel.y = ( ( float ) rand() / RAND_MAX * rnd_accel - rnd_accel / 2 );
			//accel.z = ( ( float ) rand() / RAND_MAX * rnd_accel - rnd_accel / 2 );

			//pos.normalize();

			const Vector4 startColor = { cParticleColor_.x,cParticleColor_.y,cParticleColor_.z,0.5f };

			const Vector4 endColor = { cParticleColor_.x,cParticleColor_.y,cParticleColor_.z,0 };

			//追加
			PlayerBulletManager::Instance()->GetParticle()->Add(life,obj_->GetPosition(),velocity,accel,1.5f,0.0f,startColor,endColor);
		}
		for ( int i = 0; i < 30; i++ )
		{
			//消えるまでの時間
			const uint32_t rnd_life = 10;
			//最低限のライフ
			const uint32_t constlife = 20;
			uint32_t life = ( rand() / RAND_MAX * rnd_life ) + constlife;

			//XYZの広がる距離
			const float rnd_velocityY = 0.3f;
			const float rnd_velocityX = 0.3f;
			const float rnd_velocityZ = 0.3f;
			Vector3 velocity{};
			velocity.x = ( float ) rand() / RAND_MAX * rnd_velocityX - rnd_velocityX / 2;
			velocity.y = ( ( float ) rand() / RAND_MAX * rnd_velocityY - rnd_velocityY / 2 );
			velocity.z = ( ( float ) rand() / RAND_MAX * rnd_velocityZ - rnd_velocityZ / 2 );

					//XYZの広がる距離
			//const float rnd_accel = 0.05f;
			Vector3 accel = { 0,0,0 };
			//accel.x = ( float ) rand() / RAND_MAX * rnd_accel - rnd_accel / 2;
			//accel.y = ( ( float ) rand() / RAND_MAX * rnd_accel - rnd_accel / 2 );
			//accel.z = ( ( float ) rand() / RAND_MAX * rnd_accel - rnd_accel / 2 );

			//pos.normalize();

			const Vector4 startColor = { cBulletColor_.x,cBulletColor_.y,cBulletColor_.z,0.5f };

			const Vector4 endColor = { cBulletColor_.x,cBulletColor_.y,cBulletColor_.z,0 };

			//追加
			PlayerBulletManager::Instance()->GetParticle()->Add(life,obj_->GetPosition(),velocity,accel,10.0f,1.0f,startColor,endColor);
		}
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

void PlayerBullet::SetLight(LightGroup* light,int32_t lightIndex)
{
	light_ = light;

	lightIndex_ = lightIndex;

	if ( lightIndex_ >= 0 )
	{

		light_->SetPointActive(lightIndex_,true);

		light_->SetPointPos(lightIndex_,obj_->GetPosition());

		light_->SetPointColor(lightIndex_,{ cBulletColor_ });

		light_->SetPointAtten(lightIndex_,{ 0.03f,0.01f,0.01f });
	}
}
