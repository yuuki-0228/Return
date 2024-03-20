#pragma once
#include "EnemyStruct.h"
#include "..\..\Character.h"

/**************************************************
*	�G�N���X.
*		���c�F�P.
**/
class CEnemy final
	: public CCharacter
{
public:
	CEnemy();
	virtual ~CEnemy();

	// ������.
	virtual bool Init() override;

	// �X�V.
	virtual void Update( const float& DeltaTime ) override;

	// �`��.
	virtual void Render() override;

	// �����蔻��֐�.
	virtual void Collision( CActor* pActor ) override;

	// �X�|�[��������.
	void Spown( const D3DXPOSITION3& Pos, const SEnemyState State );

protected:
	// �����蔻��̏�����.
	//	Init�֐��̍Ō�ɌĂ�.
	virtual void InitCollision() override;
	// �����蔻��̍X�V.
	//	Update�֐��̍Ō�ɌĂ�.
	virtual void UpdateCollision() override;

private:
	// �_���[�W���󂯂邩�̓����蔻��.
	void DamageCollision( CActor* pActor );

	// �N�[���^�C���̍X�V.
	void CoolTimeUpdate();

	// �G�̍s��.
	void Action();

	// �U��.
	void Attack();
	// �U�����s�����m�F.
	bool AttackCheck();
	
	// �ҋ@.
	void Wait();

	// �ړ�.
	void Move();

	// �s���̃��[�v.
	void ActionLoop();

private:
	std::vector<CSprite*>			m_pHpBarList;				// HP�o�[�̉摜���X�g.
	std::vector<SSpriteRenderState>	m_HpBarStateList;			// HP�o�[�̏�񃊃X�g.
	SEnemyState						m_StartEnemyState;			// �J�n���̓G�̏��.
	SEnemyState						m_EnemyState;				// �G�̏��.
	float							m_StrongDamageCoolTimeCnt;	// �����e�p�̃N�[���^�C��.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SEnemy& CONSTANT = CConstant::GetEnemy();
};
