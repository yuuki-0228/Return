#pragma once
#include "..\..\Actor.h"
#include "PotionAreaStruct.h"

/************************************************
*	�|�[�V�����G���A�N���X.
**/
class CPotionArea final
	: public CActor
{
public:
	CPotionArea();
	virtual ~CPotionArea();
	
	// ������.
	virtual bool Init() override;

	// �X�V.
	virtual void Update( const float& DeltaTime ) override;
	// �����蔻��I����Ăяo�����X�V.
	virtual void LateUpdate( const float& DeltaTime ) override;

	// �`��.
	virtual void Render() override;

	// �����蔻��֐�.
	virtual void Collision( CActor* pActor ) override;

	// �|�[�V�����G���A�̍쐬.
	void CreateArea( const D3DXPOSITION3& Pos, const float Radius, const SPotionAreaState& State );

	// ���S�ɏI���������擾.
	bool GetIsEnd() const { return m_IsEnd; }

protected:
	// �����蔻��̏�����.
	//	Init�֐��̍Ō�ɌĂ�.
	virtual void InitCollision();
	// �����蔻��̍X�V.
	//	Update�֐��̍Ō�ɌĂ�.
	virtual void UpdateCollision();

private:
	// �|�[�V�����G�t�F�N�g�̍X�V.
	void PotionEffectUpdate();

	// �G�t�F�N�g�̕t�^���Ԃ̍X�V.
	void EffectTimeUpdate();
	// �G�t�F�N�g�̕t�^���Ԃ̃��Z�b�g.
	void EffectTimeReset();

	// �G�t�F�N�g�̍Đ�.
	void EffectPlay( const D3DXPOSITION3& Pos, const bool IsStart = false );
	// �g�p���Ă��Ȃ��G�t�F�N�g�̍폜.
	void EffectDelete();

	// �G�t�F�N�g�����S�ɏI���������m�F.
	bool EndCheck();
	// �|�[�V�����G���A���I���������m�F.
	void PotionAreaEndCheck();

private:
	CSprite*								m_pEffectSprite;		// �G�t�F�N�g�摜.
	SSpriteRenderState						m_EffectStateBase;		// �G�t�F�N�g�̏��x�[�X.
	std::vector<SSpriteRenderState>			m_EffectStateList;		// �G�t�F�N�g�̏�񃊃X�g.
	SPotionAreaState						m_StartAreaState;		// �J�n���̃|�[�V�����G���A�̏��.
	SPotionAreaState						m_AreaState;			// �|�[�V�����G���A�̏��.
	std::vector<std::pair<float, CActor*>>	m_pRenderList;			// �`�惊�X�g.
	float									m_CollRadius;			// �����蔻��̔��a.
	float									m_PotionEffectTimeCnt;	// �|�[�V�����G�t�F�N�g�̑҂����ԗp�J�E���g.
	float									m_PlayerCoolTime;		// �v���C���[�̃N�[���^�C��.
	bool									m_IsEnd;				// ���S�ɏI��������.
	bool									m_IsHit;				// �����蔻�肪����������.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SPotionArea& CONSTANT = CConstant::GetPotionArea();
};