#include "SharpBullet.h"
#include "..\..\..\Character\Player\AttackEffect\AttackEffect.h"
#include "..\..\..\..\..\..\Common\SoundManeger\SoundManeger.h"
#include "..\..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\..\..\Utility\Math\Math.h"

CSharpBullet::CSharpBullet()
{
}

CSharpBullet::~CSharpBullet()
{
}

//---------------------------.
// 初期化.
//---------------------------.
bool CSharpBullet::Init()
{
	// 画像の取得.
	m_pSprite									= CSpriteResource::GetSprite( "SpecialBullet",	&m_SpriteState );
	m_pEffectSpriteList[EffectNo::BulletDelete]	= CSpriteResource::GetSprite( "BulletDelete",	&m_EffectStateList[EffectNo::BulletDelete] );
	m_pEffectSpriteList[EffectNo::StateUp]		= CSpriteResource::GetSprite( "StateUp",		&m_EffectStateList[EffectNo::StateUp] );
	m_pEffectSpriteList[EffectNo::StateDown]	= CSpriteResource::GetSprite( "StateDown",		&m_EffectStateList[EffectNo::StateDown] );
	m_pLvEffectSpriteList[EffectNo::Lv1]		= CSpriteResource::GetSprite( "ShieldLv1",		&m_LvEffectStateList[EffectNo::Lv1] );
	m_pLvEffectSpriteList[EffectNo::Lv2]		= CSpriteResource::GetSprite( "ShieldLv2",		&m_LvEffectStateList[EffectNo::Lv2] );
	m_pLvEffectSpriteList[EffectNo::Lv3]		= CSpriteResource::GetSprite( "ShieldLv3",		&m_LvEffectStateList[EffectNo::Lv3] );
	m_pLvEffectSpriteList[EffectNo::Lv4]		= CSpriteResource::GetSprite( "ShieldLv4",		&m_LvEffectStateList[EffectNo::Lv4] );

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
		e.AnimState.IsAnimation		= false;
		e.AnimState.IsOneAnimPlay	= true;
		e.AnimState.FrameTime		= CONSTANT.EFFECT_ANIMATION_SPEED;
		e.Color.w					= CONSTANT.SHIELD_ALPHA;
	}

	// 回転方向を反時計回りに変更.
	m_RotationDire = EDirection::Right;

	m_ObjectTag = EObjectTag::SpecialBullet;
	InitCollision();
	return true;
}

//---------------------------.
// 当たり判定関数.
//---------------------------.
void CSharpBullet::Collision( CActor* pActor )
{
	if ( pActor->GetObjectTag() != EObjectTag::Bullet ) return;

	// 弾を取得.
	CBulletBase* pBullet = dynamic_cast<CBulletBase*>(pActor);;

	if ( Coll2D::IsSphere2DToSphere2D(
		m_pCollisions->GetCollision<CSphere2D>(),
		pBullet->GetCollision<CSphere2D>() ) == false ) return;

	// 弾の削除.
	pBullet->Delete( m_ObjectTag );

	// 攻撃力を上げる.
	if ( m_HitBackCnt == 0 ) return;
	m_BulletState.Attack			+= pBullet->GetAttack() / SKILL_CONSTANT.GET_ADD_ATTACK;
	m_BulletState.MoveSpeed			*= SKILL_CONSTANT.DELETE_SPEED_UP;
	m_SpriteState.Transform.Scale	+= { SKILL_CONSTANT.DELERE_ADD_SCALE, SKILL_CONSTANT.DELERE_ADD_SCALE, SKILL_CONSTANT.DELERE_ADD_SCALE };
}

//---------------------------.
// 当たり判定の初期化.
//---------------------------.
void CSharpBullet::InitCollision()
{
	m_pCollisions = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollType::Sphere2D );
	m_pCollisions->GetCollision<CSphere2D>()->SetSprite( m_pSprite, &m_SpriteState );
}

//---------------------------.
// 当たり判定の更新.
//---------------------------.
void CSharpBullet::UpdateCollision()
{
}

//---------------------------.
// 弾の状態の初期化.
//---------------------------.
void CSharpBullet::InitBulletState( const SBulletState& State )
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
	m_SpriteState.Transform.Scale		= STransform::NORMAL_SCALE_VEC3;
	m_SpriteState.AnimState.AnimNumber	= State.ImageNo;

	// エフェクトを全て非表示にする.
	for ( auto& e : m_EffectStateList	) e.IsDisp = false;
	for ( auto& e : m_LvEffectStateList ) e.IsDisp = false;
}

//---------------------------.
// 弾を削除するか確認.
//---------------------------.
bool CSharpBullet::DeleteCheck( const EObjectTag DeleteTag )
{
	// 弾を小さくしていく.
	m_SpriteState.Transform.Scale -= { SKILL_CONSTANT.HIT_DEC_SCALE, SKILL_CONSTANT.HIT_DEC_SCALE, SKILL_CONSTANT.HIT_DEC_SCALE };
	if ( m_BulletState.Attack > SKILL_CONSTANT.DEC_ATTACK_MAX ) m_BulletState.Attack /= SKILL_CONSTANT.HIT_DEC_ATTACK;

	// 特定のサイズ以下になったか.
	if ( m_SpriteState.Transform.Scale.x > SKILL_CONSTANT.DELETE_SIZE ) return false;
	return true;
}

//---------------------------.
// ステージを回転するように打ち返せるかの確認.
//---------------------------.
bool CSharpBullet::RotationHitBackCheck( const int AttackNo )
{
	// 回転させたことのある弾は打ち返せない.
	if ( m_HitBackCnt != 0 ) {
		if ( m_BulletState.MoveSpeed > CONSTANT.SPEED_DONW_MAX ) m_BulletState.MoveSpeed /= CONSTANT.SHIELD_ATTACK_SPEED_DOWN;
		return false;
	}

	// シールドを減らす.
	m_BulletState.Shield -= AttackNo::Max - AttackNo;
	if ( m_BulletState.MoveSpeed > CONSTANT.SPEED_DONW_MAX ) m_BulletState.MoveSpeed /= CONSTANT.SHIELD_ATTACK_SPEED_DOWN;
	CSoundManager::PlaySE( "ShieldAttack" );

	// シールドが壊れた場合元の速度に戻す.
	if ( m_BulletState.Shield > 0 ) return false;
	m_BulletState.MoveSpeed = m_StartBulletState.MoveSpeed;
	CSoundManager::PlaySE( "ShieldBreak" );

	// バリアエフェクトを全て非表示にする.
	for ( auto& e : m_LvEffectStateList ) {
		e.IsDisp = false;
	}
	return true;
}

//---------------------------.
// マウスカーソルの方向に打ち返す.
//---------------------------.
bool CSharpBullet::AttackHitBackCheck( const int AttackNo )
{
	m_BulletState.MoveSpeed = m_StartBulletState.MoveSpeed;
	return true;
}

//---------------------------.
// レベルエフェクトの更新.
//---------------------------.
void CSharpBullet::LvEffectUpdate()
{
	if ( m_HitBackCnt != 0 ) return;
	const float Rate = static_cast<float>( m_BulletState.Shield ) / m_StartBulletState.Shield;

	// 一時的に全てのレベルエフェクトを非表示にする.
	for ( auto& e : m_LvEffectStateList ) {
		e.IsDisp = false;
	}

	// 現在の弾のレベルを調べる.
	const int OldLv = m_BulletLv;
	if (		Rate <= 0.25f ) m_BulletLv = EffectNo::Lv1;
	else if (	Rate <= 0.5f  ) m_BulletLv = EffectNo::Lv2;
	else if (	Rate <= 0.75f ) m_BulletLv = EffectNo::Lv3;
	else						m_BulletLv = EffectNo::Lv4;

	// 対応したレベルエフェクトを表示する.
	if ( m_BulletLv == EffectNo::LvEffect_None ) return;
	m_LvEffectStateList[m_BulletLv].IsDisp = true;

	// レベルが上がった場合SEを再生する.
	if ( OldLv == m_BulletLv ) return;
	CSoundManager::PlaySE( "ShieldLvDown" );
}