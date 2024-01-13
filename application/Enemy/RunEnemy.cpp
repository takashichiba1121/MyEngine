#include "RunEnemy.h"
#include"EnemyManager.h"



void RunEnemy::Initialize(Model* enemyModel,Model* bulletModel,Player* player,uint32_t number)
{
	model_ = enemyModel;

	bulletModel_ = bulletModel;

	player_ = player;

	enemyType_ = EnemyType::Run;

	obj_ = std::make_unique<Object3d>();

	obj_->Initialize();

	obj_->SetModel(model_);

	obj_->SetRot({ 0,135,0 });

	obj_->SetPolygonExplosion({ 0.0f,1.0f,6.28f,20.0f });

	number_ = number;
}

void RunEnemy::Update()
{
	if ( isDaed_ == false )
	{
		Vector3 playerPos,enemyPos,playerScale,enemyScale;

		playerPos = player_->GetObj()->GetPosition();

		enemyPos = obj_->GetPosition();

		playerScale = player_->GetObj()->GetScale();

		enemyScale = obj_->GetScale();

		float distance = static_cast< float >( sqrt(pow(playerPos.x - enemyPos.x,2) + pow(playerPos.z - enemyPos.z,2)) );

		isMove_ = ( distance <= attackRange_ && ( playerPos.y == enemyPos.y ) );

		if ( distance <= 10 && ( playerPos.y == enemyPos.y ) && isAttack_ == false )
		{
			isAttack_ = true;

			attackTimer_ = 0;

			Vector3 frontVec = player_->GetObj()->GetPosition() - obj_->GetPosition();

			attackEndRot_ = atan2f(frontVec.x,frontVec.z);

			attackStartRot_ = obj_->GetRot().y;

			attackVec_ = frontVec;

			attackVec_.y = 0;

			attackVec_.normalize();
		}

		if ( isMove_ == false )
		{
			attackTimer_ = 0;
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

void RunEnemy::Move()
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

void RunEnemy::Attack()
{
	if ( isAttack_ )
	{
		attackTimer_++;
		if ( attackTimer_ <= kAttackTimer_ - 60 )
		{
			obj_->SetRot({ 0,attackEndRot_,0 });
		}
		else if ( attackTimer_ <= kAttackTimer_ - 40 )
		{
			obj_->SetPosition(obj_->GetPosition() - attackVec_ * 0.05f);
		}
		else if ( attackTimer_ >= kAttackTimer_ - 30 )
		{
			Vector3 vec = Matrix4Math::transform({ 1,0,0 },obj_->GetMatWorld());

			vec.y = 0;

			vec.normalize();

			const uint32_t constlife = 10;
			uint32_t life = constlife;

			//XYZの広がる距離
			Vector3 verocity{};
			verocity.x = 0;
			verocity.y = -0;
			verocity.z = 0;

			Vector3 pos = obj_->GetPosition();

			pos.y -= obj_->GetScale().y;

			pos += vec;

			//追加
			EnemyManager::Instance()->GetParticle()->Add(life,pos,verocity,{ 0,0,0 },0.5f,0.5f,{ 1,0.8f,0,1 },{ 1,0.8f,0,1 });

			pos -= vec * 2;
							//追加
			EnemyManager::Instance()->GetParticle()->Add(life,pos,verocity,{ 0,0,0 },0.5f,0.5f,{ 1,0.8f,0,1 },{ 1,0.8f,0,1 });
			obj_->SetPosition(obj_->GetPosition() + attackVec_ * attackSpeed_);
			if ( attackTimer_ >= kAttackTimer_ )
			{
				isAttack_ = false;

				attackTimer_ = 0;
			}
		}
	}
}

void RunEnemy::AttackOff()
{
	isAttack_ = false;
}
