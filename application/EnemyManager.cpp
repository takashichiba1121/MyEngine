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
	ImGui::Begin("enemy");

	for (std::unique_ptr<Enemy>& enemy : Enemys_)
	{
		if (enemy->IsDaed())
		{
			for (int i = 0; i < 10; i++)
			{
				//Á‚¦‚é‚Ü‚Å‚ÌŽžŠÔ
				const uint32_t rnd_life = 10;
				//Å’áŒÀ‚Ìƒ‰ƒCƒt
				const uint32_t constlife = 60;
				uint32_t life = (rand() / RAND_MAX * rnd_life) + constlife;

				//XYZ‚ÌL‚ª‚é‹——£
				const float rnd_pos = 0.1f;
				//Y•ûŒü‚É‚ÍÅ’áŒÀ‚Ì”ò‚Ô‹——£
				const float constPosY = 1;
				Vector3 pos{};
				pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2;
				pos.y = ((float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2);
				pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2;

				//pos.normalize();

				//’Ç‰Á
				particle_->Add(life, enemy->GetObj()->GetPosition(), pos, { 0,0,0 }, 0.5f, 0.5f, { 1,1,1,1 }, { 1,1,1,1 });
			}
		}
	}

	Enemys_.remove_if([](std::unique_ptr<Enemy>& enemy) {
		return enemy->IsDaed();
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

		ImGui::Text("%d", distance <= 30 && (playerYmax >= enemyYmin && playerYmin <= enemyYmax));
	}

	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Update();
	}

	particle_->Update();

	ImGui::End();
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
