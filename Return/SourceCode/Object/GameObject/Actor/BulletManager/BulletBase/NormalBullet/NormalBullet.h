#pragma once
#include "..\BulletBase.h"

/**************************************************
*	通常の弾クラス.
*		﨑田友輝.
**/
class CNormalBullet
	: public CBulletBase
{
public:
	CNormalBullet();
	virtual ~CNormalBullet();

	// 初期化.
	virtual bool Init() override;
	
protected:
	// 当たり判定の初期化.
	//	Init関数の最後に呼ぶ.
	virtual void InitCollision() override;

	// 弾の状態の初期化.
	virtual	void InitBulletState( const SBulletState& State ) override;

	// レベルエフェクトの更新.
	virtual void LvEffectUpdate() override;
};
