#pragma once
#define DIRECTINPUT_VERSION 0x0800 //DirectInputのバージョン指定
#include<dinput.h>
#include<windows.h>
#include<wrl.h>
#include"WinApp.h"
#include<Xinput.h>

#pragma comment (lib, "xinput.lib")

//入力
class Input
{
public:

	template <class T> using ComPtr= Microsoft::WRL::ComPtr<T>;

public://メンバ関数

	//初期化
	static void Initialize(WinApp* winApp);
	//更新
	static void Update();

	/// <summary>
	/// キーの押下をチェック
	/// </summary>
	///<param name="keynumber">キー番号(DIK_0等)</param>
	/// <returns>押されているか</returns>
	static bool PushKey(BYTE keyNumber);
	/// <summary>
	/// キーのトリガーをチェック
	/// </summary>
	///<param name="keynumber">キー番号(DIK_0等)</param>
	/// <returns>トリガーか</returns>
	static bool TriggerKey(BYTE keyNumber);
private://メイン変数
	//キーボードデバイス
	static ComPtr<IDirectInputDevice8> sKeyboard;

	static ComPtr<IDirectInput8> sDirectInput;

	static BYTE sKey[256];

	static BYTE sKeyPre[256];

	//WindowAPI
	static WinApp* sWinApp;

public://メンバ関数(ゲームパッド)
	/// <summary>
	/// コントローラーの状況の取得（毎フレームする）
	/// </summary>
	/// <returns>成功か？</returns>
	static DWORD Updatekeypad();

	static XINPUT_STATE GetPad() { return sGamePad; }

	static XINPUT_STATE OldGetPad() {return sOldGamePad; }

private://メンバ変数(ゲームパッド)

	static XINPUT_STATE sGamePad;

	static XINPUT_STATE sOldGamePad;
};
