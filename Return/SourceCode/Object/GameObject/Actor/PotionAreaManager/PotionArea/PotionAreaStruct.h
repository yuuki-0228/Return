#pragma once
#include "..\..\..\..\..\Global.h"

// �|�[�V�����G���A�̏��.
struct stPotionAreaState {
	int				PotionType;		// �|�[�V�����̎��.
	int				UseNum;			// �|�[�V�����̌��ʂ̎g�p��.
	float			DeleteTime;		// ���݂̌��ʎ���.
	float			EffectTime;		// ���ʂ̗^���Ă�������.
	float			EffectValue;	// ���ʂ̋���.
	D3DXCOLOR4		EffectColor;	// �G�t�F�N�g�̐F.

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
	// �|�[�V����No.
	enum enPotionNo : unsigned char {
		None,

		Heal,		// ��(HP����).
		Poison,		// ��(�_���[�W).
		Rest,		// �x�e(�X�^�~�i����).

		Potion_Max
	} typedef EPotionNo;
}
