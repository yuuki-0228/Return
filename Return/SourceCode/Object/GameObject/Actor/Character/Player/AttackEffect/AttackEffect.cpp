#include "AttackEffect.h"
#include "..\..\..\..\..\..\Common\SoundManeger\SoundManeger.h"
#include "..\..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"

CAttackEffect::CAttackEffect()
	: m_pAttackSpriteList		( AttackNo::Max )
	, m_AttackStateBaseList		( AttackNo::Max )
	, m_AttackStateList			( AttackNo::Max )
	, m_AttackSEList			( AttackNo::Max )
	, m_IsNowEffectPlay			( nullptr )
{
}

CAttackEffect::~CAttackEffect()
{
}

//---------------------------.
// ������.
//---------------------------.
bool CAttackEffect::Init()
{
	// �U���G�t�F�N�g�̎擾.
	m_pAttackSpriteList[AttackNo::None]		= CSpriteResource::GetSprite( "NoneAttackEffect",	&m_AttackStateBaseList[AttackNo::None]		);
	m_pAttackSpriteList[AttackNo::Heavy]	= CSpriteResource::GetSprite( "HeavyAttackEffect",	&m_AttackStateBaseList[AttackNo::Heavy]		);
	m_pAttackSpriteList[AttackNo::Normal]	= CSpriteResource::GetSprite( "NormalAttackEffect",	&m_AttackStateBaseList[AttackNo::Normal]	);
	m_pAttackSpriteList[AttackNo::Weak]		= CSpriteResource::GetSprite( "WeakAttackEffect",	&m_AttackStateBaseList[AttackNo::Weak]		);
	for ( auto& b : m_AttackStateBaseList ) {
		b.Transform.Position.x			= FWND_W / 2.0f;
		b.Transform.Position.y			= FWND_H - 110.0f;
		b.Transform.Rotation.z			= Math::ToRadian( 50.0f );
		b.AnimState.FrameCountSpeed		= 2;
		b.AnimState.IsOneAnimPlay		= true;
		b.AnimState.IsAnimation			= true;
	}

	// SE�̖��O��ۑ�.
	m_AttackSEList[AttackNo::None]		= "NoneAttack";
	m_AttackSEList[AttackNo::Heavy]		= "HeavyAttack";
	m_AttackSEList[AttackNo::Normal]	= "NormalAttack";
	m_AttackSEList[AttackNo::Weak]		= "WeakAttack";
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CAttackEffect::Update( const float& DeltaTime )
{
	// �g�p���Ă��Ȃ��G�t�F�N�g���폜.
	EffectDelete();
}

//---------------------------.
// �`��.
//---------------------------.
void CAttackEffect::Render()
{
	// �U���G�t�F�N�g�̕\��.
	for ( int i = 0; i < AttackNo::Max; ++i ) {
		for ( auto& e : m_AttackStateList[i] ) {
			m_pAttackSpriteList[i]->RenderUI( &e );
		}
	}
}

//---------------------------.
// �G�t�F�N�g�̍Đ�.
//---------------------------.
void CAttackEffect::EffectPlay( const int No )
{
	// SE�̍Đ�.
	CSoundManager::PlaySE( m_AttackSEList[No] );

	// �g�p���Ă��Ȃ��̂�����Ύg�p����.
	for ( auto& e : m_AttackStateList[No] ) {
		if ( e.AnimState.IsAnimation ) continue;

		m_IsNowEffectPlay		= &e.AnimState.IsAnimation;
		e.AnimState.AnimNumber	= 0;
		e.AnimState.UV.x		= 0.0f;
		e.AnimState.UV.y		= 0.0f;
		e.AnimState.IsAnimation = true;
		return;
	}

	// �V�����쐬.
	m_AttackStateList[No].emplace_back( m_AttackStateBaseList[No] );
	m_IsNowEffectPlay = &m_AttackStateList[No].back().AnimState.IsAnimation;
}

//---------------------------.
// �g�p���Ă��Ȃ��G�t�F�N�g�̍폜.
//---------------------------.
void CAttackEffect::EffectDelete()
{
	for ( auto& l : m_AttackStateList ) {
		const int Max = static_cast<int>( l.size() ) - 1;
		for ( int i = Max; i >= 0; --i ) {
			if ( l[i].AnimState.IsAnimation ) break;
			l.pop_back();
		}
	}
}
