#pragma once
#include "..\..\..\..\..\Global.h"
#include "..\..\..\GameObjectList.h" 

// 弾の情報.
struct stBulletState {
	EObjectTag	BulletTag;		// 弾の種類.
	D3DXCOLOR3	DeleteColor;	// 破棄された時のエフェクトの色.
	std::string	SkillName;		// スキル名.
	float		MoveSpeed;		// 移動速度.
	float		Attack;			// 攻撃力.
	int			PlayerDamage;	// プレイヤーに対してのダメージ.
	int			Shield;			// バリアの耐久度.
	int			ImageNo;		// 弾の画像No.

	stBulletState()
		: stBulletState( EObjectTag::None, INIT_FLOAT3, "", INIT_FLOAT, INIT_FLOAT, INIT_INT, INIT_INT, INIT_INT)
	{
	}
	stBulletState( const EObjectTag Tag, const D3DXCOLOR3& Color, const std::string& Name, const float Spd, const float Atk, const int Dam, const int Shi, const int No )
		: BulletTag		( Tag )
		, DeleteColor	( Color )
		, SkillName		( Name )
		, MoveSpeed		( Spd )
		, Attack		( Atk )
		, PlayerDamage	( Dam )
		, Shield		( Shi )
		, ImageNo		( No )
	{
	}

	bool operator==( const stBulletState& State ) {
		return	BulletTag		== State.BulletTag &&
				MoveSpeed		== State.MoveSpeed &&
				Attack			== State.Attack &&
				PlayerDamage	== State.PlayerDamage &&
				Shield			== State.Shield &&
				ImageNo			== State.ImageNo;
	}
	bool operator!=( const stBulletState& State ) {
		return	BulletTag		!= State.BulletTag ||
				MoveSpeed		!= State.MoveSpeed ||
				Attack			!= State.Attack ||
				PlayerDamage	!= State.PlayerDamage ||
				Shield			!= State.Shield ||
				ImageNo			!= State.ImageNo;
	}
} typedef SBulletState;

namespace EffectNo {
	// エフェクトNo.
	enum enEffectNo : unsigned char {
		BulletDelete,	// 弾の削除エフェクト.
		StateUp,		// 弾が強化された時のエフェクト.
		StateDown,		// 弾が弱体化された時のエフェクト.

		Effect_Max
	} typedef EEffectNo;

	// レベルエフェクトNo.
	enum enLvEffectNo : unsigned char {
		Lv1,
		Lv2,
		Lv3,
		Lv4,

		LvEffect_Max,
		LvEffect_None
	} typedef ELvEEffectNo;
}
