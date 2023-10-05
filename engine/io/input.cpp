#include "input.h"
#include<cassert>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment (lib, "xinput.lib")

Input* Input::Instance()
{
	static Input instance;

	return &instance;
}

void Input::Initialize(WinApp* WinApp)
{
	//�؂�Ă���WinApp�̃C���X�^���X���L�^
	winApp = WinApp;

	HRESULT result;

	//DirectInput�̏�����
	result = DirectInput8Create(
		WinApp->GetInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&sDirectInput, nullptr);
	assert(SUCCEEDED(result));
	//�L�[�{�[�h�f�o�C�X�̐���
	/*ComPtr<IDirectInputDevice8> keyboard = nullptr;*/
	result = sDirectInput->CreateDevice(GUID_SysKeyboard, &sKeyboard, NULL);
	assert(SUCCEEDED(result));
	//���̓f�[�^�`���̃Z�b�g
	result = sKeyboard->SetDataFormat(&c_dfDIKeyboard);//�W���`��
	assert(SUCCEEDED(result));
	//�r�����䃌�x���̃Z�b�g
	result = sKeyboard->SetCooperativeLevel(
		WinApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));

	isLinkGamePad = XInputGetState(
		0,//�����Ȃ���Ă�Ƃ��̑I��
		&gamePad);//���̕ϐ��ɓ��͏󋵂��i�[�����
}

void Input::Update()
{
	//�O��̃L�[���͂�ۑ�
	memcpy(keyPre, key, sizeof(key));

	//�L�[�{�[�h���̎擾�J�n
	sKeyboard->Acquire();
	//�S�L�[�̓��͏�Ԃ��擾����
	sKeyboard->GetDeviceState(sizeof(key), key);

	Updatekeypad();

	if (abs(gamePad.Gamepad.sThumbLX) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		gamePad.Gamepad.sThumbLX = 0;
	}
	if (abs(gamePad.Gamepad.sThumbLY) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		gamePad.Gamepad.sThumbLY = 0;
	}
	if (abs(gamePad.Gamepad.sThumbRX) < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		gamePad.Gamepad.sThumbRX = 0;
	}
	if (abs(gamePad.Gamepad.sThumbRY) < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		gamePad.Gamepad.sThumbRY = 0;
	}
	if (gamePad.Gamepad.bLeftTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		gamePad.Gamepad.bLeftTrigger = 0;
	}
	if (gamePad.Gamepad.bRightTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		gamePad.Gamepad.bRightTrigger = 0;
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
	if (key[keyNumber] && keyPre[keyNumber] == 0) {
		return true;
	}
	//�����łȂ����false�Ԃ�
	return false;
}

//�Q�[���p�b�h

void Input::Updatekeypad()
{
	oldGamePad = gamePad;

	isLinkGamePad = XInputGetState(
		0,//�����Ȃ���Ă�Ƃ��̑I��
		&gamePad);//���̕ϐ��ɓ��͏󋵂��i�[�����
}

bool Input::IsLinkGamePad()
{
	if (isLinkGamePad == ERROR_SUCCESS)
	{
		return true;
	}
	return false;
}

bool Input::PadTriggerKey(uint32_t button)
{
	return ((gamePad.Gamepad.wButtons & button) == button && (oldGamePad.Gamepad.wButtons & button) != button);
}

bool Input::PadPushKey(uint32_t button)
{
	return gamePad.Gamepad.wButtons & button;
}

float Input::GetPadStick(PadStick Stick)
{
	switch (Stick)
	{
	case PadStick::LT:
		return (float)gamePad.Gamepad.bLeftTrigger / 255;
	case PadStick::RT:
		return (float)gamePad.Gamepad.bRightTrigger / 255;
	case PadStick::LX:
		return (float)gamePad.Gamepad.sThumbLX / SHRT_MAX;
	case PadStick::LY:
		return (float)gamePad.Gamepad.sThumbLY / SHRT_MAX;
	case PadStick::RX:
		return (float)gamePad.Gamepad.sThumbRX / SHRT_MAX;
	case PadStick::RY:
		return (float)gamePad.Gamepad.sThumbRY / SHRT_MAX;
	default:
		return 0;
	}
}

float Input::GetOldPadStick(PadStick Stick)
{
	switch (Stick)
	{
	case PadStick::LT:
		return (float)oldGamePad.Gamepad.bLeftTrigger / 255;
	case PadStick::RT:
		return (float)oldGamePad.Gamepad.bRightTrigger / 255;
	case PadStick::LX:
		return (float)oldGamePad.Gamepad.sThumbLX / SHRT_MAX;
	case PadStick::LY:
		return (float)oldGamePad.Gamepad.sThumbLY / SHRT_MAX;
	case PadStick::RX:
		return (float)oldGamePad.Gamepad.sThumbRX / SHRT_MAX;
	case PadStick::RY:
		return (float)oldGamePad.Gamepad.sThumbRY / SHRT_MAX;
	default:
		return 0;
	}
}
