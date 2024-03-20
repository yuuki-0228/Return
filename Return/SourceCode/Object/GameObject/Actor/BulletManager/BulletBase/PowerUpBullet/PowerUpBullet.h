#pragma once
#include "..\BulletBase.h"

/************************************************
*	�e�̃��x�����グ��e�N���X.
*		���c�F�P.
**/
class CPowerUpBullet final
	: public CBulletBase
{
public:
	CPowerUpBullet();
	virtual ~CPowerUpBullet();

	// �����蔻��I����Ăяo�����X�V.
	virtual void LateUpdate( const float& DeltaTime ) override;

	// ������.
	virtual bool Init() override;

	// �����蔻��֐�.
	virtual void Collision( CActor* pActor ) override;

protected:
	// �����蔻��̏�����.
	//	Init�֐��̍Ō�ɌĂ�.
	virtual void InitCollision() override;
	// �����蔻��̍X�V.
	//	Update�֐��̍Ō�ɌĂ�.
	virtual void UpdateCollision() override;

	// �e�̏�Ԃ̏�����.
	virtual	void InitBulletState( const SBulletState& State ) override;

	// �X�e�[�W����]����悤�ɑł��Ԃ��邩�̊m�F.
	virtual bool RotationHitBackCheck( const int AttackNo ) override;
	// �}�E�X�J�[�\���̕����ɑł��Ԃ�.
	virtual bool AttackHitBackCheck( const int AttackNo ) override;

	// �e�̃X�L���̍X�V.
	virtual void SkillUpdate() override;
	// ���x���G�t�F�N�g�̍X�V.
	virtual void LvEffectUpdate() override;

private:
	float	m_PowerUpCoolTime;	// �e����������N�[���^�C��.
	bool	m_IsPowerUp;		// �p���[�A�b�v��������.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SPowerUpBullet& SKILL_CONSTANT = CConstant::GetPowerUpBullet();
};