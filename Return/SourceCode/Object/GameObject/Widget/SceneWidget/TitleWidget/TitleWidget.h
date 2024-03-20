#pragma once
#include "..\SceneWidget.h"

/************************************************
*	タイトルUIクラス.
*		﨑田友輝.
**/
class CTitleWidget final
	: public CSceneWidget
{
public:
	CTitleWidget();
	virtual ~CTitleWidget();

	// 初期化.
	virtual bool Init() override;

	// 更新.
	virtual void Update( const float& DeltaTime ) override;

	// 描画.
	virtual void Render() override;
};
