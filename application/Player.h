#pragma once
#include"object3d.h"
#include"ParticleManager.h"
#include"PlayerBullet.h"


class Player
{
public:
	void Initialize(Model* model,Model* bulletModel);
	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �����S�ʂ̏���
	/// </summary>
	void Move();

	void Attack();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �p�[�e�B�N���̕`��
	/// </summary>
	void ParticleDraw();

	void SetMapData(std::vector<std::unique_ptr<Object3d>>* objects);

	/// <summary>
/// json����ǂݎ�����n�`�f�[�^�ƃv���C���[�̓����蔻��
/// </summary>
	Vector3 MapCollision();

	void SetGoal(Vector3 goalPosition, Vector3 goalScale);

	void SetSpawn(Vector3 spawnPosition);

	bool GetClear() {return isClear ; }
private:

	Model* bulletModel_;

	std::vector<std::unique_ptr<Object3d>>* objects_;

	std::unique_ptr<Object3d> obj_;

	Vector3 move_;

	float fallSpeed_ =0;

	const float fallAcceleration_ = 0.03f;

	const float StartJumpSpeed_ = -0.5;

	bool onGround_;

	std::unique_ptr<ParticleManager> paMan_;

	Vector3 spawnPosition_ = { 0,0,0 };

	Vector3 goalPosition_ = { 0,0,0 };

	Vector3 goalScale_ = { 0,0,0 };

	bool isClear = false;

	//�v���C���[���猩���J�����̈ʒu
	const Vector3 cameraPos = {0,20.0f,-10.0f};

	const float cameraSpeed=0.1f;

	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	double y;

};

