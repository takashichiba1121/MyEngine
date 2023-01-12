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
	bool PushKey(BYTE keyNumber);
	/// <summary>
	/// キーのトリガーをチェック
	/// </summary>
	///<param name="keynumber">キー番号(DIK_0等)</param>
	/// <returns>トリガーか</returns>
	bool TriggerKey(BYTE keyNumber);
private://メイン変数
	//キーボードデバイス
	static ComPtr<IDirectInputDevice8> keyboard;

	static ComPtr<IDirectInput8> directInput;

	static BYTE key[256];

	static BYTE keyPre[256];

	//WindowAPI
	static WinApp* winApp_;

public://メンバ関数(ゲームパッド)
	/// <summary>
	/// コントローラーの状況の取得（毎フレームする）
	/// </summary>
	///<param name="dwUserIndex">つながれてるコントローラーが複数の場合の指定（一つなら0でOK）</param>
	/// <returns>成功か？</returns>
	DWORD Updatekeypad(DWORD dwUserIndex);

	/// <summary>
	/// 左アナログスティックのX方向の入力状況
	/// </summary>
	/// <returns>左アナログスティックのX方向の入力状況を返す(-1～+1)</returns>
	float PadAnalogStickLX();

	/// <summary>
	/// 左アナログスティックのY方向の入力状況
	/// </summary>
	/// <returns>左アナログスティックのY方向の入力状況を返す(-1～+1)</returns>
	float PadAnalogStickLY();

	/// <summary>
	/// 右アナログスティックのX方向の入力状況
	/// </summary>
	/// <returns>右アナログスティックのX方向の入力状況を返す(-1～+1)</returns>
	float PadAnalogStickRX();

	/// <summary>
	/// 右アナログスティックのY方向の入力状況
	/// </summary>
	/// <returns>右アナログスティックのY方向の入力状況を返す(-1～+1)</returns>
	float PadAnalogStickRY();

	/// <summary>
	/// 左トリガー(LZとか呼ばれるやつ)の入力状況
	/// </summary>
	/// <returns>左トリガーの入力状況を返す(0～+1)</returns>
	float PadLTrigger();

	/// <summary>
	/// 右トリガー(RZとか呼ばれるやつ)の入力状況
	/// </summary>
	/// <returns>左トリガーの入力状況を返す(0～+1)</returns>
	float PadRTrigger();

	/// <summary>
	/// コントローラーのボタンの入力状況
	/// </summary>
	/// ///<param name="button">ボタンの種類(XINPUT_GAMEPAD_で始まる定数で指定)</param>
	/// <returns>押されてるか？</returns>
	bool PadKey(int button);

private://メンバ変数(ゲームパッド)

	XINPUT_STATE gamePad;
};
