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
// 読み込み.
//---------------------------.
bool CGameClear::Init()
{
	// 定数の初期化.
	CConstant::GameClearLoad();

	m_pGameClearWidget = std::make_unique<CGameClearWidget>();
	m_pGameClearWidget->Init();

	// カメラの設定.
	m_pCameras = std::make_unique<CCameras>();
	m_pCameras->InitCamera( ECameraType::NormalCamera );
	CCameraManager::SetCamera( m_pCameras->GetCamera<CNormalCamera>() );

	// BGM名の設定.
	m_BGMName = "GameClear";

	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CGameClear::Update( const float& DeltaTime )
{
	// UIの更新.
	m_pGameClearWidget->Update( DeltaTime );

	// BGMの再生.
	CSoundManager::PlayBGM( m_BGMName );
}

//---------------------------.
// モデルの描画.
//---------------------------.
void CGameClear::ModelRender()
{
}

//---------------------------.
// スプライト(UI)の描画.
//---------------------------.
void CGameClear::SpriteUIRender()
{
	// UIの描画.
	m_pGameClearWidget->UIRender();
	m_pGameClearWidget->Render();
}

//---------------------------.
// スプライト(3D)/Effectの描画.
//	_A：後に表示される / _B：先に表示される.
//---------------------------.
void CGameClear::Sprite3DRender_A()
{
}
void CGameClear::Sprite3DRender_B()
{
}
