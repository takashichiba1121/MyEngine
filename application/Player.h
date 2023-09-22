#pragma once
#include"object3d.h"
#include"ParticleManager.h"
#include"PlayerBullet.h"


class Player
{
public:
	/// <summary>
	/// �������S��
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="bulletModel">�e�̃��f��</param>
	void Initialize(Model* bulletModel);
	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �����S�ʂ̏���
	/// </summary>
	void Move();

	/// <summary>
	/// �U���S��
	/// </summary>
	void Attack();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �p�[�e�B�N���̕`��
	/// </summary>
	void ParticleDraw();

	void Reset();

	void SetMapData(std::vector<std::unique_ptr<Object3d>>* objects);

	/// <summary>
	/// json����ǂݎ�����n�`�f�[�^�ƃv���C���[�̓����蔻��
	/// </summary>
	Vector3 MapCollision();

	void EnemyCollision();

	void SetGoal(Vector3 goalPosition, Vector3 goalScale);

	void SetSpawn(Vector3 spawnPosition);

	bool IsClear() { return isClear; }

	Object3d* GetObj() { return obj_.get(); }

	bool IsDaed() { return isDaed_; }
private:
	std::unique_ptr<Model> model;

	Model* bulletModel_;

	std::vector<std::unique_ptr<Object3d>>* objects_;

	std::unique_ptr<Object3d> obj_;

	Vector3 move_;

	float fallSpeed_ = 0;

	const float fallAcceleration_ = 0.03f;

	const float StartJumpSpeed_ = -0.7f;

	bool onGround_;

	std::unique_ptr<ParticleManager> paMan_;

	Vector3 spawnPosition_ = { 0,0,0 };

	Vector3 goalPosition_ = { 0,0,0 };
	
	Vector3 goalScale_ = { 0,0,0 };

	bool isClear = false;

	//�v���C���[���猩���J�����̈ʒu
	const Vector3 cameraPos = { 0.0f,40.0f,-40.0f };

	const float cameraSpeed = 0.1f;

	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	bool isKnockBack = false;

	bool isDaed_ = false;

    const uint32_t maxHp = 1;

	uint32_t hp = maxHp;
};

