#include "Player.h"
#include"input.h"
#include"Collider.h"
#include<imgui.h>
#include"Texture.h"
#include"Easing.h"
#include<math.h>
#include"EnemyManager.h"


void Player::Initialize(Model* bulletModel)
{
	model.reset(Model::LoadFormOBJ("player",true));

	obj_ = std::make_unique<Object3d>();

	obj_->Initialize();

	obj_->SetModel(model.get());

	obj_->SetPosition({ 5,5,5 });

	obj_->SetPolygonExplosion({ 0.0f,1.0f,6.28f,100.0f });

	paMan_ = std::make_unique<ParticleManager>();

	paMan_->Initialize();

	paMan_->SetTextureHandle(Texture::LoadTexture(L"Resources/effect4.png"));

	obj_->Update();

	bulletModel_ = bulletModel;
}

void Player::Update()
{
	//ÉfÉXÉtÉâÉOÇÃóßÇ¡ÇΩíeÇçÌèú
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
		});

	move_ = { 0,0,0 };

	Move();

	obj_->Update();

	Attack();

	for (std::unique_ptr<PlayerBullet>& bullet:bullets_)
	{
		bullet->Update();
	}

	Object3d::SetTarget(Object3d::GetTarget() + ((obj_->GetPosition() - Object3d::GetTarget()) * cameraSpeed));

	Object3d::SetEye(Object3d::GetTarget() + cameraPos);

	paMan_->Update();

	Cube A, B;

	A.Pos = obj_->GetPosition();

	A.scale = obj_->GetScale();

	B.Pos = goalPosition_;

	B.scale = goalScale_;

	if (Collider::CubeAndCube(A, B))
	{
		isClear = true;
		paMan_->Clear();
	}

	ImGui::Begin("Player");

	ImGui::Text("bullet:%d", bullets_.size());

	ImGui::Text("Goal:%d", isClear);

	ImGui::Text("KnockBack:%d", isKnockBack);

	ImGui::End();
}

void Player::Move()
{
	if (Input::IsLinkGamePad())
	{

		move_ += {Input::GetPadStick(PadStick::LX) / 5, 0, Input::GetPadStick(PadStick::LY) / 5};

		if (Input::PadTriggerKey(XINPUT_GAMEPAD_A)&&onGround_ == false)
		{
			fallSpeed_ = StartJumpSpeed_;
			onGround_ = true;

			for (int i = 0; i < 10; i++)
			{
				//è¡Ç¶ÇÈÇ‹Ç≈ÇÃéûä‘
				const uint32_t rnd_life = 10;
				//ç≈í·å¿ÇÃÉâÉCÉt
				const uint32_t constlife = 60;
				uint32_t life = (rand() / RAND_MAX * rnd_life) + constlife;

				//XYZÇÃçLÇ™ÇÈãóó£
				const float rnd_pos = 0.1f;
				//Yï˚å¸Ç…ÇÕç≈í·å¿ÇÃîÚÇ‘ãóó£
				const float constPosY = 1;
				Vector3 pos{};
				pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2;
				pos.y = ((float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2);
				pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2;

				//pos.normalize();

				//í«â¡
				paMan_->Add(life, obj_->GetPosition(), pos, { 0,0,0 }, 0.5f, 0.5f, { 1,1,1,1 }, { 1,1,1,1 });
			}
		}

	}
	else
	{
		if (Input::PushKey(DIK_W))
		{
			move_ += {0, 0, 0.1f};
		}
		if (Input::PushKey(DIK_A))
		{
			move_ += {-0.1f, 0, 0};
		}
		if (Input::PushKey(DIK_S))
		{
			move_ += {0, 0, -0.1f};
		}
		if (Input::PushKey(DIK_D))
		{
			move_ += {0.1f, 0, 0};
		}
		if (Input::TriggerKey(DIK_SPACE) && onGround_ == false)
		{
			fallSpeed_ = StartJumpSpeed_;
			onGround_ = true;

			for (int i = 0; i < 10; i++)
			{
				//è¡Ç¶ÇÈÇ‹Ç≈ÇÃéûä‘
				const uint32_t rnd_life = 10;
				//ç≈í·å¿ÇÃÉâÉCÉt
				const uint32_t constlife = 60;
				uint32_t life = (rand() / RAND_MAX * rnd_life) + constlife;

				//XYZÇÃçLÇ™ÇÈãóó£
				const float rnd_pos = 0.1f;
				//Yï˚å¸Ç…ÇÕç≈í·å¿ÇÃîÚÇ‘ãóó£
				const float constPosY = 1;
				Vector3 pos{};
				pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2;
				pos.y = ((float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2);
				pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2;

				//pos.normalize();

				//í«â¡
				paMan_->Add(life, obj_->GetPosition(), pos, { 0,0,0 }, 0.5f, 0.5f, { 1,1,1,1 }, { 1,1,1,1 });
			}
		}
	}
	if (obj_->GetPosition().y <= -5)
	{
		obj_->SetPosition(spawnPosition_);
		fallSpeed_ = 0;
	}

	if (onGround_)
	{
		fallSpeed_ += fallAcceleration_;
		move_.y -= fallSpeed_;
	}

	Vector3 cameForward = { 0,0,-1 };

	Vector3  cameRight = {1,0,0};

	Vector3 frontVec = {0,0,0};

	if (move_.x!=0.0f|| move_.z != 0.0f)
	{
		frontVec = cameForward * move_.z + cameRight * move_.x;
	}

	if (frontVec.x != 0.0f|| frontVec.z != 0.0f)
	{
		obj_->SetRot({0, atan2f(frontVec.x, -frontVec.z),0 });
	}

	EnemyCollision();

	move_ = MapCollision();

	obj_->SetPosition(move_);
}

void Player::Attack()
{
	if (Input::IsLinkGamePad())
	{

		if (Input::GetPadStick(PadStick::RT)>=0.5&&Input::GetOldPadStick(PadStick::RT)<0.5)
		{
			//íeÇÃë¨ìx
			const float kBulletSpeed = 0.5f;
			Vector3 velocity(0, 0, kBulletSpeed);
			velocity = Matrix4Math::transform(velocity, obj_->GetMatWorld());
			float len = sqrt(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);
			if (len != 0)
			{
				velocity /= len;
			}
			velocity *= kBulletSpeed;

			//íeÇÃê∂ê¨ÇµÅAèâä˙âª
			std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
			newBullet->Initialize(bulletModel_, { velocity.x,velocity.z }, obj_->GetPosition());

			//íeÇÃìoò^Ç∑ÇÈ
			bullets_.push_back(std::move(newBullet));
		}

	}
	else if (Input::TriggerKey(DIK_Z))
	{
		//íeÇÃë¨ìx
		const float kBulletSpeed = 0.5f;
		Vector3 velocity(0, 0, kBulletSpeed);
		velocity = Matrix4Math::transform(velocity,obj_->GetMatWorld());
		float len = sqrt(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);
		if (len != 0)
		{
			velocity /= len;
		}
		velocity *= kBulletSpeed;

		//íeÇÃê∂ê¨ÇµÅAèâä˙âª
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(bulletModel_, { velocity.x,velocity.z }, obj_->GetPosition());

		//íeÇÃìoò^Ç∑ÇÈ
		bullets_.push_back(std::move(newBullet));
	}
}

void Player::Draw()
{
	obj_->Draw();

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Draw();
	}
}

void Player::ParticleDraw()
{
	paMan_->Draw();
}

void Player::SetMapData(std::vector<std::unique_ptr<Object3d>>* objects)
{
	assert(objects);

	objects_ = objects;

	isClear = false;
}

Vector3 Player::MapCollision()
{
	bool Ground = false;
	Vector3 pos = obj_->GetPosition() + move_;
	for (const std::unique_ptr<Object3d>& MapObj : *objects_)
	{

		Cube mapCube, objCube;
		mapCube.Pos = MapObj->GetPosition();
		mapCube.scale = MapObj->GetScale();
		objCube.Pos = obj_->GetPosition() + move_;
		objCube.oldPos = obj_->GetPosition();
		objCube.scale = obj_->GetScale();
		if (Collider::CubeAndCube(mapCube, objCube) == true)
		{
			if (mapCube.Pos.y - mapCube.scale.y >= objCube.oldPos.y + objCube.scale.y && onGround_)
			{
				pos.y = mapCube.Pos.y - (mapCube.scale.y + objCube.scale.y);

				move_.y = 0;

				fallSpeed_ = 0;
			}
			else if (mapCube.Pos.y + mapCube.scale.y <= objCube.oldPos.y - objCube.scale.y && onGround_)
			{
				pos.y = mapCube.Pos.y + mapCube.scale.y + objCube.scale.y;

				move_.y = 0;

				onGround_ = false;

				Ground = true;
			}
			else
			{

				if (mapCube.Pos.x + mapCube.scale.x <= objCube.oldPos.x - objCube.scale.x)
				{

					pos.x = mapCube.Pos.x + mapCube.scale.x + objCube.scale.x;

					move_.x = 0;
				}
				else if (mapCube.Pos.x - mapCube.scale.x >= objCube.oldPos.x + objCube.scale.x)
				{
					pos.x = mapCube.Pos.x - (mapCube.scale.x + objCube.scale.x);

					move_.x = 0;
				}
				if (mapCube.Pos.z + mapCube.scale.z <= objCube.oldPos.z - objCube.scale.z)
				{

					pos.z = mapCube.Pos.z + mapCube.scale.z + objCube.scale.z;

					move_.z = 0;
				}
				else if (mapCube.Pos.z - mapCube.scale.z >= objCube.oldPos.z + objCube.scale.z)
				{
					pos.z = mapCube.Pos.z - (mapCube.scale.z + objCube.scale.z);

					move_.z = 0;
				}
			}
		}

		for (std::unique_ptr<PlayerBullet>& bullet:bullets_)
		{
			objCube.Pos = bullet->GetPosition();
			objCube.scale = bullet->GetScale();
			if (Collider::CubeAndCube(mapCube, objCube) == true)
			{
				bullet->OnCollision();
			}
		}
	}

	if (Ground == false && onGround_ == false)
	{
		for (const std::unique_ptr<Object3d>& MapObj : *objects_)
		{

			Cube mapCube, objCube;
			mapCube.Pos = MapObj->GetPosition();
			mapCube.scale = MapObj->GetScale();
			objCube.Pos = obj_->GetPosition() + move_;
			objCube.oldPos = obj_->GetPosition();
			objCube.scale = obj_->GetScale();

			if (Collider::QuadAndQuad(mapCube, objCube))
			{
				if (mapCube.Pos.y + mapCube.scale.y <= objCube.Pos.y - objCube.scale.y - fallAcceleration_)
				{
					onGround_ = true;

					Ground = true;
				}
				else
				{
					onGround_ = false;

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
	for (std::unique_ptr<Enemy>& enemy:EnemyManager::GetEnemys())
	{

		Cube enemyCube, playerCube;
		enemyCube.Pos=enemy->GetEnemyObj()->GetPosition();
		enemyCube.scale = enemy->GetEnemyObj()->GetScale();
		playerCube.Pos = obj_->GetPosition() + move_;
		playerCube.scale = obj_->GetScale();
		for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
		{
			Cube bulletCube;

			bulletCube.Pos = bullet->GetPosition();
			bulletCube.scale = bullet->GetScale();
			if (Collider::CubeAndCube(enemyCube, bulletCube) == true)
			{
				bullet->OnCollision();

				enemy->OnCollision();
			}
		}

		if (Collider::CubeAndCube(enemyCube, playerCube) == true)
		{
			isKnockBack = true;
		}
	}

	for (std::unique_ptr<EnemyBullet>& bullet : EnemyManager::GetBullets())
	{
		Cube bulletCube, playerCube;
		bulletCube.Pos =bullet->GetPosition();
		bulletCube.scale = bullet->GetScale();
		playerCube.Pos = obj_->GetPosition() + move_;
		playerCube.scale = obj_->GetScale();

		if (Collider::CubeAndCube(bulletCube, playerCube) == true)
		{
			isKnockBack = true;

			bullet->OnCollision();
		}
	}
}

void Player::SetGoal(Vector3 goalPosition, Vector3 goalScale)
{
	goalPosition_ = goalPosition;

	goalScale_ = goalScale;
}

void Player::SetSpawn(Vector3 spawnPosition)
{
	spawnPosition_ = spawnPosition;

	obj_->SetPosition(spawnPosition_);
	Object3d::SetTarget(spawnPosition_);

	Object3d::SetEye(spawnPosition_ + cameraPos);
}
