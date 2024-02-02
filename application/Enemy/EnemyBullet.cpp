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

	obj_->SetColor(bulletColor_/2);
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

		const Vector4 startColor = { bulletColor_.x,bulletColor_.y,bulletColor_.z,1 };

		const Vector4 endColor = { bulletColor_.x,bulletColor_.y,bulletColor_.z,0.5f };

		//追加
		EnemyManager::Instance()->GetParticle()->Add(life,obj_->GetPosition(),pos,{ 0,0,0 },1.0f,1.0f,startColor,endColor);
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

		light_->SetPointColor(lightIndex_,bulletColor_);

		light_->SetPointAtten(lightIndex_,{ 0.03f,0.01f,0.01f });
	}
}
