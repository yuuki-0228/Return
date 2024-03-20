#include "KeyInput.h"
#include "..\..\FileManager\FileManager.h"

namespace {
	constexpr char WINDOW_SETTING_FILE_PATH[] = "Data\\Parameter\\Config\\WindowSetting.json";	// �E�B���h�E�̐ݒ�̃t�@�C���p�X.
}

CKeyInput::CKeyInput()
	: m_NowState		()
	, m_OldState		()
	, m_KeyStop			( false, u8"�L�[�{�[�h�̃L�[���~���邩", "Input" )
	, m_IsNotActiveStop	( false )
{
	// �E�B���h�E�̐ݒ�̎擾.
	json WndSetting		= FileManager::JsonLoad( WINDOW_SETTING_FILE_PATH );
	m_IsNotActiveStop	= WndSetting["IsInputNotActiveStop"];
}

CKeyInput::~CKeyInput()
{
}

//----------------------------.
// �C���X�^���X�̎擾.
//----------------------------.
CKeyInput* CKeyInput::GetInstance()
{
	static std::unique_ptr<CKeyInput> pInstance = std::make_unique<CKeyInput>();
	return pInstance.get();
}

//----------------------------.
// ��Ԃ��X�V.
//----------------------------.
void CKeyInput::Update()
{
	CKeyInput* pI = GetInstance();

	// ��Ԃ��X�V����O�Ɍ��݂̏�Ԃ��R�s�[.
	memcpy_s( pI->m_OldState, sizeof( m_OldState ), pI->m_NowState, sizeof( m_NowState ) );

	// �����Ă���L�[�𒲂ׂ�.
	if( !GetKeyboardState( pI->m_NowState ) ) return;
}

//----------------------------.
// �L�[�������ꂽ���Ƃ��擾����.
//	�L�[����~���Ȃ珈���͍s��Ȃ�.
//----------------------------.
bool CKeyInput::IsKeyPress( const int Key )
{
	CKeyInput* pI = GetInstance();
	if ( pI->m_IsNotActiveStop && !CDirectX11::IsWindowActive() ) return false;

	if (		pI->m_KeyStop.get() ) return false;
	return (	pI->m_NowState[Key] & 0x80 ) != 0;
}

//----------------------------.
// �������u��.
//	�L�[����~���Ȃ珈���͍s��Ȃ�.
//----------------------------.
bool CKeyInput::IsKeyDown( const int Key )
{
	CKeyInput* pI = GetInstance();
	if ( pI->m_IsNotActiveStop && !CDirectX11::IsWindowActive() ) return false;

	// ���ݓ��͂őO����͂��ĂȂ����.
	if (		pI->m_KeyStop.get() ) return false;
	return	(	pI->m_OldState[Key] & 0x80 ) == 0 &&
			(	pI->m_NowState[Key] & 0x80 ) != 0;
}

//----------------------------.
// �������u��.
//	�L�[����~���Ȃ珈���͍s��Ȃ�.
//----------------------------.
bool CKeyInput::IsKeyUp( const int Key )
{
	CKeyInput* pI = GetInstance();
	if ( pI->m_IsNotActiveStop && !CDirectX11::IsWindowActive() ) return false;

	// ���݂����͂őO������͂Ȃ�.
	if (		pI->m_KeyStop.get() ) return false;
	return	(	pI->m_OldState[Key] & 0x80 ) != 0 &&
			(	pI->m_NowState[Key] & 0x80 ) == 0;
}

//----------------------------.
// ���������Ă���.
//	�L�[����~���Ȃ珈���͍s��Ȃ�.
//----------------------------.
bool CKeyInput::IsKeyRepeat( const int Key )
{
	CKeyInput* pI = GetInstance();
	if ( pI->m_IsNotActiveStop && !CDirectX11::IsWindowActive() ) return false;

	// ���݂����͂őO������͂Ȃ�.
	if (		pI->m_KeyStop.get() ) return false;
	return	(	pI->m_OldState[Key] & 0x80 ) != 0 &&
			(	pI->m_NowState[Key] & 0x80 ) != 0;
}
