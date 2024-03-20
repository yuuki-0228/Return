#pragma once
#include "..\..\UIWidget.h"

class CPlayer;

/************************************************
*	�X�^�~�iUI�N���X.
**/
class CStaminaUI final
	: public CUIWidget
{
public:
	CStaminaUI();
	virtual ~CStaminaUI();

	// ������.
	virtual bool Init() override;

	// �X�V.
	virtual void Update( const float& DeltaTime ) override;

	// �`��.
	virtual void Render() override;

	// �v���C���[�̐ݒ�.
	void SetPlayer( CPlayer* pPlayer ) { m_pPlayer = pPlayer; }

private:
	CPlayer*			m_pPlayer;				// �v���C���[.
	CSprite*			m_pStaminaBack;			// �X�^�~�i�̔w�i.
	CSprite*			m_pStaminaFrame;		// �X�^�~�i�̃t���[��.
	SSpriteRenderState	m_StaminaBackState;		// �X�^�~�i�̔w�i���.
	SSpriteRenderState	m_StaminaGaugeState;	// �X�^�~�i�̃Q�[�W���.
	SSpriteRenderState	m_StaminaFrameState;	// �X�^�~�i�̃t���[�����.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SPlayer& CONSTANT = CConstant::GetPlayer();
};