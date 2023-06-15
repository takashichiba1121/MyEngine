#pragma once
#include<Windows.h>
class WinApp
{
public://�ÓI�����o�֐�
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
public://�����o�֐�
	//������
	void Initialize();

	void Finalize();

	//���b�Z�[�W�̏���
	bool ProcessMessage();

	//getter
	HWND GetHwnd() const { return hwnd_; }

	HINSTANCE GetInstance() const { return w_.hInstance; }
public://�萔
//�E�B���h�E����
	static const int window_width = 1280;
	//�E�B���h�E�c��
	static const int window_height = 720;
private:
	//�E�B���h�E�n���h��
	HWND hwnd_ = nullptr;
	//�E�B���h�E�N���X�̐ݒ�
	WNDCLASSEX w_{};

};

