#include "Player.h"
#include"input.h"
#include"Collider.h"

void Player::Initialize(Model* model)
{
	obj_ = std::make_unique<Object3d>();

	obj_->Initialize();

	obj_->SetModel(model);

	obj_->SetPosition({5,5,5});

	obj_->SetPolygonExplosion({ 0.0f,1.0f,6.28f,100.0f });
}

void Player::Update()
{
	move = {0,0,0};

	Move();

	Attack();

	obj_->Update();
}

void Player::Move()
{
	Vector3 rot = obj_->GetRot();
	if (Input::IsLinkGamePad())
	{

		move += {Input::GetPadStick(PadStick::LX), 0, Input::GetPadStick(PadStick::LY)};

		rot += {Input::GetPadStick(PadStick::RY), Input::GetPadStick(PadStick::RX), 0};

	}
	else
	{
		if (Input::PushKey(DIK_W))
		{
			move += {0, 0, 0.1f};
		}
		if (Input::PushKey(DIK_A))
		{
			move += {-0.1f, 0, 0};
		}
		if (Input::PushKey(DIK_S))
		{
			move += {0, 0, -0.1f};
		}
		if (Input::PushKey(DIK_D))
		{
			move += {0.1f, 0, 0};
		}
		if (Input::PushKey(DIK_X))
		{
			move += {0, 0.5f, 0};
		}
	}

	if (onGround)
	{
		fallSpeed += fallAcceleration;
		move.y -= fallSpeed;
	}


	 Vector3 pos=MapCollision();

	 obj_->SetPosition(pos);
}

void Player::Attack()
{
}

void Player::Draw()
{
	obj_->Draw();
}

void Player::SetMapData(std::vector<std::unique_ptr<Object3d>>* objects)
{
	assert(objects);

	objects_=objects;
}

Vector3 Player::MapCollision()
{

Vector3 pos= obj_->GetPosition() + move;
	for (const std::unique_ptr<Object3d>& MapObj:*objects_)
	{

		Cube mapCube, objCube;
		mapCube.Pos = MapObj->GetPosition();
		mapCube.scale = MapObj->GetScale();
		objCube.Pos = obj_->GetPosition() + move;
		objCube.oldPos = obj_->GetPosition();
		objCube.scale = obj_->GetScale();
		if (Collider::CubeAndCube(mapCube, objCube) == true)
		{
			if (mapCube.Pos.y - mapCube.scale.y >= objCube.oldPos.y + objCube.scale.y&&onGround)
			{
				pos.y = mapCube.Pos.y - (mapCube.scale.y + objCube.scale.y);

				move.y = 0;

				fallSpeed = 0;
			}
			else if (mapCube.Pos.y + mapCube.scale.y <= objCube.oldPos.y - objCube.scale.y&& onGround)
			{
				pos.y = mapCube.Pos.y + mapCube.scale.y + objCube.scale.y;

				move.y = 0;

				onGround = false;
			}
			else
			{
				
				if (mapCube.Pos.x + mapCube.scale.x <= objCube.oldPos.x - objCube.scale.x)
				{

					pos.x = mapCube.Pos.x + mapCube.scale.x + objCube.scale.x;

					move.x = 0;
				}
				else if (mapCube.Pos.x - mapCube.scale.x >= objCube.oldPos.x + objCube.scale.x)
				{
					pos.x = mapCube.Pos.x - (mapCube.scale.x + objCube.scale.x);

					move.x = 0;
				}
				if (mapCube.Pos.z + mapCube.scale.z <= objCube.oldPos.z - objCube.scale.z)
				{

					pos.z = mapCube.Pos.z + mapCube.scale.z + objCube.scale.z;

					move.z = 0;
				}
				else if (mapCube.Pos.z - mapCube.scale.z >= objCube.oldPos.z + objCube.scale.z)
				{
					pos.z = mapCube.Pos.z - (mapCube.scale.z + objCube.scale.z);

					move.z = 0;
				}
			}
		}
		if (mapCube.Pos.y + mapCube.scale.y >= objCube.Pos.y - objCube.scale.y&&onGround==false)
		{
			onGround = true;

			fallSpeed = 0;
		}
	}
	return pos;
}
