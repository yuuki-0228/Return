#pragma once
#include "..\FadeBase.h"

/************************************************
*	UFO型に切り抜かれたフェードクラス.
*		�ｱ田友輝.
**/
class CUFOFade final
	: public CFadeBase
{
public:
	CUFOFade();
	virtual ~CUFOFade();

	// 初期化.
	virtual bool Init() override;
	// 描画.
	virtual void Render() override;

	// フェードイン.
	virtual bool FadeIn( const float Speed ) override;
	// フェードアウト.
	virtual bool FadeOut( const float Speed ) override;

private:
	CSprite*			m_pSpriteUFO;	// UFO画像.
	SSpriteRenderState	m_UFOState;		// UFOの情報.
};