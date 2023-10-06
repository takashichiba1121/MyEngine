 #include "WinApp.h"
#pragma comment(lib,"winmm.lib")
#include<imgui_impl_win32.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd,UINT msg,WPARAM waram,LPARAM lparam);

//ウィンドウプロージャー
LRESULT WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {

	//iimGui用ウィンドウプロシージャ呼び出し
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
	{
		return true;
	}

	//メッセージに応じてゲーム固有の処理を行う
	switch (msg)
	{
		//ウインドウが破棄された
	case WM_DESTROY:
		//OSに対して、アプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void WinApp::Initialize()
{

	//ウインドウクラスの設定
	w_.cbSize = sizeof(WNDCLASSEX);
	w_.lpfnWndProc = (WNDPROC)WindowProc;//ウインドウプロ―ジャーの設定
	w_.lpszClassName = L"DirectXGame";//ウインドウクラス名
	w_.hInstance = GetModuleHandle(nullptr);//ウインドウハンドル
	w_.hCursor = LoadCursor(NULL, IDC_ARROW);//カーソル指定

	//ウインドウクラスをOSに登録する
	RegisterClassEx(&w_);
	//ウインドウサイズ｛X座標　Y座標　横幅　縦幅｝
	RECT wrc = { 0,0,window_width,window_height };
	//自動サイズを補正する
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	//ウインドウオブジェクトの生成
	hwnd_ = CreateWindow(w_.lpszClassName,//クラス名
		L"ハコニワ",//タイトルバーの文字
		WS_OVERLAPPEDWINDOW,//標準的なウインドウスタイル
		CW_USEDEFAULT,//表示X座標（OSに任せる）
		CW_USEDEFAULT,//表示Y座標（OSに任せる）
		wrc.right - wrc.left,//ウインドウ横幅
		wrc.bottom - wrc.top,//ウインドウ縦幅
		nullptr,//親ウインドウハンドル
		nullptr,//メニューハンドル
		w_.hInstance,//呼び出しアプリケーションハンドル
		nullptr);//オプション

	//ウインドウを表示状態にする
	ShowWindow(hwnd_, SW_SHOW);

	//システムタイマーの分解能を上げる
	timeBeginPeriod(1);
}

void WinApp::Finalize()
{
	//ウインドウクラスを登録解除
	UnregisterClass(w_.lpszClassName, w_.hInstance);
}

bool WinApp::ProcessMessage()
{
	MSG msg{};

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_QUIT)
	{
		return true;
	}

	return false;
}
