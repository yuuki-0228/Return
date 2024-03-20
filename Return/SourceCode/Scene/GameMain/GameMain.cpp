#include "GameMain.h"
#include "..\..\Common\Sprite\Sprite.h"
#include "..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\Object\GameObject\Actor\Character\Player\Player.h"
#include "..\..\Object\GameObject\Actor\Character\EnemyManager\EnemyManager.h"
#include "..\..\Object\GameObject\Actor\StageManager\StageManager.h"
#include "..\..\Object\GameObject\Widget\SceneWidget\GameMainWidget\GameMainWidget.h"
#include "..\..\Utility\Constant\Constant.h"

CGameMain::CGameMain()
	: m_pBack			( nullptr )
	, m_pPlayer			( nullptr )
	, m_pStageManager	( nullptr )
	, m_GameMainWidget	( nullptr )
{
}

CGameMain::~CGameMain()
{
}

//---------------------------.
// ������.
//---------------------------.
bool CGameMain::Init()
{
	// �萔�̏�����.
	CConstant::GameMainLoad();

	// �J�����̐ݒ�.
	m_pCameras = std::make_unique<CCameras>();
	m_pCameras->InitCamera( ECameraType::NormalCamera );
	CCameraManager::SetCamera( m_pCameras->GetCamera<CNormalCamera>() );

	// �w�i�̎擾.
	m_pBack = CSpriteResource::GetSprite( "Back" );

	m_pPlayer			= std::make_shared<CPlayer>();
	m_pStageManager		= std::make_shared<CStageManager>();
	m_GameMainWidget	= std::make_unique<CGameMainWidget>();

	m_pPlayer->Init();
	m_pStageManager->Init();
	m_GameMainWidget->Init();
	m_GameMainWidget->SetPlayer( m_pPlayer.get() );

	// BGM�̐ݒ�.
	m_BGMName = "12-13";
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CGameMain::Update( const float& DeltaTime )
{
	// �X�V.
	m_pPlayer->Update( DeltaTime );
	m_pStageManager->Update( DeltaTime );

	// �����蔻�菈��.
	CActorCollisionManager::Collision();

	// �����蔻��I����̍X�V.
	m_pPlayer->LateUpdate( DeltaTime );
	m_pStageManager->LateUpdate( DeltaTime );

	// �f�o�b�N�̍X�V.
	m_pPlayer->DebugUpdate( DeltaTime );
	m_pStageManager->DebugUpdate( DeltaTime );


	// UI�̍X�V.
	m_GameMainWidget->Update( DeltaTime );
}

//---------------------------.
// �`��.
//---------------------------.
void CGameMain::ModelRender()
{
}

//---------------------------.
// �X�v���C�g(UI)�̕`��.
//---------------------------.
void CGameMain::SpriteUIRender()
{
	m_pBack->RenderUI();

	m_pStageManager->Render();
	m_pPlayer->Render();

	m_GameMainWidget->UIRender();
	m_GameMainWidget->Render();
}

//---------------------------.
// �X�v���C�g(3D)/Effect�̕`��.
//	_A�F��ɕ\������� / _B�F��ɕ\�������.
//---------------------------.
void CGameMain::Sprite3DRender_A()
{
}
void CGameMain::Sprite3DRender_B()
{
}
