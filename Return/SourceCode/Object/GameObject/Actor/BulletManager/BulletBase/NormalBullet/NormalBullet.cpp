#include "NormalBullet.h"
#include "..\..\..\Character\Player\AttackEffect\AttackEffect.h"
#include "..\..\..\..\..\..\Common\SoundManeger\SoundManeger.h"
#include "..\..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\..\..\Utility\Math\Math.h"

CNormalBullet::CNormalBullet()
{
}

CNormalBullet::~CNormalBullet()
{
}

//---------------------------.
// 初期化.
//---------------------------.
bool CNormalBullet::Init()
{
	// 画像の取得.
	m_pSprite									= CSpriteResource::GetSprite( "Bullet",			&m_SpriteState );
	m_pEffectSpriteList[EffectNo::BulletDelete]	= CSpriteResource::GetSprite( "BulletDelete",	&m_EffectStateList[EffectNo::BulletDelete] );
	m_pEffectSpriteList[EffectNo::StateUp]		= CSpriteResource::GetSprite( "StateUp",		&m_EffectStateList[EffectNo::StateUp] );
	m_pEffectSpriteList[EffectNo::StateDown]	= CSpriteResource::GetSprite( "StateDown",		&m_EffectStateList[EffectNo::StateDown] );
	m_pLvEffectSpriteList[EffectNo::Lv1]		= CSpriteResource::GetSprite( "BulletLv1",		&m_LvEffectStateList[EffectNo::Lv1] );
	m_pLvEffectSpriteList[EffectNo::Lv2]		= CSpriteResource::GetSprite( "BulletLv2",		&m_LvEffectStateList[EffectNo::Lv2] );
	m_pLvEffectSpriteList[EffectNo::Lv3]		= CSpriteResource::GetSprite( "BulletLv3",		&m_LvEffectStateList[EffectNo::Lv3] );
	m_pLvEffectSpriteList[EffectNo::Lv4]		= CSpriteResource::GetSprite( "BulletLv4",		&m_LvEffectStateList[EffectNo::Lv4] );

	// アニメーション番号を設定できるように設定.
	m_SpriteState.AnimState.IsSetAnimNumber = true;

	// エフェクトの設定.
	for ( auto& e : m_EffectStateList ) {
		e.IsDisp					= false;
		e.AnimState.IsAnimation		= true;
		e.AnimState.IsOneAnimPlay	= true;
		e.AnimState.FrameTime		= CONSTANT.EFFECT_ANIMATION_SPEED;
	}

	// レベルエフェクトの設定.
	for ( auto& e : m_LvEffectStateList ) {
		e.IsDisp					= false;
		e.AnimState.IsAnimation		= true;
		e.AnimState.FrameTime		= CONSTANT.EFFECT_ANIMATION_SPEED;
	}

	m_ObjectTag = EObjectTag::Bullet;
	InitCollision();
	return true;
}

//---------------------------.
// 当たり判定の初期化.
//---------------------------.
void CNormalBullet::InitCollision()
{
	m_pCollisions = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollType::Sphere2D );
	m_pCollisions->GetCollision<CSphere2D>()->SetSprite( m_pSprite, &m_SpriteState );
}

//---------------------------.
// 弾の状態の初期化.
//---------------------------.
void CNormalBullet::InitBulletState( const SBulletState& State )
{
	// 状態の初期化.
	m_BulletLv														= EffectNo::LvEffect_None;
	m_HitBackCnt													= INIT_INT;
	m_IsRotationMove												= false;
	m_IsCollisionStop												= false;
	m_EffectStateList[EffectNo::BulletDelete].IsDisp				= false;
	m_EffectStateList[EffectNo::BulletDelete].AnimState.IsAnimation	= true;
	m_EffectStateList[EffectNo::BulletDelete].Color					= Color4::RGBA( State.DeleteColor );

	// 弾の情報を設定する.
	m_StartBulletState	= State;
	m_BulletState		= State;

	// 弾の見た目を変更する.
	m_SpriteState.AnimState.AnimNumber = State.ImageNo;

	// エフェクトを全て非表示にする.
	for ( auto& e : m_EffectStateList	) e.IsDisp = false;
	for ( auto& e : m_LvEffectStateList ) e.IsDisp = false;
}

//---------------------------.
// レベルエフェクトの更新.
//---------------------------.
void CNormalBullet::LvEffectUpdate()
{
	const float Rate = m_BulletState.Attack / m_StartBulletState.Attack;

	// 一時的に全てのレベルエフェクトを非表示にする.
	for ( auto& e : m_LvEffectStateList ) {
		e.IsDisp = false;
	}

	// 現在の弾のレベルを調べる.
	const int OldLv = m_BulletLv;
	if (		Rate >= CONSTANT.LEVEL_POWER[EffectNo::Lv4] ) m_BulletLv = EffectNo::Lv4;
	else if (	Rate >= CONSTANT.LEVEL_POWER[EffectNo::Lv3] ) m_BulletLv = EffectNo::Lv3;
	else if (	Rate >= CONSTANT.LEVEL_POWER[EffectNo::Lv2] ) m_BulletLv = EffectNo::Lv2;
	else if (	Rate >  CONSTANT.LEVEL_POWER[EffectNo::Lv1] ) m_BulletLv = EffectNo::Lv1;

	// 対応したレベルエフェクトを表示する.
	if ( m_BulletLv == EffectNo::LvEffect_None ) return;
	m_LvEffectStateList[m_BulletLv].IsDisp = true;

	// レベルが上がった場合SEを再生する.
	if ( OldLv == m_BulletLv ) return;
	CSoundManager::PlaySE( "LvUp" );
}