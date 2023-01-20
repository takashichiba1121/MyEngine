#pragma once
#include"Model.h"
#include"object3d.h"
#include"SpriteCommon.h"
#include"Sprite.h"
#include"input.h"
#include"Texture.h"
class GameScene
{
public:
public: // �����o�֐�
	/// <summary>
	/// �R���X�g�N���^
	/// </summary>
	GameScene();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~GameScene();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(GameScene* gameScene);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void ObjectDraw();

	/// <summary>
	/// �`��
	/// </summary>
	void SpriteDraw();

private:
	Model* model;

	Object3d* obj = new Object3d;

	Model* model2;

	Object3d* obj2 = new Object3d;
};

