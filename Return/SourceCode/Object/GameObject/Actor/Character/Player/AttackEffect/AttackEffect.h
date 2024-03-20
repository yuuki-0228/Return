#pragma once
#include "..\..\..\Actor.h"

namespace AttackNo{
	// 攻撃の番号.
	enum enAttackNo : unsigned char {
		None,

		Heavy,	// 強攻撃.
		Normal,	// 通常攻撃.
		Weak,	// 弱攻撃.

		Max,
	} typedef EAttackNo;
}

/************************************************
*	攻撃エフェクトクラス.
*		﨑田友輝.
**/
class CAttackEffect final
	: public CActor
{
public:
	CAttackEffect();
	~CAttackEffect();

	// 初期化.
	virtual bool Init() override;

	// 更新.
	virtual void Update( const float& DeltaTime ) override;

	// 描画.
	virtual void Render() override;

	// エフェクトの再生.
	void EffectPlay( const int No );

	// 最後に再生したエフェクトが終了したか取得.
	bool GetIsEffectPlay() const { return *m_IsNowEffectPlay; }

private:
	// 使用していないエフェクトの削除.
	void EffectDelete();

private:
	std::vector<CSprite*>							m_pAttackSpriteList;	// 攻撃エフェクトリスト.
	std::vector<SSpriteRenderState>					m_AttackStateBaseList;	// 攻撃エフェクトの情報ベースリスト.
	std::vector<std::vector<SSpriteRenderState>>	m_AttackStateList;		// 攻撃エフェクトの情報リスト.
	std::vector<std::string>						m_AttackSEList;			// 攻撃のSEリスト.
	bool*											m_IsNowEffectPlay;		// 現在のエフェクトが再生中か.
};
