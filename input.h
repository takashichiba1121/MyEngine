#pragma once
#define DIRECTINPUT_VERSION 0x0800 //DirectInputのバージョン指定
#include<dinput.h>
#include<windows.h>
#include<wrl.h>

//入力
class Input
{
public:

	template <class T> using ComPtr= Microsoft::WRL::ComPtr<T>;

public://メンバ関数

	//初期化
	void Initialize(HINSTANCE hInstance,HWND hwnd);
	//更新
	void Update();

private://メイン変数
	//キーボードデバイス
	ComPtr<IDirectInputDevice8> keyboard = nullptr;
};
