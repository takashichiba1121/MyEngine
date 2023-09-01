#pragma once
#include"Enemy.h"
#include"EnemyBullet .h"
#include"ParticleManager.h"
#include"Player.h"
class EnemyManager
{
public:

	static void Initialize();

	static void Update();

	static void Draw();

	static void ParticleDraw();

	static void AddBullet(std::unique_ptr<EnemyBullet> newBullet);

	static std::list<std::unique_ptr<Enemy>>& GetEnemys() { return Enemys_; }

	static std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	static void AddEnemy(std::unique_ptr<Enemy> newEnemy);

	static void SetMapData(std::vector<std::unique_ptr<Object3d>>* objects);

	static void Fin();

	static void Collision();

	static void Clear();

	static void SetPlayer(Player* player) {player_=player ; }
private:
	static std::list<std::unique_ptr<Enemy>> Enemys_;

	static std::list<std::unique_ptr<EnemyBullet>> bullets_;

	static std::vector<std::unique_ptr<Object3d>>* objects_;

	static std::unique_ptr<ParticleManager> particle_;

	static Player* player_;
};