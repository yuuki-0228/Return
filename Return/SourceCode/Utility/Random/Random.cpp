#include "Random.h"
#include <fstream>
#include <sstream>
#include "..\FileManager\FileManager.h"

namespace{
	constexpr char TEXT_PATH[] = "Data\\Parameter\\Config\\Random.json";
}

CRandom::CRandom()
	: m_rnd			()
	, m_mt			()
	, m_RandLock	( false )
{
}

CRandom::~CRandom()
{
}

//----------------------------.
// �C���X�^���X�̎擾.
//----------------------------.
CRandom* CRandom::GetInstance()
{
	static std::unique_ptr<CRandom> pInstance = std::make_unique<CRandom>();
	return pInstance.get();
}

//----------------------------.
// �����̏�����.
//----------------------------.
HRESULT CRandom::Init()
{
	// �����̏������������擾.
	int InitNum = GetInstance()->m_rnd();

	// �����̌Œ艻���L���Ȃ�.
	if ( GetInstance()->m_RandLock ){
		// �O��̏������̒l�ŏ�����.
		const json& Text = FileManager::JsonLoad( TEXT_PATH );
		InitNum			 = Text["Init"];
	}
	else{
		// �����̏������̒l��ۑ�.
		json j;
		j[".Comment"]	= u8"��������������l";
		j["Init"]		= InitNum;
		FileManager::JsonSave( TEXT_PATH, j );
	}

	// �����̏�����.
	std::mt19937 InitMt( InitNum );
	GetInstance()->m_mt = InitMt;

	CLog::PushLog( "�����̏����� : ����" );
	return S_OK;
}

//----------------------------.
// �����_���Ȑ��l( float�^ )���擾.
//	Min�ȏ�AMax������Ԃ�.
//----------------------------.
float CRandom::GetRand( float min, float max )
{
	// �����Ə�����w�肵�����_���Ő��l��Ԃ�.
	std::uniform_real_distribution<float>	Rand_Num( min, max );
	return Rand_Num( GetInstance()->m_mt );
}

//----------------------------.
// �����_���Ȑ��l( int�^ )���擾.
//	Min�ȏ�AMax�ȉ���Ԃ�.
//----------------------------.
int CRandom::GetRand( int min, int max )
{
	// �����Ə�����w�肵�����_���Ő��l��Ԃ�.
	std::uniform_int_distribution<>	Rand_Num( min, max );
	return Rand_Num( GetInstance()->m_mt );
}

//----------------------------.
// �m��( ���� or �Z/�Z ).
//----------------------------.
bool CRandom::Probability( int Molecule, int Denominator )
{
	return Probability( static_cast<float>( Molecule ) / static_cast<float>( Denominator ) );
}
bool CRandom::Probability( float Molecule, float Denominator )
{
	return Probability( Molecule / Denominator );
}
bool CRandom::Probability( float Ratio )
{
	return static_cast<float>( GetRand( 1, 1000 ) ) <= Ratio * 1000.0f;
}
