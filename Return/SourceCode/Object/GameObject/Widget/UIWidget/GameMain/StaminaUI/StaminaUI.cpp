#include "StaminaUI.h"
#include "..\..\..\..\Actor\Character\Player\Player.h"
#include "..\..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"

namespace {
	// �Q�[�W�̐F.
	const D3DXCOLOR4 GAUGE_COLOR = Color4::RGBA( 0.35f, 0.66f, 1.0f );
}

CStaminaUI::CStaminaUI()
	: m_pPlayer				( nullptr )
	, m_pStaminaBack		( nullptr )
	, m_pStaminaFrame		( nullptr )
	, m_StaminaBackState	()
	, m_StaminaGaugeState	()
	, m_StaminaFrameState	()
{
}

CStaminaUI::~CStaminaUI()
{
}

//---------------------------.
// ������.
//---------------------------.
bool CStaminaUI::Init()
{
	// �X�^�~�i�̉摜���擾.
	m_pStaminaBack		= CSpriteResource::GetSprite( "StaminaBack",  &m_StaminaBackState );
	m_pStaminaFrame		= CSpriteResource::GetSprite( "StaminaFrame", &m_StaminaFrameState );
	m_StaminaGaugeState = m_StaminaBackState;

	// �\���ʒu�̐ݒ�.
	m_StaminaBackState.Transform.Position.x		= FWND_W / 2.0f;
	m_StaminaBackState.Transform.Position.y		= FWND_H - CONSTANT.STAMINA_UI_POS_Y;
	m_StaminaGaugeState.Transform.Position.x	= FWND_W / 2.0f;
	m_StaminaGaugeState.Transform.Position.y	= FWND_H - CONSTANT.STAMINA_UI_POS_Y;
	m_StaminaFrameState.Transform.Position.x	= FWND_W / 2.0f;
	m_StaminaFrameState.Transform.Position.y	= FWND_H - CONSTANT.STAMINA_UI_POS_Y;

	// �F�̐ݒ�.
	m_StaminaBackState.Color	= Color4::DarkGray;
	m_StaminaGaugeState.Color	= GAUGE_COLOR;
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CStaminaUI::Update( const float& DeltaTime )
{
	const float	NowStamina		= m_pPlayer->GetStamina();
	const float StaminaRatio	= NowStamina / CONSTANT.STAMINA_MAX;

	// ���݂̃X�^�~�i�ɑΉ������T�C�Y�ɍX�V.
	m_StaminaGaugeState.Transform.Scale.x = StaminaRatio;

	// �Q�[�W�̐F��ύX.
	const D3DXCOLOR4& GaugeColor = NowStamina >= CONSTANT.ATTACK_STAMINA ? GAUGE_COLOR : Color4::Red;
	m_StaminaGaugeState.Color = GaugeColor;
}

//---------------------------.
// �`��.
//---------------------------.
void CStaminaUI::Render()
{
	m_pStaminaBack->RenderUI( &m_StaminaBackState );
	m_pStaminaBack->RenderUI( &m_StaminaGaugeState );
	m_pStaminaFrame->RenderUI( &m_StaminaFrameState );
}