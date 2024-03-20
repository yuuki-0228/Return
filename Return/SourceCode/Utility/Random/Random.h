#pragma once
#include "..\..\Global.h"
#include <random>

/************************************************
*	ランダムクラス.
*		﨑田友輝.
**/
class CRandom final
{
public:
	CRandom();
	~CRandom();

	// インスタンスの取得.
	static CRandom* GetInstance();

	// 乱数の初期化.
	static HRESULT Init();

	// 乱数を固定するか.
	static void IsRandLock( bool Flag ) { GetInstance()->m_RandLock = Flag; }

	// ランダムな数値( float型 )を取得.
	//	Min以上、Max未満を返す.
	static float GetRand( float Min, float Max );
	// ランダムな数値( int型 )を取得.
	//	Min以上、Max以下を返す.
	static int	 GetRand( int	Min,  int  Max );

	// 確率( 割合 or 〇/〇 ).
	static bool Probability( int	Molecule, int	Denominator );
	static bool Probability( float	Molecule, float Denominator );
	static bool Probability( float	Ratio );

private:
	// ランダムの設定.
	std::random_device	m_rnd;
	std::mt19937		m_mt;

	// 乱数を固定するか.
	bool				m_RandLock;

private:
	// コピー・ムーブコンストラクタ, 代入演算子の削除.
	CRandom( const CRandom & )				= delete;
	CRandom& operator = ( const CRandom & ) = delete;
	CRandom( CRandom && )					= delete;
	CRandom& operator = ( CRandom && )		= delete;
};