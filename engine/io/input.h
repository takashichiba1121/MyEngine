#pragma once
#define DIRECTINPUT_VERSION 0x0800 //DirectInput�̃o�[�W�����w��
#include<dinput.h>
#include<windows.h>
#include<wrl.h>
#include"WinApp.h"
#include<Xinput.h>
#include<stdint.h>

enum class PadStick
{
	LT,
	RT,
	LX,
	LY,
	RX,
	RY,
};

//����
class Input
{
#pragma region Singleton
private:
	Input() {};

	~Input() {};
public:
	Input(const Input& carManager) = delete;

	Input& operator=(const Input& carManager) = delete;

	static Input* Instance();
#pragma endregion
public:

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public://�����o�֐�

	//������
	void Initialize(WinApp* winApp);
	//�X�V
	void Update();

	/// <summary>
	/// �L�[�̉������`�F�b�N
	/// </summary>
	///<param name="keynumber">�L�[�ԍ�(DIK_0��)</param>
	/// <returns>������Ă��邩</returns>
	bool PushKey(BYTE keyNumber);
	/// <summary>
	/// �L�[�̃g���K�[���`�F�b�N
	/// </summary>
	///<param name="keynumber">�L�[�ԍ�(DIK_0��)</param>
	/// <returns>�g���K�[��</returns>
	bool TriggerKey(BYTE keyNumber);
private://���C���ϐ�
	//�L�[�{�[�h�f�o�C�X
	ComPtr<IDirectInputDevice8> sKeyboard;

	ComPtr<IDirectInput8> sDirectInput;

	BYTE sKey[256];

	BYTE sKeyPre[256];

	//WindowAPI
	WinApp* sWinApp;

public://�����o�֐�(�Q�[���p�b�h)
	/// <summary>
	/// �R���g���[���[�̏󋵂̎擾�i���t���[������j
	/// </summary>
	void Updatekeypad();

	bool IsLinkGamePad();

	XINPUT_STATE GetPad() { return sGamePad; }

	XINPUT_STATE GetOldPad() { return sOldGamePad; }

	/// <summary>
	/// �R���g���[���[�̃{�^���̓��͂����u��
	/// </summary>
	/// ///<param name="button">�{�^���̎��(XINPUT_GAMEPAD_�Ŏn�܂�萔�Ŏw��)</param>
	/// <returns>������Ă邩�H</returns>
	bool PadTriggerKey(uint32_t button);

	/// <summary>
/// �R���g���[���[�̃{�^���̓��͏�
/// </summary>
/// ///<param name="button">�{�^���̎��(XINPUT_GAMEPAD_�Ŏn�܂�萔�Ŏw��)</param>
/// <returns>������Ă邩�H</returns>
	bool PadPushKey(uint32_t button);

	float GetPadStick(PadStick Stick);

	float GetOldPadStick(PadStick Stick);

private://�����o�ϐ�(�Q�[���p�b�h)

	XINPUT_STATE sGamePad;

	XINPUT_STATE sOldGamePad;

	DWORD isLinkGamePad;
};
