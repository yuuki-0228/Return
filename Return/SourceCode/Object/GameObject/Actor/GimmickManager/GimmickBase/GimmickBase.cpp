#include "GimmickBase.h"

CGimmickBase::CGimmickBase()
	: m_pSprite		( nullptr )
	, m_SpriteState	()
{
}

CGimmickBase::~CGimmickBase()
{
}

//---------------------------.
// 更新.
//---------------------------.
void CGimmickBase::Update( const float& DeltaTime )
{
	m_DeltaTime = DeltaTime;

	GimmickUpdate();	// ギミックの更新.

	// トランスフォームの更新.
	m_Transform = m_SpriteState.Transform;

	UpdateCollision();
	CActorCollisionManager::PushObject( this, ECollObjType::BeAHit );
}

//---------------------------.
// 描画.
//---------------------------.
void CGimmickBase::Render()
{
	m_pSprite->RenderUI( &m_SpriteState );

	CCollisionRender::PushSphere2D( m_pCollisions->GetCollision<CSphere2D>() );
}

//---------------------------.
// ギミックの配置.
//---------------------------.
void CGimmickBase::Arrangement( const D3DXPOSITION3& Pos, const SGimmickState& State )
{
	// 敵の情報を保存.
	m_StartGimmickState = State;
	m_GimmickState		= State;

	// 表示位置の設定.
	m_SpriteState.Transform.Position	= Pos;
	m_IsDisp							= true;
}
