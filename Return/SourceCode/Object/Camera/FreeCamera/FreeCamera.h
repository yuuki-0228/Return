#pragma once
#include "..\CameraBase.h"
#include "..\..\..\Utility\Constant\Constant.h"

/************************************************
*	���R�ɓ�������J�����N���X.
*		���c�F�P.
**/
class CFreeCamera final
	: public CCameraBase
{
public:
	CFreeCamera();
	~CFreeCamera();

	// �X�V.
	virtual void Update( const float& DeltaTime ) override;

private:
	D3DXPOSITION3	m_CameraPosition;	// �J�����̍��W.
	D3DXVECTOR3		m_MoveVec;			// �ړ��x�N�g��.
	D3DXVECTOR3		m_SideMoveVec;		// ���ړ��x�N�g��.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SFreeCamera& CONSTANT = CConstant::GetFreeCamera();
};
