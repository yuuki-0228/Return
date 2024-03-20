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
// 初期化.
//---------------------------.
bool CGameMain::Init()
{
	// 定数の初期化.
	CConstant::GameMainLoad();

	// カメラの設定.
	m_pCameras = std::make_unique<CCameras>();
	m_pCameras->InitCamera( ECameraType::NormalCamera );
	CCameraManager::SetCamera( m_pCameras->GetCamera<CNormalCamera>() );

	// 背景の取得.
	m_pBack = CSpriteResource::GetSprite( "Back" );

	m_pPlayer			= std::make_shared<CPlayer>();
	m_pStageManager		= std::make_shared<CStageManager>();
	m_GameMainWidget	= std::make_unique<CGameMainWidget>();

	m_pPlayer->Init();
	m_pStageManager->Init();
	m_GameMainWidget->Init();
	m_GameMainWidget->SetPlayer( m_pPlayer.get() );

	// BGMの設定.
	m_BGMName = "12-13";
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CGameMain::Update( const float& DeltaTime )
{
	// 更新.
	m_pPlayer->Update( DeltaTime );
	m_pStageManager->Update( DeltaTime );

	// 当たり判定処理.
	CActorCollisionManager::Collision();

	// 当たり判定終了後の更新.
	m_pPlayer->LateUpdate( DeltaTime );
	m_pStageManager->LateUpdate( DeltaTime );

	// デバックの更新.
	m_pPlayer->DebugUpdate( DeltaTime );
	m_pStageManager->DebugUpdate( DeltaTime );


	// UIの更新.
	m_GameMainWidget->Update( DeltaTime );
}

//---------------------------.
// 描画.
//---------------------------.
void CGameMain::ModelRender()
{
}

//---------------------------.
// スプライト(UI)の描画.
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
// スプライト(3D)/Effectの描画.
//	_A：後に表示される / _B：先に表示される.
//---------------------------.
void CGameMain::Sprite3DRender_A()
{
}
void CGameMain::Sprite3DRender_B()
{
}
