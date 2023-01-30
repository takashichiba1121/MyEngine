#include "WinApp.h"
#pragma comment(lib,"winmm.lib")
#include<imgui_impl_win32.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd,UINT msg,WPARAM waram,LPARAM lparam);

//�E�B���h�E�v���[�W���[
LRESULT WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {

	//iimGui�p�E�B���h�E�v���V�[�W���Ăяo��
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
	{
		return true;
	}

	//���b�Z�[�W�ɉ����ăQ�[���ŗL�̏������s��
	switch (msg)
	{
		//�E�C���h�E���j�����ꂽ
	case WM_DESTROY:
		//OS�ɑ΂��āA�A�v���̏I����`����
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void WinApp::Initialize()
{

	//�E�C���h�E�N���X�̐ݒ�
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc;//�E�C���h�E�v���\�W���[�̐ݒ�
	w.lpszClassName = L"DirectXGame";//�E�C���h�E�N���X��
	w.hInstance = GetModuleHandle(nullptr);//�E�C���h�E�n���h��
	w.hCursor = LoadCursor(NULL, IDC_ARROW);//�J�[�\���w��

	//�E�C���h�E�N���X��OS�ɓo�^����
	RegisterClassEx(&w);
	//�E�C���h�E�T�C�Y�oX���W�@Y���W�@�����@�c���p
	RECT wrc = { 0,0,window_width,window_heigit };
	//�����T�C�Y��␳����
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	//�E�C���h�E�I�u�W�F�N�g�̐���
	hwnd = CreateWindow(w.lpszClassName,//�N���X��
		L"DirectXGame",//�^�C�g���o�[�̕���
		WS_OVERLAPPEDWINDOW,//�W���I�ȃE�C���h�E�X�^�C��
		CW_USEDEFAULT,//�\��X���W�iOS�ɔC����j
		CW_USEDEFAULT,//�\��Y���W�iOS�ɔC����j
		wrc.right - wrc.left,//�E�C���h�E����
		wrc.bottom - wrc.top,//�E�C���h�E�c��
		nullptr,//�e�E�C���h�E�n���h��
		nullptr,//���j���[�n���h��
		w.hInstance,//�Ăяo���A�v���P�[�V�����n���h��
		nullptr);//�I�v�V����

	//�E�C���h�E��\����Ԃɂ���
	ShowWindow(hwnd, SW_SHOW);

	//�V�X�e���^�C�}�[�̕���\���グ��
	timeBeginPeriod(1);
}

void WinApp::Finalize()
{
	//�E�C���h�E�N���X��o�^����
	UnregisterClass(w.lpszClassName, w.hInstance);
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
