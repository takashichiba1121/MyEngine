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
	DWORD Updatekeypad(DWORD dwUserIndex);

	/// <summary>
	/// ���A�i���O�X�e�B�b�N��X�����̓��͏�
	/// </summary>
	/// <returns>���A�i���O�X�e�B�b�N��X�����̓��͏󋵂�Ԃ�(-1�`+1)</returns>
	float PadAnalogStickLX();

	/// <summary>
	/// ���A�i���O�X�e�B�b�N��Y�����̓��͏�
	/// </summary>
	/// <returns>���A�i���O�X�e�B�b�N��Y�����̓��͏󋵂�Ԃ�(-1�`+1)</returns>
	float PadAnalogStickLY();

	/// <summary>
	/// �E�A�i���O�X�e�B�b�N��X�����̓��͏�
	/// </summary>
	/// <returns>�E�A�i���O�X�e�B�b�N��X�����̓��͏󋵂�Ԃ�(-1�`+1)</returns>
	float PadAnalogStickRX();

	/// <summary>
	/// �E�A�i���O�X�e�B�b�N��Y�����̓��͏�
	/// </summary>
	/// <returns>�E�A�i���O�X�e�B�b�N��Y�����̓��͏󋵂�Ԃ�(-1�`+1)</returns>
	float PadAnalogStickRY();

	/// <summary>
	/// ���g���K�[(LZ�Ƃ��Ă΂����)�̓��͏�
	/// </summary>
	/// <returns>���g���K�[�̓��͏󋵂�Ԃ�(0�`+1)</returns>
	float PadLTrigger();

	/// <summary>
	/// �E�g���K�[(RZ�Ƃ��Ă΂����)�̓��͏�
	/// </summary>
	/// <returns>���g���K�[�̓��͏󋵂�Ԃ�(0�`+1)</returns>
	float PadRTrigger();

	/// <summary>
	/// �R���g���[���[�̃{�^���̓��͏�
	/// </summary>
	/// ///<param name="button">�{�^���̎��(XINPUT_GAMEPAD_�Ŏn�܂�萔�Ŏw��)</param>
	/// <returns>������Ă邩�H</returns>
	bool PadKey(int button);

private://�����o�ϐ�(�Q�[���p�b�h)

	XINPUT_STATE gamePad;
};
