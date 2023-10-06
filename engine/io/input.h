#pragma once
#define DIRECTINPUT_VERSION 0x0800 //DirectInputのバージョン指定
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

//入力
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

public://メンバ関数

	//初期化
	void Initialize(WinApp* winApp);
	//更新
	void Update();

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
	ComPtr<IDirectInputDevice8> sKeyboard;

	ComPtr<IDirectInput8> sDirectInput;

	BYTE key[256] = {};

	BYTE keyPre[256] = {};

	//WindowAPI
	WinApp* winApp=nullptr;

public://メンバ関数(ゲームパッド)
	/// <summary>
	/// コントローラーの状況の取得（毎フレームする）
	/// </summary>
	void Updatekeypad();

	bool IsLinkGamePad();

	XINPUT_STATE GetPad() { return gamePad; }

	XINPUT_STATE GetOldPad() { return oldGamePad; }

	/// <summary>
	/// コントローラーのボタンの入力した瞬間
	/// </summary>
	/// ///<param name="button">ボタンの種類(XINPUT_GAMEPAD_で始まる定数で指定)</param>
	/// <returns>押されてるか？</returns>
	bool PadTriggerKey(uint32_t button);

	/// <summary>
/// コントローラーのボタンの入力状況
/// </summary>
/// ///<param name="button">ボタンの種類(XINPUT_GAMEPAD_で始まる定数で指定)</param>
/// <returns>押されてるか？</returns>
	bool PadPushKey(uint32_t button);

	float GetPadStick(PadStick Stick);

	float GetOldPadStick(PadStick Stick);

private://メンバ変数(ゲームパッド)

	XINPUT_STATE gamePad = {};

	XINPUT_STATE oldGamePad = {};

	DWORD isLinkGamePad = 0;
};
