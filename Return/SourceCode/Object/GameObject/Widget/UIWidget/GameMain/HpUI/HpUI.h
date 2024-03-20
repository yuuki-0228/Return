#pragma once
#include "..\..\UIWidget.h"

class CPlayer;

/************************************************
*	HP��UI�N���X.
**/
class CHpUI final
	: public CUIWidget
{
public:
	CHpUI();
	virtual ~CHpUI();

	// ������.
	virtual bool Init() override;

	// �X�V.
	virtual void Update( const float& DeltaTime ) override;

	// �`��.
	virtual void Render() override;

	// �v���C���[�̐ݒ�.
	void SetPlayer( CPlayer* pPlayer ) { m_pPlayer = pPlayer; }

private:
	CPlayer*			m_pPlayer;		// �v���C���[.
	CSprite*			m_pHpBack;		// HP�̔w�i.
	CSprite*			m_pHpFrame;		// HP�̃t���[��.
	SSpriteRenderState	m_HpBackState;	// HP�̔w�i���.
	SSpriteRenderState	m_HpFrameState;	// HP�̃t���[�����.
	float				m_AnimAng;		// �A�j���[�V�����p�̊p�x(�x).

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SPlayer& CONSTANT = CConstant::GetPlayer();
};