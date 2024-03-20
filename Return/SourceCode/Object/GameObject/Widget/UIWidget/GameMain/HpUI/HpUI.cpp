#include "HpUI.h"
#include "..\..\..\..\Actor\Character\Player\Player.h"
#include "..\..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"

namespace {
	constexpr float UV_MIN			= 0.15f;	// UV�̍ŏ��l.
	constexpr float SCALE_ANIM_SIZE = 10.0f;	// UV�̍ŏ��l.
}

CHpUI::CHpUI()
	: m_pPlayer			( nullptr )
	, m_pHpBack			( nullptr )
	, m_pHpFrame		( nullptr )
	, m_HpBackState		()
	, m_HpFrameState	()
	, m_AnimAng			( INIT_FLOAT )
{
}

CHpUI::~CHpUI()
{
}

//---------------------------.
// ������.
//---------------------------.
bool CHpUI::Init()
{
	// HP�̉摜���擾.
	m_pHpBack	= CSpriteResource::GetSprite( "HpBack",  &m_HpBackState );
	m_pHpFrame	= CSpriteResource::GetSprite( "HpFrame", &m_HpFrameState );

	// �}�X�N�摜��ݒ�.
	m_pHpBack->SetMaskTexture( CSpriteResource::GetSprite( "HpMask" )->GetTexture() );

	// �\���ʒu�̐ݒ�.
	m_HpBackState.Transform.Position.x		= FWND_W / 2.0f;
	m_HpBackState.Transform.Position.y		= FWND_H - CONSTANT.HP_UI_POS_Y;
	m_HpFrameState.Transform.Position.x		= FWND_W / 2.0f;
	m_HpFrameState.Transform.Position.y		= FWND_H - CONSTANT.HP_UI_POS_Y;

	// �A�j���[�V��������悤�ɐݒ�.
	m_HpBackState.AnimState.UV.y			= CONSTANT.HP_UV_Y;
	m_HpBackState.AnimState.IsAnimationX	= true;
	m_HpBackState.AnimState.FrameTime		= 5;
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CHpUI::Update( const float& DeltaTime )
{
	const int	NowHp	= m_pPlayer->GetHp();
	const float HpRatio = static_cast<float>( NowHp ) / CONSTANT.HP_MAX;

	// HP���Ȃ��ꍇHP����ɕύX.
	if ( NowHp == 0 ) {
		m_HpBackState.AnimState.UV.y			= 0.0f;
		m_HpBackState.AnimState.IsAnimationX	= false;
		return;
	}

	// ���݂�HP�ɑΉ�����UV�̈ʒu�ɍX�V.
 	m_HpBackState.AnimState.UV.y = ( CONSTANT.HP_UV_Y - UV_MIN ) * HpRatio + UV_MIN;

	// �A�j���[�V�����p�̊p�x�𑝂₷.
	m_AnimAng += 3.0f * ( 2.0f - HpRatio );
	if ( m_AnimAng >= Math::DEGREE_MID ) m_AnimAng -= Math::DEGREE_MID;

	// HP���g�k����.
	const float Scale = STransform::NORMAL_SCALE + sinf( Math::ToRadian( m_AnimAng ) ) / SCALE_ANIM_SIZE;
	m_HpBackState.Transform.Scale	= { Scale, Scale, Scale };
	m_HpFrameState.Transform.Scale	= { Scale, Scale, Scale };
}

//---------------------------.
// �`��.
//---------------------------.
void CHpUI::Render()
{
	m_pHpBack->RenderUI( &m_HpBackState );
	m_pHpFrame->RenderUI( &m_HpFrameState );
}