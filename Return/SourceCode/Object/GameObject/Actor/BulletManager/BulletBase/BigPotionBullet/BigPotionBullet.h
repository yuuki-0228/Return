#pragma once
#include "..\BulletBase.h"

/************************************************
*	�傫���|�[�V�����̒e�N���X.
**/
class CBigPotionBullet final
	: public CBulletBase
{
public:
	CBigPotionBullet();
	~CBigPotionBullet();

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

	// �e���폜���邩�m�F.
	virtual bool DeleteCheck( const EObjectTag DeleteTag ) override;
	// �X�e�[�W����]����悤�ɑł��Ԃ��邩�̊m�F.
	virtual bool RotationHitBackCheck( const int AttackNo ) override;
	// �}�E�X�J�[�\���̕����ɑł��Ԃ�.
	virtual bool AttackHitBackCheck( const int AttackNo ) override;

	// ���x���G�t�F�N�g�̍X�V.
	virtual void LvEffectUpdate() override;

private:
	float m_PotionTiem;	// �|�[�V�����̌��ʎ���.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SBigPotionBullet& SKILL_CONSTANT = CConstant::GetBigPotionBullet();
};