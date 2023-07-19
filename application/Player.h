#pragma once
#include"object3d.h"


class Player
{
public:
	void Initialize(Model* model);
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

	void SetMapData(std::vector<std::unique_ptr<Object3d>>* objects);

	/// <summary>
/// json����ǂݎ�����n�`�f�[�^�ƃv���C���[�̓����蔻��
/// </summary>
	Vector3 MapCollision();
private:

	std::vector<std::unique_ptr<Object3d>>* objects_;

	std::unique_ptr<Object3d> obj_;

	Vector3 move;

	float fallSpeed =0;

	const float fallAcceleration = 0.03f;

	const float StartJumpSpeed = -0.5;

	bool onGround;
};

