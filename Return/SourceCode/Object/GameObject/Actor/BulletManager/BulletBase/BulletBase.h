#pragma once
#include "BulletStruct.h"
#include "..\..\Actor.h"
#include "..\..\..\..\..\Common\Sprite\Sprite.h"

/**************************************************
*	�e�x�[�X�N���X.
*		���c�F�P.
**/
class CBulletBase
	: public CActor
{
public:
	CBulletBase();
	virtual ~CBulletBase();

	// �X�V.
	virtual void Update( const float& DeltaTime ) override final;

	// �`��.
	virtual void Render() override final;
	
	// �I���������擾.
	bool GetIsEnd() const { return m_IsEnd; }
	// �G�Ƀ_���[�W��^���邱�Ƃ̂ł���e���擾.
	bool GetIsDamageBullet() const { return m_HitBackCnt > 0; }
	// ��]���Ĉړ����Ă��邩�擾.
	bool GetIsRotationMove() const { return m_IsRotationMove; }

	// �e�̃^�O���擾.
	EObjectTag GetBulletTag() const { return m_BulletState.BulletTag; }

	// �v���C���[�ɑ΂��Ẵ_���[�W���擾.
	int GetPlayerDamage() const { return m_BulletState.PlayerDamage; }
	// �U���͂̎擾.
	float GetAttack() const { return m_BulletState.Attack; }
	// �e�̏����擾.
	SBulletState GetBulletState() const { return m_BulletState; }
	// �ړ��x�N�g���̎擾.
	D3DXVECTOR3 GetMoveVecotr() const { return m_MoveVector; }

	// �e�̃��x���̐ݒ�.
	bool SetBulletLv( const int Lv );
	// �e�̃��x�����グ��.
	bool BulletLvUp();

	// �X�e�[�W����]����悤�ɔ���.
	void StageRotationShot( const D3DXPOSITION3& Pos, const SBulletState& State );
	// �ʏ�̔���.
	void NormalShot( const D3DXPOSITION3& Pos, const SBulletState& State );
	// ��������悤�ɔ���.
	void DropShot( const D3DXPOSITION3& Pos, const SBulletState& State );
	// �p�x�ɂ����������ɒe�𔭎˂���( �オ0�x ).
	void DirectionShot( const D3DXPOSITION3& Pos, float Ang, const SBulletState& State );

	// �X�e�[�W����]����悤�ɑł��Ԃ�.
	void RotationHitBack( const int AttackNo );
	// �}�E�X�J�[�\���̕����ɑł��Ԃ�.
	void AttackHitBack( const int AttackNo );

	// �e������.
	void Delete( const EObjectTag DeleteTag = EObjectTag::None, const bool CheckSkip = false, const bool IsEffectPlay = true );

protected:
	// �e�̏�Ԃ̏�����.
	virtual	void InitBulletState( const SBulletState& State ) = 0;

	// �e���폜���邩�m�F.
	virtual bool DeleteCheck( const EObjectTag DeleteTag ) { return true; }
	// �X�e�[�W����]����悤�ɑł��Ԃ��邩�̊m�F.
	virtual bool RotationHitBackCheck( const int AttackNo ) { return true; }
	// �}�E�X�J�[�\���̕����ɑł��Ԃ�.
	virtual bool AttackHitBackCheck( const int AttackNo ) { return true; }

	// �e�̃X�L���̍X�V.
	virtual void SkillUpdate() {}
	// ���x���G�t�F�N�g�̍X�V.
	virtual void LvEffectUpdate() = 0;

private:
	// �e����]������.
	void BulletRotation();

	// �ʏ�̈ړ�.
	void NormalMove();
	// ��������e�̈ړ�.
	void DropMove();
	// �X�e�[�W����]����ړ�( ���v��� ).
	void LeftRotationMove();
	// �X�e�[�W����]����ړ�( �����v��� ).
	void RightRotationMove();

	// �G�t�F�N�g�̍X�V.
	void EffectUpdate();

	// �X�e�[�W�O���`�F�b�N.
	void StageOutCheck();
	// �e�̓��삪���S�ɏI���������m�F.
	void EndCheck();

protected:
	CSprite*							m_pSprite;				// �e�̉摜.
	SSpriteRenderState					m_SpriteState;			// �e�̉摜�̏��.
	D3DXVECTOR3							m_MoveVector;			// �ړ��x�N�g��.
	std::vector<CSprite*>				m_pEffectSpriteList;	// �G�t�F�N�g�̉摜���X�g.
	std::vector<CSprite*>				m_pLvEffectSpriteList;	// �e�̃��x���G�t�F�N�g�̉摜���X�g.
	std::vector<SSpriteRenderState>		m_EffectStateList;		// �G�t�F�N�g�̉摜�̏�񃊃X�g.
	std::vector<SSpriteRenderState>		m_LvEffectStateList;	// �e�̃��x���G�t�F�N�g�̉摜�̏�񃊃X�g.
	SBulletState						m_BulletState;			// �e�̏��.
	SBulletState						m_StartBulletState;		// �J�n���̒e�̏��.
	EDirection							m_RotationDire;			// ��]����.
	float								m_RotationAng;			// ��]�p�̊p�x.
	float								m_RotationRadius;		// ��]�p�̔��a.
	int									m_HitBackCnt;			// ���˕Ԃ�����.
	int									m_BulletLv;				// �e�̃��x��.
	int									m_WeakAttackCnt;		// ��U����A���ł��ꂽ��.
	bool								m_IsDrop;				// ��������e��.
	bool								m_IsRotationMove;		// ��]����ړ����s����.
	bool								m_IsEnd;				// �I��������.

protected:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SBullet& CONSTANT = CConstant::GetBullet();
};
