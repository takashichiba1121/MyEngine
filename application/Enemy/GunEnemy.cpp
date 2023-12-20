#include "GunEnemy.h"
#include"EnemyManager.h"

LightGroup* Enemy::light_;

void GunEnemy::Initialize(Model* enemyModel,Model* bulletModel,const Vector3& position,Player* player,EnemyType enemyType,uint32_t number)
{
	model_ = enemyModel;

	bulletModel_ = bulletModel;

	player_ = player;

	enemyType_ = enemyType;

	obj_ = std::make_unique<Object3d>();

	obj_->Initialize();

	obj_->SetModel(model_);

	obj_->SetPosition(position);

	obj_->SetRot({ 0,135,0 });

	obj_->SetPolygonExplosion({ 0.0f,1.0f,6.28f,20.0f });

	number_ = number;

	attackSE_.Load("Resources/Sound/PlayerAttack.wav");
}

void GunEnemy::Update()
{
	if ( isDaed_ == false )
	{
		if ( isMove_ )
		{
			Attack();

			Move();
		}
		Vector3 playerPos,enemyPos,playerScale,enemyScale;

		playerPos = player_->GetObj()->GetPosition();

		enemyPos = obj_->GetPosition();

		playerScale = player_->GetObj()->GetScale();

		enemyScale = obj_->GetScale();

		float distance = static_cast< float >( sqrt(pow(playerPos.x - enemyPos.x,2) + pow(playerPos.z - enemyPos.z,2)) );

		float playerYmax,playerYmin,enemyYmax,enemyYmin;

		playerYmax = playerPos.y + playerScale.y;

		playerYmin = playerPos.y - playerScale.y;

		enemyYmax = enemyPos.y + ( enemyScale.y + 3 );

		enemyYmin = enemyPos.y - ( enemyScale.y + 3 );

		isMove_ = ( distance <= attackRange_ && ( playerYmax >= enemyYmin && playerYmin <= enemyYmax ) );
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

void GunEnemy::Move()
{
	Vector3 frontVec = player_->GetObj()->GetPosition() - obj_->GetPosition();

	obj_->SetRot({ 0, atan2f(frontVec.x, frontVec.z),0 });
}

void GunEnemy::Attack()
{
	if ( IntervalTimer_ > 0 )
	{
		IntervalTimer_--;
	}

	if ( IntervalTimer_ <= 0 )
	{
		IntervalTimer_ = kIntervalTime_;

		Vector3 bulletPosition = { obj_->GetPosition().x ,obj_->GetPosition().y + 3 ,obj_->GetPosition().z };

		//弾の速度
		const float kBulletSpeed = 0.7f;
		Vector3 velocity = player_->GetObj()->GetPosition() - bulletPosition;
		velocity.normalize();
		velocity *= kBulletSpeed;

		//弾の生成し、初期化
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		newBullet->Initialize(bulletModel_,velocity,bulletPosition);

		newBullet->SetPhase(EnemyBullet::Phase::Charge);

		for ( int i = 0; i < 20; i++ )
		{
			if ( light_->GetPointActive(i) == false )
			{
				newBullet->SetLight(i);

				break;
			}
			if (i>=19)
			{
				newBullet->SetLight(-1);
			}
		}
		//弾の登録する
		EnemyManager::Instance()->AddBullet(std::move(newBullet));

		attackSE_.Play(false,0.3f);
	}
}

void GunEnemy::AttackOff()
{
	isAttack_ = false;
}
