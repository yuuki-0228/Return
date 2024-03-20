#pragma once
#include "..\..\..\..\..\Global.h"

// ポーションエリアの情報.
struct stPotionAreaState {
	int				PotionType;		// ポーションの種類.
	int				UseNum;			// ポーションの効果の使用回数.
	float			DeleteTime;		// 現在の効果時間.
	float			EffectTime;		// 効果の与えていく時間.
	float			EffectValue;	// 効果の強さ.
	D3DXCOLOR4		EffectColor;	// エフェクトの色.

	stPotionAreaState()
		: stPotionAreaState( INIT_INT, INIT_INT, INIT_FLOAT, INIT_FLOAT, INIT_FLOAT, INIT_FLOAT3 )
	{
	}
	stPotionAreaState( const int Type, const int Max, const float dTime, const float eTime, const float Value, const D3DXCOLOR4& Color )
		: PotionType	( Type )
		, UseNum		( Max )
		, DeleteTime	( dTime )
		, EffectTime	( eTime )
		, EffectValue	( Value )
		, EffectColor	( Color )
	{
	}
} typedef SPotionAreaState;

namespace PotionNo {
	// ポーションNo.
	enum enPotionNo : unsigned char {
		None,

		Heal,		// 回復(HPが回復).
		Poison,		// 毒(ダメージ).
		Rest,		// 休憩(スタミナが回復).

		Potion_Max
	} typedef EPotionNo;
}
