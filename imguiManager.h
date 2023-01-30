#pragma once
#include"WinApp.h"
#include"DirectXCommon.h"
class imguiManager
{
public:


	///<summary>
	///������
	///</summary>
	void Initialize(WinApp* winApp, DirectXCommon* dxCommon);

	/// <summary>
	/// �I��
	/// </summary>
	void Finalize();

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
	void Draw();

private:
	//SRV�p�f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;

	DirectXCommon* dxCommon_;

	WinApp* winApp_;
};

