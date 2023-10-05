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
	sWinApp = WinApp;

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
		&sGamePad);//���̕ϐ��ɓ��͏󋵂��i�[�����
}

void Input::Update()
{
	//�O��̃L�[���͂�ۑ�
	memcpy(sKeyPre, sKey, sizeof(sKey));

	//�L�[�{�[�h���̎擾�J�n
	sKeyboard->Acquire();
	//�S�L�[�̓��͏�Ԃ��擾����
	sKeyboard->GetDeviceState(sizeof(sKey), sKey);

	Updatekeypad();

	if (abs(sGamePad.Gamepad.sThumbLX) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		sGamePad.Gamepad.sThumbLX = 0;
	}
	if (abs(sGamePad.Gamepad.sThumbLY) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		sGamePad.Gamepad.sThumbLY = 0;
	}
	if (abs(sGamePad.Gamepad.sThumbRX) < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		sGamePad.Gamepad.sThumbRX = 0;
	}
	if (abs(sGamePad.Gamepad.sThumbRY) < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		sGamePad.Gamepad.sThumbRY = 0;
	}
	if (sGamePad.Gamepad.bLeftTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		sGamePad.Gamepad.bLeftTrigger = 0;
	}
	if (sGamePad.Gamepad.bRightTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		sGamePad.Gamepad.bRightTrigger = 0;
	}


}

bool Input::PushKey(BYTE keyNumber)
{
	//�w��L�[�������Ă����true��Ԃ�
	if (sKey[keyNumber]) {
		return true;
	}
	//�����łȂ����false�Ԃ�
	return false;
}

bool Input::TriggerKey(BYTE keyNumber)
{
	//�w��L�[�������Ă����true��Ԃ�
	if (sKey[keyNumber] && sKeyPre[keyNumber] == 0) {
		return true;
	}
	//�����łȂ����false�Ԃ�
	return false;
}

//�Q�[���p�b�h

void Input::Updatekeypad()
{
	sOldGamePad = sGamePad;

	isLinkGamePad = XInputGetState(
		0,//�����Ȃ���Ă�Ƃ��̑I��
		&sGamePad);//���̕ϐ��ɓ��͏󋵂��i�[�����
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
	return ((sGamePad.Gamepad.wButtons & button) == button && (sOldGamePad.Gamepad.wButtons & button) != button);
}

bool Input::PadPushKey(uint32_t button)
{
	return sGamePad.Gamepad.wButtons & button;
}

float Input::GetPadStick(PadStick Stick)
{
	switch (Stick)
	{
	case PadStick::LT:
		return (float)sGamePad.Gamepad.bLeftTrigger / 255;
	case PadStick::RT:
		return (float)sGamePad.Gamepad.bRightTrigger / 255;
	case PadStick::LX:
		return (float)sGamePad.Gamepad.sThumbLX / SHRT_MAX;
	case PadStick::LY:
		return (float)sGamePad.Gamepad.sThumbLY / SHRT_MAX;
	case PadStick::RX:
		return (float)sGamePad.Gamepad.sThumbRX / SHRT_MAX;
	case PadStick::RY:
		return (float)sGamePad.Gamepad.sThumbRY / SHRT_MAX;
	default:
		return 0;
	}
}

float Input::GetOldPadStick(PadStick Stick)
{
	switch (Stick)
	{
	case PadStick::LT:
		return (float)sOldGamePad.Gamepad.bLeftTrigger / 255;
	case PadStick::RT:
		return (float)sOldGamePad.Gamepad.bRightTrigger / 255;
	case PadStick::LX:
		return (float)sOldGamePad.Gamepad.sThumbLX / SHRT_MAX;
	case PadStick::LY:
		return (float)sOldGamePad.Gamepad.sThumbLY / SHRT_MAX;
	case PadStick::RX:
		return (float)sOldGamePad.Gamepad.sThumbRX / SHRT_MAX;
	case PadStick::RY:
		return (float)sOldGamePad.Gamepad.sThumbRY / SHRT_MAX;
	default:
		return 0;
	}
}
