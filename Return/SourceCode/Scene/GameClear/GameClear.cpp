#include "GameClear.h"
#include "..\..\Object\GameObject\Widget\SceneWidget\GameClearWidget\GameClearWidget.h"
#include "..\..\Utility\Constant\Constant.h"

CGameClear::CGameClear()
	: m_pGameClearWidget			( nullptr )
{
}

CGameClear::~CGameClear()
{
}

//---------------------------.
// �ǂݍ���.
//---------------------------.
bool CGameClear::Init()
{
	// �萔�̏�����.
	CConstant::GameClearLoad();

	m_pGameClearWidget = std::make_unique<CGameClearWidget>();
	m_pGameClearWidget->Init();

	// �J�����̐ݒ�.
	m_pCameras = std::make_unique<CCameras>();
	m_pCameras->InitCamera( ECameraType::NormalCamera );
	CCameraManager::SetCamera( m_pCameras->GetCamera<CNormalCamera>() );

	// BGM���̐ݒ�.
	m_BGMName = "GameClear";

	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CGameClear::Update( const float& DeltaTime )
{
	// UI�̍X�V.
	m_pGameClearWidget->Update( DeltaTime );

	// BGM�̍Đ�.
	CSoundManager::PlayBGM( m_BGMName );
}

//---------------------------.
// ���f���̕`��.
//---------------------------.
void CGameClear::ModelRender()
{
}

//---------------------------.
// �X�v���C�g(UI)�̕`��.
//---------------------------.
void CGameClear::SpriteUIRender()
{
	// UI�̕`��.
	m_pGameClearWidget->UIRender();
	m_pGameClearWidget->Render();
}

//---------------------------.
// �X�v���C�g(3D)/Effect�̕`��.
//	_A�F��ɕ\������� / _B�F��ɕ\�������.
//---------------------------.
void CGameClear::Sprite3DRender_A()
{
}
void CGameClear::Sprite3DRender_B()
{
}
