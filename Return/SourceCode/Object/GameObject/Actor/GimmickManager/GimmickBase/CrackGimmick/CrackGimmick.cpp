#include "CrackGimmick.h"
#include "..\..\..\BulletManager\BulletManager.h"
#include "..\..\..\..\..\..\Common\SoundManeger\SoundManeger.h"
#include "..\..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\..\..\Utility\Math\Math.h"

namespace {
	constexpr int CRACKED_NO = 1;	// 少し壊れている画像No.
	constexpr int BROKEN_NO	 = 2;	// 壊れている画像No.
}

CCrackGimmick::CCrackGimmick()
{
}

CCrackGimmick::~CCrackGimmick()
{
}

//---------------------------.
// 初期化.
//---------------------------.
bool CCrackGimmick::Init()
{
	// 画像の取得.
	m_pSprite = CSpriteResource::GetSprite( "Barrel", &m_SpriteState );

	// 見た目を変更できるように変更.
	m_SpriteState.AnimState.IsSetAnimNumber = true;

	m_ObjectTag = EObjectTag::Gimmick;
	InitCollision();
	return true;
}

//---------------------------.
// 当たり判定関数.
//---------------------------.
void CCrackGimmick::Collision( CActor* pActor )
{
	if ( m_GimmickState.Hp <= 0 ) return;
	const EObjectTag Tag = pActor->GetObjectTag();
	if ( Tag != EObjectTag::Bullet &&
		 Tag != EObjectTag::ItemBullet &&
		 Tag != EObjectTag::SpecialBullet ) return;

	// 弾を取得.
	CBulletBase* pBullet = dynamic_cast<CBulletBase*>(pActor);

	// ダメージを受けるかの当たり判定.
	if ( Coll2D::IsSphere2DToSphere2D(
		m_pCollisions->GetCollision<CSphere2D>(),
		pBullet->GetCollision<CSphere2D>() ) == false ) return;

	// 弾を削除する.
	pBullet->Delete( m_ObjectTag );

	// 耐久力を減らす.
	if ( Tag == EObjectTag::Bullet ) m_GimmickState.Hp--;
	else							 m_GimmickState.Hp = 0;

	// アイテムを落とす.
	if ( m_GimmickState.Hp > 0 ) return;
	m_GimmickState.Hp = 0;
	CBulletManager::DropShot( m_SpriteState.Transform.Position, m_GimmickState.DropBulletName );
}

//---------------------------.
// 当たり判定の初期化.
//---------------------------.
void CCrackGimmick::InitCollision()
{
	m_pCollisions = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollType::Sphere2D );
	m_pCollisions->GetCollision<CSphere2D>()->SetSprite( m_pSprite, &m_SpriteState );
}

//---------------------------.
// 当たり判定の更新.
//---------------------------.
void CCrackGimmick::UpdateCollision()
{
}

//---------------------------.
// ギミックの更新.
//---------------------------.
void CCrackGimmick::GimmickUpdate()
{
	// 耐久力が0なら壊れた見た目に変更.
	if ( m_GimmickState.Hp <= 0 ) {
		m_SpriteState.AnimState.AnimNumber = BROKEN_NO;
		return;
	}

	// 耐久力が半分以下なら壊れかけの見た目に変更.
	const float Rate = static_cast<float>( m_GimmickState.Hp ) / m_StartGimmickState.Hp;
	if ( Rate > 0.5 ) return;
	m_SpriteState.AnimState.AnimNumber = CRACKED_NO;
}
