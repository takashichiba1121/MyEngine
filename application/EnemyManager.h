#pragma once
#include"Enemy.h"
#include"EnemyBullet .h"
class EnemyManager
{
public:

	static void Update();

	static void Draw();

	static void AddBullet(std::unique_ptr<EnemyBullet> newBullet);

	static std::list<std::unique_ptr<Enemy>>& GetEnemys() { return Enemys_; }

	static std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	static void AddEnemy(std::unique_ptr<Enemy> newEnemy);

	static void SetMapData(std::vector<std::unique_ptr<Object3d>>* objects);

	static void Fin();

	static void Collision();
private:
	static std::list<std::unique_ptr<Enemy>> Enemys_;

	static std::list<std::unique_ptr<EnemyBullet>> bullets_;

	static std::vector<std::unique_ptr<Object3d>>* objects_;
};