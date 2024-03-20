#include "WindowSetting.h"

namespace {
	constexpr char WINDOW_SETTING_FILE_PATH[] = "Data\\Parameter\\Config\\WindowSetting.json";	// �E�B���h�E�̐ݒ�̃t�@�C���p�X.
}

CWindowSetting::CWindowSetting()
	: m_Size					( 0.0f, 0.0f )
	, m_Pos						( 0.0f, 0.0f )
	, m_AppName					( "" )
	, m_WndName					( "" )
	, m_IsDispCenter			( false )
	, m_IsDispMouseCursor		( false )
	, m_IsFPSRender				( false )
	, m_IsFullScreenLock		( false )
	, m_IsSizeLock				( false )
	, m_IsMaxLock				( false )
	, m_IsMinLock				( false )
	, m_IsPopUpWnd				( false )
	, m_IsShowOnTop				( false )
	, m_IsLogStop				( false )
	, m_IsInputNotActiveStop	( false )
{
	Init();
}

CWindowSetting::~CWindowSetting()
{
}

//---------------------------.
// �������֐�.
//---------------------------.
void CWindowSetting::Init()
{
	// ���݂̐ݒ���擾����.
	json WndData = FileManager::JsonLoad( WINDOW_SETTING_FILE_PATH );
	m_Size.x				= WndData["Size"]["w"];
	m_Size.y				= WndData["Size"]["h"];
	m_Pos.x					= WndData["Pos"]["x"];
	m_Pos.y					= WndData["Pos"]["y"];
	m_AppName				= WndData["Name"]["App"];
	m_WndName				= WndData["Name"]["Wnd"];
	m_IsDispCenter			= WndData["IsDispCenter"];
	m_IsDispMouseCursor		= WndData["IsDispMouseCursor"];
	m_IsFPSRender			= WndData["IsFPSRender"];
	m_IsFullScreenLock		= WndData["IsFullScreenLock"];
	m_IsInputNotActiveStop	= WndData["IsInputNotActiveStop"];
	m_IsSizeLock			= WndData["IsSizeLock"];
	m_IsMaxLock				= WndData["IsMaxLock"];
	m_IsMinLock				= WndData["IsMinLock"];
	m_IsPopUpWnd			= WndData["IsPopUpWnd"];
	m_IsShowOnTop			= WndData["IsShowOnTop"];
	m_IsLogStop				= WndData["IsLogStop"];
	m_IsDispCloseMessage	= WndData["IsDispCloseMessage"];

	// �`�揈���̐ݒ�.
	m_RenderFunc = [&] () {
		ImGui::Text( u8"�E�B���h�E�̐ݒ�����邱�Ƃ��ł��܂��B" );
		ImGui::Text( u8"�ۑ���A�ċN�����邱�ƂŐݒ肪���f����܂��B" );
		ImGui::Separator();

		// �A�v�����̋L����.
		ImGui::Text( u8"�A�v���P�[�V�������@�@: " );
		ImGui::SameLine();
		ImGui::InputText( "##AppName", &m_AppName );

		// �E�B���h�E���̋L����.
		ImGui::Text( u8"�E�B���h�E���@�@�@�@�@: " );
		ImGui::SameLine();
		ImGui::InputText( "##WndName", &m_WndName );

		// �E�B���h�E�̃T�C�Y�w��.
		ImGui::Separator();
		ImGui::Text( u8"�E�B���h�E�T�C�Y�@�@�@: " );
		ImGui::SameLine();
		ImGui::InputFloat2( "##InputWndSize", m_Size );

		// �E�B���h�E�̕\���ʒu�w��.
		ImGui::Text( u8"�E�B���h�E�̕\���ʒu�@: " );
		ImGui::SameLine();
		ImGui::InputFloat2( "##InputWndPose", m_Pos );

		// �e�t���O�̃`�F�b�N�{�b�N�X�̕\��.
		ImGui::Separator();
		CImGuiManager::CheckBox( u8"�����ɕ\��",				&m_IsDispCenter );
		ImGui::SameLine();
		CImGuiManager::HelpMarker( u8"�N�����A��ʂ̒����ɔz�u���܂��B" );
		CImGuiManager::CheckBox( u8"�t���X�N���[���̋֎~",	&m_IsFullScreenLock );
		ImGui::SameLine();
		CImGuiManager::HelpMarker( u8"�uALT + Enter�v�Ńt���X�N���[���ɂł��Ȃ��悤�ɂ��܂��B" );
		CImGuiManager::CheckBox( u8"�T�C�Y�̌Œ艻",			&m_IsSizeLock );
		ImGui::SameLine();
		CImGuiManager::HelpMarker( u8"�T�C�Y��ύX�ł��Ȃ��悤�ɂ��܂��B" );
		CImGuiManager::CheckBox( u8"�ő剻�̖�����",			&m_IsMaxLock );
		ImGui::SameLine();
		CImGuiManager::HelpMarker( u8"�ő剻�{�^���������Ȃ��悤�ɂ��܂��B" );
		CImGuiManager::CheckBox( u8"�ŏ����̖�����",			&m_IsMinLock );
		ImGui::SameLine();
		CImGuiManager::HelpMarker( u8"�ŏ����{�^���������Ȃ��悤�ɂ��܂��B" );
		CImGuiManager::CheckBox( u8"�|�b�v�A�b�v�E�B���h",	&m_IsPopUpWnd );
		ImGui::SameLine();
		CImGuiManager::HelpMarker( u8"�E�B���h�E���|�b�v�A�b�v�E�B���h�E�i �g���� �j�ɂ��܂��B" );
		CImGuiManager::CheckBox( u8"�őO�ʂŌŒ�",			&m_IsShowOnTop );
		ImGui::SameLine();
		CImGuiManager::HelpMarker( u8"�E�B���h�E���őO�ʂŌŒ肵�܂��B" );
		CImGuiManager::CheckBox( u8"���O�t�@�C���̒�~",		&m_IsLogStop );
		ImGui::SameLine();
		CImGuiManager::HelpMarker( u8"���O�t�@�C���̍쐬���~���܂��B" );
		CImGuiManager::CheckBox( u8"�A�N�e�B�u�E�B���h�E�ł͖����ꍇ�̑���̒�~", &m_IsInputNotActiveStop );
		ImGui::SameLine();
		CImGuiManager::HelpMarker( u8"�A�N�e�B�u�E�B���h�E�ł͖����ꍇ������~�����܂��B" );
		CImGuiManager::CheckBox( u8"���b�Z�[�W�{�b�N�X�̕\��", &m_IsDispCloseMessage );
		ImGui::SameLine();
		CImGuiManager::HelpMarker( u8"�uEscape�v�����������ɃE�B���h�E����邩�̃��b�Z�[�W�{�b�N�X��\�������܂��B" );;
		CImGuiManager::CheckBox( u8"FPS�̕\��", &m_IsFPSRender );
		ImGui::SameLine();
		CImGuiManager::HelpMarker( u8"��ʂ�FPS��\�������܂��B" );
		CImGuiManager::CheckBox( u8"�}�E�X�J�[�\���̕\��", &m_IsDispMouseCursor );
		ImGui::SameLine();
		CImGuiManager::HelpMarker( u8"�}�E�X�J�[�\������ʂɕ\�������܂��B" );

		// �ۑ��{�^��.
		ImGui::Separator();
		if ( CImGuiManager::Button( u8"�ۑ�##WindowSettingSave" ) ) {
			// ���݂̃E�B���h�E�ݒ��json�`���ŕۑ�����.
			json j;
			j[".Comment"] = {
				u8"�E�B���h�E�̐ݒ肪�s���܂��B",
				u8"----------------------------------------------",
				u8"[IsDispCenter        ] : �N�����ɉ�ʂ̒����ɔz�u���邩",
				u8"[IsDispCloseMessage  ] : �uEscape�v�����������Ƀ��b�Z�[�W�{�b�N�X��\�������邩",
				u8"[IsDispMouseCursor   ] : ��ʂɃ}�E�X�J�[�\����\�����邩",
				u8"[IsFPSRender         ] : ��ʂ�FPS��`�悷�邩",
				u8"[IsFullScreenLock    ] : �uALT + Enter�v�Ńt���X�N���[���ɂł��Ȃ��悤�ɂ��邩",
				u8"[IsInputNotActiveStop] : �A�N�e�B�u�E�B���h�E�ł͖����ꍇ������~�����邩",
				u8"[IsLogStop           ] : ���O�t�@�C���̍쐬���~���邩",
				u8"[IsMaxLock           ] : �ő剻�{�^���������Ȃ��悤�ɂ��邩",
				u8"[IsMinLock           ] : �ŏ����{�^���������Ȃ��悤�ɂ��邩",
				u8"[IsPopUpWnd          ] : �E�B���h�E���|�b�v�A�b�v�E�B���h�E�i �g���� �j�ɂ��邩",
				u8"[IsShowOnTop         ] : �E�B���h�E���őO�ʂŌŒ肷�邩",
				u8"[IsSizeLock          ] : �T�C�Y��ύX�ł��Ȃ��悤�ɂ��邩",
				u8"[Name                ] : �A�v�� / �E�B���h�E��",
				u8"[Pos                 ] : �E�B���h�E�̕\���ʒu",
				u8"[Size                ] : �E�B���h�E�̃T�C�Y",
				u8"----------------------------------------------"
			};
			j["Size"]["w"]				= m_Size.x;
			j["Size"]["h"]				= m_Size.y;
			j["Pos"]["x"]				= m_Pos.x;
			j["Pos"]["y"]				= m_Pos.y;
			j["Name"]["App"]			= m_AppName;
			j["Name"]["Wnd"]			= m_WndName;
			j["IsDispCenter"]			= m_IsDispCenter;
			j["IsDispCloseMessage"]		= m_IsDispCloseMessage;
			j["IsDispMouseCursor"]		= m_IsDispMouseCursor;
			j["IsFPSRender"]			= m_IsFPSRender;
			j["IsFullScreenLock"]		= m_IsFullScreenLock;
			j["IsInputNotActiveStop"]	= m_IsInputNotActiveStop;
			j["IsSizeLock"]				= m_IsSizeLock;
			j["IsMaxLock"]				= m_IsMaxLock;
			j["IsMinLock"]				= m_IsMinLock;
			j["IsPopUpWnd"]				= m_IsPopUpWnd;
			j["IsShowOnTop"]			= m_IsShowOnTop;
			j["IsLogStop"]				= m_IsLogStop;
			FileManager::JsonSave( WINDOW_SETTING_FILE_PATH, j );
		}
	};

	// �E�B���h�E���̐ݒ�.
	InitWndName( "WindowSetting" );
}