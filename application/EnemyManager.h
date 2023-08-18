#pragma once
#include"Enemy.h"
#include"EnemyBullet .h"
class EnemyManager
{
public:

	static void Update();

	static void Draw();

	static void AddBullet(std::unique_ptr<EnemyBullet> newBullet);

	static std::list<std::unique_ptr<Enemy>>* GetEnemys() { return &Enemys_; }

	static void AddEnemy(std::unique_ptr<Enemy> newEnemy);

	static void Fin();
private:
	static std::list<std::unique_ptr<Enemy>> Enemys_;

	static std::list<std::unique_ptr<EnemyBullet>> bullets_;
};