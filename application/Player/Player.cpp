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

	paMan_->SetTextureHandle(TextureManager::Instance()->LoadTexture("Resources/effect4.png"));

	obj_->Update();

	bulletModel_.reset(Model::LoadFormOBJ("playerBullet",true));

	PlayerBulletManager::Instance()->Initialize();

	normalAttack = std::make_unique<Sprite>();

	normalAttack->Initialize(TextureManager::Instance()->LoadTexture("Resources/normalAttack.png"));

	normalAttack->SetPosition({148,158});

	normalAttack->SetScale({138,138});

	normalAttack->SetColor({ 1,1,1,1 });

	normalAttack->SetAnchorPoint({0.5f,1});

	normalAttack->Update();

	threeWayAttack = std::make_unique<Sprite>();

	threeWayAttack->Initialize(TextureManager::Instance()->LoadTexture("Resources/3WayAttack.png"));

	threeWayAttack->SetPosition({ 148,158 });

	threeWayAttack->SetScale({ 128,128 });

	threeWayAttack->SetColor({1,1,1,0.5f});

	threeWayAttack->SetAnchorPoint({ 1,0.5f });

	threeWayAttack->Update();

	divisionAttack = std::make_unique<Sprite>();

	divisionAttack->Initialize(TextureManager::Instance()->LoadTexture("Resources/DivisionAttack.png"));

	divisionAttack->SetPosition({ 148,158 });

	divisionAttack->SetColor({ 1,1,1,0.5f });

	divisionAttack->SetAnchorPoint({ 0,0.5f });

	divisionAttack->Update();

	bombAttack = std::make_unique<Sprite>();

	bombAttack->Initialize(TextureManager::Instance()->LoadTexture("Resources/BombAttack.png"));

	bombAttack->SetPosition({ 148,158 });

	bombAttack->SetAnchorPoint({ 0.5f,0 });

	bombAttack->SetColor({ 1,1,1,0.5f });

	bombAttack->Update();

	attackSE_.Load("Resources/Sound/PlayerAttack.wav");

	jampSE_.Load("Resources/Sound/Jamp.wav");

	avoidSE_.Load("Resources/Sound/PlayerAvoid.wav");

	deadSE_.Load("Resources/Sound/Dead.wav");

	landingSE_.Load("Resources/Sound/Landing.wav");
}

void Player::Update()
{

	move_ = { 0,0,0 };
	if ( isDaed_ == false )
	{
		Move();
		if ( hp_ <= 0 )
		{
			isDaed_ = true;
			deadSE_.Play(false,0.3f);
		}

		if ( Input::Instance()->PadTriggerKey(XINPUT_GAMEPAD_DPAD_UP) )
		{
			normalAttack->SetScale({ 138,138 });

			normalAttack->SetColor({ 1,1,1,1 });

			threeWayAttack->SetScale({ 128,128 });

			threeWayAttack->SetColor({ 1,1,1,0.5f });

			divisionAttack->SetScale({ 128,128 });

			divisionAttack->SetColor({ 1,1,1,0.5f });

			bombAttack->SetScale({ 128,128 });

			bombAttack->SetColor({ 1,1,1,0.5f });

			attackType = AttackType::Normal;
		}
		else if ( Input::Instance()->PadTriggerKey(XINPUT_GAMEPAD_DPAD_LEFT) )
		{
			normalAttack->SetScale({ 128,128 });

			normalAttack->SetColor({ 1,1,1,0.5f });

			threeWayAttack->SetScale({ 138,138 });

			threeWayAttack->SetColor({ 1,1,1,1 });

			divisionAttack->SetScale({ 128,128 });

			divisionAttack->SetColor({ 1,1,1,0.5f });

			bombAttack->SetScale({ 128,128 });

			bombAttack->SetColor({ 1,1,1,0.5f });

			attackType = AttackType::ThreeWay;
		}
		else if ( Input::Instance()->PadTriggerKey(XINPUT_GAMEPAD_DPAD_RIGHT) )
		{
			normalAttack->SetScale({ 128,128 });

			normalAttack->SetColor({ 1,1,1,0.5f });

			threeWayAttack->SetScale({ 128,128 });

			threeWayAttack->SetColor({ 1,1,1,0.5f });

			divisionAttack->SetScale({ 138,138 });

			divisionAttack->SetColor({ 1,1,1,1 });

			bombAttack->SetScale({ 128,128 });

			bombAttack->SetColor({ 1,1,1,0.5f });

			attackType = AttackType::Division;
		}
		else if ( Input::Instance()->PadTriggerKey(XINPUT_GAMEPAD_DPAD_DOWN) )
		{
			normalAttack->SetScale({ 128,128 });

			normalAttack->SetColor({ 1,1,1,0.5f });

			threeWayAttack->SetScale({ 128,128 });

			threeWayAttack->SetColor({ 1,1,1,0.5f });

			divisionAttack->SetScale({ 128,128 });

			divisionAttack->SetColor({ 1,1,1,0.5f });

			bombAttack->SetScale({ 138,138 });

			bombAttack->SetColor({ 1,1,1,1 });

			attackType = AttackType::Bomb;
		}

		EnemyCollision();

		move_ = MapCollision();

		obj_->SetPosition(move_);

		Object3d::SetTarget(Object3d::GetTarget() + ( ( obj_->GetPosition() - Object3d::GetTarget() ) * cameraSpeed_ ));

		Object3d::SetEye(Object3d::GetTarget() + cameraPos_);
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

	PlayerBulletManager::Instance()->Update();

	obj_->Update();

	paMan_->Update();

	normalAttack->Update();
	threeWayAttack->Update();
	divisionAttack->Update();
	bombAttack->Update();

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
	if ( AttackInterval_ == 0 )
	{
		if ( isAvoid_ == false )
		{
			if ( Input::Instance()->IsLinkGamePad() )
			{

				move_ += {Input::Instance()->GetPadStick(PadStick::LX),0,Input::Instance()->GetPadStick(PadStick::LY)};

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
					jampSE_.Play(false,0.1f);
				}

			}
			else
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

					jampSE_.Play(false,0.1f);
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
		move_ = move_.normalize() * moveSpeed_;
		if ( avoidInterval_ == 0 )
		{
			if ( isAvoid_ == false && Input::Instance()->TriggerKey(DIK_Q) || Input::Instance()->PadTriggerKey(XINPUT_GAMEPAD_X) )
			{
				isAvoid_ = true;

				avoidInterval_ = maxAvoidInterval_;

				initialRot_ = obj_->GetRot();

				Vector3 velocity(0,0,1);
				avoidVec_ = Matrix4Math::transform(velocity,obj_->GetMatWorld());
				avoidVec_.normalize();

				avoidSE_.Play(false,0.3f);
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
	if ( Input::Instance()->IsLinkGamePad() )
	{

		if ( Input::Instance()->GetPadStick(PadStick::RT) >= 0.5 && Input::Instance()->GetOldPadStick(PadStick::RT) < 0.5 )
		{
			std::unique_ptr<PlayerBullet> newBullet[3];
			Vector3 pos[3];
			Vector3 velocity[ 3 ]
			{
				{0,0,1},
				{0.3f,0,1},
				{-0.3f,0,1},
			};

			switch ( attackType )
			{
			case Player::AttackType::Normal:

				velocity[0] = { 0,0,1 };
				velocity[0] = Matrix4Math::transform(velocity[0],obj_->GetMatWorld());
				velocity[0].normalize();
				velocity[0] *= kBulletSpeed_;

				pos[0] = obj_->GetPosition() + ( velocity[ 0 ] * 5 );

				//弾の生成し、初期化
				newBullet[0] = std::make_unique<PlayerBullet>();
				newBullet[0]->Initialize(bulletModel_.get(),{ velocity[0].x,velocity[0].z },pos[0],bulletLife_);

				newBullet[0]->SetPhase(PlayerBullet::Phase::Charge);

				newBullet[0]->SetRot(obj_->GetRot());

				for ( uint32_t i = 0; i < LightGroup::cPointLightNum; i++ )
				{
					if ( light_->GetPointActive(i) == false )
					{
						newBullet[0]->SetLight(light_,i);

						break;
					}
					if ( i <= 19 )
					{
						newBullet[0]->SetLight(light_,-1);
					}
				}

				newBullet[0]->SetChageTime(normalAttackInterval_);

				AttackInterval_ = normalAttackInterval_;

				//弾の登録する
				PlayerBulletManager::Instance()->AddBullet(std::move(newBullet[0]));

				attackSE_.Play(false,0.3f);
				break;
			case Player::AttackType::ThreeWay:
				for ( int i = 0; i < 3; i++ )
				{
					velocity[ i ] = Matrix4Math::transform(velocity[ i ],obj_->GetMatWorld());
					velocity[ i ].normalize();
					velocity[ i ] *= kBulletSpeed_;

					newBullet[ i ] = std::make_unique<PlayerBullet>();

					pos[i] = obj_->GetPosition() + ( velocity[ 0 ] * 5 );

					newBullet[ i ]->Initialize(bulletModel_.get(),{ velocity[ i ].x,velocity[ i ].z },pos[i],bulletLife_);

					newBullet[ i ]->SetPhase(PlayerBullet::Phase::Charge);

					newBullet[ i ]->SetChageTime(threeWayAttackInterval_);

					for ( int j = 0; j < LightGroup::cPointLightNum; j++ )
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
				AttackInterval_ = threeWayAttackInterval_;

				attackSE_.Play(false,0.3f);
				break;
			case Player::AttackType::Division:
				velocity[ 0 ] = { 0,0,1 };
				velocity[ 0 ] = Matrix4Math::transform(velocity[ 0 ],obj_->GetMatWorld());
				velocity[ 0 ].normalize();
				velocity[ 0 ] *= kBulletSpeed_;

				pos[ 0 ] = obj_->GetPosition() + ( velocity[ 0 ] * 5 );

				pos[ 0 ].y += 2.5f;

				//弾の生成し、初期化
				newBullet[ 0 ] = std::make_unique<DivisionPlayerBullet>();
				newBullet[ 0 ]->Initialize(bulletModel_.get(),{ velocity[ 0 ].x,velocity[ 0 ].z },pos[ 0 ],bulletLife_);

				newBullet[ 0 ]->SetPhase(PlayerBullet::Phase::Charge);

				newBullet[ 0 ]->SetRot(obj_->GetRot());

				for ( uint32_t i = 0; i < LightGroup::cPointLightNum; i++ )
				{
					if ( light_->GetPointActive(i) == false )
					{
						newBullet[ 0 ]->SetLight(light_,i);

						break;
					}
					if ( i <= 19 )
					{
						newBullet[ 0 ]->SetLight(light_,-1);
					}
				}

				newBullet[ 0 ]->SetChageTime(divisionAttackInterval_);

				AttackInterval_ = divisionAttackInterval_;

				//弾の登録する
				PlayerBulletManager::Instance()->AddBullet(std::move(newBullet[ 0 ]));

				attackSE_.Play(false,0.3f);
				break;
			case Player::AttackType::Bomb:

				velocity[ 0 ] = { 0,0,1 };
				velocity[ 0 ] = Matrix4Math::transform(velocity[ 0 ],obj_->GetMatWorld());
				velocity[ 0 ].normalize();
				velocity[ 0 ] *= kBulletSpeed_;

				pos[ 0 ] = obj_->GetPosition() + ( velocity[ 0 ] * 5 );

				//弾の生成し、初期化
				newBullet[ 0 ] = std::make_unique<BombPlayerBullet>();
				newBullet[ 0 ]->Initialize(bulletModel_.get(),{ velocity[ 0 ].x,velocity[ 0 ].z },pos[ 0 ],bulletLife_);

				newBullet[ 0 ]->SetPhase(PlayerBullet::Phase::Charge);

				newBullet[ 0 ]->SetRot(obj_->GetRot());

				for ( uint32_t i = 0; i < LightGroup::cPointLightNum; i++ )
				{
					if ( light_->GetPointActive(i) == false )
					{
						newBullet[ 0 ]->SetLight(light_,i);

						break;
					}
					if ( i <= 19 )
					{
						newBullet[ 0 ]->SetLight(light_,-1);
					}
				}

				newBullet[ 0 ]->SetChageTime(bombAttackInterval_);

				AttackInterval_ = bombAttackInterval_;

				//弾の登録する
				PlayerBulletManager::Instance()->AddBullet(std::move(newBullet[ 0 ]));

				attackSE_.Play(false,0.3f);
				break;
			}

		}
	}
	else
	{
		if ( Input::Instance()->TriggerKey(DIK_Z) )
		{
			std::unique_ptr<PlayerBullet> newBullet[ 3 ];
			Vector3 pos[ 3 ];
			Vector3 velocity[ 3 ]
			{
				{0,0,1},
				{0.3f,0,1},
				{-0.3f,0,1},
			};

			switch ( attackType )
			{
			case Player::AttackType::Normal:

				velocity[ 0 ] = { 0,0,1 };
				velocity[ 0 ] = Matrix4Math::transform(velocity[ 0 ],obj_->GetMatWorld());
				velocity[ 0 ].normalize();
				velocity[ 0 ] *= kBulletSpeed_;

				pos[ 0 ] = obj_->GetPosition() + ( velocity[ 0 ] * 5 );

				//弾の生成し、初期化
				newBullet[ 0 ] = std::make_unique<PlayerBullet>();
				newBullet[ 0 ]->Initialize(bulletModel_.get(),{ velocity[ 0 ].x,velocity[ 0 ].z },pos[ 0 ],bulletLife_);

				newBullet[ 0 ]->SetPhase(PlayerBullet::Phase::Charge);

				newBullet[ 0 ]->SetRot(obj_->GetRot());

				for ( uint32_t i = 0; i < LightGroup::cPointLightNum; i++ )
				{
					if ( light_->GetPointActive(i) == false )
					{
						newBullet[ 0 ]->SetLight(light_,i);

						break;
					}
					if ( i <= 19 )
					{
						newBullet[ 0 ]->SetLight(light_,-1);
					}
				}

				newBullet[ 0 ]->SetChageTime(normalAttackInterval_);

				AttackInterval_ = normalAttackInterval_;

				//弾の登録する
				PlayerBulletManager::Instance()->AddBullet(std::move(newBullet[ 0 ]));

				attackSE_.Play(false,0.3f);
				break;
			case Player::AttackType::ThreeWay:
				for ( int i = 0; i < 3; i++ )
				{
					velocity[ i ] = Matrix4Math::transform(velocity[ i ],obj_->GetMatWorld());
					velocity[ i ].normalize();
					velocity[ i ] *= kBulletSpeed_;

					newBullet[ i ] = std::make_unique<PlayerBullet>();

					pos[ i ] = obj_->GetPosition() + ( velocity[ 0 ] * 5 );

					newBullet[ i ]->Initialize(bulletModel_.get(),{ velocity[ i ].x,velocity[ i ].z },pos[ i ],bulletLife_);

					newBullet[ i ]->SetPhase(PlayerBullet::Phase::Charge);

					newBullet[ i ]->SetChageTime(threeWayAttackInterval_);

					for ( int j = 0; j < LightGroup::cPointLightNum; j++ )
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
				AttackInterval_ = threeWayAttackInterval_;

				attackSE_.Play(false,0.3f);
				break;
			case Player::AttackType::Division:
				velocity[ 0 ] = { 0,0,1 };
				velocity[ 0 ] = Matrix4Math::transform(velocity[ 0 ],obj_->GetMatWorld());
				velocity[ 0 ].normalize();
				velocity[ 0 ] *= kBulletSpeed_;

				pos[ 0 ] = obj_->GetPosition() + ( velocity[ 0 ] * 5 );

				pos[ 0 ].y += 2.5f;

				//弾の生成し、初期化
				newBullet[ 0 ] = std::make_unique<DivisionPlayerBullet>();
				newBullet[ 0 ]->Initialize(bulletModel_.get(),{ velocity[ 0 ].x,velocity[ 0 ].z },pos[ 0 ],bulletLife_);

				newBullet[ 0 ]->SetPhase(PlayerBullet::Phase::Charge);

				newBullet[ 0 ]->SetRot(obj_->GetRot());

				for ( uint32_t i = 0; i < LightGroup::cPointLightNum; i++ )
				{
					if ( light_->GetPointActive(i) == false )
					{
						newBullet[ 0 ]->SetLight(light_,i);

						break;
					}
					if ( i <= 19 )
					{
						newBullet[ 0 ]->SetLight(light_,-1);
					}
				}

				newBullet[ 0 ]->SetChageTime(divisionAttackInterval_);

				AttackInterval_ = divisionAttackInterval_;

				//弾の登録する
				PlayerBulletManager::Instance()->AddBullet(std::move(newBullet[ 0 ]));

				attackSE_.Play(false,0.3f);
				break;
			case Player::AttackType::Bomb:

				velocity[ 0 ] = { 0,0,1 };
				velocity[ 0 ] = Matrix4Math::transform(velocity[ 0 ],obj_->GetMatWorld());
				velocity[ 0 ].normalize();
				velocity[ 0 ] *= kBulletSpeed_;

				pos[ 0 ] = obj_->GetPosition() + ( velocity[ 0 ] * 5 );

				//弾の生成し、初期化
				newBullet[ 0 ] = std::make_unique<BombPlayerBullet>();
				newBullet[ 0 ]->Initialize(bulletModel_.get(),{ velocity[ 0 ].x,velocity[ 0 ].z },pos[ 0 ],bulletLife_);

				newBullet[ 0 ]->SetPhase(PlayerBullet::Phase::Charge);

				newBullet[ 0 ]->SetRot(obj_->GetRot());

				for ( uint32_t i = 0; i < LightGroup::cPointLightNum; i++ )
				{
					if ( light_->GetPointActive(i) == false )
					{
						newBullet[ 0 ]->SetLight(light_,i);

						break;
					}
					if ( i <= 19 )
					{
						newBullet[ 0 ]->SetLight(light_,-1);
					}
				}

				newBullet[ 0 ]->SetChageTime(bombAttackInterval_);

				AttackInterval_ = bombAttackInterval_;

				//弾の登録する
				PlayerBulletManager::Instance()->AddBullet(std::move(newBullet[ 0 ]));

				attackSE_.Play(false,0.3f);
				break;
			}
		}
	}
}

void Player::Avoid()
{
	if ( isAvoid_ )
	{
		avoidTime_++;
		move_ += avoidVec_ * avoidSpeed_;

		obj_->SetRot({ 0,obj_->GetRot().y - 0.314f,0 });

		const uint32_t constlife = 10;
		uint32_t life = constlife;

		//XYZの広がる距離
		Vector3 verocity{};
		verocity.x = 0;
		verocity.y = -0;
		verocity.z = 0;

		Vector3 pos = obj_->GetPosition();

		//追加
		paMan_->Add(life,pos,verocity,{ 0,0,0 },1.0f,0.0f,{ 1,1,1,1 },{ 1,1,1,1 });

		if ( avoidTime_ >= 20 )
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
	normalAttack->Draw();
	threeWayAttack->Draw();
	divisionAttack->Draw();
	bombAttack->Draw();
}

void Player::Reset()
{
	isDelete_ = false;

	isDaed_ = false;

	hp_ = maxHp_;

	ExplosionFrame_=0;

	obj_->SetDestruction(0);

	obj_->Setalpha(1);

	PlayerBulletManager::Instance()->Clear();
}

void Player::SetMapData(std::vector<std::unique_ptr<Object3d>>* objects)
{
	assert(objects);

	objects_ = objects;

	paMan_->Clear();

	PlayerBulletManager::Instance()->Clear();

	for ( int i = 0; i < LightGroup::cPointLightNum; i++ )
	{
		light_->SetPointActive(i,false);
	}
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

				landingSE_.Play(false,0.3f);
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
				if ( ( bulletCube.Pos.x + bulletCube.Pos.y + bulletCube.Pos.z / 3 ) - ( enemyCube.Pos.x + enemyCube.Pos.y + enemyCube.Pos.z / 3 ) <= 3
					&& ( bulletCube.Pos.x + bulletCube.Pos.y + bulletCube.Pos.z / 3 ) - ( enemyCube.Pos.x + enemyCube.Pos.y + enemyCube.Pos.z / 3 ) >= -3 )
				{
					if ( Collider::CubeAndCube(enemyCube,bulletCube,Collider::Collsion) == true )
					{
						bullet->OnCollision();

						enemy->OnCollision();

						landingSE_.Play(false,0.3f);

						deadSE_.Play(false,0.3f);
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
				Vector3 pos{};
				pos.x = ( float ) rand() / RAND_MAX * rnd_pos - rnd_pos / 2;
				pos.y = ( ( float ) rand() / RAND_MAX * rnd_pos - rnd_pos / 2 );
				pos.z = ( float ) rand() / RAND_MAX * rnd_pos - rnd_pos / 2;

				//pos.normalize();

				//追加
				paMan_->Add(life,obj_->GetPosition(),pos,{ 0,0,0 },0.5f,0.5f,{ 1,1,1,1 },{ 1,1,1,1 });
			}

			bullet->OnCollision();

			landingSE_.Play(false,0.3f);
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
