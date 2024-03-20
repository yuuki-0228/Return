#pragma once
#include "..\..\..\Global.h"
#include "..\..\..\Common\Sprite\Sprite.h"

/************************************************
*	ロード中の描画クラス.
*		﨑田友輝.
**/
class CLoadRender final
{
public:
	CLoadRender();
	~CLoadRender();

	// 読み込み.
	HRESULT Load();

	// 更新.
	void Update();
	// 描画.
	void Render();

private:
	std::unique_ptr<CSprite>	m_pSpriteBack;	// 背景画像.
	std::unique_ptr<CSprite>	m_pSpriteText;	// "Now Loading"画像.
	std::unique_ptr<CSprite>	m_pSpriteDot;	// "..."画像.
	std::unique_ptr<CSprite>	m_pSpriteUFO;	// UFO画像.
	std::unique_ptr<CSprite>	m_pMask;		// マスク画像.
	SSpriteRenderState			m_DotState;		// "..."の情報.
	SHORT						m_DotAnimCnt;	// "..."のアニメーション用カウント.
};
