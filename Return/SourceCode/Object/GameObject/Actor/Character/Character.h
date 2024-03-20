#pragma once
#include "..\Actor.h"
#include "..\..\..\..\Common\Sprite\Sprite.h"

/************************************************
*	キャラクタークラス.
*		﨑田友輝.
**/
class CCharacter
	: public CActor
{
public:
	CCharacter();
	virtual ~CCharacter();

	// HPの取得.
	int GetHp() const { return m_Hp; }
	// スタミナの取得.
	float GetStamina() const { return m_Stamina; }

	// HPの設定.
	void SetHp( const int HP );
	// スタミナの設定.
	void SetStamina( const float Stamina );

	// 回復する.
	void HealHp( const float	Heal );
	void HealHp( const int		Heal );
	// ダメージを受ける.
	void DamageHp( const float	Damage );
	void DamageHp( const int	Damage );
	// スタミナの追加.
	void AddStamina( const float Stamina );

protected:
	CSprite*			m_pSprite;		// キャラクターの画像.
	SSpriteRenderState	m_SpriteState;	// キャラクターの画像の情報.
	int					m_Hp;			// HP.
	int					m_HpMax;		// HPの上限.
	float				m_Stamina;		// スタミナ.
	float				m_StaminaMax;	// スタミナ.
};
