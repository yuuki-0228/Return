#pragma once
#include "..\GimmickBase.h"

/************************************************
*	壊れるギミック.
**/
class CCrackGimmick final
	: public CGimmickBase
{
public:
	CCrackGimmick();
	~CCrackGimmick();

	// 初期化.
	virtual bool Init() override;

	// 当たり判定関数.
	virtual void Collision( CActor* pActor ) override;

protected:
	// 当たり判定の初期化.
	//	Init関数の最後に呼ぶ.
	virtual void InitCollision() override;
	// 当たり判定の更新.
	//	Update関数の最後に呼ぶ.
	virtual void UpdateCollision() override;

	// ギミックの更新.
	virtual void GimmickUpdate() override;
};