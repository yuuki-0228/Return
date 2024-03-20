#pragma once
#include "GimmickStruct.h"
#include "..\..\Actor.h"
#include "..\..\..\..\..\Common\Sprite\Sprite.h"

/**************************************************
*	ステージギミックベースクラス.
*		﨑田友輝.
**/
class CGimmickBase
	: public CActor
{
public:
	CGimmickBase();
	virtual ~CGimmickBase();

	// 更新.
	virtual void Update( const float& DeltaTime ) override final;

	// 描画.
	virtual void Render() override final;

	// ギミックの配置.
	void Arrangement( const D3DXPOSITION3& Pos, const SGimmickState& State );

protected:
	// ギミックの更新.
	virtual void GimmickUpdate() {};

protected:
	CSprite*			m_pSprite;				// ステージギミックの画像.
	SSpriteRenderState	m_SpriteState;			// ステージギミックの画像の情報.
	SGimmickState		m_StartGimmickState;	// 開始時のギミックの情報.
	SGimmickState		m_GimmickState;			// ギミックの情報.
};
