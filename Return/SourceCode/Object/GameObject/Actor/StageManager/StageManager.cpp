#include "StageManager.h"
#include "..\GimmickManager\GimmickManager.h"
#include "..\PotionAreaManager\PotionAreaManager.h"
#include "..\Character\EnemyManager\EnemyManager.h"
#include "..\BulletManager\BulletManager.h"
#include <algorithm>

CStageManager::CStageManager()
	: m_pRenderList		()
{
}

CStageManager::~CStageManager()
{
}

//---------------------------.
// 初期化.
//---------------------------.
bool CStageManager::Init()
{
	CGimmickManager::Init();
	CEnemyManager::Init();
	CBulletManager::Init();
	CPotionAreaManager::Init();
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CStageManager::Update( const float& DeltaTime )
{
	CGimmickManager::Update( DeltaTime );
	CEnemyManager::Update( DeltaTime );
	CBulletManager::Update( DeltaTime );
	CPotionAreaManager::Update( DeltaTime );
}

//---------------------------.
// 当たり判定終了後呼び出される更新.
//---------------------------.
void CStageManager::LateUpdate( const float& DeltaTime )
{
//	CGimmickManager::LateUpdate( DeltaTime );
//	CEnemyManager::LateUpdate( DeltaTime );
	CBulletManager::LateUpdate( DeltaTime );
	CPotionAreaManager::LateUpdate( DeltaTime );
}

//---------------------------.
// 描画.
//---------------------------.
void CStageManager::Render()
{
	// 描画リストの更新.
	RenderListUpdate();

	for ( auto& [Y, Obj] : m_pRenderList ) {
		Obj->Render();
	}
}

//---------------------------.
// 描画リストの更新.
//---------------------------.
void CStageManager::RenderListUpdate()
{
	m_pRenderList.clear();

	// 描画リストの結合.
	m_pRenderList											= CGimmickManager::GetRenderList();
	const std::vector<std::pair<float, CActor*>> EnemyList	= CEnemyManager::GetRenderList();
	const std::vector<std::pair<float, CActor*>> BulletList	= CBulletManager::GetRenderList();
	const std::vector<std::pair<float, CActor*>> AreaList	= CPotionAreaManager::GetRenderList();
	m_pRenderList.insert( m_pRenderList.end(), EnemyList.begin(),	EnemyList.end() );
	m_pRenderList.insert( m_pRenderList.end(), BulletList.begin(),	BulletList.end() );
	m_pRenderList.insert( m_pRenderList.end(), AreaList.begin(),	AreaList.end() );

	// 描画リストを並び変える.
	std::sort( m_pRenderList.begin(), m_pRenderList.end() );
}
