#include "Player.h"
#include"input.h"
#include"Collider.h"
#include<imgui.h>
#include"TextureManager.h"
#include"Easing.h"
#include<math.h>
#include"EnemyManager.h"
#include"PlayerBulletManager.h"


void Player::Initialize()
{
	model_.reset(Model::LoadFormOBJ("Player",false));

	obj_ = std::make_unique<Object3d>();

	obj_->Initialize();

	obj_->SetModel(model_.get());

	obj_->SetPosition({ 5,5,5 });

	obj_->SetPolygonExplosion({ 0.0f,1.0f,6.28f,20.0f });

	paMan_ = std::make_unique<ParticleManager>();

	paMan_->Initialize();

	paMan_->SetTextureHandle(TextureManager::Instance()->LoadTexture("Resources/effect4.png"));

	obj_->Update();

	bulletModel_.reset(Model::LoadFormOBJ("playerBullet",true));

	PlayerBulletManager::Instance()->Initialize();
}

void Player::Update()
{

	move_ = { 0,0,0 };
	if ( isDaed_ == false )
	{
		if ( interval == 0 )
		{
			Move();
			if ( onGround_ == false )
			{
				Attack();
			}
		}
		else
		{
			interval--;
		}

		if ( hp_ <= 0 )
		{
			isDaed_ = true;
		}
	}
	else
	{
		ExplosionFrame++;

		float a = ExplosionFrame / ExplosionMaxFrame;

		obj_->SetDestruction(a);

		obj_->Setalpha(static_cast< float >( ( ExplosionMaxFrame - ExplosionFrame ) / ExplosionMaxFrame ));
		if ( ExplosionFrame >= ExplosionMaxFrame )
		{
			isDelete_ = true;
		}
	}

	EnemyCollision();

	move_ = MapCollision();

	obj_->SetPosition(move_);

	Object3d::SetTarget(Object3d::GetTarget() + ( ( obj_->GetPosition() - Object3d::GetTarget() ) * cameraSpeed_ ));

	Object3d::SetEye(Object3d::GetTarget() + cameraPos_);

	PlayerBulletManager::Instance()->Update();

	obj_->Update();

	paMan_->Update();

#ifdef _DEBUG

	ImGui::Begin("player");

	ImGui::SliderFloat("gravityAcceleration",&gravityAcceleration_,0.0f,0.1f,"%1.2f");
	ImGui::SliderFloat("StartJumpSpeed",&StartJumpSpeed_,-1.0f,0.0f,"%1.2f");
	ImGui::SliderInt("resetPoint",&resetPoint_,-15,15);
	ImGui::SliderFloat("kBulletSpeed",&kBulletSpeed_,0.0f,1.0f,"%1.2f");
	ImGui::SliderInt("bulletLife",&bulletLife_,0,300);

	ImGui::SliderFloat3("camera",&cameraPos_.x,-100,100,"%3.0f");

	ImGui::End();

#endif
}

void Player::Move()
{
	if ( Input::Instance()->IsLinkGamePad() )
	{

		move_ += {Input::Instance()->GetPadStick(PadStick::LX) / 4,0,Input::Instance()->GetPadStick(PadStick::LY) / 4};

		if ( Input::Instance()->PadTriggerKey(XINPUT_GAMEPAD_A) && onGround_ == false )
		{
			fallSpeed_ = StartJumpSpeed_;
			onGround_ = true;

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
				paMan_->Add(life,obj_->GetPosition(),pos,{ 0,0,0 },0.5f,0.5f,{ 1,1,1,1 },{ 1,1,1,1 });
			}
		}

	}
	else
	{
		if ( Input::Instance()->PushKey(DIK_W) )
		{
			move_ += {0,0,0.25f};
		}
		if ( Input::Instance()->PushKey(DIK_A) )
		{
			move_ += {-0.25f,0,0};
		}
		if ( Input::Instance()->PushKey(DIK_S) )
		{
			move_ += {0,0,-0.25f};
		}
		if ( Input::Instance()->PushKey(DIK_D) )
		{
			move_ += {0.25f,0,0};
		}
		if ( Input::Instance()->TriggerKey(DIK_SPACE) && onGround_ == false )
		{
			fallSpeed_ = StartJumpSpeed_;
			onGround_ = true;

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
				paMan_->Add(life,obj_->GetPosition(),pos,{ 0,0,0 },0.5f,0.5f,{ 1,1,1,1 },{ 1,1,1,1 });
			}
		}
	}
	if ( obj_->GetPosition().y <= resetPoint_ )
	{
		obj_->SetPosition(spawnPosition_);
		fallSpeed_ = 0;
	}

	if ( onGround_ )
	{
		fallSpeed_ += gravityAcceleration_;
		move_.y -= fallSpeed_;
	}

	Vector3 cameForward = { 0,0,-1 };

	Vector3  cameRight = { 1,0,0 };

	Vector3 frontVec = { 0,0,0 };

	if ( move_.x != 0.0f || move_.z != 0.0f )
	{
		frontVec = cameForward * move_.z + cameRight * move_.x;
	}

	if ( frontVec.x != 0.0f || frontVec.z != 0.0f )
	{
		obj_->SetRot({ 0, atan2f(frontVec.x, -frontVec.z),0 });
	}
}

void Player::Finalize()
{
	paMan_->Clear();
}

void Player::Attack()
{
	if ( Input::Instance()->IsLinkGamePad() )
	{

		if ( Input::Instance()->GetPadStick(PadStick::RT) >= 0.5 && Input::Instance()->GetOldPadStick(PadStick::RT) < 0.5 )
		{
			Vector3 velocity(0,0,1);
			velocity = Matrix4Math::transform(velocity,obj_->GetMatWorld());
			velocity.normalize();
			velocity *= kBulletSpeed_;

			//弾の生成し、初期化
			std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
			newBullet->Initialize(bulletModel_.get(),{ velocity.x,velocity.z },obj_->GetPosition() + ( velocity * 5 ),bulletLife_);

			newBullet->SetPhase(PlayerBullet::Phase::Charge);

			newBullet->SetChageTime(10);

			interval = 10;

			//弾の登録する
			PlayerBulletManager::Instance()->AddBullet(std::move(newBullet));
		}
		if ( Input::Instance()->PadTriggerKey(XINPUT_GAMEPAD_RIGHT_THUMB))
		{
			Vector3 velocity(0,0,1);

			velocity = Matrix4Math::transform(velocity,obj_->GetMatWorld());
			velocity.normalize();
			velocity *= kBulletSpeed_;

			Vector3 pos[ 3 ]
			{
				{0,0,0},
				{1,0,0},
				{-1,0,0},
			};

			//弾の生成し、初期化
			std::unique_ptr<PlayerBullet> newBullet[ 3 ];
			for ( int i = 0; i < 3; i++ )
			{
				pos[ i ] = Matrix4Math::transform(pos[ i ],obj_->GetMatWorld());
				pos[ i ].normalize();
				pos[ i ] *= 3;

				newBullet[ i ] = std::make_unique<PlayerBullet>();
				newBullet[ i ]->Initialize(bulletModel_.get(),{ velocity.x,velocity.z },( obj_->GetPosition() + ( velocity * 3 ) ) + pos[ i ],bulletLife_);

				newBullet[ i ]->SetPhase(PlayerBullet::Phase::Charge);

				newBullet[ i ]->SetChageTime(30);

							//弾の登録する
				PlayerBulletManager::Instance()->AddBullet(std::move(newBullet[ i ]));
			}

			interval = 30;
		}
	}
	else
	{
		if ( Input::Instance()->TriggerKey(DIK_Z) )
		{
			Vector3 velocity(0,0,1);
			velocity = Matrix4Math::transform(velocity,obj_->GetMatWorld());
			velocity.normalize();
			velocity *= kBulletSpeed_;

			//弾の生成し、初期化
			std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
			newBullet->Initialize(bulletModel_.get(),{ velocity.x,velocity.z },obj_->GetPosition() + ( velocity * 5 ),bulletLife_);

			newBullet->SetPhase(PlayerBullet::Phase::Charge);

			newBullet->SetChageTime(10);

			for (uint32_t i=0;i<20;i++ )
			{
				if ( light_->GetPointActive(i) == false )
				{
					newBullet->SetLight(light_,i);

					break;
				}
			}

			interval = 10;

			//弾の登録する
			PlayerBulletManager::Instance()->AddBullet(std::move(newBullet));
		}

		if ( Input::Instance()->TriggerKey(DIK_X) )
		{
			Vector3 velocity[ 3 ]
			{
				{0,0,1},
				{0.3f,0,1},
				{-0.3f,0,1},
			};

			//弾の生成し、初期化
			std::unique_ptr<PlayerBullet> newBullet[ 3 ];
			for ( int i = 0; i < 3; i++ )
			{
				velocity[i] = Matrix4Math::transform(velocity[i],obj_->GetMatWorld());
				velocity[i].normalize();
				velocity[i] *= kBulletSpeed_;

				newBullet[ i ] = std::make_unique<PlayerBullet>();
				newBullet[ i ]->Initialize(bulletModel_.get(),{ velocity[i].x,velocity[i].z},( obj_->GetPosition() + ( velocity[ 0 ] * 3 ) ),bulletLife_);

				newBullet[ i ]->SetPhase(PlayerBullet::Phase::Charge);

				newBullet[ i ]->SetChageTime(20);

				for (int j = 0; j < 20; j++ )
				{
					if ( light_->GetPointActive(j) == false )
					{
						newBullet[i]->SetLight(light_,j);

						break;
					}
				}

							//弾の登録する
				PlayerBulletManager::Instance()->AddBullet(std::move(newBullet[ i ]));
			}

			interval = 20;
		}
	}
}

void Player::Draw()
{
	obj_->Draw();

	PlayerBulletManager::Instance()->Draw();
}

void Player::ParticleDraw()
{
	paMan_->Draw();

	PlayerBulletManager::Instance()->ParticleDraw();
}

void Player::Reset()
{
	isDelete_ = false;

	hp_ = maxHp_;

	PlayerBulletManager::Instance()->Clear();

	isKnockBack_ = false;
}

void Player::SetMapData(std::vector<std::unique_ptr<Object3d>>* objects)
{
	assert(objects);

	objects_ = objects;

	paMan_->Clear();

	PlayerBulletManager::Instance()->Clear();
}

Vector3 Player::MapCollision()
{
	bool Ground = false;
	Vector3 pos = obj_->GetPosition() + move_;
	for ( const std::unique_ptr<Object3d>& MapObj : *objects_ )
	{

		Collider::Cube mapCube,objCube;
		mapCube.Pos = MapObj->GetPosition();
		mapCube.scale = MapObj->GetScale();
		objCube.Pos = obj_->GetPosition() + move_;
		objCube.oldPos = obj_->GetPosition();
		objCube.scale = obj_->GetScale();
		if ( Collider::CubeAndCube(mapCube,objCube,Collider::Collsion) == true )
		{
			if ( mapCube.Pos.y - mapCube.scale.y >= objCube.oldPos.y + objCube.scale.y && onGround_ )
			{
				pos.y = mapCube.Pos.y - ( mapCube.scale.y + objCube.scale.y );

				move_.y = 0;

				fallSpeed_ = 0;
			}
			else if ( mapCube.Pos.y + mapCube.scale.y <= objCube.oldPos.y - objCube.scale.y && onGround_ )
			{
				pos.y = mapCube.Pos.y + mapCube.scale.y + objCube.scale.y;

				move_.y = 0;

				onGround_ = false;

				Ground = true;
			}
			else
			{

				if ( mapCube.Pos.x + mapCube.scale.x <= objCube.oldPos.x - objCube.scale.x )
				{

					pos.x = mapCube.Pos.x + mapCube.scale.x + objCube.scale.x + 0.1f;

					move_.x = 0;
				}
				else if ( mapCube.Pos.x - mapCube.scale.x >= objCube.oldPos.x + objCube.scale.x )
				{
					pos.x = mapCube.Pos.x - ( mapCube.scale.x + objCube.scale.x ) - 0.1f;

					move_.x = 0;
				}
				if ( mapCube.Pos.z + mapCube.scale.z <= objCube.oldPos.z - objCube.scale.z )
				{

					pos.z = mapCube.Pos.z + mapCube.scale.z + objCube.scale.z + 0.1f;

					move_.z = 0;
				}
				else if ( mapCube.Pos.z - mapCube.scale.z >= objCube.oldPos.z + objCube.scale.z )
				{
					pos.z = mapCube.Pos.z - ( mapCube.scale.z + objCube.scale.z ) - 0.1f;

					move_.z = 0;
				}
			}
		}

		for ( std::unique_ptr<PlayerBullet>& bullet : PlayerBulletManager::Instance()->GetBullets() )
		{
			objCube.Pos = bullet->GetPosition();
			objCube.scale = bullet->GetScale();
			if ( Collider::CubeAndCube(mapCube,objCube,Collider::Collsion) == true )
			{
				bullet->OnCollision();
			}
		}
	}

	if ( Ground == false && onGround_ == false )
	{
		for ( const std::unique_ptr<Object3d>& MapObj : *objects_ )
		{

			Collider::Cube mapCube,objCube;
			mapCube.Pos = MapObj->GetPosition();
			mapCube.scale = MapObj->GetScale();
			objCube.Pos = obj_->GetPosition() + move_;
			objCube.oldPos = obj_->GetPosition();
			objCube.scale = obj_->GetScale();

			if ( Collider::QuadAndQuad(mapCube,objCube,Collider::Collsion) )
			{
				if ( mapCube.Pos.y + mapCube.scale.y <= objCube.Pos.y - objCube.scale.y - gravityAcceleration_ )
				{
					onGround_ = true;

					Ground = true;
				}
				else if ( mapCube.Pos.y - mapCube.scale.y >= objCube.Pos.y + objCube.scale.y - gravityAcceleration_ )
				{
					onGround_ = true;

					Ground = true;
				}
				else
				{
					onGround_ = false;

					fallSpeed_ = 0;

					break;
				}
			}
			else
			{
				onGround_ = true;

				Ground = true;
			}
		}
	}
	return pos;
}

void Player::EnemyCollision()
{
	for ( std::unique_ptr<Enemy>& enemy : EnemyManager::Instance()->GetEnemys() )
	{
		if ( enemy->IsDaed() == false )
		{
			Collider::Cube enemyCube,playerCube;
			enemyCube.Pos = enemy->GetObj()->GetPosition();
			enemyCube.scale = enemy->GetObj()->GetScale();
			playerCube.Pos = obj_->GetPosition() + move_;
			playerCube.scale = obj_->GetScale();
			for ( std::unique_ptr<PlayerBullet>& bullet : PlayerBulletManager::Instance()->GetBullets() )
			{
				Collider::Cube bulletCube;

				bulletCube.Pos = bullet->GetPosition();
				bulletCube.scale = bullet->GetScale();
				if ( Collider::CubeAndCube(enemyCube,bulletCube,Collider::Collsion) == true )
				{
					bullet->OnCollision();

					enemy->OnCollision();
				}
			}

			if ( Collider::CubeAndCube(enemyCube,playerCube,Collider::Collsion) == true )
			{
				if ( isKnockBack_ == false )
				{
					hp_--;

					isKnockBack_ = true;

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
						paMan_->Add(life,obj_->GetPosition(),pos,{ 0,0,0 },0.5f,0.5f,{ 1,1,1,1 },{ 1,1,1,1 });
					}
				}
			}
		}
	}

	for ( std::unique_ptr<EnemyBullet>& bullet : EnemyManager::Instance()->GetBullets() )
	{
		Collider::Cube bulletCube,playerCube;
		bulletCube.Pos = bullet->GetPosition();
		bulletCube.scale = bullet->GetScale();
		playerCube.Pos = obj_->GetPosition() + move_;
		playerCube.scale = obj_->GetScale();

		if ( Collider::CubeAndCube(bulletCube,playerCube,Collider::Collsion) == true )
		{
			if ( isKnockBack_ == false )
			{
				hp_--;

				isKnockBack_ = true;

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
					paMan_->Add(life,obj_->GetPosition(),pos,{ 0,0,0 },0.5f,0.5f,{ 1,1,1,1 },{ 1,1,1,1 });
				}
			}

			bullet->OnCollision();
		}
	}
}

//void Player::SetGoal(Vector3 goalPosition, Vector3 goalScale)
//{
//	goalPosition_ = goalPosition;
//
//	goalScale_ = goalScale;
//}

void Player::SetSpawn(Vector3 spawnPosition)
{
	spawnPosition_ = spawnPosition;

	obj_->SetPosition(spawnPosition_);

	obj_->SetRot({ 0,0,0 });

	obj_->Update();
}
