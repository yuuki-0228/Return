#pragma once
#include "..\BulletBase.h"

/**************************************************
*	�|�[�V�����̒e�N���X.
*		���c�F�P.
**/
class CPotionBullet
	: public CBulletBase
{
public:
	CPotionBullet();
	CPotionBullet( const bool IsBadPotion );
	virtual ~CPotionBullet();

	// ������.
	virtual bool Init() override;

protected:
	// �����蔻��̏�����.
	//	Init�֐��̍Ō�ɌĂ�.
	virtual void InitCollision() override;

	// �e�̏�Ԃ̏�����.
	virtual	void InitBulletState( const SBulletState& State ) override;

	// �e���폜���邩�m�F.
	virtual bool DeleteCheck( const EObjectTag DeleteTag ) override;
	// �X�e�[�W����]����悤�ɑł��Ԃ��邩�̊m�F.
	virtual bool RotationHitBackCheck( const int AttackNo ) override;
	// �}�E�X�J�[�\���̕����ɑł��Ԃ�.
	virtual bool AttackHitBackCheck( const int AttackNo ) override;

	// �e�̃X�L���̍X�V.
	virtual void SkillUpdate() override;
	// ���x���G�t�F�N�g�̍X�V.
	virtual void LvEffectUpdate() override;

private:
	int		m_DamageLv;		// �_���[�W���Lv.
	bool	m_IsBadPotion;	// �����|�[�V������.
};