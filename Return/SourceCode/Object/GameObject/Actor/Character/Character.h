#pragma once
#include "..\Actor.h"
#include "..\..\..\..\Common\Sprite\Sprite.h"

/************************************************
*	�L�����N�^�[�N���X.
*		���c�F�P.
**/
class CCharacter
	: public CActor
{
public:
	CCharacter();
	virtual ~CCharacter();

	// HP�̎擾.
	int GetHp() const { return m_Hp; }
	// �X�^�~�i�̎擾.
	float GetStamina() const { return m_Stamina; }

	// HP�̐ݒ�.
	void SetHp( const int HP );
	// �X�^�~�i�̐ݒ�.
	void SetStamina( const float Stamina );

	// �񕜂���.
	void HealHp( const float	Heal );
	void HealHp( const int		Heal );
	// �_���[�W���󂯂�.
	void DamageHp( const float	Damage );
	void DamageHp( const int	Damage );
	// �X�^�~�i�̒ǉ�.
	void AddStamina( const float Stamina );

protected:
	CSprite*			m_pSprite;		// �L�����N�^�[�̉摜.
	SSpriteRenderState	m_SpriteState;	// �L�����N�^�[�̉摜�̏��.
	int					m_Hp;			// HP.
	int					m_HpMax;		// HP�̏��.
	float				m_Stamina;		// �X�^�~�i.
	float				m_StaminaMax;	// �X�^�~�i.
};
