#include"EnemyManager.h"

 std::list<std::unique_ptr<Enemy>> EnemyManager::Enemys_;

std::list<std::unique_ptr<EnemyBullet>> EnemyManager::bullets_;

void EnemyManager::Update()
{
	Enemys_.remove_if([](std::unique_ptr<Enemy>& enemy) {
		return enemy->IsDaed();
		});

	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsDead();
		});

	for (std::unique_ptr<Enemy>& enemy : Enemys_)
	{
		enemy->Update();
	}

	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Update();
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
}
