#pragma once
#include<Windows.h>
class WinApp
{
public://静的メンバ関数
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wapram, LPARAM lparam);
public://メンバ関数
	//初期化
	void Initialize();
	//更新
	void Update();
};

