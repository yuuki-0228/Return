#include "DebugKeyInput.h"

CDebugKeyInput::CDebugKeyInput()
{
}

CDebugKeyInput::~CDebugKeyInput()
{
}

//----------------------------.
// �L�[�������ꂽ���Ƃ��擾����.
//	Release�̏ꍇ�͏����͍s��Ȃ�.
//----------------------------.
bool CDebugKeyInput::IsKeyPress( const int Key )
{
#ifdef _DEBUG
	return CKeyInput::IsKeyPress( Key );
#endif // #ifdef _DEBUG.
	return false;
}

//----------------------------.
// �������u��.
//	Release�̏ꍇ�͏����͍s��Ȃ�.
//----------------------------.
bool CDebugKeyInput::IsKeyDown( const int Key )
{
#ifdef _DEBUG
	return CKeyInput::IsKeyDown( Key );
#endif // #ifdef _DEBUG.
	return false;
}

//----------------------------.
// �������u��.
//	Release�̏ꍇ�͏����͍s��Ȃ�.
//----------------------------.
bool CDebugKeyInput::IsKeyUp( const int Key )
{
#ifdef _DEBUG
	return CKeyInput::IsKeyUp( Key );
#endif // #ifdef _DEBUG.
	return false;
}

//----------------------------.
// ���������Ă���.
//	Release�̏ꍇ�͏����͍s��Ȃ�.
//----------------------------.
bool CDebugKeyInput::IsKeyRepeat( const int Key )
{
#ifdef _DEBUG
	return CKeyInput::IsKeyRepeat( Key );
#endif // #ifdef _DEBUG.
	return false;
}