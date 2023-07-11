#include "Texture.h"
#include<string.h>
#include<cassert>

using namespace DirectX;
using namespace Microsoft::WRL;


ComPtr<ID3D12DescriptorHeap>Texture::sDescHeap;			//デスクリプタヒープ
D3D12_RESOURCE_DESC Texture::sTextureResourceDesc{};
std::array< Microsoft::WRL::ComPtr<ID3D12Resource>, Texture::sSpriteSRVCount > Texture::sTexBuffuers;
DirectXCommon* Texture::sDxCommon_=nullptr;
uint32_t Texture::sSrvIncrementIndex=0;

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

	ComPtr<ID3D12Resource> texBuff;

	//ファイルが見つかったら通常のファイル読み込み
		//ミップマップ生成
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}
	// 読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = MakeSRGB(metadata.format);
	//ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	//リソース設定
	sTextureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	sTextureResourceDesc.Format = metadata.format;
	sTextureResourceDesc.Width = metadata.width;	// 幅
	sTextureResourceDesc.Height = (UINT)metadata.height;	// 高さ
	sTextureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	sTextureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	sTextureResourceDesc.SampleDesc.Count = 1;
	result = sDxCommon_->GetDevice()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&sTextureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff));
	//全ミップマップについて
	for (size_t i = 0; i < metadata.mipLevels; i++) {
		//ミップマップレベルを指定してイメージを取得
		const Image* img = scratchImg.GetImage(i, 0, 0);
		//テクスチャバッファにデータ転送
		result = texBuff->WriteToSubresource(
			(UINT)i,
			nullptr,				//全領域へコピー
			img->pixels,			//元データアドレス
			(UINT)img->rowPitch,	//1ラインサイズ
			(UINT)img->slicePitch	//全サイズ
		);
		assert(SUCCEEDED(result));
	}

	for (int i = 0; i < sTexBuffuers.size(); i++) {

		//まだテクスチャ情報が割り当てられていないところにテクスチャ情報を入れる
		if (!sTexBuffuers[i]) {
			sTexBuffuers[i] = texBuff;
			break;
		}
	}
	return -1;
}

void Texture::Initialize(DirectXCommon* DxCommon)
{
	HRESULT result;
	//デバイスのインスタンスを借りる
	sDxCommon_ = DxCommon;

	//デスクリプタヒープ生成
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = sSpriteSRVCount;
	result = sDxCommon_->GetDevice()->CreateDescriptorHeap(&descHeapDesc,
		IID_PPV_ARGS(&sDescHeap));
	assert(SUCCEEDED(result));

}

void Texture::CreateSRV(ID3D12Resource* texBuff, D3D12_CPU_DESCRIPTOR_HANDLE& srvHandle)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = sTextureResourceDesc.Format;//RGBA float
	srvDesc.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = sTextureResourceDesc.MipLevels;

	//ハンドルの指す位置にシェーダーリソースビュー作成
	sDxCommon_->GetDevice()->CreateShaderResourceView(texBuff, &srvDesc, srvHandle);
}

