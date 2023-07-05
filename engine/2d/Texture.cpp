#include "Texture.h"
#include<string.h>
#include<cassert>

using namespace DirectX;
using namespace Microsoft::WRL;


ComPtr<ID3D12DescriptorHeap>Texture::sDescHeap;			//�f�X�N���v�^�q�[�v
D3D12_RESOURCE_DESC Texture::sTextureResourceDesc{};
std::array< Microsoft::WRL::ComPtr<ID3D12Resource>, Texture::sSpriteSRVCount > Texture::sTexBuffuers;
DirectXCommon* Texture::sDxcommon_=nullptr;
uint32_t Texture::sSrvIncrementIndex=0;

//uint32_t Texture::LoadTexture(const wchar_t* fileName)
//{
//	DirectX::TexMetadata metadata{};
//	DirectX::ScratchImage scratchImg{};
//	DirectX::ScratchImage mipChain{};
//
//	HRESULT result;
//	result = LoadFromWICFile(
//		fileName,
//		WIC_FLAGS_NONE,
//		&metadata, scratchImg);
//
//	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;
//
//	//�t�@�C��������������ʏ�̃t�@�C���ǂݍ���
//		//�~�b�v�}�b�v����
//	result = GenerateMipMaps(
//		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
//		TEX_FILTER_DEFAULT, 0, mipChain);
//	if (SUCCEEDED(result)) {
//		scratchImg = std::move(mipChain);
//		metadata = scratchImg.GetMetadata();
//	}
//	// �ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
//	metadata.format = MakeSRGB(metadata.format);
//	//�q�[�v�ݒ�
//	D3D12_HEAP_PROPERTIES textureHeapProp{};
//	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
//	textureHeapProp.CPUPageProperty =
//		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
//	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
//	//���\�[�X�ݒ�
//	sTextureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
//	sTextureResourceDesc.Format = metadata.format;
//	sTextureResourceDesc.Width = metadata.width;	// ��
//	sTextureResourceDesc.Height = (uint32_t)metadata.height;	// ����
//	sTextureResourceDesc.DepthOrArraySize = (uint32_t)metadata.arraySize;
//	sTextureResourceDesc.MipLevels = (uint32_t)metadata.mipLevels;
//	sTextureResourceDesc.SampleDesc.Count = 1;
//	result = sDxcommon_->GetDevice()->CreateCommittedResource(
//		&textureHeapProp,
//		D3D12_HEAP_FLAG_NONE,
//		&sTextureResourceDesc,
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&texBuff));
//	//�S�~�b�v�}�b�v�ɂ���
//	for (uint32_t i = 0; i < metadata.mipLevels; i++) {
//		//�~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
//		const Image* img = scratchImg.GetImage(i, 0, 0);
//		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
//		result = texBuff->WriteToSubresource(
//			(uint32_t)i,
//			nullptr,				//�S�̈�փR�s�[
//			img->pixels,			//���f�[�^�A�h���X
//			(uint32_t)img->rowPitch,	//1���C���T�C�Y
//			(uint32_t)img->slicePitch	//�S�T�C�Y
//		);
//		assert(SUCCEEDED(result));
//	}
//	return -1;
//}

uint32_t Texture::LoadTexture(const wchar_t* fileName)
{
	DirectX::TexMetadata metadata{};
	DirectX::ScratchImage scratchImg{};
	DirectX::ScratchImage mipChain{};

	HRESULT result;
	result = LoadFromWICFile(
		fileName,
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
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty =
		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	//���\�[�X�ݒ�
	sTextureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	sTextureResourceDesc.Format = metadata.format;
	sTextureResourceDesc.Width = metadata.width;	// ��
	sTextureResourceDesc.Height = (uint32_t)metadata.height;	// ����
	sTextureResourceDesc.DepthOrArraySize = (uint32_t)metadata.arraySize;
	sTextureResourceDesc.MipLevels = (uint32_t)metadata.mipLevels;
	sTextureResourceDesc.SampleDesc.Count = 1;
	result = sDxcommon_->GetDevice()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&sTextureResourceDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&texBuff));

	CreateSRV(texBuff.Get());

	for (uint32_t i = 0; i < sTexBuffuers.size(); i++) {

		//�܂��e�N�X�`����񂪊��蓖�Ă��Ă��Ȃ��Ƃ���Ƀe�N�X�`����������
		if (!sTexBuffuers[i]) {
			sTexBuffuers[i] = texBuff;
		}
	}

	ID3D12Resource* Texture=UploadTextureData(texBuff.Get(), scratchImg, sDxcommon_->GetDevice(), sDxcommon_->GetCommandList());

	ExcuteComandList();

	return sSrvIncrementIndex++;
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
	CD3DX12_RESOURCE_DESC sTextureResourceDesc1 =
		CD3DX12_RESOURCE_DESC::Buffer(intermediateSize);

	//Microsoft::WRL::ComPtr<ID3D12Resource> uploadBuffer;

	// �e�N�X�`���o�b�t�@�̐���
	result = device->
		CreateCommittedResource(
			&textureHeapProp1,
			D3D12_HEAP_FLAG_NONE,
			&sTextureResourceDesc1,
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
	commandList->ResourceBarrier(1,&barrier);
	return intermediateResource;
}

void Texture::ExcuteComandList()
{
	ID3D12GraphicsCommandList* commandList = sDxcommon_->GetCommandList();

	commandList->Close();

	ID3D12CommandQueue* commandQueue = sDxcommon_->GetCommandQueue();

	ID3D12CommandList* list[] = { commandList };
	commandQueue->ExecuteCommandLists(1, list);

	sDxcommon_->PreIncrimentFenceValue();

	// �R�}���h�̎��s������҂�
	commandQueue->Signal(sDxcommon_->GetFence(), sDxcommon_->GetFenceValue());

	auto test = sDxcommon_->GetFence()->GetCompletedValue();
	if (test != sDxcommon_->GetFenceValue())
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		sDxcommon_->GetFence()->SetEventOnCompletion(sDxcommon_->GetFenceValue(), event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	HRESULT result;

	// �L���[���N���A
	result = sDxcommon_->GetCommandAllocator()->Reset();
	assert(SUCCEEDED(result));
	// �ĂуR�}���h���X�g�𒙂߂鏀��
	result = commandList->Reset(sDxcommon_->GetCommandAllocator(), nullptr);
	assert(SUCCEEDED(result));
}

void Texture::Initialize(DirectXCommon* DxCommon)
{
	HRESULT result;
	//�f�o�C�X�̃C���X�^���X���؂��
	sDxcommon_ = DxCommon;

	//�f�X�N���v�^�q�[�v����
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = sSpriteSRVCount;
	result = sDxcommon_->GetDevice()->CreateDescriptorHeap(&descHeapDesc,
		IID_PPV_ARGS(&sDescHeap));
	assert(SUCCEEDED(result));

}

void Texture::CreateSRV(ID3D12Resource* texBuff)
{
	// SRV�q�[�v�̐擪�n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE srvCpuHandle = sDescHeap->GetCPUDescriptorHandleForHeapStart();
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = sDescHeap->GetGPUDescriptorHandleForHeapStart();

	uint32_t descriptorSize = sDxcommon_->GetDevice()->
		GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	srvCpuHandle.ptr += (SIZE_T)(descriptorSize * sSrvIncrementIndex);
	srvGpuHandle.ptr += (SIZE_T)(descriptorSize * sSrvIncrementIndex);

	// �V�F�[�_�[���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	// srv�ݒ�\����
	srvDesc.Format = texBuff->GetDesc().Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	// 2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = sTextureResourceDesc.MipLevels;

	// �n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	sDxcommon_->GetDevice()->
		CreateShaderResourceView(texBuff, &srvDesc, srvCpuHandle);
}

