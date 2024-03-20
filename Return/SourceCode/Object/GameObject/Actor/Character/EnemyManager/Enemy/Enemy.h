#pragma once
#include "EnemyStruct.h"
#include "..\..\Character.h"

/**************************************************
*	敵クラス.
*		﨑田友輝.
**/
class CEnemy final
	: public CCharacter
{
public:
	CEnemy();
	virtual ~CEnemy();

	// 初期化.
	virtual bool Init() override;

	// 更新.
	virtual void Update( const float& DeltaTime ) override;

	// 描画.
	virtual void Render() override;

	// 当たり判定関数.
	virtual void Collision( CActor* pActor ) override;

	// スポーンさせる.
	void Spown( const D3DXPOSITION3& Pos, const SEnemyState State );

protected:
	// 当たり判定の初期化.
	//	Init関数の最後に呼ぶ.
	virtual void InitCollision() override;
	// 当たり判定の更新.
	//	Update関数の最後に呼ぶ.
	virtual void UpdateCollision() override;

private:
	// ダメージを受けるかの当たり判定.
	void DamageCollision( CActor* pActor );

	// クールタイムの更新.
	void CoolTimeUpdate();

	// 敵の行動.
	void Action();

	// 攻撃.
	void Attack();
	// 攻撃を行うか確認.
	bool AttackCheck();
	
	// 待機.
	void Wait();

	// 移動.
	void Move();

	// 行動のループ.
	void ActionLoop();

private:
	std::vector<CSprite*>			m_pHpBarList;				// HPバーの画像リスト.
	std::vector<SSpriteRenderState>	m_HpBarStateList;			// HPバーの情報リスト.
	SEnemyState						m_StartEnemyState;			// 開始時の敵の情報.
	SEnemyState						m_EnemyState;				// 敵の情報.
	float							m_StrongDamageCoolTimeCnt;	// 強い弾用のクールタイム.

private:
	// 外部データの定数.
	const ConstantStruct::SEnemy& CONSTANT = CConstant::GetEnemy();
};
