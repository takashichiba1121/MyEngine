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

	obj_->SetColor({ bulletColor_ });

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

	Vector2 objUV = obj_->GetUVShift();

	objUV += {0.01f,-0.01f};

	obj_->SetUVShift(objUV);

	obj_->Update();
}

void DivisionPlayerBullet::OnCollision()
{
	isDead_ = true;
	if ( lightIndex_ >= 0 )
	{
		light_->SetPointActive(lightIndex_,false);
	}

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

		for ( int j = 0; j < LightGroup::cPointLightNum; j++ )
		{
			if ( light_->GetPointActive(j) == false )
			{
				newBullet[ i ]->SetLight(light_,j);

				break;
			}
			if ( i >= LightGroup::cPointLightNum-1 )
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

		light_->SetPointColor(lightIndex_,{ bulletColor_ });

		light_->SetPointAtten(lightIndex_,{ 0.03f,0.01f,0.01f });
	}
}
