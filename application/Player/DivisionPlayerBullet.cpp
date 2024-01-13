#include "DivisionPlayerBullet.h"
#include"PlayerBulletManager.h"

void DivisionPlayerBullet::Initialize(Model* model,Vector2 velocity,Vector3 position,uint32_t life)
{
	obj_ = std::make_unique<Object3d>();

	obj_->Initialize();

	velocity_ = velocity;

	model_ =model;

	obj_->SetModel(model_);

	obj_->SetPosition(position);

	obj_->SetColor({ 0.88f,1,0.59f });

	life_ = life;

	type = Type::Division;
}

void DivisionPlayerBullet::Update()
{
	Vector3 move;

	float f;
	switch ( phase_ )
	{
	case Phase::Charge:
		chageTimer_++;
		f = ( float ) chageTimer_ / chageTime_;

		f *= 3;

		obj_->SetScale({ f,f,f });

		if ( chageTimer_ >= chageTime_ )
		{
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
	default:
		break;
	}

	obj_->Update();
}

void DivisionPlayerBullet::OnCollision()
{
	isDead_ = true;
	if ( lightIndex_ >= 0 )
	{
		light_->SetPointActive(lightIndex_,false);
	}
	//for ( int i = 0; i < 50; i++ )
	//{
	//	//消えるまでの時間
	//	const uint32_t rnd_life = 10;
	//	//最低限のライフ
	//	const uint32_t constlife = 30;
	//	uint32_t life = ( rand() / RAND_MAX * rnd_life ) + constlife;

	//	//XYZの広がる距離
	//	const float rnd_pos = 0.3f;
	//	Vector3 pos{};
	//	pos.x = ( float ) rand() / RAND_MAX * rnd_pos - rnd_pos / 2;
	//	pos.y = ( ( float ) rand() / RAND_MAX * rnd_pos - rnd_pos / 2 );
	//	pos.z = -abs(( float ) rand() / RAND_MAX * rnd_pos - rnd_pos / 2);

	//	//pos.normalize();

	//	//追加
	//	PlayerBulletManager::Instance()->GetParticle()->Add(life,obj_->GetPosition(),pos,{ 0,0,0 },1.0f,1.0f,{  0.88f,1,0.59f,1 },{  0.88f,1,0.59f,0.5f });
	//}

	//弾の生成し、初期化
	std::unique_ptr<PlayerBullet> newBullet[ 3 ];

	Vector3 velocity[ 3 ]
	{
		{0,0,1},
		{1,0,0},
		{-1,0,0},
	};
	for ( int i = 0; i < 3; i++ )
	{
		velocity[ i ] = Matrix4Math::transform(velocity[ i ],obj_->GetMatWorld());
		velocity[ i ].normalize();
		velocity[ i ] *= kBulletSpeed_;

		newBullet[ i ] = std::make_unique<PlayerBullet>();

		Vector3 pos = obj_->GetPosition();

		pos.y -= 2.5f;

		newBullet[ i ]->Initialize(model_,{ velocity[ i ].x,velocity[ i ].z },pos,bulletLife_);

		newBullet[ i ]->SetPhase(PlayerBullet::Phase::Attack);

		for ( int j = 0; j < 20; j++ )
		{
			if ( light_->GetPointActive(j) == false )
			{
				newBullet[ i ]->SetLight(light_,j);

				break;
			}
			if ( i >= 19 )
			{
				newBullet[ i ]->SetLight(light_,-1);
			}
		}

					//弾の登録する
		PlayerBulletManager::Instance()->AddBullet(std::move(newBullet[ i ]));
	}
}

void DivisionPlayerBullet::SetLight(LightGroup* light,int32_t lightIndex)
{
	light_ = light;

	lightIndex_ = lightIndex;

	if ( lightIndex_ >= 0 )
	{

		light_->SetPointActive(lightIndex_,true);

		light_->SetPointPos(lightIndex_,obj_->GetPosition());

		light_->SetPointColor(lightIndex_,{ 0.88f,1,0.59f });

		light_->SetPointAtten(lightIndex_,{ 0.03f,0.01f,0.01f });
	}
}
