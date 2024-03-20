#pragma once
#include "..\BulletBase.h"

/**************************************************
*	ポーションの弾クラス.
*		﨑田友輝.
**/
class CPotionBullet
	: public CBulletBase
{
public:
	CPotionBullet();
	CPotionBullet( const bool IsBadPotion );
	virtual ~CPotionBullet();

	// 初期化.
	virtual bool Init() override;

protected:
	// 当たり判定の初期化.
	//	Init関数の最後に呼ぶ.
	virtual void InitCollision() override;

	// 弾の状態の初期化.
	virtual	void InitBulletState( const SBulletState& State ) override;

	// 弾を削除するか確認.
	virtual bool DeleteCheck( const EObjectTag DeleteTag ) override;
	// ステージを回転するように打ち返せるかの確認.
	virtual bool RotationHitBackCheck( const int AttackNo ) override;
	// マウスカーソルの方向に打ち返す.
	virtual bool AttackHitBackCheck( const int AttackNo ) override;

	// 弾のスキルの更新.
	virtual void SkillUpdate() override;
	// レベルエフェクトの更新.
	virtual void LvEffectUpdate() override;

private:
	int		m_DamageLv;		// ダメージ具合のLv.
	bool	m_IsBadPotion;	// 悪いポーションか.
};