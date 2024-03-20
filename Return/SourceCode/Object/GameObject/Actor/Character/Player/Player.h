#pragma once
#include "..\Character.h"
#include "AttackEffect\AttackEffect.h"

class CBullet;
class CStrongBullet;

/**************************************************
*	プレイヤークラス.
*		﨑田友輝.
**/
class CPlayer final
	: public CCharacter
{
public:
	CPlayer();
	virtual ~CPlayer();

	// 初期化.
	virtual bool Init() override;

	// 更新.
	virtual void Update( const float& DeltaTime ) override;
	// 当たり判定終了後呼び出される更新.
	virtual void LateUpdate( const float& DeltaTime ) override;
	// デバックの更新.
	virtual void DebugUpdate( const float& DeltaTime ) override;

	// 描画.
	virtual void Render() override;

	// 当たり判定関数.
	virtual void Collision( CActor* pActor ) override;

protected:
	// 当たり判定の初期化.
	//	Init関数の最後に呼ぶ.
	virtual void InitCollision() override;
	// 当たり判定の更新.
	//	Update関数の最後に呼ぶ.
	virtual void UpdateCollision() override;

private:
	// 弾のダメージを受けるかの当たり判定.
	void DamageCollision( CActor* pActor );
	// 弾の攻撃の当たり判定.
	void AttackCollision( CActor* pActor );

	// 攻撃によるスタミナの減少.
	void AttackStaminaDown();
	// スタミナの回復.
	void RecoveryStamina();

	// クールタイムの終了させる.
	void CoolTimeEnd();
	// 攻撃を当てた時のクールタイムの終了処理.
	void HitCoolTimeEnd();

	// 空振り.
	void MissAttack();

private:
	std::unique_ptr<CAttackEffect>			m_pAttackEffect;		// 攻撃エフェクト.
	std::vector<std::unique_ptr<CSphere2D>>	m_pAttackCollList;		// 攻撃用の当たり判定リスト.
	int										m_HitAttackNo;			// 攻撃をあてた攻撃の種類.
	bool									m_IsAttackCoolTime;		// 攻撃のクールタイムを設けるか

private:
	// 外部データの定数.
	const ConstantStruct::SPlayer& CONSTANT = CConstant::GetPlayer();
};
