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
	//借りてきたWinAppのインスタンスを記録
	winApp_ = WinApp;

	HRESULT result;

	//DirectInputの初期化
	result = DirectInput8Create(
		WinApp->GetInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr);
	assert(SUCCEEDED(result));
	//キーボードデバイスの生成
	/*ComPtr<IDirectInputDevice8> keyboard = nullptr;*/
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));
	//入力データ形式のセット
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);//標準形式
	assert(SUCCEEDED(result));
	//排他制御レベルのセット
	result = keyboard->SetCooperativeLevel(
		WinApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result)); 
}

void Input::Update()
{
	//前回のキー入力を保存
	memcpy(keyPre, key, sizeof(key));

	//キーボード情報の取得開始
	keyboard->Acquire();
	//全キーの入力状態を取得する
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
	if (key[keyNumber]&&keyPre[keyNumber]==0) {
		return true;
	}
	//そうでなければfalse返す
	return false;
}

//ゲームパッド

DWORD Input::Updatekeypad()
{
	oldGamePad = gamePad;

	return XInputGetState(
		0,//複数つながれてるときの選択
		&gamePad);//この変数に入力状況が格納される
}