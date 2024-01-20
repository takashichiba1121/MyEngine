#include "jumpEnemy.h"
#include"EnemyManager.h"

void jumpEnemy::Initialize(Model* enemyModel,Model* bulletModel,Player* player,uint32_t number)
{
	model_ = enemyModel;

	bulletModel_ = bulletModel;

	player_ = player;

	enemyType_ = EnemyType::Jump;

	obj_ = std::make_unique<Object3d>();

	obj_->Initialize();

	obj_->SetModel(model_);

	obj_->SetPolygonExplosion({ 0.0f,1.0f,6.28f,20.0f });

	number_ = number;
}

void jumpEnemy::Update()
{
	if ( isDaed_ == false )
	{
		Vector3 playerPos,enemyPos,playerScale,enemyScale;

		playerPos = player_->GetObj()->GetPosition();

		enemyPos = obj_->GetPosition();

		playerScale = player_->GetObj()->GetScale();

		enemyScale = obj_->GetScale();

		float distance = static_cast< float >( sqrt(pow(playerPos.x - enemyPos.x,2) + pow(playerPos.z - enemyPos.z,2)) );

		isMove_ = ( distance <= attackRange_&&(playerPos.y == enemyPos.y));

		if ( distance <= 20&& isAttack_ == false&&player_->GetOnGround()==false)
		{
			isAttack_ = true;

			attackTimer_ =0;

			fallSpeed_ = StartJumpSpeed_;

			for ( int i = 0; i < 10; i++ )
			{
				//消えるまでの時間
				const uint32_t rnd_life = 10;
				//最低限のライフ
				const uint32_t constlife = 60;
				uint32_t life = ( rand() / RAND_MAX * rnd_life ) + constlife;

				//XYZの広がる距離
				const float rnd_pos = 0.1f;
				Vector3 pos{};
				pos.x = ( float ) rand() / RAND_MAX * rnd_pos - rnd_pos / 2;
				pos.y = ( ( float ) rand() / RAND_MAX * rnd_pos - rnd_pos / 2 );
				pos.z = ( float ) rand() / RAND_MAX * rnd_pos - rnd_pos / 2;

				//pos.normalize();

				//追加
				EnemyManager::Instance()->GetParticle()->Add(life,obj_->GetPosition(),pos,{ 0,0,0 },0.5f,0.5f,{ 1,1,1,1 },{ 1,1,1,1 });
			}
		}

		Move();

		Attack();
	}
	else
	{
		ExplosionFrame_++;

		float a = ExplosionFrame_ / ExplosionMaxFrame_;

		obj_->SetDestruction(a);

		obj_->Setalpha(static_cast< float >( ( ExplosionMaxFrame_ - ExplosionFrame_ ) / ExplosionMaxFrame_ ));
		if ( ExplosionFrame_ >= ExplosionMaxFrame_ )
		{
			isDelete_ = true;
		}
	}

	obj_->Update();
}

void jumpEnemy::Move()
{
	if ( isMove_ && isAttack_ == false )
	{
		Vector3 frontVec = player_->GetObj()->GetPosition() - obj_->GetPosition();

		obj_->SetRot({ 0, atan2f(frontVec.x, frontVec.z),0 });

		attackVec_ = frontVec;

		attackVec_.y = 0;

		attackVec_.normalize();

		obj_->SetPosition(obj_->GetPosition() + attackVec_ * moveSpeed_);
	}
}

void jumpEnemy::Attack()
{
	if ( isAttack_ )
	{
		attackTimer_++;
		if ( attackTimer_ <= 20 )
		{
			fallSpeed_ += gravityAcceleration_;

			Vector3 pos = obj_->GetPosition();

			pos.y -= fallSpeed_;

			obj_->SetPosition(pos);
		}
		else if(attackTimer_ <= 30 )
		{
			Vector3 frontVec = player_->GetObj()->GetPosition() - obj_->GetPosition();

			obj_->SetRot({ 0, atan2f(frontVec.x, frontVec.z),0 });

			if (attackTimer_==30 )
			{
				attackVec_ = frontVec;

				attackVec_.y = 0;

				attackVec_.normalize();

				attackEndPoint_ = obj_->GetPosition() + attackVec_ * 20;

				attackEndPoint_.y = player_->GetObj()->GetPosition().y;

				attackStartPoint_ = obj_->GetPosition();

				Vector3 playerPos,enemyPos,playerScale,enemyScale;

				playerPos = player_->GetObj()->GetPosition();

				enemyPos = obj_->GetPosition();

				playerScale = player_->GetObj()->GetScale();

				enemyScale = obj_->GetScale();

				float distance = static_cast< float >( sqrt(pow(playerPos.x - enemyPos.x,2) + pow(playerPos.z - enemyPos.z,2)) );

				if ( distance <= 20)
				{
					attackEndPoint_ = player_->GetObj()->GetPosition();
				}

				if ( player_->GetOnGround())
				{
					attackTimer_ = 29;
				}
			}
		}
		else if ( attackTimer_ <= 60 )
		{
			float timeRate = ( float(attackTimer_ - 30) / float(kAttackTimer_ - 30) );

			Vector3 a = attackStartPoint_ * ( 1.0f - timeRate ) + attackEndPoint_ * timeRate;

			obj_->SetPosition(a);

			const uint32_t constlife = 10;
			uint32_t life = constlife;

			//XYZの広がる距離
			Vector3 verocity{};
			verocity.x = 0;
			verocity.y = -0;
			verocity.z = 0;

			Vector3 pos = obj_->GetPosition();

			//追加
			EnemyManager::Instance()->GetParticle()->Add(life,pos,verocity,{0,0,0},1.0f,0.0f,{1,1,1,1},{1,1,1,1});
		}
		else
		{
			if ( attackTimer_ >= kAttackTimer_+20 )
			{
				isAttack_ = false;
			}
		}
	}
}

void jumpEnemy::AttackOff()
{
	isAttack_ = false;
}

void jumpEnemy::OnEnemyCollision(Vector3 reject)
{
	reject.y = 0;

	obj_->SetPosition(obj_->GetPosition() + ( reject ));

	attackEndPoint_ += reject;
	attackStartPoint_ += reject;
}
