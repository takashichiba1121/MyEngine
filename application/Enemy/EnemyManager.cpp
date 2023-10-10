#include"EnemyManager.h"
#include"Collider.h"
#include"Texture.h"
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

	particle_->SetTextureHandle(Texture::Instance()->LoadTexture("Resources/effect4.png"));
}

void EnemyManager::Update()
{
	Enemys_.remove_if([](std::unique_ptr<Enemy>& enemy) {
		return enemy->IsDelete();
		});

	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsDead();
		});

	Collision();

	for (std::unique_ptr<Enemy>& enemy : Enemys_)
	{
		Vector3 playerPos,enemyPos, playerScale, enemyScale;

		playerPos = player_->GetObj()->GetPosition();

		enemyPos = enemy->GetObj()->GetPosition();

		playerScale = player_->GetObj()->GetScale();

		enemyScale = enemy->GetObj()->GetScale();

		float distance = static_cast<float>(sqrt(pow(playerPos.x - enemyPos.x, 2) + pow(playerPos.z - enemyPos.z, 2)));

		float playerYmax, playerYmin, enemyYmax, enemyYmin;

		playerYmax = playerPos.y + playerScale.y;

		playerYmin = playerPos.y - playerScale.y;

		enemyYmax = enemyPos.y + enemyScale.y;

		enemyYmin = enemyPos.y - enemyScale.y;

		enemy->SetIsAttack(distance<=25&&(playerYmax>=enemyYmin&& playerYmin <= enemyYmax));

		enemy->Update();
	}

	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Update();
	}

	particle_->Update();
}

void EnemyManager::ParticleDraw()
{
	particle_->Draw();
}

void EnemyManager::ObjectUpdate()
{
	for (std::unique_ptr<Enemy>& enemy : Enemys_)
	{
		enemy->ObjectUpdate();
	}
}

void EnemyManager::Draw()
{
	for (std::unique_ptr<Enemy>& enemy : Enemys_)
	{
		enemy->Draw();
	}

	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
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
	for (const std::unique_ptr<Object3d>& MapObj : *objects_)
	{
		Cube mapCube, objCube;
		mapCube.Pos = MapObj->GetPosition();
		mapCube.scale = MapObj->GetScale();
		for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
		{
			objCube.Pos = bullet->GetPosition();
			objCube.scale = bullet->GetScale();
			if (Collider::CubeAndCube(mapCube, objCube) == true)
			{
				bullet->OnCollision();
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
