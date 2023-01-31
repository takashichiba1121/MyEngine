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
	static ComPtr<IDirectInputDevice8> keyboard;

	static ComPtr<IDirectInput8> directInput;

	static BYTE key[256];

	static BYTE keyPre[256];

	//WindowAPI
	static WinApp* winApp_;

public://�����o�֐�(�Q�[���p�b�h)
	/// <summary>
	/// �R���g���[���[�̏󋵂̎擾�i���t���[������j
	/// </summary>
	///<param name="dwUserIndex">�Ȃ���Ă�R���g���[���[�������̏ꍇ�̎w��i��Ȃ�0��OK�j</param>
	/// <returns>�������H</returns>
	static DWORD Updatekeypad();

	static XINPUT_STATE GetPad() { return gamePad; }

	static XINPUT_STATE OldGetPad() {return oldGamePad; }

private://�����o�ϐ�(�Q�[���p�b�h)

	static XINPUT_STATE gamePad;

	static XINPUT_STATE oldGamePad;
};
