#include "GameMainWidget.h"
#include "..\..\UIWidget\GameMain\TimingBarUI\TimingBarUI.h"
#include "..\..\UIWidget\GameMain\ValueRenderUI\ValueRenderUI.h"
#include "..\..\UIWidget\GameMain\HpUI\HpUI.h"
#include "..\..\UIWidget\GameMain\StaminaUI\StaminaUI.h"
#include "..\..\..\..\..\Common\SoundManeger\SoundManeger.h"

CGameMainWidget::CGameMainWidget()
	: m_pFrame		( nullptr )
	, m_pHpUI		( nullptr )
	, m_pStaminaUI	( nullptr )
{
}

CGameMainWidget::~CGameMainWidget()
{
}

//---------------------------.
// 初期化.
//---------------------------.
bool CGameMainWidget::Init()
{
	SetSceneName( ESceneList::GameMain );
	GetSpriteList();

	// 枠の取得.
	m_pFrame = CSpriteResource::GetSprite( "Frame" );

	// タイミングバーの初期化.
	CTimingBarUI::Init();

	// HPの初期化.
	m_pHpUI = std::make_unique<CHpUI>();
	m_pHpUI->Init();

	// スタミナの初期化.
	m_pStaminaUI = std::make_unique<CStaminaUI>();
	m_pStaminaUI->Init();

	// ダメージの表示の初期化.
	CValueRenderUI::Init();
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CGameMainWidget::Update( const float& DeltaTime )
{
	CTimingBarUI::Update( DeltaTime );
	m_pHpUI->Update( DeltaTime );
	m_pStaminaUI->Update( DeltaTime );
	CValueRenderUI::Update( DeltaTime );
}

//---------------------------.
// 描画.
//---------------------------.
void CGameMainWidget::Render()
{
	m_pFrame->RenderUI();
	CTimingBarUI::Render();
	m_pHpUI->Render();
	m_pStaminaUI->Render();
	CValueRenderUI::Render();
}

//---------------------------.
// プレイヤーの設定.
//---------------------------.
void CGameMainWidget::SetPlayer( CPlayer* pPlayer )
{
	m_pHpUI->SetPlayer( pPlayer );
	m_pStaminaUI->SetPlayer( pPlayer );
}
