#pragma once
#define DIRECTINPUT_VERSION 0x0800 //DirectInput�̃o�[�W�����w��
#include<dinput.h>
#include<windows.h>
#include<wrl.h>
#include"WinApp.h"
#include<Xinput.h>

#pragma comment (lib, "xinput.lib")

//����
class Input
{
public:

	template <class T> using ComPtr= Microsoft::WRL::ComPtr<T>;

public://�����o�֐�

	//������
	static void Initialize(WinApp* winApp);
	//�X�V
	static void Update();

	/// <summary>
	/// �L�[�̉������`�F�b�N
	/// </summary>
	///<param name="keynumber">�L�[�ԍ�(DIK_0��)</param>
	/// <returns>������Ă��邩</returns>
	static bool PushKey(BYTE keyNumber);
	/// <summary>
	/// �L�[�̃g���K�[���`�F�b�N
	/// </summary>
	///<param name="keynumber">�L�[�ԍ�(DIK_0��)</param>
	/// <returns>�g���K�[��</returns>
	static bool TriggerKey(BYTE keyNumber);
private://���C���ϐ�
	//�L�[�{�[�h�f�o�C�X
	static ComPtr<IDirectInputDevice8> sKeyboard;

	static ComPtr<IDirectInput8> sDirectInput;

	static BYTE sKey[256];

	static BYTE sKeyPre[256];

	//WindowAPI
	static WinApp* sWinApp;

public://�����o�֐�(�Q�[���p�b�h)
	/// <summary>
	/// �R���g���[���[�̏󋵂̎擾�i���t���[������j
	/// </summary>
	/// <returns>�������H</returns>
	static DWORD Updatekeypad();

	static XINPUT_STATE GetPad() { return sGamePad; }

	static XINPUT_STATE OldGetPad() {return sOldGamePad; }

private://�����o�ϐ�(�Q�[���p�b�h)

	static XINPUT_STATE sGamePad;

	static XINPUT_STATE sOldGamePad;
};
