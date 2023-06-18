#include "imguiManager.h"
#include"imgui.h"
#include"imgui_impl_win32.h"
#include"imgui_impl_dx12.h"

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> imguiManager::sSrvHeap;

DirectXCommon* imguiManager::sDxCommon;

WinApp* imguiManager::sWinApp;

void imguiManager::StaticInitialize(WinApp* winApp, DirectXCommon* dxCommon)
{
	HRESULT result;

	sWinApp = winApp;

	sDxCommon = dxCommon;

	//Imgui�̃R���e�L�X�g�𐶐�
	ImGui::CreateContext();
	//ImGui�̃X�^�C����ݒ�
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(sWinApp->GetHwnd());

	//�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = 1;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	//�f�X�N���v�^�q�[�v����
	result = sDxCommon->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&sSrvHeap));
	assert(SUCCEEDED(result));

	ImGui_ImplDX12_Init(
		sDxCommon->GetDevice(),
		static_cast<int>(sDxCommon->GetBackBufferCount()),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, sSrvHeap.Get(),
		sSrvHeap->GetCPUDescriptorHandleForHeapStart(),
		sSrvHeap->GetGPUDescriptorHandleForHeapStart());
}

void imguiManager::Finalize()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	//�f�X�N���v�^�q�[�v�����
	sSrvHeap.Reset();
}

void imguiManager::Begin()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void imguiManager::End()
{
	//�`�揀��
	ImGui::Render();
}

void imguiManager::Draw()
{
	ID3D12GraphicsCommandList* commandList = sDxCommon->GetCommandList();


	//�f�X�N���v�^�q�[�v�̔z����Z�b�g����R�}���h
	ID3D12DescriptorHeap* ppHeaps[] = { sSrvHeap.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//�`��R�}���h�𔭍s
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(),commandList);
}
