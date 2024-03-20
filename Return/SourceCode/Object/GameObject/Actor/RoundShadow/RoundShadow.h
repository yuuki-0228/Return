#pragma once
#include "..\Actor.h"
#include "..\..\..\..\Global.h"
#include "..\..\..\..\Common\Sprite\Sprite.h"

/************************************************
*	丸影(スプライト)クラス.
*		﨑田友輝.
**/
class CRoundShadow final
	: public CActor
{
public:
	CRoundShadow( CActor* pObj, CStaticMesh* pModel, float vScale = 1.0f );
	virtual ~CRoundShadow();

	// 初期化.
	virtual bool Init() override;

	// 更新.
	virtual void Update( const float& DeltaTime ) override;
	// 描画.
	virtual void Render() override;

private:
	// 初期の影のサイズを取得.
	D3DXSCALE3 GetShadowScale( CStaticMesh* pModel, float vScale );

private:
	CSprite*			m_pSpriteShadow;	// 丸影の画像.
	SSpriteRenderState	m_ShadowState;		// 丸影の情報.
	CActor*				m_pObject;			// 影を付けるオブジェクト.

private:
	// デフォルトコンストラクタ禁止.
	CRoundShadow() = delete;
	CRoundShadow( const CRoundShadow& ) = delete;
	CRoundShadow& operator = ( const CRoundShadow& rhs ) = default;
};