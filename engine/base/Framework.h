#pragma once
#include"WinApp.h"
#include"DirectXCommon.h"
#include<memory>
class Framework
{
public:
	virtual ~Framework() = default;

	//������
	virtual void Initialize();

	virtual void Finalize();

	virtual void Update();

	virtual void Draw()=0;

	//�I���t���O�̃`�F�b�N
	virtual bool IsEndRequst() { return endRequst_; }

	virtual DirectXCommon* GetDxCommon() {return dxCommon_.get() ; }

	void Run();
private:
	bool endRequst_ = false;

	std::unique_ptr<WinApp> winApp_;

	std::unique_ptr<DirectXCommon> dxCommon_=nullptr;
};

