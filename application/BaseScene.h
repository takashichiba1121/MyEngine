#pragma once
#include"DirectXCommon.h"
class BaseScene
{
public:
	virtual ~BaseScene() = default;
public:
	virtual void Initialize()=0;

	virtual void Finalize()=0;

	/// <summary>
	/// –ˆƒtƒŒ[ƒ€ˆ—
	/// </summary>
	virtual void Update()=0;

	/// <summary>
	/// •`‰æ
	/// </summary>
	virtual void Draw(DirectXCommon* dxCommon)=0;


};

