 #include"LightGroup.h"
#include<assert.h>
#include <d3dx12.h>
#include"Vector3.h"

ID3D12Device* LightGroup::sDevice = nullptr;

void LightGroup::StaticInitialize(ID3D12Device* device)
{
	//再初期化チェック
	assert(!LightGroup::sDevice);

	//nullptrチェック
	assert(device);

	LightGroup::sDevice = device;
}

LightGroup* LightGroup::Create()
{
	//ライトのオブジェクトのインスタンスを生成
	LightGroup* instance = new LightGroup();

	//初期化
	instance->Initialize();

	return instance;
}

void LightGroup::Initialize()
{
	//ライトの標準設定
	DefaultLightSetting();

	HRESULT result;
	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(constBuff_) + 0xff) & ~0xff);

	// 定数バッファの生成
	result = sDevice->CreateCommittedResource(
		&heapProps, // アップロード可能
		D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result));

	//定数バッファへデータ転送
	TransferConstBuffer();
}

void LightGroup::Update()
{
	//値の更新があった時だけ定数バッファに転送する
	if (dirty_)
	{
		TransferConstBuffer();
		dirty_ = false;
	}
}

void LightGroup::Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex)
{
	//定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(rootParameterIndex,
		constBuff_->GetGPUVirtualAddress());
}

void  LightGroup::TransferConstBuffer()
{
	HRESULT result;

	// 定数バッファへデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuff_->Map(0, nullptr, (void**)&constMap);
	constMap->ambientColor = ambientColor_;
	for (uint32_t i=0;i<cDirLightNum;i++)
	{
		if (dirLights_[i].IsActive())
		{
			constMap->dirLights[i].active = 1;
			constMap->dirLights[i].lightv = -dirLights_[i].GetLightDir();
			constMap->dirLights[i].lightcolor = dirLights_[i].GetLightColor();
		}
		else
		{
			constMap->dirLights[i].active = 0;
		}
		if (pointLight_[i].IsActive())
		{
			constMap->pointLight[i].active = 1;
			constMap->pointLight[i].lightPos = -pointLight_[i].GetLightPos();
			constMap->pointLight[i].lightcolor = pointLight_[i].GetLightColor();
			constMap->pointLight[i].lightatten = pointLight_[i].GetLightAtten();
		}
		else
		{
			constMap->pointLight[i].active = 0;
		}
	}
	constBuff_->Unmap(0, nullptr);
}

void LightGroup::SetAmbientColor(const Vector3& color)
{
	ambientColor_ = color;
	dirty_ = true;
}

void LightGroup::SetDirLightActive(uint32_t index, bool active)
{
	assert(0 <= index && index < cDirLightNum);
	dirLights_[index].SetActive(active);
}

void LightGroup::SetDirLightColor(uint32_t index, const Vector3& lightcolor)
{
	assert(0 <= index && index < cDirLightNum);
	dirLights_[index].SetLightColor(lightcolor);
	dirty_ = true;
}

void LightGroup::SetPointActive(uint32_t index, bool active)
{
	assert(0 <= index && index < cDirLightNum);
	pointLight_[index].SetActive(active);
}

bool LightGroup::GetPointActive(uint32_t index)
{
	assert(0 <= index && index < cDirLightNum);
	return pointLight_[ index ].IsActive();
}

void LightGroup::SetPointPos(uint32_t index, const Vector3& lightPos)
{
	assert(0 <= index && index < cDirLightNum);
	pointLight_[index].SetLightPos(lightPos);
	dirty_ = true;
}

void LightGroup::SetPointColor(uint32_t index, const Vector3& lightcolor)
{
	assert(0 <= index && index < cDirLightNum);
	pointLight_[index].SetLightColor(lightcolor);
	dirty_ = true;
}

void LightGroup::SetPointAtten(uint32_t index, const Vector3& lightAtten)
{
	assert(0 <= index && index < cDirLightNum);
	pointLight_[index].SetLightAtten(lightAtten);
	dirty_ = true;
}

void LightGroup::DefaultLightSetting()
{
	dirLights_[0].SetActive(true);
	dirLights_[0].SetLightColor({1.0f,1.0f,1.0f});
	dirLights_[0].SetLightDir({0.0f,1.0f,0.0f});

	dirLights_[1].SetActive(true);
	dirLights_[1].SetLightColor({ 1.0f,1.0f,1.0f });
	dirLights_[1].SetLightDir({ 0.0f,1.0f,0.0f });

	dirLights_[2].SetActive(true);
	dirLights_[2].SetLightColor({ 1.0f,1.0f,1.0f });
	dirLights_[2].SetLightDir({ 0.0f,1.0f,0.0f });

	pointLight_[0].SetActive(false);
	pointLight_[0].SetLightPos({0.0f,0.0f,0.0f});
	pointLight_[0].SetLightColor({0.0f,0.0f,0.0f});
	pointLight_[0].SetLightAtten({1.0f,1.0f,1.0f});

	pointLight_[1].SetActive(false);
	pointLight_[1].SetLightPos({ 0.0f,0.0f,0.0f });
	pointLight_[1].SetLightColor({ 0.0f,0.0f,0.0f });
	pointLight_[1].SetLightAtten({ 1.0f,1.0f,1.0f });

	pointLight_[2].SetActive(false);
	pointLight_[2].SetLightPos({ 0.0f,0.0f,0.0f });
	pointLight_[2].SetLightColor({ 0.0f,0.0f,0.0f });
	pointLight_[2].SetLightAtten({ 1.0f,1.0f,1.0f });
}

void LightGroup::SetDirLightDir(uint32_t index, const Vector3& lightdir)
{
	assert(0 <= index && index < cDirLightNum);
	dirLights_[index].SetLightDir(lightdir);
	dirty_ = true;
}
