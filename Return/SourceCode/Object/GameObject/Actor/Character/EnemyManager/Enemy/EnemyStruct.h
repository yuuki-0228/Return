#pragma once
#include "..\..\..\..\..\..\Global.h"
#include "..\..\..\..\..\..\Utility\FileManager\FileManager.h"

// 移動の種類.
enum class enMoveType : unsigned char {
	None,

	Step,			// 横に少し動く.
	LateralMove,	// 横移動.
	Teleport,		// ランダムな位置に移動する.

} typedef EMoveType;

// 敵の行動の情報.
struct stEnemyActionState {
	std::string ActionName; // 行う行動の名前.
	//----------------------------------------------.
	//	攻撃用変数( ActionName == "Attack" ).
	//----------------------------------------------.
	float		AttackTimeCnt;	// 攻撃用の待ち時間用のカウント.
	float		Interval;		// 間隔.
	float		BulletAddTime;	// ステージを回している弾1つに付き増やす時間( "Wait"でも使用 ).
	int			ShotNum;		// 弾を発射する回数.
	int			AttackNum;		// 攻撃した回数.
	std::string	BulletName;		// 弾の名前.
	//----------------------------------------------.
	//	待機用変数( ActionName == "Wait" ).
	//----------------------------------------------.
	float		WaitTimeCnt;	// 待機時間用のカウント.
	float		WaitTime;		// 待機時間.
	//----------------------------------------------.
	//	移動用変数( ActionName == "Move" ).
	//----------------------------------------------.
	float		MoveSpeed;		// 移動速度.
	EMoveType	MoveType;		// 移動の種類.
	bool		IsActionnStop;	// 移動中行動を停止させるか.
	//----------------------------------------------.
	//	行動のループ用変数( ActionName == "Loop" ).
	//----------------------------------------------.
	int			GoActionNo;		// 次行うクション番号.
	int			NowLoopNum;		// 現在のループ回数.
	int			LoopEndNum;		// ループを終了する回数.

	//--------------------------.
	// 行動の初期化.
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
	// 攻撃の初期化( ActionName == "Attack" ).
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
	// 待機の初期化( ActionName == "Wait" ).
	//--------------------------.
	inline void WaitInit( const float Time, const float AddTime ) {
		WaitTimeCnt		= INIT_FLOAT;
		WaitTime		= Time;
		BulletAddTime	= AddTime;
	}
	//--------------------------.
	// 移動の初期化( ActionName == "Move" ).
	//--------------------------.
	inline void MoveInit( const float Speed, const std::string& Name, const bool Flag ) {
		MoveSpeed		= Speed;
		MoveType		= GetMoveType( Name );
		IsActionnStop	= Flag;
	}
	//--------------------------.
	// 行動のループの初期化( ActionName == "Loop" ).
	//--------------------------.
	inline void LoopInit( const int Go, const int Num ) {
		GoActionNo		= Go;
		NowLoopNum		= INIT_INT;
		LoopEndNum		= Num;
	}

	//--------------------------.
	// 移動の種類を取得.
	//--------------------------.
	inline EMoveType GetMoveType( const std::string& Name ) {
		if ( Name == "Step"			) return EMoveType::Step;
		if ( Name == "LateralMove"	) return EMoveType::LateralMove;
		if ( Name == "Teleport"		) return EMoveType::Teleport;
		return EMoveType::None;
	}
} typedef SEnemyActionState;

// 敵の情報.
struct stEnemyState {
	std::string						TypeName;		// 種類の名前.
	std::vector<SEnemyActionState>	ActionState;	// 行動リスト.
	int								NowActionNo;	// 現在のアクション番号.
	int								ActionSize;		// 行動リストのサイズ.
	int								MoveActionNo;	// 移動のアクション番号.
	bool							IsMove;			// 移動させるか.

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
	// 現在のアクションを取得.
	//--------------------------.
	SEnemyActionState* GetNowAction() {
		return &ActionState[NowActionNo];
	}
	//--------------------------.
	// 移動のアクションを取得.
	//--------------------------.
	SEnemyActionState* GetMoveAction() {
		if ( IsMove == false ) return nullptr;
		return &ActionState[MoveActionNo];
	}

} typedef SEnemyState;

