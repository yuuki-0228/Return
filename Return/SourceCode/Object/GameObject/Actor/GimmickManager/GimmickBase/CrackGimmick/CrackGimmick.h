#pragma once
#include "..\GimmickBase.h"

/************************************************
*	����M�~�b�N.
**/
class CCrackGimmick final
	: public CGimmickBase
{
public:
	CCrackGimmick();
	~CCrackGimmick();

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

	// �M�~�b�N�̍X�V.
	virtual void GimmickUpdate() override;
};