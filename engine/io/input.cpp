#include "input.h"
#include<cassert>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment (lib, "xinput.lib")

Microsoft::WRL::ComPtr<IDirectInputDevice8> Input::sKeyboard;

Microsoft::WRL::ComPtr<IDirectInput8> Input::sDirectInput;

 BYTE Input::sKey[256];

 BYTE Input::sKeyPre[256];

 WinApp* Input::sWinApp;

 XINPUT_STATE Input::sGamePad;

 XINPUT_STATE Input::sOldGamePad;

void Input::Initialize(WinApp* WinApp)
{
	//借りてきたWinAppのインスタンスを記録
	sWinApp = WinApp;

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
}

void Input::Update()
{
	//前回のキー入力を保存
	memcpy(sKeyPre, sKey, sizeof(sKey));

	//キーボード情報の取得開始
	sKeyboard->Acquire();
	//全キーの入力状態を取得する
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
	//指定キーを押していればtrueを返す
	if (sKey[keyNumber]) {
		return true;
	}
	//そうでなければfalse返す
	return false;
}

bool Input::TriggerKey(BYTE keyNumber)
{
	//指定キーを押していればtrueを返す
	if (sKey[keyNumber]&&sKeyPre[keyNumber]==0) {
		return true;
	}
	//そうでなければfalse返す
	return false;
}

//ゲームパッド

DWORD Input::Updatekeypad()
{
	sOldGamePad = sGamePad;

	return XInputGetState(
		0,//複数つながれてるときの選択
		&sGamePad);//この変数に入力状況が格納される
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
		break;
	case PadStick::RT:
		return (float)sGamePad.Gamepad.bRightTrigger / 255;
		break;
	case PadStick::LX:
		return (float)sGamePad.Gamepad.sThumbLX / SHRT_MAX;
		break;
	case PadStick::LY:
		return (float)sGamePad.Gamepad.sThumbLY / SHRT_MAX;
		break;
	case PadStick::RX:
		return (float)sGamePad.Gamepad.sThumbRX / SHRT_MAX;
		break;
	case PadStick::RY:
		return (float)sGamePad.Gamepad.sThumbRY / SHRT_MAX;
		break;
	default:
		return 0;
		break;
	}
	return 0;
}
