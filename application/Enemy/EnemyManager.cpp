#include"EnemyManager.h"
#include"Collider.h"
#include"TextureManager.h"
#include<math.h>
#include<imgui.h>

EnemyManager* EnemyManager::Instance()
{
	static EnemyManager instance;

	return &instance;
}

void EnemyManager::Initialize()
{
	particle_ = std::make_unique<ParticleManager>();

	particle_->Initialize();

	particle_->SetTextureHandle(TextureManager::Instance()->LoadTexture("Resources/effect4.png"));
}

void EnemyManager::Update()
{


	Enemys_.remove_if([ ] (std::unique_ptr<Enemy>& enemy){
	 return enemy->IsDelete();
		});

	bullets_.remove_if([ ] (std::unique_ptr<EnemyBullet>& bullet){
	 return bullet->IsDead();
		});

	Collision();

	for ( std::unique_ptr<Enemy>& enemy : Enemys_ )
	{
		enemy->Update();
	}

	for ( std::unique_ptr<EnemyBullet>& bullet : bullets_ )
	{
		bullet->Update();
	}

	particle_->Update();
#ifdef _DEBUG
	ImGui::Begin("Eneny");

	ImGui::Text("bullet%d",bullets_.size());

	ImGui::End();
#endif
}

void EnemyManager::ParticleDraw()
{
	particle_->Draw();
}

void EnemyManager::ObjectUpdate()
{
	for ( std::unique_ptr<Enemy>& enemy : Enemys_ )
	{
		enemy->ObjectUpdate();
	}
}

void EnemyManager::Draw()
{
	for ( std::unique_ptr<Enemy>& enemy : Enemys_ )
	{
		enemy->Draw();
	}
	for ( std::unique_ptr<EnemyBullet>& bullet : bullets_ )
	{
		bullet->Draw();
	}
}

void EnemyManager::AddBullet(std::unique_ptr<EnemyBullet> newBullet)
{
	bullets_.push_back(std::move(newBullet));
}

void EnemyManager::AddEnemy(std::unique_ptr<Enemy> newEnemy)
{
	Enemys_.push_back(std::move(newEnemy));
}

void EnemyManager::Fin()
{

	bullets_.clear();

	Enemys_.clear();

	particle_ = nullptr;
}

void EnemyManager::SetMapData(std::vector<std::unique_ptr<Object3d>>* objects)
{
	assert(objects);

	objects_ = objects;
}

void EnemyManager::Collision()
{
	for ( const std::unique_ptr<Object3d>& MapObj : *objects_ )
	{
		Collider::Cube mapCube,objCube;
		mapCube.Pos = MapObj->GetPosition();
		mapCube.scale = MapObj->GetScale();

		for ( std::unique_ptr<Enemy>& enemy : Enemys_ )
		{
			if ( enemy->IsAttack()||enemy->IsMove())
			{
				objCube.Pos = enemy->GetObj()->GetPosition();
				objCube.scale = enemy->GetObj()->GetScale();

				Vector3 pos = enemy->GetObj()->GetPosition();

				float mapYmax,enemyYmin;

				mapYmax = mapCube.Pos.y + mapCube.scale.y;

				enemyYmin = objCube.Pos.y - objCube.scale.y-1;
				if ( Collider::QuadAndQuad(mapCube,objCube,Collider::Type::Inside)==false&&
					Collider::QuadAndQuad(mapCube,objCube,Collider::Type::Collsion)&&
					(mapYmax>=enemyYmin))
				{
					if ( mapCube.Pos.x + mapCube.scale.x < objCube.Pos.x + objCube.scale.x )
					{
						pos.x = mapCube.Pos.x + mapCube.scale.x - objCube.scale.x;

						enemy->AttackOff();
					}
					else if ( mapCube.Pos.x - mapCube.scale.x > objCube.Pos.x - objCube.scale.x )
					{
						pos.x = mapCube.Pos.x - mapCube.scale.x + objCube.scale.x;

						enemy->AttackOff();
					}
					if ( mapCube.Pos.z + mapCube.scale.z < objCube.Pos.z + objCube.scale.z )
					{
						pos.z = mapCube.Pos.z + mapCube.scale.z - objCube.scale.z;

						enemy->AttackOff();
					}
					else if ( mapCube.Pos.z - mapCube.scale.z > objCube.Pos.z - objCube.scale.z )
					{
						pos.z = mapCube.Pos.z - mapCube.scale.z + objCube.scale.z;

						enemy->AttackOff();
					}

					enemy->GetObj()->SetPosition(pos);
				}
			}
		}
		for ( std::unique_ptr<EnemyBullet>& bullet : bullets_ )
		{
			objCube.Pos = bullet->GetPosition();
			objCube.scale = bullet->GetScale();
			if ( Collider::CubeAndCube(mapCube,objCube,Collider::Collsion) == true )
			{
				bullet->OnCollision();
			}
		}
	}
	for ( std::unique_ptr<Enemy>& enemy1 : Enemys_ )
	{
		if (enemy1->IsAttack()||enemy1->IsMove()&&enemy1->GetType()!=Enemy::EnemyType::Gun )
		{
			Collider::Sphere A;
			A.Pos = enemy1->GetObj()->GetPosition();
			A.scale = 1;

			for ( std::unique_ptr<Enemy>& enemy2 : Enemys_ )
			{
				if (enemy1->GetNumber()!=enemy2->GetNumber() )
				{
					Collider::Sphere B;
					B.Pos = enemy2->GetObj()->GetPosition();
					B.scale = 1;
					Vector3 reject = {0,0,0};

					if ( Collider::SphereAndSphere(A,B,Collider::Type::Collsion,&reject) )
					{

						enemy1->OnEnemyCollision(reject);
					}
				}
			}
		}
	}
}

void EnemyManager::Clear()
{
	Enemys_.clear();

	bullets_.clear();

	particle_->Clear();
}
