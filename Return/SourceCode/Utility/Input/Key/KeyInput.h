#pragma once
#include "..\..\..\Global.h"
#include "..\..\..\Common\DirectX\DirectX11.h"

/************************************************
*	�L�[�{�[�h�̃L�[���̓N���X.
*		���c�F�P.
**/
class CKeyInput final
{
private:
	static constexpr int KEY_MAX = 256;	// �L�[�̍ő�l.

public:
	CKeyInput();
	~CKeyInput();

	// �C���X�^���X�̎擾.
	static CKeyInput* GetInstance();

	// ��Ԃ��X�V.
	static void Update();

	// �S�ẴL�[���~���邩.
	inline static void IsAllKeyStop( const bool Flag ) { GetInstance()->m_KeyStop = Flag; };

	// �L�[�������ꂽ���Ƃ��擾����.
	static bool IsKeyPress( const int Key );
	// �������u��.
	static bool IsKeyDown( const int Key );
	// �������u��.
	static bool IsKeyUp( const int Key );
	// ���������Ă���.
	static bool IsKeyRepeat( const int Key );

	// �����̃L�[�̂ǂꂩ�������ꂽ���Ƃ��擾����.
	template<typename... T>
	static bool IsORPress( const int Key, T... t );
	// �����̃L�[�̂ǂꂩ���������u��.
	template<typename... T>
	static bool IsORKeyDown( const int Key, T... t );
	template<typename... T>
	// �����̃L�[�̂ǂꂩ�𗣂����u��.
	static bool IsORKeyUp( const int Key, T... t );
	template<typename... T>
	// �����̃L�[�̂ǂꂩ�����������Ă���.
	static bool IsORKeyRepeat( const int Key, T... t );

	// �����̃L�[���S�ĉ����ꂽ���Ƃ��擾����.
	template<typename... T>
	static bool IsANDPress( const int Key, T... t );
	// �����̃L�[���������u��.
	//	���Ō�Ɏw�肵���L�[�������ꂽ�u�ԂɑS�ĉ����Ă����K�v������.
	template<typename... T>
	static bool IsANDKeyDown( const int Key, T... t );
	// �����̃L�[���������u��.
	//	���Ō�Ɏw�肵���L�[�������ꂽ�u�ԂɑS�ė����Ă����K�v������.
	template<typename... T>
	static bool IsANDKeyUp( const int Key, T... t );
	// �����̃L�[���S�ĉ��������Ă���.
	template<typename... T>
	static bool IsANDKeyRepeat( const int Key, T... t );

private:
	BYTE	m_NowState[KEY_MAX];	// ���݂̃L�[�̏��.
	BYTE	m_OldState[KEY_MAX];	// �O��̃L�[�̏��.
	CBool	m_KeyStop;				// �S�ẴL�[���~����.
	bool	m_IsNotActiveStop;		// �A�N�e�B�u�E�B���h�E�ł͂Ȃ����ɒ�~�����邩.

private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CKeyInput( const CKeyInput& )				= delete;
	CKeyInput( CKeyInput&& )					= delete;
	CKeyInput& operator = ( const CKeyInput& )	= delete;
	CKeyInput& operator = ( CKeyInput&& )		= delete;
};


//----------------------------.
// �����̃L�[�̂ǂꂩ�������ꂽ���Ƃ��擾����.
//----------------------------.
template<typename... T>
bool CKeyInput::IsORPress( const int Key, T... t )
{
	CKeyInput* pI = GetInstance();
	if ( pI->m_IsNotActiveStop && !CDirectX11::IsWindowActive() ) return false;

	std::vector<int> KeyList = { Key, t... };
	for ( auto& k : KeyList ) {
		if ( IsKeyPress( k ) ) return true;
	}
	return false;
}

//----------------------------.
// �����̃L�[�̂ǂꂩ���������u��.
//----------------------------.
template<typename... T>
bool CKeyInput::IsORKeyDown( const int Key, T... t )
{
	CKeyInput* pI = GetInstance();
	if ( pI->m_IsNotActiveStop && !CDirectX11::IsWindowActive() ) return false;

	std::vector<int> KeyList = { Key, t... };
	for ( auto& k : KeyList ) {
		if ( IsKeyDown( k ) ) return true;
	}
	return false;
}

//----------------------------.
// �����̃L�[�̂ǂꂩ�𗣂����u��.
//----------------------------.
template<typename... T>
bool CKeyInput::IsORKeyUp( const int Key, T... t )
{
	CKeyInput* pI = GetInstance();
	if ( pI->m_IsNotActiveStop && !CDirectX11::IsWindowActive() ) return false;

	std::vector<int> KeyList = { Key, t... };
	for ( auto& k : KeyList ) {
		if ( IsKeyUp( k ) ) return true;
	}
	return false;
}

//----------------------------.
// �����̃L�[�̂ǂꂩ�����������Ă���.
//----------------------------.
template<typename... T>
bool CKeyInput::IsORKeyRepeat( const int Key, T... t )
{
	CKeyInput* pI = GetInstance();
	if ( pI->m_IsNotActiveStop && !CDirectX11::IsWindowActive() ) return false;

	std::vector<int> KeyList = { Key, t... };
	for ( auto& k : KeyList ) {
		if ( IsKeyRepeat( k ) ) return true;
	}
	return false;
}

//----------------------------.
// �����̃L�[���S�ĉ����ꂽ���Ƃ��擾����.
//----------------------------.
template<typename... T>
bool CKeyInput::IsANDPress( const int Key, T... t )
{
	CKeyInput* pI = GetInstance();
	if ( pI->m_IsNotActiveStop && !CDirectX11::IsWindowActive() ) return false;

	std::vector<int> KeyList = { Key, t... };
	for ( auto& k : KeyList ) {
		if ( IsKeyPress( k ) == false ) return false;
	}
	return true;
}

//----------------------------.
// �����̃L�[���������u��.
//----------------------------.
template<typename... T>
bool CKeyInput::IsANDKeyDown( const int Key, T... t )
{
	CKeyInput* pI = GetInstance();
	if ( pI->m_IsNotActiveStop && !CDirectX11::IsWindowActive() ) return false;

	std::vector<int> KeyList = { Key, t... };
	if ( IsKeyDown( KeyList.back() ) == false ) return false;
	for ( auto& k : KeyList ) {
		if ( IsKeyPress( k ) == false ) return false;
	}
	return true;
}

//----------------------------.
// �����̃L�[���������u��.
//----------------------------.
template<typename... T>
bool CKeyInput::IsANDKeyUp( const int Key, T... t )
{
	CKeyInput* pI = GetInstance();
	if ( pI->m_IsNotActiveStop && !CDirectX11::IsWindowActive() ) return false;

	std::vector<int> KeyList = { Key, t... };
	if ( IsKeyUp( KeyList.back() ) == false ) return false;
	for ( auto& k : KeyList ) {
		if ( IsKeyPress( k ) ) return false;
	}
	return true;
}

//----------------------------.
// �����̃L�[���S�ĉ��������Ă���.
//----------------------------.
template<typename... T>
bool CKeyInput::IsANDKeyRepeat( const int Key, T... t )
{
	CKeyInput* pI = GetInstance();
	if ( pI->m_IsNotActiveStop && !CDirectX11::IsWindowActive() ) return false;

	std::vector<int> KeyList = { Key, t... };
	for ( auto& k : KeyList ) {
		if ( IsKeyRepeat( k ) == false ) return false;
	}
	return true;
}
