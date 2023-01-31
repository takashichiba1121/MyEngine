#include "input.h"
#include<cassert>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

Microsoft::WRL::ComPtr<IDirectInputDevice8> Input::keyboard;

Microsoft::WRL::ComPtr<IDirectInput8> Input::directInput;

 BYTE Input::key[256];

 BYTE Input::keyPre[256];

 WinApp* Input::winApp_;

 XINPUT_STATE Input::gamePad;

 XINPUT_STATE Input::oldGamePad;

void Input::Initialize(WinApp* WinApp)
{
	//�؂�Ă���WinApp�̃C���X�^���X���L�^
	winApp_ = WinApp;

	HRESULT result;

	//DirectInput�̏�����
	result = DirectInput8Create(
		WinApp->GetInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr);
	assert(SUCCEEDED(result));
	//�L�[�{�[�h�f�o�C�X�̐���
	/*ComPtr<IDirectInputDevice8> keyboard = nullptr;*/
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));
	//���̓f�[�^�`���̃Z�b�g
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);//�W���`��
	assert(SUCCEEDED(result));
	//�r�����䃌�x���̃Z�b�g
	result = keyboard->SetCooperativeLevel(
		WinApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result)); 
}

void Input::Update()
{
	//�O��̃L�[���͂�ۑ�
	memcpy(keyPre, key, sizeof(key));

	//�L�[�{�[�h���̎擾�J�n
	keyboard->Acquire();
	//�S�L�[�̓��͏�Ԃ��擾����
	keyboard->GetDeviceState(sizeof(key), key);

	Updatekeypad();

	if (abs(gamePad.Gamepad.sThumbLX) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		gamePad.Gamepad.sThumbLX = 0;
	}
	if (abs(gamePad.Gamepad.sThumbLY) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		gamePad.Gamepad.sThumbLY = 0;
	}
	if (abs(gamePad.Gamepad.sThumbRX) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		gamePad.Gamepad.sThumbRX = 0;
	}
	if (abs(gamePad.Gamepad.sThumbRY) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		gamePad.Gamepad.sThumbRY = 0;
	}
}

bool Input::PushKey(BYTE keyNumber)
{
	//�w��L�[�������Ă����true��Ԃ�
	if (key[keyNumber]) {
		return true;
	}
	//�����łȂ����false�Ԃ�
	return false;
}

bool Input::TriggerKey(BYTE keyNumber)
{
	//�w��L�[�������Ă����true��Ԃ�
	if (key[keyNumber]&&keyPre[keyNumber]==0) {
		return true;
	}
	//�����łȂ����false�Ԃ�
	return false;
}

//�Q�[���p�b�h

DWORD Input::Updatekeypad()
{
	oldGamePad = gamePad;

	return XInputGetState(
		0,//�����Ȃ���Ă�Ƃ��̑I��
		&gamePad);//���̕ϐ��ɓ��͏󋵂��i�[�����
}