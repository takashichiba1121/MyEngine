#pragma once
#include"Enemy.h"
#include"EnemyBullet.h"
#include"ParticleManager.h"
#include"Player.h"
/*
* エネミーの管理
*/
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
	//初期化
	void Initialize();

	//更新
	void Update();

	/// <summary>
	/// 入力は受け付けないがアップデートはする(カメラ座標を動かすときなどに使う)
	/// </summary>
	void ObjectUpdate();

	//モデルの描画
	void Draw();

	//パーティクルの描画
	void ParticleDraw();

	//弾の追加
	void AddBullet(std::unique_ptr<EnemyBullet> newBullet);

	std::list<std::unique_ptr<Enemy>>& GetEnemys() { return Enemys_; }

	std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	//敵の追加
	void AddEnemy(std::unique_ptr<Enemy> newEnemy);

	void SetMapData(std::vector<std::unique_ptr<Object3d>>* objects);

	//終了
	void Fin();

	//衝突チェック
	void Collision();

	//敵と弾の消去
	void Clear();

	void SetPlayer(Player* player) { player_ = player; }

	ParticleManager* GetParticle() {return particle_.get() ;}
private:
	std::list<std::unique_ptr<Enemy>> Enemys_;

	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	std::vector<std::unique_ptr<Object3d>>* objects_=nullptr;

	std::unique_ptr<ParticleManager> particle_;

	Player* player_=nullptr;

	float attackRange_ = 40.0f;

	Sound landingSE_;
};