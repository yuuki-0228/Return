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
// 読み込み.
//---------------------------.
bool CTitle::Init()
{
	// 定数の初期化.
	CConstant::TitleLoad();

	m_pTitleWidget = std::make_unique<CTitleWidget>();
	m_pTitleWidget->Init();

	// カメラの設定.
	m_pCameras = std::make_unique<CCameras>();
	m_pCameras->InitCamera( ECameraType::NormalCamera );
	CCameraManager::SetCamera( m_pCameras->GetCamera<CNormalCamera>() );

	// BGM名の設定.
	m_BGMName = "Title";
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CTitle::Update( const float& DeltaTime )
{
	// UIの更新.
	m_pTitleWidget->Update( DeltaTime );

	// BGMの再生.
	CSoundManager::PlayBGM( m_BGMName );

	if ( CKeyInput::IsKeyDown( VK_SPACE ) ) {
		CSceneManager::SceneChange( ESceneList::GameMain );
	}
}

//---------------------------.
// モデルの描画.
//---------------------------.
void CTitle::ModelRender()
{
}

//---------------------------.
// スプライト(UI)の描画.
//---------------------------.
void CTitle::SpriteUIRender()
{
	// UIの描画.
	m_pTitleWidget->UIRender();
	m_pTitleWidget->Render();
}

//---------------------------.
// スプライト(3D)/Effectの描画.
//	_A：後に表示される / _B：先に表示される.
//---------------------------.
void CTitle::Sprite3DRender_A()
{
}
void CTitle::Sprite3DRender_B()
{
}
