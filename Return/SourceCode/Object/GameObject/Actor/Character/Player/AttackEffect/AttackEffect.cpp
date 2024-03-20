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
// 初期化.
//---------------------------.
bool CAttackEffect::Init()
{
	// 攻撃エフェクトの取得.
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

	// SEの名前を保存.
	m_AttackSEList[AttackNo::None]		= "NoneAttack";
	m_AttackSEList[AttackNo::Heavy]		= "HeavyAttack";
	m_AttackSEList[AttackNo::Normal]	= "NormalAttack";
	m_AttackSEList[AttackNo::Weak]		= "WeakAttack";
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CAttackEffect::Update( const float& DeltaTime )
{
	// 使用していないエフェクトを削除.
	EffectDelete();
}

//---------------------------.
// 描画.
//---------------------------.
void CAttackEffect::Render()
{
	// 攻撃エフェクトの表示.
	for ( int i = 0; i < AttackNo::Max; ++i ) {
		for ( auto& e : m_AttackStateList[i] ) {
			m_pAttackSpriteList[i]->RenderUI( &e );
		}
	}
}

//---------------------------.
// エフェクトの再生.
//---------------------------.
void CAttackEffect::EffectPlay( const int No )
{
	// SEの再生.
	CSoundManager::PlaySE( m_AttackSEList[No] );

	// 使用していないのがあれば使用する.
	for ( auto& e : m_AttackStateList[No] ) {
		if ( e.AnimState.IsAnimation ) continue;

		m_IsNowEffectPlay		= &e.AnimState.IsAnimation;
		e.AnimState.AnimNumber	= 0;
		e.AnimState.UV.x		= 0.0f;
		e.AnimState.UV.y		= 0.0f;
		e.AnimState.IsAnimation = true;
		return;
	}

	// 新しく作成.
	m_AttackStateList[No].emplace_back( m_AttackStateBaseList[No] );
	m_IsNowEffectPlay = &m_AttackStateList[No].back().AnimState.IsAnimation;
}

//---------------------------.
// 使用していないエフェクトの削除.
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
