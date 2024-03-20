#pragma once
#include "..\Character.h"
#include "AttackEffect\AttackEffect.h"

class CBullet;
class CStrongBullet;

/**************************************************
*	�v���C���[�N���X.
*		���c�F�P.
**/
class CPlayer final
	: public CCharacter
{
public:
	CPlayer();
	virtual ~CPlayer();

	// ������.
	virtual bool Init() override;

	// �X�V.
	virtual void Update( const float& DeltaTime ) override;
	// �����蔻��I����Ăяo�����X�V.
	virtual void LateUpdate( const float& DeltaTime ) override;
	// �f�o�b�N�̍X�V.
	virtual void DebugUpdate( const float& DeltaTime ) override;

	// �`��.
	virtual void Render() override;

	// �����蔻��֐�.
	virtual void Collision( CActor* pActor ) override;

protected:
	// �����蔻��̏�����.
	//	Init�֐��̍Ō�ɌĂ�.
	virtual void InitCollision() override;
	// �����蔻��̍X�V.
	//	Update�֐��̍Ō�ɌĂ�.
	virtual void UpdateCollision() override;

private:
	// �e�̃_���[�W���󂯂邩�̓����蔻��.
	void DamageCollision( CActor* pActor );
	// �e�̍U���̓����蔻��.
	void AttackCollision( CActor* pActor );

	// �U���ɂ��X�^�~�i�̌���.
	void AttackStaminaDown();
	// �X�^�~�i�̉�.
	void RecoveryStamina();

	// �N�[���^�C���̏I��������.
	void CoolTimeEnd();
	// �U���𓖂Ă����̃N�[���^�C���̏I������.
	void HitCoolTimeEnd();

	// ��U��.
	void MissAttack();

private:
	std::unique_ptr<CAttackEffect>			m_pAttackEffect;		// �U���G�t�F�N�g.
	std::vector<std::unique_ptr<CSphere2D>>	m_pAttackCollList;		// �U���p�̓����蔻�胊�X�g.
	int										m_HitAttackNo;			// �U�������Ă��U���̎��.
	bool									m_IsAttackCoolTime;		// �U���̃N�[���^�C����݂��邩

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SPlayer& CONSTANT = CConstant::GetPlayer();
};
