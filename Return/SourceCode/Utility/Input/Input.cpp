#include "Input.h"
#include "InputList.h"
#include "Key\KeyInput.h"
#include "Controller\XInput\XInput.h"
#include "KeyLog\KeyLog.h"
#include "..\Random\Random.h"
#include "..\FileManager\FileManager.h"
#include "..\..\Scene\FadeManager\FadeManager.h"

namespace {
	constexpr char KEY_BIND_FILE_PATH[]			= "Data\\Parameter\\Config\\KeyBind.json";	// �L�[�o�C���h�̕ۑ��ꏊ.
	constexpr char WINDOW_SETTING_FILE_PATH[]	= "Data\\Parameter\\Config\\WindowSetting.json";	// �E�B���h�E�̐ݒ�̃t�@�C���p�X.
}

CInput::CInput()
	: m_hWnd			()
	, m_KeyBindList		()
	, m_pKeyLog			( nullptr )
	, m_MouseNowPoint	( { 0, 0 } )
	, m_MouseOldPoint	( { 0, 0 } )
	, m_MouseWheel		( 0 )
	, m_IsGrab			( false )
	, m_IsNotActiveStop	( false )
	, m_IsUpdateStop	( false )

	// �L�[���O���Đ��������ꍇ��( true )�ɂ���.
	, m_IsKeyLogPlay	( false )
{
	m_pKeyLog = std::make_unique<CKeyLog>( m_IsKeyLogPlay );

	// �E�B���h�E�̐ݒ�̎擾.
	json WndSetting		= FileManager::JsonLoad( WINDOW_SETTING_FILE_PATH );
	m_IsNotActiveStop	= WndSetting["IsInputNotActiveStop"];
}

CInput::~CInput()
{
}

//----------------------------.
// �C���X�^���X�̎擾.
//----------------------------.
CInput* CInput::GetInstance()
{
	static std::unique_ptr<CInput> pInstance = std::make_unique<CInput>();
	return pInstance.get();
}

//----------------------------.
// ��Ԃ��X�V.
//----------------------------.
void CInput::Update()
{
	if ( GetInstance()->m_IsUpdateStop || CFadeManager::GetIsFade() ) return;

	CKeyInput	::Update();
	CXInput		::Update();

	// �}�E�X�̍X�V.
	UpdateMouse();
}

//----------------------------.
// ���O���o��.
//----------------------------.
void CInput::KeyLogOutput()
{
	CInput* pI = GetInstance();

	if( pI->m_IsKeyLogPlay == false )	pI->m_pKeyLog->Output();
	else								pI->m_pKeyLog->AddFrameCnt();
}

//----------------------------.
// �����ݒ�.
//----------------------------.
HRESULT CInput::Init()
{
	CInput* pI = GetInstance();

	// ���݂̐ݒ���擾����.
	json KeyBindData = FileManager::JsonLoad( KEY_BIND_FILE_PATH );
	for ( auto& [ActionName, Obj] : KeyBindData.items() ) {
		for ( auto& [KeyType, Value] : Obj.items() ) {
			// �L�[���蓖�Ă̎擾.
			if ( KeyType == "Key" ) {
				for ( auto& v : Value ) pI->m_KeyBindList[ActionName].Key.emplace_back( ToKeyData( v ) );
			}
			// �{�^�����蓖�Ă̎擾.
			else if ( KeyType == "But" ) {
				for ( auto& v : Value ) pI->m_KeyBindList[ActionName].But.emplace_back( ToButData( v ) );
			}
		}
	}

	// ���O���Đ����Ȃ�.
	if ( pI->m_IsKeyLogPlay == true ){
		// �L�[���O��ǂݍ���.
		pI->m_pKeyLog->ReadKeyLog();

		// �L�[���~�߂�.
		CKeyInput	::IsAllKeyStop( true );
		CXInput		::IsAllKeyStop( true );

		// �������Œ�ɂ���.
		CRandom::IsRandLock( true );
	}

	CLog::PushLog( "���̓N���X�̏����� : ����" );
	return S_OK;
}

//----------------------------.
// �����ꂽ���Ƃ��擾����.
//----------------------------.
bool CInput::IsKeyPress( const std::string& Key )
{
	CInput* pI = GetInstance();
	if ( pI->m_IsNotActiveStop && !CDirectX11::IsWindowActive() ) return false;

	bool IsKeyPress = false;	// �L�[��������Ă��邩.
	bool IsButPress = false;	// �{�^���������ꂽ��.

	// �L�[�{�[�h��������Ă��邩���ׂ�.
	for ( auto& k : pI->m_KeyBindList[Key].Key ) {
		IsKeyPress = CKeyInput::IsKeyPress( k );
		if ( IsKeyPress ) break;
	}
	// �R���g���[����������Ă��邩���ׂ�.
	//	�L�[�{�[�h��������Ă����ꍇ���ׂ�K�v���Ȃ����ߏ����͍s��Ȃ�.
	if ( IsKeyPress == false ) {
		for ( auto& b : pI->m_KeyBindList[Key].But ) {
			IsButPress = CXInput::IsKeyPress( b );
			if ( IsButPress ) break;
		}
	}

	// �z��ԍ����擾.
	int No = 0;
	for ( auto& [k, v] : pI->m_KeyBindList ) {
		if ( k == Key ) break;
		No++;
	}

	// ���������ǂ���.
	if ( IsKeyPress || IsButPress ||
		 pI->m_pKeyLog->ReadFrameKey() & 1 << No )
	{
		pI->m_pKeyLog->AddKeyState( No );
		return true;
	}
	return false;
}

//----------------------------.
// �������u��.
//----------------------------.
bool CInput::IsKeyDown( const std::string& Key )
{
	CInput* pI = GetInstance();
	if ( pI->m_IsNotActiveStop && !CDirectX11::IsWindowActive() ) return false;

	bool IsKeyPress = false;	// �L�[��������Ă��邩.
	bool IsButPress = false;	// �{�^���������ꂽ��.

	// �L�[�{�[�h��������Ă��邩���ׂ�.
	for ( auto& k : pI->m_KeyBindList[Key].Key ) {
		IsKeyPress = CKeyInput::IsKeyDown( k );
		if ( IsKeyPress ) break;
	}
	// �R���g���[����������Ă��邩���ׂ�.
	//	�L�[�{�[�h��������Ă����ꍇ���ׂ�K�v���Ȃ����ߏ����͍s��Ȃ�.
	if ( IsKeyPress == false ) {
		for ( auto& b : pI->m_KeyBindList[Key].But ) {
			IsButPress = CXInput::IsKeyDown( b );
			if ( IsButPress ) break;
		}
	}

	// �z��ԍ����擾.
	int No = 0;
	for ( auto& [k, v] : pI->m_KeyBindList ) {
		if ( k == Key ) break;
		No++;
	}

	// ���������ǂ���.
	if ( IsKeyPress || IsButPress ||
		 pI->m_pKeyLog->ReadFrameKey() & 1 << No )
	{
		pI->m_pKeyLog->AddKeyState( No );
		return true;
	}
	return false;
}

//----------------------------.
// �������u��.
//----------------------------.
bool CInput::IsKeyUp( const std::string& Key )
{
	CInput* pI = GetInstance();
	if ( pI->m_IsNotActiveStop && !CDirectX11::IsWindowActive() ) return false;

	bool IsKeyPress = false;	// �L�[��������Ă��邩.
	bool IsButPress = false;	// �{�^���������ꂽ��.

	// �L�[�{�[�h��������Ă��邩���ׂ�.
	for ( auto& k : pI->m_KeyBindList[Key].Key ) {
		IsKeyPress = CKeyInput::IsKeyUp( k );
		if ( IsKeyPress ) break;
	}
	// �R���g���[����������Ă��邩���ׂ�.
	//	�L�[�{�[�h��������Ă����ꍇ���ׂ�K�v���Ȃ����ߏ����͍s��Ȃ�.
	if ( IsKeyPress == false ) {
		for ( auto& b : pI->m_KeyBindList[Key].But ) {
			IsButPress = CXInput::IsKeyUp( b );
			if ( IsButPress ) break;
		}
	}

	// �z��ԍ����擾.
	int No = 0;
	for ( auto& [k, v] : pI->m_KeyBindList ) {
		if ( k == Key ) break;
		No++;
	}

	// ���������ǂ���.
	if ( IsKeyPress || IsButPress ||
		 pI->m_pKeyLog->ReadFrameKey() & 1 << No )
	{
		pI->m_pKeyLog->AddKeyState( No );
		return true;
	}
	return false;
}

//----------------------------.
// ���������Ă���.
//----------------------------.
bool CInput::IsKeyRepeat( const std::string& Key )
{
	CInput* pI = GetInstance();
	if ( pI->m_IsNotActiveStop && !CDirectX11::IsWindowActive() ) return false;

	bool IsKeyPress = false;	// �L�[��������Ă��邩.
	bool IsButPress = false;	// �{�^���������ꂽ��.

	// �L�[�{�[�h��������Ă��邩���ׂ�.
	for ( auto& k : pI->m_KeyBindList[Key].Key ) {
		IsKeyPress = CKeyInput::IsKeyRepeat( k );
		if ( IsKeyPress ) break;
	}
	// �R���g���[����������Ă��邩���ׂ�.
	//	�L�[�{�[�h��������Ă����ꍇ���ׂ�K�v���Ȃ����ߏ����͍s��Ȃ�.
	if ( IsKeyPress == false ) {
		for ( auto& b : pI->m_KeyBindList[Key].But ) {
			IsButPress = CXInput::IsKeyRepeat( b );
			if ( IsButPress ) break;
		}
	}

	// �z��ԍ����擾.
	int No = 0;
	for ( auto& [k, v] : pI->m_KeyBindList ) {
		if ( k == Key ) break;
		No++;
	}

	// ���������ǂ���.
	if ( IsKeyPress || IsButPress ||
		 pI->m_pKeyLog->ReadFrameKey() & 1 << No )
	{
		pI->m_pKeyLog->AddKeyState( No );
		return true;
	}
	return false;
}

//---------------------------.
// �}�E�X���X�N���[���̒���.
//---------------------------.
bool CInput::IsScreenMiddleMouse()
{
	return false;
}

//---------------------------.
// �L�[�o�C���h�̃f�[�^�ϊ�.
//---------------------------.
int CInput::ToKeyData( std::string Key )
{
	for ( auto& l : InputList::KeyList ) {
		if ( l.first == Key ) return l.second;
	}
	return 0;
}
WORD CInput::ToButData( std::string But )
{
	for ( auto& l : InputList::ButList ) {
		if ( l.first == But ) return l.second;
	}
	return 0;
}

//---------------------------.
// �}�E�X�̍X�V.
//---------------------------.
void CInput::UpdateMouse()
{
	CInput* pI = GetInstance();

	// �O���ݒ�.
	pI->m_MouseOldPoint = pI->m_MouseNowPoint;
	// �}�E�X�J�[�\���ʒu�擾.
	GetCursorPos( &pI->m_MouseNowPoint );
	// �X�N���[�����W���N���C�A���g���W�ɕϊ�.
	ScreenToClient( pI->m_hWnd, &pI->m_MouseNowPoint );
}