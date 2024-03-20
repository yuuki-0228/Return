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
// ������.
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
// �X�V.
//---------------------------.
void CStageManager::Update( const float& DeltaTime )
{
	CGimmickManager::Update( DeltaTime );
	CEnemyManager::Update( DeltaTime );
	CBulletManager::Update( DeltaTime );
	CPotionAreaManager::Update( DeltaTime );
}

//---------------------------.
// �����蔻��I����Ăяo�����X�V.
//---------------------------.
void CStageManager::LateUpdate( const float& DeltaTime )
{
//	CGimmickManager::LateUpdate( DeltaTime );
//	CEnemyManager::LateUpdate( DeltaTime );
	CBulletManager::LateUpdate( DeltaTime );
	CPotionAreaManager::LateUpdate( DeltaTime );
}

//---------------------------.
// �`��.
//---------------------------.
void CStageManager::Render()
{
	// �`�惊�X�g�̍X�V.
	RenderListUpdate();

	for ( auto& [Y, Obj] : m_pRenderList ) {
		Obj->Render();
	}
}

//---------------------------.
// �`�惊�X�g�̍X�V.
//---------------------------.
void CStageManager::RenderListUpdate()
{
	m_pRenderList.clear();

	// �`�惊�X�g�̌���.
	m_pRenderList											= CGimmickManager::GetRenderList();
	const std::vector<std::pair<float, CActor*>> EnemyList	= CEnemyManager::GetRenderList();
	const std::vector<std::pair<float, CActor*>> BulletList	= CBulletManager::GetRenderList();
	const std::vector<std::pair<float, CActor*>> AreaList	= CPotionAreaManager::GetRenderList();
	m_pRenderList.insert( m_pRenderList.end(), EnemyList.begin(),	EnemyList.end() );
	m_pRenderList.insert( m_pRenderList.end(), BulletList.begin(),	BulletList.end() );
	m_pRenderList.insert( m_pRenderList.end(), AreaList.begin(),	AreaList.end() );

	// �`�惊�X�g����ѕς���.
	std::sort( m_pRenderList.begin(), m_pRenderList.end() );
}
