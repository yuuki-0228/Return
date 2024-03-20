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
// ������.
//---------------------------.
bool CGameMainWidget::Init()
{
	SetSceneName( ESceneList::GameMain );
	GetSpriteList();

	// �g�̎擾.
	m_pFrame = CSpriteResource::GetSprite( "Frame" );

	// �^�C�~���O�o�[�̏�����.
	CTimingBarUI::Init();

	// HP�̏�����.
	m_pHpUI = std::make_unique<CHpUI>();
	m_pHpUI->Init();

	// �X�^�~�i�̏�����.
	m_pStaminaUI = std::make_unique<CStaminaUI>();
	m_pStaminaUI->Init();

	// �_���[�W�̕\���̏�����.
	CValueRenderUI::Init();
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CGameMainWidget::Update( const float& DeltaTime )
{
	CTimingBarUI::Update( DeltaTime );
	m_pHpUI->Update( DeltaTime );
	m_pStaminaUI->Update( DeltaTime );
	CValueRenderUI::Update( DeltaTime );
}

//---------------------------.
// �`��.
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
// �v���C���[�̐ݒ�.
//---------------------------.
void CGameMainWidget::SetPlayer( CPlayer* pPlayer )
{
	m_pHpUI->SetPlayer( pPlayer );
	m_pStaminaUI->SetPlayer( pPlayer );
}
