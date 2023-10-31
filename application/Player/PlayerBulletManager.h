#pragma once
#include"Player.h"
#include"PlayerBullet.h"
class PlayerBulletManager
{
#pragma region Singleton
private:
	PlayerBulletManager() {};

	~PlayerBulletManager() {};
public:
	PlayerBulletManager(const PlayerBulletManager& carManager) = delete;

	PlayerBulletManager& operator=(const PlayerBulletManager& carManager) = delete;

	static PlayerBulletManager* Instance();
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

	void AddBullet(std::unique_ptr<PlayerBullet> newBullet);

	ParticleManager* GetParticle() {return particle_.get();}

	std::list<std::unique_ptr<PlayerBullet>>& GetBullets() {
		return bullets_;
	}

	void Fin();

	void Clear();
private:
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	std::unique_ptr<ParticleManager> particle_;
};

