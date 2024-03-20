#pragma once
#include "..\BulletBase.h"

/************************************************
*	��������e�����ڂ��Ȃ����Ԓe�N���X.
*		���c�F�P.
**/
class CSpillBullet final
	: public CBulletBase
{
public:
	CSpillBullet();
	virtual ~CSpillBullet();

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

	// �e�̃X�L���̍X�V.
	virtual void SkillUpdate() override;
	// ���x���G�t�F�N�g�̍X�V.
	virtual void LvEffectUpdate() override;

private:
	float	SpownTimeCnt;		// �e����������p�̃J�E���g.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SSpillBullet& SKILL_CONSTANT = CConstant::GetSpillBullet();
};