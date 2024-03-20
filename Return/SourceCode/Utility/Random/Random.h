#pragma once
#include "..\..\Global.h"
#include <random>

/************************************************
*	�����_���N���X.
*		���c�F�P.
**/
class CRandom final
{
public:
	CRandom();
	~CRandom();

	// �C���X�^���X�̎擾.
	static CRandom* GetInstance();

	// �����̏�����.
	static HRESULT Init();

	// �������Œ肷�邩.
	static void IsRandLock( bool Flag ) { GetInstance()->m_RandLock = Flag; }

	// �����_���Ȑ��l( float�^ )���擾.
	//	Min�ȏ�AMax������Ԃ�.
	static float GetRand( float Min, float Max );
	// �����_���Ȑ��l( int�^ )���擾.
	//	Min�ȏ�AMax�ȉ���Ԃ�.
	static int	 GetRand( int	Min,  int  Max );

	// �m��( ���� or �Z/�Z ).
	static bool Probability( int	Molecule, int	Denominator );
	static bool Probability( float	Molecule, float Denominator );
	static bool Probability( float	Ratio );

private:
	// �����_���̐ݒ�.
	std::random_device	m_rnd;
	std::mt19937		m_mt;

	// �������Œ肷�邩.
	bool				m_RandLock;

private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CRandom( const CRandom & )				= delete;
	CRandom& operator = ( const CRandom & ) = delete;
	CRandom( CRandom && )					= delete;
	CRandom& operator = ( CRandom && )		= delete;
};