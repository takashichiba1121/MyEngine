#pragma once
#include"SpriteCommon.h"
#include"Vector2.h"
#include"Vector3.h"
#include"Vector4.h"
#include"Matrix4.h"


struct Vertex
{
	Vector3 pos;    //xyz���W
	Vector2 uv;     //uv���W
};

//�萔�o�b�t�@�p�f�[�^�\���́i�}�e���A���j
struct ConstBufferDataMaterial {
	Vector4 color;//�F�iRGBA�j
};
//�萔�o�b�t�@�p�f�[�^�\���́i�s��j
struct ConstBufferDateTransform {
	Matrix4 mat;
};

enum VertexNumber {
	LB,//����
	LT,//����
	RB,//�E��
	RT,//�E��
};

class Sprite
{
public:

	void static StaticInitialize(SpriteCommon* SpriteCommon);

	//������
	void Initialize(uint32_t textureIndex = UINT32_MAX);

	void Update();

	void Draw();

	void SetTexture(uint32_t textureIndex);

	uint32_t GetTexture() {return textureIndex ; }

	void SetPosition(const Vector2& position) { this->position_ = position; }

	Vector2 GetPosition() const { return position_; }

	void SetRotation(const float& rotation) { this->rotation_ = rotation; }

	float GetRotation() const { return rotation_; }

	void SetColor(Vector4 color) { constMapMaterial->color = color; }

	Vector4 GetColor() { return constMapMaterial->color; }

	Vector2 GetScale() const { return scale_; }

	void SetScale(const Vector2& scale) { this->scale_ = scale; }

	Vector2 GetAnchorPoint() const { return anchorPoint_; }

	void SetAnchorPoint(const Vector2& anchorPoint) { this->anchorPoint_ = anchorPoint; }

	void SetFlipX(bool FlipX) { isFlipX_ = FlipX; }

	bool GetFlipX() { return isFlipX_; }

	void SetFlipY(bool FlipY) { isFlipY_ = FlipY; }

	bool GetFlipY() { return isFlipY_; }

	void SetInvisible(bool Tnvisible) { isInvisible_ = Tnvisible; }

	bool GetTnvisible() { return isInvisible_; }

	void SetSize(const Vector2& Size) { this->textureSize_ = Size; }

	Vector2 GetSize() const { return textureSize_; }

	void SetLeftTop_(const Vector2& textureLeftTop_) { this->textureLeftTop_ = textureLeftTop_; }

	Vector2 GetLeftTop_() const { return anchorPoint_; }

private:
	//�e�N�X�`���T�C�Y���C���[�W�ɍ��킹��
	void AdjustTextureSize();

private:
	Vertex vertices[4] = {};

	Vertex* vertMap = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;	//���_�o�b�t�@

	//���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	ConstBufferDataMaterial* constMapMaterial = nullptr;

	ConstBufferDateTransform* constMapTransform = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff = nullptr;

	Microsoft::WRL::ComPtr <ID3D12DescriptorHeap> srvHeap = nullptr;

	uint32_t textureIndex;

	float rotation_ = 0.0f;

	Vector2 position_ = { 100.0f,100.0f };

	Vector2 scale_ = { 100.0f,100.0f };

	Vector2 anchorPoint_ = { 0.0f,0.0f };

	//���E�t���b�v
	bool isFlipX_ = false;
	//�㉺�t���b�v
	bool isFlipY_ = false;

	bool isInvisible_ = false;

	//�e�N�X�`��������W
	Vector2 textureLeftTop_ = {0.0f,0.0f};
	//�e�N�X�`���؂�o���T�C�Y
	Vector2 textureSize_ = {0.0f,0.0f};

private:
	static SpriteCommon* spriteCommon;

	static DirectXCommon* dxCommon;

};

