#include "TimingBarUI.h"
#include "..\..\..\..\Actor\Character\Player\AttackEffect\AttackEffect.h"
#include "..\..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"

CTimingBarUI::CTimingBarUI()
	: m_pBarSprite				( nullptr )
	, m_pMoveIconSprite			( nullptr )
	, m_pDeleteIconList			( AttackNo::Max )
	, m_BarState				()
	, m_MoveIconStateBase		()
	, m_DeleteIconStateBase		()
	, m_DeleteIconStateList		()
	, m_LeftMoveIconState		()
	, m_RightMoveIconState		()
{
}

CTimingBarUI::~CTimingBarUI()
{
}

//----------------------------.
// �C���X�^���X�̎擾.
//----------------------------.
CTimingBarUI* CTimingBarUI::GetInstance()
{
	static std::unique_ptr<CTimingBarUI> pInstance = std::make_unique<CTimingBarUI>();
	return pInstance.get();
}

//---------------------------.
// ������.
//---------------------------.
bool CTimingBarUI::Init()
{
	CTimingBarUI* pI = GetInstance();

	pI->m_LeftMoveIconState.clear();
	pI->m_RightMoveIconState.clear();

	// �^�C�~���O�o�[�̎擾.
	pI->m_pBarSprite = CSpriteResource::GetSprite( "TimingBar", &pI->m_BarState );

	// �^�C�~���O�o�[�̈ʒu�̐ݒ�.
	pI->m_BarState.Transform.Position.x = FWND_W / 2.0f;
	pI->m_BarState.Transform.Position.y = FWND_H - pI->CONSTANT.HP_UI_POS_Y;

	// �ړ�����A�C�R���̎擾.
	pI->m_pMoveIconSprite	= CSpriteResource::GetSprite( "MoveIcon", &pI->m_MoveIconStateBase );
	pI->m_MoveIconStateBase.AnimState.IsSetAnimNumber	= true;
	pI->m_MoveIconStateBase.Transform.Position.y		= FWND_H - pI->CONSTANT.HP_UI_POS_Y;

	// �폜�A�C�R���̎擾.
	pI->m_pDeleteIconList[AttackNo::Heavy]	= CSpriteResource::GetSprite( "HeavyHitEffect", &pI->m_DeleteIconStateBase );
	pI->m_pDeleteIconList[AttackNo::Normal] = CSpriteResource::GetSprite( "NormalHitEffect" );
	pI->m_pDeleteIconList[AttackNo::Weak]	= CSpriteResource::GetSprite( "WeakHitEffect"	);
	pI->m_DeleteIconStateBase.AnimState.IsAnimation		= true;
	pI->m_DeleteIconStateBase.AnimState.IsOneAnimPlay	= true;
	pI->m_DeleteIconStateBase.AnimState.FrameTime		= 5;
	pI->m_DeleteIconStateBase.Transform.Position.y		= FWND_H - pI->CONSTANT.HP_UI_POS_Y;
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CTimingBarUI::Update( const float& DeltaTime )
{
	CTimingBarUI* pI = GetInstance();

	// �g�p���Ă��Ȃ��G�t�F�N�g���폜����.
	const int Max = static_cast<int>( pI->m_DeleteIconStateList.size() ) - 1;
	for ( int i = Max; i >= 0; --i ) {
		if ( pI->m_DeleteIconStateList[i].first.AnimState.IsAnimation ) break;
		pI->m_DeleteIconStateList.pop_back();
	}

	// �I�������G�t�F�N�g���\���ɂ���.
	for ( auto& [State, No] : pI->m_DeleteIconStateList ) {
		if ( State.AnimState.IsAnimation ) continue;
		State.IsDisp = false;
	}
}

//---------------------------.
// �`��.
//---------------------------.
void CTimingBarUI::Render()
{
	CTimingBarUI* pI = GetInstance();

	// �o�[�̕\��.
	pI->m_pBarSprite->RenderUI( &pI->m_BarState );
	
	// �폜�A�C�R���̕\��.
	for ( auto& [State, No] : pI->m_DeleteIconStateList ) {
		pI->m_pDeleteIconList[No]->RenderUI( &State );
	}

	// �����A�C�R���̕\��.
	for ( auto& l : pI->m_LeftMoveIconState ) {
		pI->m_pMoveIconSprite->RenderUI( &l );
	}
	for ( auto& r : pI->m_RightMoveIconState ) {
		pI->m_pMoveIconSprite->RenderUI( &r );
	}

	// �ړ�����A�C�R���̏����폜����.
	pI->m_LeftMoveIconState.clear();
	pI->m_RightMoveIconState.clear();
}

//---------------------------.
// �ړ�����A�C�R���̒ǉ�.
//---------------------------.
void CTimingBarUI::PushIcon( const float Distance, const bool IsStrong, const bool IsItem )
{
	CTimingBarUI* pI = GetInstance();

	// �A�C�R���̏���ǉ�.
	pI->m_LeftMoveIconState.emplace_back( pI->m_MoveIconStateBase );
	pI->m_RightMoveIconState.emplace_back( pI->m_MoveIconStateBase );

	// �����ڂ̐ݒ�.
	if ( IsItem ) {
		pI->m_LeftMoveIconState.back().AnimState.AnimNumber		= 4;
		pI->m_RightMoveIconState.back().AnimState.AnimNumber	= 5;
	}
	else if ( IsStrong ) {
		pI->m_LeftMoveIconState.back().AnimState.AnimNumber		= 2;
		pI->m_RightMoveIconState.back().AnimState.AnimNumber	= 3;
	}
	else {
		pI->m_LeftMoveIconState.back().AnimState.AnimNumber		= 0;
		pI->m_RightMoveIconState.back().AnimState.AnimNumber	= 1;
	}

	// �\���ʒu�̐ݒ�.
	pI->m_LeftMoveIconState.back().Transform.Position.x		= FWND_W / 2.0f - Distance;
	pI->m_RightMoveIconState.back().Transform.Position.x	= FWND_W / 2.0f + Distance;
}

//---------------------------.
// �폜�A�C�R���̒ǉ�.
//---------------------------.
void CTimingBarUI::PushDeleteIcon( const int TypeNo, const float Distance )
{
	CTimingBarUI* pI = GetInstance();

	// ���E�ɃA�C�R���̏���ǉ�.
	CreateDeleteIcon( TypeNo, FWND_W / 2.0f - Distance );
	CreateDeleteIcon( TypeNo, FWND_W / 2.0f + Distance );
}

//---------------------------.
// �폜�A�C�R���̍쐬.
//---------------------------.
void CTimingBarUI::CreateDeleteIcon( const int TypeNo, const float x )
{
	CTimingBarUI* pI = GetInstance();

	// �g�p���Ă��Ȃ��̂�����Ύg�p����.
	for ( auto &[State, No] : pI->m_DeleteIconStateList ){
		if ( State.AnimState.IsAnimation ) continue;

		// �G�t�F�N�g�̍Đ�.
		State.IsDisp					= true;
		State.AnimState.IsAnimation		= true;
		State.Transform.Position.x		= x;
		State.AnimState.PatternNo.x		= 0;
		State.AnimState.PatternNo.y		= 0;
		No								= TypeNo;
		return;
	}

	// �V�����쐬.
	pI->m_DeleteIconStateList.emplace_back( std::make_pair( pI->m_DeleteIconStateBase, TypeNo ) );
	pI->m_DeleteIconStateList.back().first.Transform.Position.x = x;
}
