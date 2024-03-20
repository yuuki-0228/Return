#pragma once
#include "..\SceneBase.h"

class CTitleWidget;

/************************************************
*	タイトルクラス.
*		�ｱ田友輝.
**/
class CTitle final
	: public CSceneBase
{
public:
	CTitle();
	virtual ~CTitle();

	// 初期化.
	virtual bool Init() override;

	// 更新.
	virtual void Update( const float& DeltaTime ) override;

	// モデルの描画.
	virtual void ModelRender() override;
	// スプライト(UI)の描画.
	virtual void SpriteUIRender() override;
	// スプライト(3D)/Effectの描画.
	//	_A：後に表示される / _B：先に表示される.
	virtual void Sprite3DRender_A() override;
	virtual void Sprite3DRender_B() override;

private:
	std::unique_ptr<CTitleWidget>			m_pTitleWidget;				// UI.
};