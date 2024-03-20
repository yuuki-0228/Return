#pragma once
#include "..\..\..\Actor.h"

namespace AttackNo{
	// �U���̔ԍ�.
	enum enAttackNo : unsigned char {
		None,

		Heavy,	// ���U��.
		Normal,	// �ʏ�U��.
		Weak,	// ��U��.

		Max,
	} typedef EAttackNo;
}

/************************************************
*	�U���G�t�F�N�g�N���X.
*		���c�F�P.
**/
class CAttackEffect final
	: public CActor
{
public:
	CAttackEffect();
	~CAttackEffect();

	// ������.
	virtual bool Init() override;

	// �X�V.
	virtual void Update( const float& DeltaTime ) override;

	// �`��.
	virtual void Render() override;

	// �G�t�F�N�g�̍Đ�.
	void EffectPlay( const int No );

	// �Ō�ɍĐ������G�t�F�N�g���I���������擾.
	bool GetIsEffectPlay() const { return *m_IsNowEffectPlay; }

private:
	// �g�p���Ă��Ȃ��G�t�F�N�g�̍폜.
	void EffectDelete();

private:
	std::vector<CSprite*>							m_pAttackSpriteList;	// �U���G�t�F�N�g���X�g.
	std::vector<SSpriteRenderState>					m_AttackStateBaseList;	// �U���G�t�F�N�g�̏��x�[�X���X�g.
	std::vector<std::vector<SSpriteRenderState>>	m_AttackStateList;		// �U���G�t�F�N�g�̏�񃊃X�g.
	std::vector<std::string>						m_AttackSEList;			// �U����SE���X�g.
	bool*											m_IsNowEffectPlay;		// ���݂̃G�t�F�N�g���Đ�����.
};
