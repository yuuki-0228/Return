#pragma once
#include "..\Actor.h"

class CGimmickManager;
class CEnemyManager;
class CBulletManager;

/************************************************
*	�X�e�[�W�}�l�[�W���[.
*		���c�F�P.
**/
class CStageManager
	: public CActor
{
public:
	CStageManager();
	virtual ~CStageManager();

	// ������.
	virtual bool Init() override;

	// �X�V.
	virtual void Update( const float& DeltaTime ) override;
	// �����蔻��I����Ăяo�����X�V.
	virtual void LateUpdate( const float& DeltaTime ) override;

	// �`��.
	virtual void Render() override;

private:
	// �`�惊�X�g�̍X�V.
	void RenderListUpdate();

private:
	std::vector<std::pair<float, CActor*>>	m_pRenderList;		// �`�惊�X�g.

private:
	// �O���f�[�^�̒萔.
	ConstantStruct::stStage CONSTANT = CConstant::GetStage();
};