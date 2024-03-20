#pragma once
#include "..\BulletBase.h"

/************************************************
*	貫通弾クラス.
*		﨑田友輝.
**/
class CSharpBullet final
	: public CBulletBase
{
public:
	CSharpBullet();
	virtual ~CSharpBullet();

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

	// レベルエフェクトの更新.
	virtual void LvEffectUpdate() override;

private:
	// 外部データの定数.
	const ConstantStruct::SSharpBullet& SKILL_CONSTANT = CConstant::GetSharpBullet();
};