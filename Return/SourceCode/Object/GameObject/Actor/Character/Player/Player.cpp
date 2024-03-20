#include "Player.h"
#include "..\..\BulletManager\BulletManager.h"
#include "..\..\..\..\..\Common\SoundManeger\SoundManeger.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\..\Object\Collision\CollisionRender\CollisionRender.h"
#include "..\..\..\..\..\Utility\Input\Input.h"
#include "..\..\..\..\..\Utility\Math\Math.h"

CPlayer::CPlayer()
	: m_pAttackEffect				( nullptr )
	, m_pAttackCollList				( AttackNo::Max )
	, m_HitAttackNo					( AttackNo::None )
	, m_IsAttackCoolTime			( false )
{
}

CPlayer::~CPlayer()
{
}

//---------------------------.
// 初期化.
//---------------------------.
bool CPlayer::Init()
{
	// 攻撃エフェクトの作成.
	m_pAttackEffect = std::make_unique<CAttackEffect>();
	m_pAttackEffect->Init();

	// 画像の取得.
	m_pSprite = CSpriteResource::GetSprite( "Player", &m_SpriteState );

	// 表示位置の設定.
	m_SpriteState.Transform.Position	= CONSTANT.POSITION;
	m_Transform.Position				= CONSTANT.POSITION;

	// アニメーションするように変更.
	m_SpriteState.AnimState.IsAnimation = true;

	// HPの設定.
	m_HpMax			= CONSTANT.HP_MAX;
	m_Hp			= m_HpMax;
	// スタミナの設定.
	m_StaminaMax	= CONSTANT.STAMINA_MAX;
	m_Stamina		= m_StaminaMax;

	m_ObjectTag		= EObjectTag::Player;
	InitCollision();
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CPlayer::Update( const float& DeltaTime )
{
	m_DeltaTime = DeltaTime;

	m_pAttackEffect->Update( DeltaTime );	// 攻撃エフェクトの更新.
	RecoveryStamina();						// スタミナの回復.
	CoolTimeEnd();							// クールタイムの終了処理.

	if ( GetAsyncKeyState( 'P' ) & 0x0001 ) m_Hp = 0;
	if ( GetAsyncKeyState( 'O' ) & 0x0001 ) m_Hp = CONSTANT.HP_MAX;
	if ( GetAsyncKeyState( 'I' ) & 0x0001 ) CBulletManager::AllDelete();

	UpdateCollision();
	CActorCollisionManager::PushObject( this, ECollObjType::Both, ECollSort::Near );
}

//---------------------------.
// 当たり判定終了後呼び出される更新.
//---------------------------.
void CPlayer::LateUpdate( const float& DeltaTime )
{
	AttackStaminaDown();// 攻撃によるスタミナの減少.
	MissAttack();		// 空振り.
	HitCoolTimeEnd();	// 攻撃を当てた時場合クールタイムを無くす.
}

//---------------------------.
// デバックの更新.
//---------------------------.
void CPlayer::DebugUpdate( const float& DeltaTime )
{
}

//---------------------------.
// 描画.
//---------------------------.
void CPlayer::Render()
{
	m_pSprite->RenderUI( &m_SpriteState );

	// 攻撃エフェクトの表示.
	m_pAttackEffect->Render();

	CCollisionRender::PushSphere2D( m_pCollisions->GetCollision<CSphere2D>() );
	for ( int i = 1; i < AttackNo::Max; ++i ) {
		CCollisionRender::PushSphere2D( m_pAttackCollList[i].get() );
	}
}

//---------------------------.
// 当たり判定関数.
//---------------------------.
void CPlayer::Collision( CActor* pActor )
{
	const EObjectTag Tag = pActor->GetObjectTag();
	if ( Tag != EObjectTag::Bullet &&
		 Tag != EObjectTag::ItemBullet &&
		 Tag != EObjectTag::SpecialBullet	) return;
	if ( pActor->GetIsCollisionStop()		) return;

	// 打ち返しの当たり判定.
	AttackCollision( pActor );

	// ダメージを受けるかの当たり判定.
	if ( pActor->GetIsCollisionStop()		) return;
	DamageCollision( pActor );
}

//---------------------------.
// 当たり判定の初期化.
//---------------------------.
void CPlayer::InitCollision()
{
	// ダメージ判定の当たり判定の作成.
	m_pCollisions = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollType::Sphere2D );
	m_pCollisions->GetCollision<CSphere2D>()->SetSprite( m_pSprite, &m_SpriteState );
	m_pCollisions->GetCollision<CSphere2D>()->SetRadius( CONSTANT.DAMAGE_COLL_SIZE );

	// 強攻撃の当たり判定の作成.
	m_pAttackCollList[AttackNo::Heavy] = std::make_unique<CSphere2D>();
	m_pAttackCollList[AttackNo::Heavy]->SetSprite( m_pSprite, &m_SpriteState );
	m_pAttackCollList[AttackNo::Heavy]->SetRadius( CONSTANT.HEAVY_COLL_SIZE );

	// 強攻撃の当たり判定の作成.
	m_pAttackCollList[AttackNo::Normal] = std::make_unique<CSphere2D>();
	m_pAttackCollList[AttackNo::Normal]->SetSprite( m_pSprite, &m_SpriteState );
	m_pAttackCollList[AttackNo::Normal]->SetRadius( CONSTANT.NORMAL_COLL_SIZE );

	// 弱攻撃の当たり判定の作成.
	m_pAttackCollList[AttackNo::Weak] = std::make_unique<CSphere2D>();
	m_pAttackCollList[AttackNo::Weak]->SetSprite( m_pSprite, &m_SpriteState );
	m_pAttackCollList[AttackNo::Weak]->SetRadius( CONSTANT.WEAK_COLL_SIZE );
}

//---------------------------.
// 当たり判定の更新.
//---------------------------.
void CPlayer::UpdateCollision()
{
}

//---------------------------.
// ダメージを受けるかの当たり判定.
//---------------------------.
void CPlayer::DamageCollision( CActor* pActor )
{
	if ( m_Hp <= 0 ) return;

	// 弾を取得.
	CBulletBase* pBullet = dynamic_cast<CBulletBase*>( pActor );

	// ダメージを受けるかの当たり判定.
	if ( Coll2D::IsSphere2DToSphere2D(
		m_pCollisions->GetCollision<CSphere2D>(),
		pBullet->GetCollision<CSphere2D>() ) == false ) return;

	// Hpを減らす.
	DamageHp( pBullet->GetPlayerDamage() );

	// SEの再生.
	CSoundManager::PlaySE( "PlayerDamage" );

	// 弾を全て削除.
	CBulletManager::AllDelete();
}

//---------------------------.
// 弾の攻撃の当たり判定.
//---------------------------.
void CPlayer::AttackCollision( CActor* pActor )
{
	if ( m_Stamina < CONSTANT.ATTACK_STAMINA ) return;

	// 弾を取得.
	CBulletBase* pBullet = dynamic_cast<CBulletBase*>( pActor );

	// 打ち返すかの当たり判定.
	for ( int i = 1; i < AttackNo::Max; ++i ) {
		if ( m_HitAttackNo != AttackNo::None && m_HitAttackNo != i ) continue;
		if ( Coll2D::IsSphere2DToSphere2D(
			m_pAttackCollList[i].get(),
			pBullet->GetCollision<CSphere2D>() ) == false ) continue;

		// 左クリックを押している時.
		if ( CKeyInput::IsKeyDown( VK_LBUTTON ) ) {
			// ステージを回転するように弾を打ち返す.
			pBullet->RotationHitBack( i ) ;
			m_HitAttackNo		= i;
			m_pAttackEffect->EffectPlay( i );
			return;
		}

		// 右クリックを押している時.
		if ( CKeyInput::IsKeyDown( VK_RBUTTON ) ) {
			// マウスカーソルの方向に飛ぶように弾を打ち返す.
			pBullet->AttackHitBack( i );
			m_HitAttackNo		= i;
			m_IsAttackCoolTime	= true;
			m_pAttackEffect->EffectPlay( i );

			// スタミナを減らす.
			m_Stamina += CONSTANT.RECOVERY_STAMINA - CONSTANT.ATTACK_TYPE_DOWN_STAMINA * i;
			if ( m_Stamina > CONSTANT.STAMINA_MAX ) m_Stamina = CONSTANT.STAMINA_MAX;
			return;
		}
	}
}

//---------------------------.
// 攻撃によるスタミナの減少.
//---------------------------.
void CPlayer::AttackStaminaDown()
{
	if ( m_HitAttackNo						== AttackNo::None	) return;
	if ( m_IsAttackCoolTime					== true				) return;
	if ( CKeyInput::IsKeyDown( VK_LBUTTON )	== false			) return;

	// スタミナを減らす.
	m_Stamina -= CONSTANT.ATTACK_STAMINA - CONSTANT.ATTACK_TYPE_DOWN_STAMINA * ( AttackNo::Max - m_HitAttackNo );
	m_IsAttackCoolTime = true;
}

//---------------------------.
// スタミナの回復.
//---------------------------.
void CPlayer::RecoveryStamina()
{
	if ( m_Stamina >= CONSTANT.STAMINA_MAX ) return;

	m_Stamina += CONSTANT.STAMINA_RECOVERY_SPEED * m_DeltaTime;
	if ( m_Stamina > CONSTANT.STAMINA_MAX ) m_Stamina = CONSTANT.STAMINA_MAX;
}

//---------------------------.
// クールタイムの終了.
//---------------------------.
void CPlayer::CoolTimeEnd()
{
	if ( m_IsAttackCoolTime == false ) return;
		
	// 最後に再生したエフェクトが終了しているか.
	if ( m_pAttackEffect->GetIsEffectPlay() ) return;

	// クールタイムを終了させる.
	m_IsAttackCoolTime = false;
}

//---------------------------.
// 攻撃を当てた時のクールタイムの終了処理.
//---------------------------.
void CPlayer::HitCoolTimeEnd()
{
	if ( m_HitAttackNo == AttackNo::None ) return;

	// 攻撃のクールタイムを無くす.
	m_IsAttackCoolTime	= false;
	m_HitAttackNo		= AttackNo::None;
}

//---------------------------.
// 空振り.
//---------------------------.
void CPlayer::MissAttack()
{
	if ( m_Stamina											<  CONSTANT.ATTACK_STAMINA	) return;
	if ( m_HitAttackNo										!= AttackNo::None			) return;
	if ( m_IsAttackCoolTime									== true						) return;
	if ( CKeyInput::IsORKeyDown( VK_LBUTTON, VK_RBUTTON )	== false					) return;
	
	// 空振りのエフェクトの再生.
	m_IsAttackCoolTime	= true;
	m_HitAttackNo		= AttackNo::None;
	m_pAttackEffect->EffectPlay( AttackNo::None );

	// スタミナを減らす.
	m_Stamina -= CONSTANT.ATTACK_STAMINA;
}
