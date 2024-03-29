#include "Player.h"
#include"input.h"
#include"Collider.h"
#include<imgui.h>
#include"TextureManager.h"
#include"Easing.h"
#include<math.h>
#include"EnemyManager.h"
#include"PlayerBulletManager.h"
#include"DivisionPlayerBullet.h"
#include"BombPlayerBullet.h"


void Player::Initialize()
{
	model_.reset(Model::LoadFormOBJ("Player",false));

	obj_ = std::make_unique<Object3d>();

	obj_->Initialize();

	obj_->SetModel(model_.get());

	obj_->SetPolygonExplosion({ 0.0f,1.0f,6.28f,20.0f });

	paMan_ = std::make_unique<ParticleManager>();

	paMan_->Initialize();

	paMan_->SetTextureHandle(TextureManager::Instance()->LoadTexture("Resources/Texture/effect4.png"));

	obj_->Update();

	bulletModel_.reset(Model::LoadFormOBJ("Bullet",true));

	PlayerBulletManager::Instance()->Initialize();

	normalAttack_ = std::make_unique<Sprite>();

	normalAttack_->Initialize(TextureManager::Instance()->LoadTexture("Resources/Texture/normalAttack.png"));

	normalAttack_->SetPosition({ 148,158 });

	normalAttack_->SetScale({ 138,138 });

	normalAttack_->SetColor({ 1,1,1,1 });

	normalAttack_->SetAnchorPoint({ 0.5f,1 });

	normalAttack_->Update();

	threeWayAttack_ = std::make_unique<Sprite>();

	threeWayAttack_->Initialize(TextureManager::Instance()->LoadTexture("Resources/Texture/3WayAttack.png"));

	threeWayAttack_->SetPosition({ 148,158 });

	threeWayAttack_->SetScale({ 128,128 });

	threeWayAttack_->SetColor({ 1,1,1,0.5f });

	threeWayAttack_->SetAnchorPoint({ 1,0.5f });

	threeWayAttack_->Update();

	divisionAttack_ = std::make_unique<Sprite>();

	divisionAttack_->Initialize(TextureManager::Instance()->LoadTexture("Resources/Texture/divisionAttack.png"));

	divisionAttack_->SetPosition({ 148,158 });

	divisionAttack_->SetColor({ 1,1,1,0.5f });

	divisionAttack_->SetAnchorPoint({ 0,0.5f });

	divisionAttack_->Update();

	bombAttack_ = std::make_unique<Sprite>();

	bombAttack_->Initialize(TextureManager::Instance()->LoadTexture("Resources/Texture/bombAttack.png"));

	bombAttack_->SetPosition({ 148,158 });

	bombAttack_->SetAnchorPoint({ 0.5f,0 });

	bombAttack_->SetColor({ 1,1,1,0.5f });

	bombAttack_->Update();
}

void Player::Update()
{

	move_ = { 0,0,0 };
	if ( isDaed_ == false )
	{
		Move();

		if ( move_.x == 0 && move_.y == 0 && move_.z == 0 )
		{
			notMovingFrame_++;
		}
		else
		{
			notMovingFrame_ = 0;
		}

		if ( notMovingFrame_ == 512 )
		{
			notMovingFrame_ = 1;
		}

		if ( hp_ <= 0 )
		{
			isDaed_ = true;
		}

		if ( Input::Instance()->PadTriggerKey(XINPUT_GAMEPAD_DPAD_UP) || Input::Instance()->TriggerKey(DIK_UP) )
		{
			normalAttack_->SetScale({ 138,138 });

			normalAttack_->SetColor({ 1,1,1,1 });

			threeWayAttack_->SetScale({ 128,128 });

			threeWayAttack_->SetColor({ 1,1,1,0.5f });

			divisionAttack_->SetScale({ 128,128 });

			divisionAttack_->SetColor({ 1,1,1,0.5f });

			bombAttack_->SetScale({ 128,128 });

			bombAttack_->SetColor({ 1,1,1,0.5f });

			attackType_ = AttackType::Normal;
		}
		else if ( Input::Instance()->PadTriggerKey(XINPUT_GAMEPAD_DPAD_LEFT) || Input::Instance()->TriggerKey(DIK_LEFT) )
		{
			normalAttack_->SetScale({ 128,128 });

			normalAttack_->SetColor({ 1,1,1,0.5f });

			threeWayAttack_->SetScale({ 138,138 });

			threeWayAttack_->SetColor({ 1,1,1,1 });

			divisionAttack_->SetScale({ 128,128 });

			divisionAttack_->SetColor({ 1,1,1,0.5f });

			bombAttack_->SetScale({ 128,128 });

			bombAttack_->SetColor({ 1,1,1,0.5f });
			
			attackType_ = AttackType::ThreeWay;
		}
		else if ( Input::Instance()->PadTriggerKey(XINPUT_GAMEPAD_DPAD_RIGHT) || Input::Instance()->TriggerKey(DIK_RIGHT) )
		{
			normalAttack_->SetScale({ 128,128 });

			normalAttack_->SetColor({ 1,1,1,0.5f });

			threeWayAttack_->SetScale({ 128,128 });

			threeWayAttack_->SetColor({ 1,1,1,0.5f });

			divisionAttack_->SetScale({ 138,138 });

			divisionAttack_->SetColor({ 1,1,1,1 });

			bombAttack_->SetScale({ 128,128 });

			bombAttack_->SetColor({ 1,1,1,0.5f });

			attackType_ = AttackType::Division;
		}
		else if ( Input::Instance()->PadTriggerKey(XINPUT_GAMEPAD_DPAD_DOWN) || Input::Instance()->TriggerKey(DIK_DOWN) )
		{
			normalAttack_->SetScale({ 128,128 });

			normalAttack_->SetColor({ 1,1,1,0.5f });

			threeWayAttack_->SetScale({ 128,128 });

			threeWayAttack_->SetColor({ 1,1,1,0.5f });

			divisionAttack_->SetScale({ 128,128 });

			divisionAttack_->SetColor({ 1,1,1,0.5f });

			bombAttack_->SetScale({ 138,138 });

			bombAttack_->SetColor({ 1,1,1,1 });

			attackType_ = AttackType::Bomb;
		}

		CameraPosChange();

		EnemyCollision();

		MapCollision();

		obj_->SetPosition(move_);

		Object3d::SetTarget(Object3d::GetTarget() + ( ( obj_->GetPosition() - Object3d::GetTarget() ) * cCameraSpeed_ ));

		Object3d::SetEye(Object3d::GetTarget() + cameraPos_);
	}
	else
	{
		ExplosionFrame_++;

		float a = ExplosionFrame_ / cEndExplosionFrame_;

		obj_->SetDestruction(a);

		obj_->Setalpha(static_cast< float >( ( cEndExplosionFrame_ - ExplosionFrame_ ) / cEndExplosionFrame_ ));
		if ( ExplosionFrame_ >= cEndExplosionFrame_ )
		{
			isDelete_ = true;
		}
	}

	PlayerBulletManager::Instance()->Update();

	obj_->Update();

	paMan_->Update();

	normalAttack_->Update();
	threeWayAttack_->Update();
	divisionAttack_->Update();
	bombAttack_->Update();

#ifdef _DEBUG

	ImGui::Begin("player");

	ImGui::Text("PlayerPos:%f,%f,%f",obj_->GetPosition().x,obj_->GetPosition().y,obj_->GetPosition().z);

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
	if ( AttackInterval_ == 0 )
	{
		if ( isAvoid_ == false )
		{
			if ( Input::Instance()->IsLinkGamePad() )
			{
				if ( cameraPos_.x == 0 && cameraPos_.z == -30 )
				{
					move_ += {Input::Instance()->GetPadStick(PadStick::LX),0,Input::Instance()->GetPadStick(PadStick::LY)};
				}
				else if ( cameraPos_.x == 0 && cameraPos_.z == 30 )
				{
					move_ += {-Input::Instance()->GetPadStick(PadStick::LX),0,-Input::Instance()->GetPadStick(PadStick::LY)};
				}
				else if ( cameraPos_.x == -30 && cameraPos_.z == 0 )
				{
					move_ += {Input::Instance()->GetPadStick(PadStick::LY),0,-Input::Instance()->GetPadStick(PadStick::LX)};
				}
				else if ( cameraPos_.x == 30 && cameraPos_.z == 0 )
				{
					move_ += {-Input::Instance()->GetPadStick(PadStick::LY),0,Input::Instance()->GetPadStick(PadStick::LX)};
				}

				if ( Input::Instance()->PadTriggerKey(XINPUT_GAMEPAD_A) && onGround_ == false )
				{
					fallSpeed_ = StartJumpSpeed_;
					onGround_ = true;

					Vector3 velocity[ 12 ] =
					{
						{1,0,0},
						{-1,0,0},
						{0,0,1},
						{0,0,-1},
						{0.5f,0,1},
						{0.5f,0,-1},
						{1,0,0.5f},
						{1,0,-0.5f},
						{-0.5f,0,1},
						{-0.5f,0,-1},
						{-1,0,0.5f},
						{-1,0,-0.5f},
					};

					for ( int i = 0; i < 12; i++ )
					{
						//消えるまでの時間
						const uint32_t rnd_life = 10;
						//最低限のライフ
						const uint32_t constlife = 30;
						uint32_t life = ( rand() / RAND_MAX * rnd_life ) + constlife;

						//XYZの広がる距離
						const float constPos = 0.1f;
						Vector3 pos = velocity[ i ] * constPos;


						//pos.normalize();

						//追加
						paMan_->Add(life,obj_->GetPosition(),pos,{ 0,0,0 },0.5f,0.5f,{ 1,1,1,1 },{ 1,1,1,1 });
					}
				}

			}
			else
			{
				if ( cameraPos_.x == 0 && cameraPos_.z == -30 )
				{
					if ( Input::Instance()->PushKey(DIK_W) )
					{
						move_ += {0,0,1};
					}
					if ( Input::Instance()->PushKey(DIK_A) )
					{
						move_ += {-1,0,0};
					}
					if ( Input::Instance()->PushKey(DIK_S) )
					{
						move_ += {0,0,-1};
					}
					if ( Input::Instance()->PushKey(DIK_D) )
					{
						move_ += {1,0,0};
					}
				}
				else if ( cameraPos_.x == 0 && cameraPos_.z == 30 )
				{
					if ( Input::Instance()->PushKey(DIK_W) )
					{
						move_ += {0,0,-1};
					}
					if ( Input::Instance()->PushKey(DIK_A) )
					{
						move_ += {1,0,0};
					}
					if ( Input::Instance()->PushKey(DIK_S) )
					{
						move_ += {0,0,1};
					}
					if ( Input::Instance()->PushKey(DIK_D) )
					{
						move_ += {-1,0,0};
					}
				}
				else if ( cameraPos_.x == -30 && cameraPos_.z == 0 )
				{
					if ( Input::Instance()->PushKey(DIK_W) )
					{
						move_ += {1,0,0};
					}
					if ( Input::Instance()->PushKey(DIK_A) )
					{
						move_ += {0,0,1};
					}
					if ( Input::Instance()->PushKey(DIK_S) )
					{
						move_ += {-1,0,0};
					}
					if ( Input::Instance()->PushKey(DIK_D) )
					{
						move_ += {0,0,-1};
					}
				}
				else if ( cameraPos_.x == 30 && cameraPos_.z == 0 )
				{
					if ( Input::Instance()->PushKey(DIK_W) )
					{
						move_ += {-1,0,0};
					}
					if ( Input::Instance()->PushKey(DIK_A) )
					{
						move_ += {0,0,-1};
					}
					if ( Input::Instance()->PushKey(DIK_S) )
					{
						move_ += {1,0,0};
					}
					if ( Input::Instance()->PushKey(DIK_D) )
					{
						move_ += {0,0,1};
					}
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

			if ( onGround_ == false )
			{
				Attack();
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
		move_ = move_.normalize() * cMoveSpeed_;
		if ( avoidInterval_ == 0 )
		{
			if ( isAvoid_ == false && Input::Instance()->TriggerKey(DIK_Q) || Input::Instance()->PadTriggerKey(XINPUT_GAMEPAD_X) )
			{
				isAvoid_ = true;

				avoidInterval_ = cEndAvoidInterval_;

				initialRot_ = obj_->GetRot();

				Vector3 velocity(0,0,1);
				avoidVec_ = Matrix4Math::transform(velocity,obj_->GetMatWorld());
				avoidVec_.normalize();
			}
		}
		else
		{
			avoidInterval_--;
			Avoid();
		}
	}
	else
	{
		AttackInterval_--;
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
}

void Player::Finalize()
{
	paMan_->Clear();
}

void Player::Attack()
{
	if ( Input::Instance()->GetPadStick(PadStick::RT) >= 0.5 && Input::Instance()->GetOldPadStick(PadStick::RT) < 0.5 || Input::Instance()->TriggerKey(DIK_Z) )
	{
		switch ( attackType_ )
		{
		case Player::AttackType::Normal:
			NormalShoot();
			break;
		case Player::AttackType::ThreeWay:
			ThreeWayShoot();
			break;
		case Player::AttackType::Division:
			DivisionShoot();
			break;
		case Player::AttackType::Bomb:
			BombShoot();
			break;
		}
	}
}

void Player::NormalShoot()
{
	Vector3 velocity = { 0,0,1 };
	velocity = Matrix4Math::transform(velocity,obj_->GetMatWorld());
	velocity.normalize();
	velocity *= kBulletSpeed_ / 1.5f;

	Vector3 pos = obj_->GetPosition() + ( velocity * 5 );

	std::unique_ptr<PlayerBullet> newBullet;

	//弾の生成し、初期化
	newBullet = std::make_unique<PlayerBullet>();
	newBullet->Initialize(bulletModel_.get(),{ velocity.x,velocity.z },pos,uint32_t(bulletLife_/1.5f));

	newBullet->SetPhase(PlayerBullet::Phase::Charge);

	newBullet->SetRot(obj_->GetRot());

	for ( uint32_t i = 0; i < LightGroup::cPointLightNum; i++ )
	{
		if ( light_->GetPointActive(i) == false )
		{
			newBullet->SetLight(light_,i);

			break;
		}
		if ( i <= LightGroup::cPointLightNum - 1 )
		{
			newBullet->SetLight(light_,-1);
		}
	}
	float closestDistance = 30;
	Enemy* closestEnemy=nullptr;
	for ( std::unique_ptr<Enemy>& enemy : EnemyManager::Instance()->GetEnemys() )
	{
		Vector3 enemyPos= enemy->GetObj()->GetPosition();
		Vector3 pleyerPos=obj_->GetPosition();
		float distance = static_cast< float >( sqrt(pow(pleyerPos.x - enemyPos.x,2) + pow(pleyerPos.z - enemyPos.z,2)) );
		if ( distance <=30&&enemy->GetType()!=Enemy::EnemyType::Tutorial&& enemy->GetType() != Enemy::EnemyType::Wall&& !enemy->IsDaed() )
		{
			if ( distance<closestDistance )
			{
				closestEnemy = enemy.get();

				closestDistance = distance;
			}
		}
	}
	if ( closestEnemy!=nullptr )
	{
		newBullet->SetEnemyPos(closestEnemy->GetObj());
	}

	newBullet->SetChageTime(cNormalAttackInterval_);

	AttackInterval_ = cNormalAttackInterval_;

	//弾の登録する
	PlayerBulletManager::Instance()->AddBullet(std::move(newBullet));
}

void Player::ThreeWayShoot()
{
	std::unique_ptr<PlayerBullet> newBullet[ 3 ];
	Vector3 pos[ 3 ];
	Vector3 velocity[ 3 ]
	{
		{0,0,1},
		{0.3f,0,1},
		{-0.3f,0,1},
	};
	Vector3 rot[ 3 ]
	{
		{0,0,0},
		{0,54.0f,0},
		{0,-54.0f,0},
	};
	for ( int i = 0; i < 3; i++ )
	{
		velocity[ i ] = Matrix4Math::transform(velocity[ i ],obj_->GetMatWorld());
		velocity[ i ].normalize();
		velocity[ i ] *= kBulletSpeed_ / 2;

		newBullet[ i ] = std::make_unique<PlayerBullet>();

		pos[ i ] = obj_->GetPosition() + ( velocity[ 0 ] * 5 );

		newBullet[ i ]->Initialize(bulletModel_.get(),{ velocity[ i ].x,velocity[ i ].z },pos[ i ],bulletLife_);

		newBullet[ i ]->SetPhase(PlayerBullet::Phase::Charge);

		newBullet[ i ]->SetChageTime(cThreeWayAttackInterval_);

		newBullet[ i ]->SetRot(obj_->GetRot() + rot[ i ]);

		for ( int j = 0; j < LightGroup::cPointLightNum; j++ )
		{
			if ( light_->GetPointActive(j) == false )
			{
				newBullet[ i ]->SetLight(light_,j);

				break;
			}
			if ( i >= LightGroup::cPointLightNum )
			{
				newBullet[ i ]->SetLight(light_,-1);
			}
		}

					//弾の登録する
		PlayerBulletManager::Instance()->AddBullet(std::move(newBullet[ i ]));
	}
	AttackInterval_ = cThreeWayAttackInterval_;
}

void Player::DivisionShoot()
{
	Vector3 velocity = { 0,0,1 };
	velocity = Matrix4Math::transform(velocity,obj_->GetMatWorld());
	velocity.normalize();
	velocity *= kBulletSpeed_ / 2;

	Vector3 pos = obj_->GetPosition() + ( velocity * 5 );

	pos.y += 2.5f;

	//弾の生成し、初期化
	std::unique_ptr<PlayerBullet> newBullet = std::make_unique<DivisionPlayerBullet>();
	newBullet->Initialize(bulletModel_.get(),{ velocity.x,velocity.z },pos,bulletLife_);

	newBullet->SetPhase(PlayerBullet::Phase::Charge);

	newBullet->SetRot(obj_->GetRot());

	for ( uint32_t i = 0; i < LightGroup::cPointLightNum; i++ )
	{
		if ( light_->GetPointActive(i) == false )
		{
			newBullet->SetLight(light_,i);

			break;
		}
		if ( i <= LightGroup::cPointLightNum - 1 )
		{
			newBullet->SetLight(light_,-1);
		}
	}

	newBullet->SetChageTime(cDivisionAttackInterval_);

	AttackInterval_ = cDivisionAttackInterval_;

			//弾の登録する
	PlayerBulletManager::Instance()->AddBullet(std::move(newBullet));
}

void Player::BombShoot()
{
	Vector3 velocity = { 0,0,1 };
	velocity = Matrix4Math::transform(velocity,obj_->GetMatWorld());
	velocity.normalize();
	velocity *= kBulletSpeed_ / 2;

	Vector3 pos = obj_->GetPosition() + ( velocity * 5 );

	//弾の生成し、初期化
	std::unique_ptr<PlayerBullet> newBullet = std::make_unique<BombPlayerBullet>();
	newBullet->Initialize(bulletModel_.get(),{ velocity.x,velocity.z },pos,bulletLife_);

	newBullet->SetPhase(PlayerBullet::Phase::Charge);

	newBullet->SetRot(obj_->GetRot());

	for ( uint32_t i = 0; i < LightGroup::cPointLightNum; i++ )
	{
		if ( light_->GetPointActive(i) == false )
		{
			newBullet->SetLight(light_,i);

			break;
		}
		if ( i <= LightGroup::cPointLightNum - 1 )
		{
			newBullet->SetLight(light_,-1);
		}
	}

	newBullet->SetChageTime(cBombAttackInterval_);

	AttackInterval_ = cBombAttackInterval_;

	//弾の登録する
	PlayerBulletManager::Instance()->AddBullet(std::move(newBullet));
}

void Player::Avoid()
{
	if ( isAvoid_ )
	{
		avoidTime_++;
		move_ += avoidVec_ * cAvoidSpeed_;

		obj_->SetRot({ 0,obj_->GetRot().y - float(6.28 / cEndAvoidTime_),0 });

		const uint32_t constlife = 10;
		uint32_t life = constlife;

		Vector3 pos = obj_->GetPosition();

		//追加
		paMan_->Add(life,pos,{ 0,0,0 },{ 0,0,0 },1.0f,0.0f,{ 1,1,1,1 },{ 1,1,1,1 });

		if ( avoidTime_ >= cEndAvoidTime_ )
		{
			avoidTime_ = 0;
			obj_->SetRot(initialRot_);
			isAvoid_ = false;
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

void Player::SpriteDraw()
{
	normalAttack_->Draw();
	threeWayAttack_->Draw();
	divisionAttack_->Draw();
	bombAttack_->Draw();
}

void Player::Reset()
{
	isDelete_ = false;

	isDaed_ = false;

	hp_ = maxHp_;

	ExplosionFrame_ = 0;

	obj_->SetDestruction(0);

	obj_->Setalpha(1);

	PlayerBulletManager::Instance()->Clear();

	objects_.clear();
}

void Player::SetMapData(std::vector<std::unique_ptr<Object3d>>* objects)
{
	assert(objects);
	for ( std::unique_ptr<Object3d>& obj : *objects )
	{
		objects_.push_back(obj.get());
	}
	paMan_->Clear();

	PlayerBulletManager::Instance()->Clear();

	for ( int i = 0; i < LightGroup::cPointLightNum; i++ )
	{
		light_->SetPointActive(i,false);
	}
}

void Player::SetGimmickData(std::vector<Object3d*> objects)
{
	for ( Object3d* obj : objects )
	{
		objects_.push_back(obj);
	}
}

void Player::MapCollision()
{
	bool Ground = false;
	Vector3 pos = obj_->GetPosition() + move_;
	for ( Object3d* MapObj : objects_ )
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

	for ( std::unique_ptr<Enemy>& enemy : EnemyManager::Instance()->GetEnemys() )
	{
		if ( enemy->GetType() == Enemy::EnemyType::Wall && !enemy->IsDaed() )
		{
			Collider::Cube mapCube,objCube;
			mapCube.Pos = enemy->GetObj()->GetPosition();
			mapCube.scale = enemy->GetObj()->GetScale();
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
		}
	}

	if ( Ground == false && onGround_ == false )
	{
		for ( Object3d* MapObj : objects_ )
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

	if ( Ground == false && onGround_ == false )
	{
		for ( Object3d* MapObj : objects_ )
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
	move_ = pos;
}

void Player::EnemyCollision()
{
	Vector3 pos{};
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
					if ( Enemy::EnemyType::Wall == enemy->GetType() )
					{
						if ( bullet->GetType() == PlayerBullet::Type::Bomb )
						{
							bullet->OnCollision();

							enemy->OnCollision();
						}
						else
						{
							bullet->OnCollision();
						}
					}
					else
					{
						bullet->OnCollision();

						enemy->OnCollision();
					}
				}
			}
			if ( isAvoid_ == false && Enemy::EnemyType::Tutorial != enemy->GetType() )
			{
				if ( ( playerCube.Pos.x + playerCube.Pos.y + playerCube.Pos.z / 3 ) - ( enemyCube.Pos.x + enemyCube.Pos.y + enemyCube.Pos.z / 3 ) <= 3
						&& ( playerCube.Pos.x + playerCube.Pos.y + playerCube.Pos.z / 3 ) - ( enemyCube.Pos.x + enemyCube.Pos.y + enemyCube.Pos.z / 3 ) >= -3 )
				{
					if ( Collider::CubeAndCube(enemyCube,playerCube,Collider::Collsion) == true )
					{
						if ( Enemy::EnemyType::Wall != enemy->GetType() )
						{

							hp_--;
						}
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
			hp_--;

			for ( int i = 0; i < 10; i++ )
			{
				//消えるまでの時間
				const uint32_t rnd_life = 10;
				//最低限のライフ
				const uint32_t constlife = 60;
				uint32_t life = ( rand() / RAND_MAX * rnd_life ) + constlife;

				//XYZの広がる距離
				const float rnd_pos = 0.1f;
				pos.x = ( float ) rand() / RAND_MAX * rnd_pos - rnd_pos / 2;
				pos.y = ( ( float ) rand() / RAND_MAX * rnd_pos - rnd_pos / 2 );
				pos.z = ( float ) rand() / RAND_MAX * rnd_pos - rnd_pos / 2;

				//pos.normalize();

				//追加
				paMan_->Add(life,obj_->GetPosition(),pos,{ 0,0,0 },0.5f,0.5f,{ 1,1,1,1 },{ 1,1,1,1 });
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

void Player::SetSpawnPos(Vector3 spawnPosition)
{
	spawnPosition_ = spawnPosition;
}

void Player::RSpawn()
{
	obj_->SetPosition(spawnPosition_);

	obj_->SetRot({ 0,0,0 });

	obj_->Update();

	Object3d::SetTarget(spawnPosition_);

	Object3d::SetEye(spawnPosition_+cameraPos_);
}

void Player::SetCameraMove(Vector3 cameraPos)
{
	isChangCamera_ = true;

	startCameraPos_ = cameraPos_;

	endCameraPos_ = cameraPos;

	cameraChangeFrame_ = 0;


}

void Player::CameraPosChange()
{
	if ( isChangCamera_ )
	{
		cameraChangeFrame_++;

		float f = static_cast< float >( cameraChangeFrame_ ) / static_cast< float >( cEndCameraCgangeFrame_ );

		cameraPos_ = ( endCameraPos_ - startCameraPos_ ) * f;
		cameraPos_ += startCameraPos_;

		if ( cameraChangeFrame_ >= cEndCameraCgangeFrame_ )
		{
			isChangCamera_ = false;
		}

	}
}


