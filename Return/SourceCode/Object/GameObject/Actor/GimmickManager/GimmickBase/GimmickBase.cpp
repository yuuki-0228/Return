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
// �X�V.
//---------------------------.
void CGimmickBase::Update( const float& DeltaTime )
{
	m_DeltaTime = DeltaTime;

	GimmickUpdate();	// �M�~�b�N�̍X�V.

	// �g�����X�t�H�[���̍X�V.
	m_Transform = m_SpriteState.Transform;

	UpdateCollision();
	CActorCollisionManager::PushObject( this, ECollObjType::BeAHit );
}

//---------------------------.
// �`��.
//---------------------------.
void CGimmickBase::Render()
{
	m_pSprite->RenderUI( &m_SpriteState );

	CCollisionRender::PushSphere2D( m_pCollisions->GetCollision<CSphere2D>() );
}

//---------------------------.
// �M�~�b�N�̔z�u.
//---------------------------.
void CGimmickBase::Arrangement( const D3DXPOSITION3& Pos, const SGimmickState& State )
{
	// �G�̏���ۑ�.
	m_StartGimmickState = State;
	m_GimmickState		= State;

	// �\���ʒu�̐ݒ�.
	m_SpriteState.Transform.Position	= Pos;
	m_IsDisp							= true;
}
