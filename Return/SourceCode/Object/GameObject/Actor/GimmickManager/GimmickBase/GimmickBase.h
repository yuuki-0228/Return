#pragma once
#include "GimmickStruct.h"
#include "..\..\Actor.h"
#include "..\..\..\..\..\Common\Sprite\Sprite.h"

/**************************************************
*	�X�e�[�W�M�~�b�N�x�[�X�N���X.
*		���c�F�P.
**/
class CGimmickBase
	: public CActor
{
public:
	CGimmickBase();
	virtual ~CGimmickBase();

	// �X�V.
	virtual void Update( const float& DeltaTime ) override final;

	// �`��.
	virtual void Render() override final;

	// �M�~�b�N�̔z�u.
	void Arrangement( const D3DXPOSITION3& Pos, const SGimmickState& State );

protected:
	// �M�~�b�N�̍X�V.
	virtual void GimmickUpdate() {};

protected:
	CSprite*			m_pSprite;				// �X�e�[�W�M�~�b�N�̉摜.
	SSpriteRenderState	m_SpriteState;			// �X�e�[�W�M�~�b�N�̉摜�̏��.
	SGimmickState		m_StartGimmickState;	// �J�n���̃M�~�b�N�̏��.
	SGimmickState		m_GimmickState;			// �M�~�b�N�̏��.
};
