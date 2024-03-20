#pragma once
#include "..\BulletBase.h"

/**************************************************
*	�ʏ�̒e�N���X.
*		���c�F�P.
**/
class CNormalBullet
	: public CBulletBase
{
public:
	CNormalBullet();
	virtual ~CNormalBullet();

	// ������.
	virtual bool Init() override;
	
protected:
	// �����蔻��̏�����.
	//	Init�֐��̍Ō�ɌĂ�.
	virtual void InitCollision() override;

	// �e�̏�Ԃ̏�����.
	virtual	void InitBulletState( const SBulletState& State ) override;

	// ���x���G�t�F�N�g�̍X�V.
	virtual void LvEffectUpdate() override;
};
