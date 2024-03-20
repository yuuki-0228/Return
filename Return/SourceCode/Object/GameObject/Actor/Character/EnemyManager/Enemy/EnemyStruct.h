#pragma once
#include "..\..\..\..\..\..\Global.h"
#include "..\..\..\..\..\..\Utility\FileManager\FileManager.h"

// �ړ��̎��.
enum class enMoveType : unsigned char {
	None,

	Step,			// ���ɏ�������.
	LateralMove,	// ���ړ�.
	Teleport,		// �����_���Ȉʒu�Ɉړ�����.

} typedef EMoveType;

// �G�̍s���̏��.
struct stEnemyActionState {
	std::string ActionName; // �s���s���̖��O.
	//----------------------------------------------.
	//	�U���p�ϐ�( ActionName == "Attack" ).
	//----------------------------------------------.
	float		AttackTimeCnt;	// �U���p�̑҂����ԗp�̃J�E���g.
	float		Interval;		// �Ԋu.
	float		BulletAddTime;	// �X�e�[�W���񂵂Ă���e1�ɕt�����₷����( "Wait"�ł��g�p ).
	int			ShotNum;		// �e�𔭎˂����.
	int			AttackNum;		// �U��������.
	std::string	BulletName;		// �e�̖��O.
	//----------------------------------------------.
	//	�ҋ@�p�ϐ�( ActionName == "Wait" ).
	//----------------------------------------------.
	float		WaitTimeCnt;	// �ҋ@���ԗp�̃J�E���g.
	float		WaitTime;		// �ҋ@����.
	//----------------------------------------------.
	//	�ړ��p�ϐ�( ActionName == "Move" ).
	//----------------------------------------------.
	float		MoveSpeed;		// �ړ����x.
	EMoveType	MoveType;		// �ړ��̎��.
	bool		IsActionnStop;	// �ړ����s�����~�����邩.
	//----------------------------------------------.
	//	�s���̃��[�v�p�ϐ�( ActionName == "Loop" ).
	//----------------------------------------------.
	int			GoActionNo;		// ���s���N�V�����ԍ�.
	int			NowLoopNum;		// ���݂̃��[�v��.
	int			LoopEndNum;		// ���[�v���I�������.

	//--------------------------.
	// �s���̏�����.
	//--------------------------.
	inline void ActionInit( const json j ) {
		ActionName = j["Type"];
		if (		ActionName == "Attack"	) AttackInit( j["Interval"], j["BulletAddTime"], j["ShotNum"], j["BulletName"] );
		else if (	ActionName == "Wait"	) WaitInit( j["Time"], j["BulletAddTime"] );
		else if (	ActionName == "Move"	) MoveInit( j["Speed"], j["MoveType"], j["IsActionnStop"] );
		else if (	ActionName == "Loop"	) LoopInit( j["Go"], j["LoopNum"] );
	}

private:
	//--------------------------.
	// �U���̏�����( ActionName == "Attack" ).
	//--------------------------.
	inline void AttackInit( const float Inte, const float AddTime, const int Num, const std::string& Name ) {
		AttackTimeCnt	= INIT_FLOAT;
		Interval		= Inte;
		BulletAddTime	= AddTime;
		ShotNum			= Num;
		AttackNum		= INIT_INT;
		BulletName		= Name;
	}
	//--------------------------.
	// �ҋ@�̏�����( ActionName == "Wait" ).
	//--------------------------.
	inline void WaitInit( const float Time, const float AddTime ) {
		WaitTimeCnt		= INIT_FLOAT;
		WaitTime		= Time;
		BulletAddTime	= AddTime;
	}
	//--------------------------.
	// �ړ��̏�����( ActionName == "Move" ).
	//--------------------------.
	inline void MoveInit( const float Speed, const std::string& Name, const bool Flag ) {
		MoveSpeed		= Speed;
		MoveType		= GetMoveType( Name );
		IsActionnStop	= Flag;
	}
	//--------------------------.
	// �s���̃��[�v�̏�����( ActionName == "Loop" ).
	//--------------------------.
	inline void LoopInit( const int Go, const int Num ) {
		GoActionNo		= Go;
		NowLoopNum		= INIT_INT;
		LoopEndNum		= Num;
	}

	//--------------------------.
	// �ړ��̎�ނ��擾.
	//--------------------------.
	inline EMoveType GetMoveType( const std::string& Name ) {
		if ( Name == "Step"			) return EMoveType::Step;
		if ( Name == "LateralMove"	) return EMoveType::LateralMove;
		if ( Name == "Teleport"		) return EMoveType::Teleport;
		return EMoveType::None;
	}
} typedef SEnemyActionState;

// �G�̏��.
struct stEnemyState {
	std::string						TypeName;		// ��ނ̖��O.
	std::vector<SEnemyActionState>	ActionState;	// �s�����X�g.
	int								NowActionNo;	// ���݂̃A�N�V�����ԍ�.
	int								ActionSize;		// �s�����X�g�̃T�C�Y.
	int								MoveActionNo;	// �ړ��̃A�N�V�����ԍ�.
	bool							IsMove;			// �ړ������邩.

	stEnemyState()
		: stEnemyState( "", INIT_INT, {} )
	{
	}
	stEnemyState( const std::string& Name, const int hp, const std::vector<SEnemyActionState>& List )
		: TypeName		( Name )
		, ActionState	( List )
		, NowActionNo	( INIT_INT )
		, ActionSize	( INIT_INT )
		, MoveActionNo	( INIT_INT )
		, IsMove		( false )
	{
		ActionSize = static_cast<int>( ActionState.size() );
	}

	//--------------------------.
	// ���݂̃A�N�V�������擾.
	//--------------------------.
	SEnemyActionState* GetNowAction() {
		return &ActionState[NowActionNo];
	}
	//--------------------------.
	// �ړ��̃A�N�V�������擾.
	//--------------------------.
	SEnemyActionState* GetMoveAction() {
		if ( IsMove == false ) return nullptr;
		return &ActionState[MoveActionNo];
	}

} typedef SEnemyState;

