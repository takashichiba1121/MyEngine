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
	//借りてきたWinAppのインスタンスを記録
	winApp = WinApp;

	HRESULT result;

	//DirectInputの初期化
	result = DirectInput8Create(
		WinApp->GetInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&sDirectInput, nullptr);
	assert(SUCCEEDED(result));
	//キーボードデバイスの生成
	/*ComPtr<IDirectInputDevice8> keyboard = nullptr;*/
	result = sDirectInput->CreateDevice(GUID_SysKeyboard, &sKeyboard, NULL);
	assert(SUCCEEDED(result));
	//入力データ形式のセット
	result = sKeyboard->SetDataFormat(&c_dfDIKeyboard);//標準形式
	assert(SUCCEEDED(result));
	//排他制御レベルのセット
	result = sKeyboard->SetCooperativeLevel(
		WinApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));

	isLinkGamePad = XInputGetState(
		0,//複数つながれてるときの選択
		&gamePad);//この変数に入力状況が格納される
}

void Input::Update()
{
	//前回のキー入力を保存
	memcpy(keyPre, key, sizeof(key));

	//キーボード情報の取得開始
	sKeyboard->Acquire();
	//全キーの入力状態を取得する
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
	//指定キーを押していればtrueを返す
	if (key[keyNumber]) {
		return true;
	}
	//そうでなければfalse返す
	return false;
}

bool Input::TriggerKey(BYTE keyNumber)
{
	//指定キーを押していればtrueを返す
	if (key[keyNumber] && keyPre[keyNumber] == 0) {
		return true;
	}
	//そうでなければfalse返す
	return false;
}

//ゲームパッド

void Input::Updatekeypad()
{
	oldGamePad = gamePad;

	isLinkGamePad = XInputGetState(
		0,//複数つながれてるときの選択
		&gamePad);//この変数に入力状況が格納される
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
