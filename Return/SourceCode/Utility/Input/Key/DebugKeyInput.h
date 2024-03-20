#pragma once
#include "KeyInput.h"

/************************************************
*	�L�[�{�[�h�̃L�[���̓N���X.
*		���c�F�P.
**/
class CDebugKeyInput final
{
public:
	CDebugKeyInput();
	~CDebugKeyInput();

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
	// �����̃L�[�̂ǂꂩ�𗣂����u��.
	template<typename... T>
	static bool IsORKeyUp( const int Key, T... t );
	// �����̃L�[�̂ǂꂩ�����������Ă���.
	template<typename... T>
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
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CDebugKeyInput( const CDebugKeyInput& )					= delete;
	CDebugKeyInput( CDebugKeyInput&& )						= delete;
	CDebugKeyInput& operator = ( const CDebugKeyInput& )	= delete;
	CDebugKeyInput& operator = ( CDebugKeyInput&& )			= delete;
};


//----------------------------.
// �����̃L�[�̂ǂꂩ�������ꂽ���Ƃ��擾����.
//	Release�̏ꍇ�͏����͍s��Ȃ�.
//----------------------------.
template<typename... T>
bool CDebugKeyInput::IsORPress( const int Key, T... t )
{
#ifdef _DEBUG
	return CKeyInput::IsORPress( Key, t... );
#endif // #ifdef _DEBUG.
	return false;
}

//----------------------------.
// �����̃L�[�̂ǂꂩ���������u��.
//	Release�̏ꍇ�͏����͍s��Ȃ�.
//----------------------------.
template<typename... T>
bool CDebugKeyInput::IsORKeyDown( const int Key, T... t )
{
#ifdef _DEBUG
	return CKeyInput::IsORKeyDown( Key, t... );
#endif // #ifdef _DEBUG.
	return false;
}

//----------------------------.
// �����̃L�[�̂ǂꂩ�𗣂����u��.
//	Release�̏ꍇ�͏����͍s��Ȃ�.
//----------------------------.
template<typename... T>
bool CDebugKeyInput::IsORKeyUp( const int Key, T... t )
{
#ifdef _DEBUG
	return CKeyInput::IsORKeyUp( Key, t... );
#endif // #ifdef _DEBUG.
	return false;
}

//----------------------------.
// �����̃L�[�̂ǂꂩ�����������Ă���.
//	Release�̏ꍇ�͏����͍s��Ȃ�.
//----------------------------.
template<typename... T>
bool CDebugKeyInput::IsORKeyRepeat( const int Key, T... t )
{
#ifdef _DEBUG
	return CKeyInput::IsORKeyRepeat( Key, t... );
#endif // #ifdef _DEBUG.
	return false;
}

//----------------------------.
// �����̃L�[���S�ĉ����ꂽ���Ƃ��擾����.
//	Release�̏ꍇ�͏����͍s��Ȃ�.
//----------------------------.
template<typename... T>
bool CDebugKeyInput::IsANDPress( const int Key, T... t )
{
#ifdef _DEBUG
	return CKeyInput::IsANDPress( Key, t... );
#endif // #ifdef _DEBUG.
	return false;
}

//----------------------------.
// �����̃L�[���������u��.
//	Release�̏ꍇ�͏����͍s��Ȃ�.
//----------------------------.
template<typename... T>
bool CDebugKeyInput::IsANDKeyDown( const int Key, T... t )
{
#ifdef _DEBUG
	return CKeyInput::IsANDKeyDown( Key, t... );
#endif // #ifdef _DEBUG.
	return false;
}

//----------------------------.
// �����̃L�[���������u��.
//	Release�̏ꍇ�͏����͍s��Ȃ�.
//----------------------------.
template<typename... T>
bool CDebugKeyInput::IsANDKeyUp( const int Key, T... t )
{
#ifdef _DEBUG
	return CKeyInput::IsANDKeyUp( Key, t... );
#endif // #ifdef _DEBUG.
	return false;
}

//----------------------------.
// �����̃L�[���S�ĉ��������Ă���.
//	Release�̏ꍇ�͏����͍s��Ȃ�.
//----------------------------.
template<typename... T>
bool CDebugKeyInput::IsANDKeyRepeat( const int Key, T... t )
{
#ifdef _DEBUG
	return CKeyInput::IsANDKeyRepeat( Key, t... );
#endif // #ifdef _DEBUG.
	return false;
}