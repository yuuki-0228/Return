#pragma once
#include "..\BulletBase.h"

/************************************************
*	落下する弾をこぼしながら飛ぶ弾クラス.
*		﨑田友輝.
**/
class CSpillBullet final
	: public CBulletBase
{
public:
	CSpillBullet();
	virtual ~CSpillBullet();

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
	float	SpownTimeCnt;		// 弾を召喚する用のカウント.

private:
	// 外部データの定数.
	const ConstantStruct::SSpillBullet& SKILL_CONSTANT = CConstant::GetSpillBullet();
};