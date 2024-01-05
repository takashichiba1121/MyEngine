#pragma once
#include"DirectXCommon.h"
/*
* シーンの基底クラス
*/
class BaseScene
{
public:
	virtual ~BaseScene() = default;
public:
	virtual void Initialize()=0;

	virtual void Finalize()=0;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	virtual void Update()=0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw(DirectXCommon* dxCommon)=0;

		/// <summary>
	/// 描画
	/// </summary>
	virtual void SpriteDraw() = 0;

	 
};

