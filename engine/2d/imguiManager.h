#pragma once
#include"WinApp.h"
#include"DirectXCommon.h"
class imguiManager
{
public:

	static void StaticInitialize(WinApp* winApp, DirectXCommon* dxCommon);

	/// <summary>
	/// �I��
	/// </summary>
	static void Finalize();

	/// <summary>
	/// ImGui��t�J�n
	/// </summary>
	void Begin();

	/// <summary>
	/// ImGui��t�I��
	/// </summary>
	void End();

	/// <summary>
	/// ��ʂւ̕`��
	/// </summary>
	static void Draw();

private:
	//SRV�p�f�X�N���v�^�q�[�v
	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;

	static DirectXCommon* dxCommon_;

	static WinApp* winApp_;
};

