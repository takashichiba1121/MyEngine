#pragma once
#include "Sprite.h"
#include"PostEffectCommon.h"
class PostEffectLuminance
{
public:

    void Initialize(PostEffectCommon* PECommon);
    
    /// <summary>
    /// �p�C�v���C������
    /// </summary>
    void CreatGraphicsPipelineState();

    /// <summary>
    /// �V�[���`��O����
    /// </summary>
    /// <param name="cmdList">�R�}���h���X�g</param>
    void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

    void Draw();

    void NormalDraw();

    /// <summary>
    /// �V�[���`��㏈��
    /// </summary>
    /// <param name="cmdList">�R�}���h����</param>
    void PostDrawScene();

    uint32_t GettextureHandle() {return textureHandle ; }

private://�ÓI�����o�ϐ�
    static const float clearColor[4];

private:

    ID3D12GraphicsCommandList* commandList=nullptr;

    Vertex vertices[4] = {};

    Vertex* vertMap = nullptr;

    Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;	//���_�o�b�t�@

    //���_�o�b�t�@�r���[�̍쐬
    D3D12_VERTEX_BUFFER_VIEW vbView{};

    Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;
    //�[�x�o�b�t�@
    Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff;
    //RTV�p�̃f�X�N���v�^�q�[�v
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapRTV;
    //DSV�p�̃f�X�N���v�^�q�[�v
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapDSV;

    Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
    Microsoft::WRL::ComPtr<ID3D12PipelineState> normalPipelineState;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;

    uint32_t textureHandle;
    
    PostEffectCommon* PECommon;
};

