#pragma once
#include"Enemy.h"
#include"EnemyBullet.h"
#include"ParticleManager.h"
#include"Player.h"
class EnemyManager
{
#pragma region Singleton
private:
	EnemyManager() {};

	~EnemyManager() {};
public:
	EnemyManager(const EnemyManager& carManager) = delete;

	EnemyManager& operator=(const EnemyManager& carManager) = delete;

	static EnemyManager* Instance();
#pragma endregion

public:

	void Initialize();

	void Update();

	/// <summary>
	/// 入力は受け付けないがアップデートはする(カメラ座標を動かすときなどに使う)
	/// </summary>
	void ObjectUpdate();

	void Draw();

	void ParticleDraw();

	void AddBullet(std::unique_ptr<EnemyBullet> newBullet);

	std::list<std::unique_ptr<Enemy>>& GetEnemys() { return Enemys_; }

	std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	void AddEnemy(std::unique_ptr<Enemy> newEnemy);

	void SetMapData(std::vector<std::unique_ptr<Object3d>>* objects);

	void Fin();

	void Collision();

	void Clear();

	void SetPlayer(Player* player) { player_ = player; }
private:
	std::list<std::unique_ptr<Enemy>> Enemys_;

	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	std::vector<std::unique_ptr<Object3d>>* objects_=nullptr;

	std::unique_ptr<ParticleManager> particle_;

	Player* player_=nullptr;

	float attackRange_ = 15.0f;
};