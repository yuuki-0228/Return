#pragma once
#include "..\..\..\..\..\Global.h"
#include "..\..\..\GameObjectList.h" 

// �e�̏��.
struct stBulletState {
	EObjectTag	BulletTag;		// �e�̎��.
	D3DXCOLOR3	DeleteColor;	// �j�����ꂽ���̃G�t�F�N�g�̐F.
	std::string	SkillName;		// �X�L����.
	float		MoveSpeed;		// �ړ����x.
	float		Attack;			// �U����.
	int			PlayerDamage;	// �v���C���[�ɑ΂��Ẵ_���[�W.
	int			Shield;			// �o���A�̑ϋv�x.
	int			ImageNo;		// �e�̉摜No.

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
	// �G�t�F�N�gNo.
	enum enEffectNo : unsigned char {
		BulletDelete,	// �e�̍폜�G�t�F�N�g.
		StateUp,		// �e���������ꂽ���̃G�t�F�N�g.
		StateDown,		// �e����̉����ꂽ���̃G�t�F�N�g.

		Effect_Max
	} typedef EEffectNo;

	// ���x���G�t�F�N�gNo.
	enum enLvEffectNo : unsigned char {
		Lv1,
		Lv2,
		Lv3,
		Lv4,

		LvEffect_Max,
		LvEffect_None
	} typedef ELvEEffectNo;
}
