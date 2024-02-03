#include"EnemyBullet.h"
#include"EnemyManager.h"

LightGroup* EnemyBullet::light_;

void EnemyBullet::Initialize(Model* model, Vector3 velocity, Vector3 position)
{
	obj_ = std::make_unique<Object3d>();

	obj_->Initialize();
	
	velocity_ = velocity;

	obj_->SetModel(model);

	obj_->SetPosition(position);

	obj_->SetColor(bulletColor_);
}

void EnemyBullet::Update()
{
	float f;
	Vector3 move;
	switch ( phase_ )
	{
	case EnemyBullet::Phase::Charge:
		chaseTimer_++;
		f = ( float ) chaseTimer_ / chaseTime_;

		obj_->SetScale({ f,f,f });

		if ( chaseTimer_ >= chaseTime_ )
		{
			phase_ = Phase::Attack;
		}

		break;
	case EnemyBullet::Phase::Attack:
		if ( --deathTimer_ <= 0 )
		{
			isDead_ = true;

			if ( lightIndex_ >= 0 )
			{
				light_->SetPointActive(lightIndex_,false);
			}
		}

		move = obj_->GetPosition();

		move += velocity_;

		obj_->SetPosition(move);

		for ( int i = 0; i < 5; i++ )
		{
			//最低限のライフ
			const uint32_t constlife = 7;
			uint32_t life = constlife;

			//XYZの広がる距離
			const float rnd_velocityX = 0.3f;
			const float rnd_velocityZ = 0.3f;
			Vector3 velocity{};
			velocity.x = ( float ) rand() / RAND_MAX * rnd_velocityX - rnd_velocityX / 2;
			velocity.y = 0;
			velocity.z = ( ( float ) rand() / RAND_MAX * rnd_velocityZ - rnd_velocityZ / 2 );

					//XYZの広がる距離
			Vector3 accel = { 0,0,0 };

			//pos.normalize();

			Vector3 particleColor = bulletColor_ * 2;

			const Vector4 startColor = { particleColor.x,particleColor.y,particleColor.z,0.5f };

			const Vector4 endColor = { particleColor.x,particleColor.y,particleColor.z,0 };

			//追加
			EnemyManager::Instance()->GetParticle()->Add(life,obj_->GetPosition(),velocity,accel,3.0f,1.0f,startColor,endColor);
		}

		if ( lightIndex_ >= 0 )
		{
			light_->SetPointPos(lightIndex_,obj_->GetPosition());
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

void EnemyBullet::Draw()
{
		obj_->Draw();
}

void EnemyBullet::OnCollision()
{
	isDead_ = true;

	if ( lightIndex_ >= 0 )
	{
		light_->SetPointActive(lightIndex_,false);
	}
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

		Vector3 particleColor = { 0.1f,1.0f,0.1f };

		const Vector4 startColor = { particleColor.x,particleColor.y,particleColor.z,0.5f };

		const Vector4 endColor = { particleColor.x,particleColor.y,particleColor.z,0 };

		//追加
		EnemyManager::Instance()->GetParticle()->Add(life,obj_->GetPosition(),velocity,accel,1.5f,0.0f,startColor,endColor);
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

		Vector3 particleColor = { 0.1f,1.0f,0.1f };

		const Vector4 startColor = { particleColor.x,particleColor.y,particleColor.z,0.5f };

		const Vector4 endColor = { particleColor.x,particleColor.y,particleColor.z,0 };

		//追加
		EnemyManager::Instance()->GetParticle()->Add(life,obj_->GetPosition(),velocity,accel,10.0f,1.0f,startColor,endColor);
	}
}

void EnemyBullet::SetPhase(Phase phase)
{
	phase_ = phase;

	if ( phase==Phase::Charge )
	{
		obj_->SetScale({0,0,0});

		obj_->Update();
	}
}

void EnemyBullet::SetLight(int32_t lightIndex)
{
	lightIndex_ = lightIndex;

	if ( lightIndex_ >= 0 )
	{

		light_->SetPointActive(lightIndex_,true);

		light_->SetPointPos(lightIndex_,obj_->GetPosition());

		light_->SetPointColor(lightIndex_,bulletColor_/2);

		light_->SetPointAtten(lightIndex_,{ 0.03f,0.01f,0.01f });
	}
}
