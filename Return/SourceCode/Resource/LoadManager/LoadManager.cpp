#include "LoadManager.h"
#include "..\MeshResource\MeshResource.h"
#include "..\SpriteResource\SpriteResource.h"
#include "..\EffectResource\EffectResource.h"
#include "..\FontResource\FontResource.h"
#include "..\..\Common\SoundManeger\SoundManeger.h"

CLoadManager::CLoadManager()
	: m_Thread				()
	, m_Mutex				()
	, m_isLoadEnd			( false )
	, m_isThreadJoined		( false )
	, m_isLoadFailed		( false )
{
}

CLoadManager::~CLoadManager()
{
	if ( m_isLoadFailed ) m_Thread.join();
}

//---------------------------.
// �ǂݍ���.
//---------------------------.
void CLoadManager::LoadResource( HWND hWnd )
{
	if ( FAILED( CSoundManager::SoundLoad( hWnd ) ) ){
		m_isLoadFailed = true;
		return;
	}

	// �ǂݍ��݊֐�.
	auto Load = [&]( HWND hWnd ) {
		std::unique_lock<std::mutex> lock( m_Mutex );

		CLog::PushLog( "------ �X���b�h���[�h�J�n ------" );
		if ( FAILED( CSpriteResource::SpriteLoad() ) ){
			m_isLoadFailed = true;
			return;
		}
		if ( FAILED( CEffectResource::EffectLoad() ) ) {
			m_isLoadFailed = true;
			return;
		}
		if ( FAILED( CFontResource::FontLoad() ) ) {
			m_isLoadFailed = true;
			return;
		}
		if ( FAILED( CMeshResource::MeshLoad() ) ) {
			m_isLoadFailed = true;
			return;
		}
		m_isLoadEnd = true;
		CLog::PushLog( "------ �X���b�h���[�h�I�� ------" );
		CLog::PushLog( "------ ���C�����[�v�J�n ------" );
	};

	// �ǂݍ���.
	m_Thread = std::thread( Load, hWnd );
}

//---------------------------.
// �X���b�h�̉��.
//---------------------------.
bool CLoadManager::ThreadRelease()
{
	if( m_isThreadJoined == true	) return true;
	if( m_isLoadEnd		 == false	) return false;

	DWORD code = -1;
	GetExitCodeThread( m_Thread.native_handle(),&code );
	if( code == 0xffffffff ){
		m_isThreadJoined = true;
	}
	if ( code == 0 ) {
		m_Thread.join();
		while ( 1 ) {
			if ( m_Thread.joinable() != true ) {
				m_isThreadJoined = true;
				break;
			}
		}
	}
	return true;
}