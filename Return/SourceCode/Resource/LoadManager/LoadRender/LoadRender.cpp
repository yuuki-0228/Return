#include "LoadRender.h"
#include "..\..\..\Common\DirectX\DirectX11.h"

namespace{
	constexpr SHORT DOT_ANIMATION_SPEED = 30;
}

CLoadRender::CLoadRender()
	: m_pSpriteBack		( nullptr )
	, m_pSpriteText		( nullptr )
	, m_pSpriteDot		( nullptr )
	, m_pSpriteUFO		( nullptr )
	, m_pMask			( nullptr )
	, m_DotState		()
	, m_DotAnimCnt		( 0 )
{
	m_pSpriteBack	= std::make_unique<CSprite>();
	m_pSpriteText	= std::make_unique<CSprite>();
	m_pSpriteDot	= std::make_unique<CSprite>();
	m_pSpriteUFO	= std::make_unique<CSprite>();
	m_pMask			= std::make_unique<CSprite>();
}

CLoadRender::~CLoadRender()
{
}

//---------------------------.
// 読み込み.
//---------------------------.
HRESULT CLoadRender::Load()
{
	if ( m_pSpriteBack->Init(	"Data\\Sprite\\Scene\\Loading\\LoadBase.png"	) ) return E_FAIL;
	if ( m_pSpriteText->Init(	"Data\\Sprite\\Scene\\Loading\\LoadText.png"	) ) return E_FAIL;
	if ( m_pSpriteDot->Init(	"Data\\Sprite\\Scene\\Loading\\LoadDot.png"		) ) return E_FAIL;
	if ( m_pSpriteUFO->Init(	"Data\\Sprite\\Scene\\Loading\\LoadUFO.png"		) ) return E_FAIL;
	m_pSpriteBack->InitSceneSpriteState( "Load", 0 );
	m_pSpriteText->InitSceneSpriteState( "Load", 0 );
	m_pSpriteDot->InitSceneSpriteState(	 "Load", 0 );
	m_pSpriteUFO->InitSceneSpriteState(	 "Load", 0 );
	m_DotState = m_pSpriteDot->GetRenderState();
	m_DotState.AnimState.IsSetAnimNumber = true;

	// マスクの設定.
	m_pSpriteBack->SetMaskTexture(	m_pMask->GetTexture() );
	m_pSpriteText->SetMaskTexture(	m_pMask->GetTexture() );
	m_pSpriteDot->SetMaskTexture(	m_pMask->GetTexture() );
	m_pSpriteUFO->SetMaskTexture(	m_pMask->GetTexture() );

	return S_OK;
}

//---------------------------.
// 更新.
//---------------------------.
void CLoadRender::Update()
{
	SSpriteAnimState* AnimState = &m_DotState.AnimState;

	// カウントを増やす.
	m_DotAnimCnt++;
	if ( m_DotAnimCnt >= AnimState->AnimNumberMax * DOT_ANIMATION_SPEED )
		m_DotAnimCnt = 0;

	// "..."のパターンの設定.
	AnimState->AnimNumber = m_DotAnimCnt / DOT_ANIMATION_SPEED;
}

//---------------------------.
// 描画.
//---------------------------.
void CLoadRender::Render()
{
	CDirectX11::SetDepth( false );
	m_pSpriteBack->RenderUI();
	m_pSpriteText->RenderUI();
	m_pSpriteDot->RenderUI( &m_DotState );
	m_pSpriteUFO->RenderUI();
	CDirectX11::SetDepth( true );
}
