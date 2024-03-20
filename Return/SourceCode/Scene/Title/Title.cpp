#include "Title.h"
#include "..\..\Object\GameObject\Widget\SceneWidget\TitleWidget\TitleWidget.h"
#include "..\..\Utility\Constant\Constant.h"

CTitle::CTitle()
	: m_pTitleWidget			( nullptr )
{
}

CTitle::~CTitle()
{
}

//---------------------------.
// �ǂݍ���.
//---------------------------.
bool CTitle::Init()
{
	// �萔�̏�����.
	CConstant::TitleLoad();

	m_pTitleWidget = std::make_unique<CTitleWidget>();
	m_pTitleWidget->Init();

	// �J�����̐ݒ�.
	m_pCameras = std::make_unique<CCameras>();
	m_pCameras->InitCamera( ECameraType::NormalCamera );
	CCameraManager::SetCamera( m_pCameras->GetCamera<CNormalCamera>() );

	// BGM���̐ݒ�.
	m_BGMName = "Title";
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CTitle::Update( const float& DeltaTime )
{
	// UI�̍X�V.
	m_pTitleWidget->Update( DeltaTime );

	// BGM�̍Đ�.
	CSoundManager::PlayBGM( m_BGMName );

	if ( CKeyInput::IsKeyDown( VK_SPACE ) ) {
		CSceneManager::SceneChange( ESceneList::GameMain );
	}
}

//---------------------------.
// ���f���̕`��.
//---------------------------.
void CTitle::ModelRender()
{
}

//---------------------------.
// �X�v���C�g(UI)�̕`��.
//---------------------------.
void CTitle::SpriteUIRender()
{
	// UI�̕`��.
	m_pTitleWidget->UIRender();
	m_pTitleWidget->Render();
}

//---------------------------.
// �X�v���C�g(3D)/Effect�̕`��.
//	_A�F��ɕ\������� / _B�F��ɕ\�������.
//---------------------------.
void CTitle::Sprite3DRender_A()
{
}
void CTitle::Sprite3DRender_B()
{
}
