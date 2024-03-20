#include "SceneManager.h"
#include "..\Title\Title.h"
#include "..\GameMain\GameMain.h"
#include "..\GameClear\GameClear.h"
#include "..\UIEdit\UIEdit.h"
#include "..\..\Common\DirectX\DirectX11.h"
#include "..\..\Common\SoundManeger\SoundManeger.h"
#include "..\..\Object\GameObject\ActorCollisionManager\ActorCollisionManager.h"
#include "..\..\Utility\ImGuiManager\DebugWindow\DebugWindow.h"
#include "..\..\Utility\ImGuiManager\MessageWindow\MessageWindow.h"
#include "..\..\Utility\StringConversion\StringConversion.h"
#include "..\..\Utility\Input\Input.h"

CSceneManager::CSceneManager()
	: m_pScene			( nullptr )
	, m_NowScene		( ESceneList::Title )
	, m_ChangeScene		( ESceneList::None )
	, m_IsSceneChange	( false )
	, m_IsFadeOut		( false )
	, m_UpdateStopFlag	( false )
	, m_IsSceneStop		( false )
	, m_IsPause			( false )
{
	m_pScene	= std::make_unique<CTitle>();

	// ������.
	m_pScene->Init();
	CFadeManager::Init();
}

CSceneManager::~CSceneManager()
{
}

//---------------------------.
// �C���X�^���X�̎擾.
//---------------------------.
CSceneManager* CSceneManager::GetInstance()
{
	static std::unique_ptr<CSceneManager> pInstance = std::make_unique<CSceneManager>();
	return pInstance.get();
}

//---------------------------.
// �X�V.
//---------------------------.
void CSceneManager::Update( const float& DeltaTime )
{
	CSceneManager* pI = GetInstance();

	// �V�[���̍X�V.
	if ( pI->m_UpdateStopFlag == false && pI->m_IsSceneStop == false ) {
		pI->m_pScene->Update( DeltaTime );
	}

	// �V�[���̕ύX�̍X�V.
	SceneChangeUpdate();

	CDebugWindow::PushProc( u8"SceneManager",
	[&] {
		if ( CImGuiManager::Button( u8"�^�C�g���ֈړ�"			) ) SceneChange( ESceneList::Title		);
		if ( CImGuiManager::Button( u8"�Q�[�����C���ֈړ�"		) ) SceneChange( ESceneList::GameMain	);
		if ( CImGuiManager::Button( u8"�Q�[���N���A�ֈړ�"		) ) SceneChange( ESceneList::GameClear	);
		if ( CImGuiManager::Button( u8"UI�G�f�B�^�ֈړ�"			) ) SceneChange( ESceneList::UIEdit		);
	} );
}

//---------------------------.
// �`��.
//---------------------------.
void CSceneManager::Render()
{
	CSceneManager* pI = GetInstance();

	// �V�[���̕`��.
	pI->m_pScene->Sprite3DRender_B();
	pI->m_pScene->ModelRender();
	pI->m_pScene->Sprite3DRender_A();
	CDirectX11::SetDepth( false );
	pI->m_pScene->SpriteUIRender();
	CDirectX11::SetDepth( true );

	// �t�F�[�h�̕`��.
	CFadeManager::Render();
}

//---------------------------.
// �V�[���̕ύX.
//---------------------------.
void CSceneManager::SceneChange( const ESceneList NewScene, const EFadeType FadeType, const float FadeSpeed )
{
	CSceneManager* pI = GetInstance();

	if ( pI->m_IsSceneChange ) return;

	// �V�[���J�ڏ���ۑ�.
	pI->m_IsSceneChange = true;
	pI->m_ChangeScene	= NewScene;
	pI->m_NowScene		= NewScene;

	// �t�F�[�h����ۑ�.
	pI->m_FadeType		= FadeType;
	pI->m_FadeSpeed		= FadeSpeed;

	CMessageWindow::PushMessage( StringConversion::Enum( NewScene ) + u8"�Ɉړ����܂���", Color4::Yellow );
}

//---------------------------.
// �V�[���̕ύX�̍X�V.
//	�V�[���̕ύX���s���Ă��Ȃ�
//	�X�V���~�߂Ă��Ȃ� �ꍇ�͏����͍s��Ȃ�.
//---------------------------.
void CSceneManager::SceneChangeUpdate()
{
	CSceneManager* pI = GetInstance();

	if ( pI->m_IsSceneChange == false && pI->m_UpdateStopFlag == false ) return;

	// �X�V���~������.
	pI->m_UpdateStopFlag = true;

	// �t�F�[�h�C�����s��.
	if ( pI->m_IsFadeOut == false && CFadeManager::FadeIn( pI->m_FadeType, pI->m_FadeSpeed ) ) {
		// �����蔻�胊�X�g������������.
		CActorCollisionManager::Reset();

		// �V�[����ύX.
		switch ( pI->m_ChangeScene ) {
		case ESceneList::Title:
			CSoundManager::BGMAllStop();
			pI->m_pScene = std::make_unique<CTitle>();
			break;
		case ESceneList::GameMain:
			CSoundManager::BGMAllStop();
			pI->m_pScene = std::make_unique<CGameMain>();
			break;
		case ESceneList::GameClear:
			CSoundManager::BGMAllStop();
			pI->m_pScene = std::make_unique<CGameClear>();
			break;
		case ESceneList::UIEdit:
			CSoundManager::BGMAllStop();
			pI->m_pScene = std::make_unique<CUIEdit>();
			break;
		default:
			break;
		}

		// �V�����V�[���̏�����.
		pI->m_pScene->Init();
		pI->m_IsFadeOut = true;

		// �R���g���[���̐U���̒�~.
		CXInput::SetVibration( 0.0f, 0.0f );
	}
	// �t�F�[�h�A�E�g���s��.
	else if ( pI->m_IsFadeOut && CFadeManager::FadeOut( pI->m_FadeType, pI->m_FadeSpeed ) ) {
		pI->m_IsFadeOut			= false;
		pI->m_UpdateStopFlag	= false;
		pI->m_IsSceneChange		= false;
		pI->m_ChangeScene		= ESceneList::None;
	}
}
