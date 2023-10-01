#include "Texture.h"
#include<string.h>
#include<cassert>

using namespace DirectX;
using namespace Microsoft::WRL;

Texture* Texture::Instance()
{
	static Texture instance;

	return &instance;
}

uint32_t Texture::LoadTexture(const std::string fileName)
{
	DirectX::TexMetadata metadata{};
	DirectX::ScratchImage scratchImg{};
	DirectX::ScratchImage mipChain{};

	for (int32_t i=0;i<=spriteSRVCount;i++)
	{
		if (fileName==textureName[i])
		{
			return i;
		}
	}
	//���j�R�[�h������ɕϊ�����
	wchar_t wfilepath[128];
	uint32_t iBufferSize = MultiByteToWideChar(CP_ACP, 0, fileName.c_str(), -1, wfilepath, _countof(wfilepath));

	HRESULT result;
	result = LoadFromWICFile(
		wfilepath,
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;

	//�t�@�C��������������ʏ�̃t�@�C���ǂݍ���
		//�~�b�v�}�b�v����
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}
	// �ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata.format = MakeSRGB(metadata.format);
	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	//���\�[�X�ݒ�
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;	// ��
	textureResourceDesc.Height = (uint32_t)metadata.height;	// ����
	textureResourceDesc.DepthOrArraySize = (uint32_t)metadata.arraySize;
	textureResourceDesc.MipLevels = (uint32_t)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;
	result = dxCommon_->GetDevice()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&texBuff));

	CreateSRV(texBuff.Get());

	for (uint32_t i = 0; i < texBuffuers.size(); i++) {

		//�܂��e�N�X�`����񂪊��蓖�Ă��Ă��Ȃ��Ƃ���Ƀe�N�X�`����������
		if (!texBuffuers[i]) {
			texBuffuers[i] = texBuff;
			textureName[i] = fileName;
			break;
		}
	}

	ID3D12Resource* Texture = UploadTextureData(texBuff.Get(), scratchImg, dxCommon_->GetDevice(), dxCommon_->GetCommandList());

	ExcuteComandList();

	Texture->Release();

	return srvIncrementIndex++;
}

[[nodiscard]]
ID3D12Resource* Texture::UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages, ID3D12Device* device,
	ID3D12GraphicsCommandList* commandList)
{
	HRESULT result;

	std::vector<D3D12_SUBRESOURCE_DATA> subresources;

	DirectX::PrepareUpload(device, mipImages.GetImages(), mipImages.GetImageCount(), mipImages.GetMetadata(), subresources);

	uint64_t intermediateSize = GetRequiredIntermediateSize(texture, 0, UINT(subresources.size()));
	ID3D12Resource* intermediateResource;
	// �q�[�v�̐ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp1{};
	textureHeapProp1.Type = D3D12_HEAP_TYPE_UPLOAD;
	CD3DX12_RESOURCE_DESC textureResourceDesc1 =
		CD3DX12_RESOURCE_DESC::Buffer(intermediateSize);

	//Microsoft::WRL::ComPtr<ID3D12Resource> uploadBuffer;

	// �e�N�X�`���o�b�t�@�̐���
	result = device->
		CreateCommittedResource(
			&textureHeapProp1,
			D3D12_HEAP_FLAG_NONE,
			&textureResourceDesc1,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&intermediateResource));
	assert(SUCCEEDED(result));

	UpdateSubresources(commandList, texture, intermediateResource, 0, 0, UINT(subresources.size()), subresources.data());

	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = texture;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	commandList->ResourceBarrier(1, &barrier);
	return intermediateResource;
}

void Texture::ExcuteComandList()
{
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	commandList->Close();

	ID3D12CommandQueue* commandQueue = dxCommon_->GetCommandQueue();

	ID3D12CommandList* list[] = { commandList };
	commandQueue->ExecuteCommandLists(1, list);

	dxCommon_->PreIncrimentFenceValue();

	// �R�}���h�̎��s������҂�
	commandQueue->Signal(dxCommon_->GetFence(), dxCommon_->GetFenceValue());

	auto test = dxCommon_->GetFence()->GetCompletedValue();
	if (test != dxCommon_->GetFenceValue())
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		dxCommon_->GetFence()->SetEventOnCompletion(dxCommon_->GetFenceValue(), event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	HRESULT result;

	// �L���[���N���A
	result = dxCommon_->GetCommandAllocator()->Reset();
	assert(SUCCEEDED(result));
	// �ĂуR�}���h���X�g�𒙂߂鏀��
	result = commandList->Reset(dxCommon_->GetCommandAllocator(), nullptr);
	assert(SUCCEEDED(result));
}

void Texture::Initialize(DirectXCommon* DxCommon)
{
	HRESULT result;
	//�f�o�C�X�̃C���X�^���X���؂��
	dxCommon_ = DxCommon;

	//�f�X�N���v�^�q�[�v����
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = static_cast<UINT>(spriteSRVCount);
	result = dxCommon_->GetDevice()->CreateDescriptorHeap(&descHeapDesc,
		IID_PPV_ARGS(&descHeap));
	assert(SUCCEEDED(result));

}

void Texture::CreateSRV(ID3D12Resource* texBuff)
{
	// SRV�q�[�v�̐擪�n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE srvCpuHandle = descHeap->GetCPUDescriptorHandleForHeapStart();
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descHeap->GetGPUDescriptorHandleForHeapStart();

	uint32_t descriptorSize = dxCommon_->GetDevice()->
		GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	srvCpuHandle.ptr += (SIZE_T)(descriptorSize * srvIncrementIndex);
	srvGpuHandle.ptr += (SIZE_T)(descriptorSize * srvIncrementIndex);

	// �V�F�[�_�[���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	// srv�ݒ�\����
	srvDesc.Format = texBuff->GetDesc().Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	// 2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	// �n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	dxCommon_->GetDevice()->
		CreateShaderResourceView(texBuff, &srvDesc, srvCpuHandle);
}
void Texture::Finalize()
{
	descHeap = nullptr;
	texBuffuers.fill(nullptr);
}

