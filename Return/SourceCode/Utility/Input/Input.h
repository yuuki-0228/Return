#pragma once
#include "..\..\Global.h"
#include "..\..\Common\DirectX\DirectX11.h"
#include <unordered_map>

#include "Controller\DirectInput\DirectInput.h"
#include "Controller\XInput\XInput.h"
#include "Key\KeyInput.h"
#include "Key\DebugKeyInput.h"
#include "KeyBind.h"

class CKeyLog;

/************************************************
*	�L�[�o�C���h�ɂ����̓N���X.
*		���c�F�P.
**/
class CInput final
{
public:
	CInput();
	~CInput();

	// �C���X�^���X�̎擾.
	static CInput* GetInstance();

	// �E�B���h�E�n���h���̐ݒ�.
	static void SethWnd( HWND hWnd ) { GetInstance()->m_hWnd = hWnd; }

	// ��Ԃ��X�V.
	static void Update();

	// ���O���o��.
	static void KeyLogOutput();

	// �����ݒ�.
	static HRESULT Init();

	// �L�[���O���Đ������擾.
	static bool GetLogAuto() { return GetInstance()->m_IsKeyLogPlay; }
	// �L�[�o�C���h��ݒ�.
	static void SetKeyBind( const KeyBind_map& KeyBind ) { GetInstance()->m_KeyBindList = KeyBind; }

	// �X�V���~�����邩�ݒ�.
	static void SetIsUpdateStop( const bool Flag ) { GetInstance()->m_IsUpdateStop = Flag; }

	// �����ꂽ���Ƃ��擾����.
	static bool IsKeyPress	( const std::string& Key );
	// �������u��.
	static bool IsKeyDown	( const std::string& Key );
	// �������u��.
	static bool IsKeyUp		( const std::string& Key );
	// ���������Ă���.
	static bool IsKeyRepeat	( const std::string& Key );

	// �����̑���̂ǂꂩ�������ꂽ���Ƃ��擾����.
	template<typename... T>
	static bool IsORPress( const std::string& Key, T... t );
	// �����̑���̂ǂꂩ���������u��.
	template<typename... T>
	static bool IsORKeyDown( const std::string& Key, T... t );
	// �����̑���̂ǂꂩ�𗣂����u��.
	template<typename... T>
	static bool IsORKeyUp( const std::string& Key, T... t );
	// �����̑���̂ǂꂩ�����������Ă���.
	template<typename... T>
	static bool IsORKeyRepeat( const std::string& Key, T... t );

	// �����̑��삪�S�ĉ����ꂽ���Ƃ��擾����.
	template<typename... T>
	static bool IsANDPress( const std::string& Key, T... t );
	// �����̑��삪�������u��.
	//	���Ō�Ɏw�肵�����삪�����ꂽ�u�ԂɑS�ĉ����Ă����K�v������.
	template<typename... T>
	static bool IsANDKeyDown( const std::string& Key, T... t );
	// �����̑��삪�������u��.
	//	���Ō�Ɏw�肵�����삪�����ꂽ�u�ԂɑS�ė����Ă����K�v������.
	template<typename... T>
	static bool IsANDKeyUp( const std::string& Key, T... t );
	// �����̑��삪�S�ĉ��������Ă���.
	template<typename... T>
	static bool IsANDKeyRepeat( const std::string& Key, T... t );

	//-----------------------------------------------.
	// �� �}�E�X�n ��
	//-----------------------------------------------.

	// ���W�̎擾.
	static D3DXPOSITION2 GetMousePosition()		{ return D3DXPOSITION2( static_cast<float>( GetInstance()->m_MouseNowPoint.x ), static_cast<float>( GetInstance()->m_MouseNowPoint.y ) ); }
	// �O��̍��W�̎擾.
	static D3DXPOSITION2 GetMouseOldPosition()	{ return D3DXPOSITION2( static_cast<float>( GetInstance()->m_MouseOldPoint.x ), static_cast<float>( GetInstance()->m_MouseOldPoint.y ) ); }
	// ���WX�̎擾.
	static float GetMousePosisionX()	{ return static_cast<float>(GetInstance()->m_MouseNowPoint.x); }
	// ���WY�̎擾.
	static float GetMousePosisionY()	{ return static_cast<float>(GetInstance()->m_MouseNowPoint.y); }
	// �O��̍��WX�̎擾.
	static float GetMouseOldPosisionX()	{ return static_cast<float>(GetInstance()->m_MouseOldPoint.x); }
	// �O��̍��WY�̎擾.
	static float GetMouseOldPosisionY()	{ return static_cast<float>(GetInstance()->m_MouseOldPoint.y); }

	// �}�E�X���X�N���[���̒���.
	static bool IsScreenMiddleMouse();
	// �}�E�X������ł��邩.
	static bool IsMousrGrab() { return GetInstance()->m_IsGrab; }

	// �}�E�X������ł��邩�ݒ�.
	static void SetIsMousrGrab( const int Flag ) { GetInstance()->m_IsGrab = Flag; }
	// �}�E�X�z�C�[���̒l�̐ݒ�.
	static void SetMouseWheelDelta( const int delta ) { GetInstance()->m_MouseWheel = delta; }
	// �}�E�X�z�C�[���̒l�̎擾.
	//	+ ������ւ̈ړ�.
	//	- �������ւ̈ړ�.
	static int GetMouseWheelDelta() { return GetInstance()->m_MouseWheel; }

private:
	// �L�[�o�C���h�̃f�[�^�ϊ�.
	static int	ToKeyData( std::string Key );
	static WORD	ToButData( std::string But );

	// �}�E�X�̍X�V.
	static void UpdateMouse();

private:
	HWND						m_hWnd;				// �E�B���h�E�n���h��.
	KeyBind_map					m_KeyBindList;		// �L�[�o�C���h�̃��X�g.
	std::unique_ptr<CKeyLog>	m_pKeyLog;			// �L�[���O.
	POINT						m_MouseNowPoint;	// ���݂̃}�E�X���W.
	POINT						m_MouseOldPoint;	// �O��̃}�E�X���W.
	int							m_MouseWheel;		// �}�E�X�z�C�[���l.
	bool						m_IsGrab;			// ����ł��邩.
	bool						m_IsKeyLogPlay;		// �L�[���O���Đ����邩.
	bool						m_IsNotActiveStop;	// �A�N�e�B�u�E�B���h�E�ł͂Ȃ����ɒ�~�����邩.
	bool						m_IsUpdateStop;		// �X�V�̒�~.

private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CInput( const CInput & )				= delete;
	CInput( CInput && )						= delete;
	CInput& operator = ( const CInput & )	= delete;
	CInput& operator = ( CInput && )		= delete;
};


//----------------------------.
// �����̑���̂ǂꂩ�������ꂽ���Ƃ��擾����.
//----------------------------.
template<typename... T>
bool CInput::IsORPress( const std::string& Key, T... t )
{
	CInput* pI = GetInstance();
	if ( pI->m_IsNotActiveStop && !CDirectX11::IsWindowActive() ) return false;

	std::vector<std::string> KeyList = { Key, t... };
	for ( auto& k : KeyList ) {
		if ( IsKeyPress( k ) ) return true;
	}
	return false;
}

//----------------------------.
// �����̑���̂ǂꂩ���������u��.
//----------------------------.
template<typename... T>
bool CInput::IsORKeyDown( const std::string& Key, T... t )
{
	CInput* pI = GetInstance();
	if ( pI->m_IsNotActiveStop && !CDirectX11::IsWindowActive() ) return false;

	std::vector<std::string> KeyList = { Key, t... };
	for ( auto& k : KeyList ) {
		if ( IsKeyDown( k ) ) return true;
	}
	return false;
}

//----------------------------.
// �����̑���̂ǂꂩ�𗣂����u��.
//----------------------------.
template<typename... T>
bool CInput::IsORKeyUp( const std::string& Key, T... t )
{
	CInput* pI = GetInstance();
	if ( pI->m_IsNotActiveStop && !CDirectX11::IsWindowActive() ) return false;

	std::vector<std::string> KeyList = { Key, t... };
	for ( auto& k : KeyList ) {
		if ( IsKeyUp( k ) ) return true;
	}
	return false;
}

//----------------------------.
// �����̑���̂ǂꂩ�����������Ă���.
//----------------------------.
template<typename... T>
bool CInput::IsORKeyRepeat( const std::string& Key, T... t )
{
	CInput* pI = GetInstance();
	if ( pI->m_IsNotActiveStop && !CDirectX11::IsWindowActive() ) return false;

	std::vector<std::string> KeyList = { Key, t... };
	for ( auto& k : KeyList ) {
		if ( IsKeyRepeat( k ) ) return true;
	}
	return false;
}

//----------------------------.
// �����̑��삪�S�ĉ����ꂽ���Ƃ��擾����.
//----------------------------.
template<typename... T>
bool CInput::IsANDPress( const std::string& Key, T... t )
{
	CInput* pI = GetInstance();
	if ( pI->m_IsNotActiveStop && !CDirectX11::IsWindowActive() ) return false;

	std::vector<std::string> KeyList = { Key, t... };
	for ( auto& k : KeyList ) {
		if ( IsKeyPress( k ) == false ) return false;
	}
	return true;
}

//----------------------------.
// �����̑��삪�������u��.
//----------------------------.
template<typename... T>
bool CInput::IsANDKeyDown( const std::string& Key, T... t )
{
	CInput* pI = GetInstance();
	if ( pI->m_IsNotActiveStop && !CDirectX11::IsWindowActive() ) return false;

	std::vector<std::string> KeyList = { Key, t... };
	if ( IsKeyDown( KeyList.back() ) == false ) return false;
	for ( auto& k : KeyList ) {
		if ( IsKeyPress( k ) == false ) return false;
	}
	return true;
}

//----------------------------.
// �����̑��삪�������u��.
//----------------------------.
template<typename... T>
bool CInput::IsANDKeyUp( const std::string& Key, T... t )
{
	CInput* pI = GetInstance();
	if ( pI->m_IsNotActiveStop && !CDirectX11::IsWindowActive() ) return false;

	std::vector<std::string> KeyList = { Key, t... };
	if ( IsKeyUp( KeyList.back() ) == false ) return false;
	for ( auto& k : KeyList ) {
		if ( IsKeyPress( k ) ) return false;
	}
	return true;
}

//----------------------------.
// �����̑��삪�S�ĉ��������Ă���.
//----------------------------.
template<typename... T>
bool CInput::IsANDKeyRepeat( const std::string& Key, T... t )
{
	CInput* pI = GetInstance();
	if ( pI->m_IsNotActiveStop && !CDirectX11::IsWindowActive() ) return false;

	std::vector<std::string> KeyList = { Key, t... };
	for ( auto& k : KeyList ) {
		if ( IsKeyRepeat( k ) == false ) return false;
	}
	return true;
}
