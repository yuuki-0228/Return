#pragma once
#include "..\..\..\..\..\..\Common\Sprite\Sprite.h"
#include "..\..\..\..\..\..\Utility\Constant\Constant.h"

/************************************************
*	�^�C�~���O�o�[��UI�N���X.
*		���c�F�P.
**/
class CTimingBarUI final
{
public:
	using RenderStateList		= std::vector<SSpriteRenderState>;
	using DeleteIconStateList	= std::vector<std::pair<SSpriteRenderState, int>>;

public:
	CTimingBarUI();
	virtual ~CTimingBarUI();

	// �C���X�^���X�̎擾.
	static CTimingBarUI* GetInstance();

	// ������.
	static bool Init();

	// �X�V.
	static void Update( const float& DeltaTime );

	// �`��.
	static void Render();

	// �ړ�����A�C�R���̒ǉ�.
	static void PushIcon( const float Distance, const bool IsStrong = false, const bool IsItem = false );
	// �폜�A�C�R���̒ǉ�.
	static void PushDeleteIcon( const int TypeNo, const float Distance );

private:
	// �폜�A�C�R���̍쐬.
	static void CreateDeleteIcon( const int TypeNo, const float x );

protected:
	CSprite*				m_pBarSprite;			// �^�C�~���O�o�[�̉摜.
	CSprite*				m_pMoveIconSprite;		// �ړ�����A�C�R���̉摜.
	std::vector<CSprite*>	m_pDeleteIconList;		// �폜�A�C�R��.
	SSpriteRenderState		m_BarState;				// �^�C�~���O�o�[�̏��.
	SSpriteRenderState		m_MoveIconStateBase;	// �ړ�����A�C�R���̏��x�[�X.
	SSpriteRenderState		m_DeleteIconStateBase;	// �폜�A�C�R���̏��x�[�X.
	DeleteIconStateList		m_DeleteIconStateList;	// �폜�A�C�R���̏��.
	RenderStateList			m_LeftMoveIconState;	// �ړ�����A�C�R���̏��.
	RenderStateList			m_RightMoveIconState;	// �ړ�����A�C�R���̏��.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SPlayer& CONSTANT = CConstant::GetPlayer();
};
