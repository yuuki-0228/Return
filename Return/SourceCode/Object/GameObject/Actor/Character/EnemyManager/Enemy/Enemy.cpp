#include "Enemy.h"
#include "..\..\..\BulletManager\BulletManager.h"
#include "..\..\..\..\..\..\Common\SoundManeger\SoundManeger.h"
#include "..\..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\..\..\Scene\SceneManager\SceneManager.h"
#include "..\..\..\..\..\..\Utility\Input\Input.h"
#include "..\..\..\..\..\..\Utility\ImGuiManager\DebugWindow\DebugWindow.h"
#include "..\..\..\..\..\..\Utility\Math\Math.h"

namespace {
	enum enHpBarNo : unsigned char {
		Back,	// 背景.
		Gauge,	// ゲージ.
		Frame,	// フレーム.

		HpBar_Max
	} typedef EHpBarNo;

	// スプライトのファイル名リスト.
	const std::string SpriteNameList[] = {
		"HpBar", "HpBar", "HpBarFrame"
	};
}

CEnemy::CEnemy()
	: m_pHpBarList				( HpBar_Max, nullptr )
	, m_HpBarStateList			( HpBar_Max )
	, m_StartEnemyState			()
	, m_EnemyState				()
	, m_StrongDamageCoolTimeCnt	( INIT_FLOAT )
{
}

CEnemy::~CEnemy()
{
}

//---------------------------.
// 初期化.
//---------------------------.
bool CEnemy::Init()
{
	// 画像の取得.
	m_pSprite		= CSpriteResource::GetSprite( m_EnemyState.TypeName, &m_SpriteState );
	for( int i = 0; i < HpBar_Max; ++i ){
		m_pHpBarList[i] = CSpriteResource::GetSprite( SpriteNameList[i], &m_HpBarStateList[i] );
	}

	// アニメーションするように変更.
	m_SpriteState.AnimState.IsAnimationX = true;

	m_ObjectTag = EObjectTag::Enemy;
	InitCollision();
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CEnemy::Update( const float& DeltaTime )
{
	if ( m_IsDisp == false ) return;

	m_DeltaTime = DeltaTime;

	if ( GetAsyncKeyState( VK_UP	) & 0x8000 ) m_SpriteState.Transform.Position.y--;
	if ( GetAsyncKeyState( VK_DOWN	) & 0x8000 ) m_SpriteState.Transform.Position.y++;
	if ( GetAsyncKeyState( VK_LEFT	) & 0x8000 ) m_SpriteState.Transform.Position.x--;
	if ( GetAsyncKeyState( VK_RIGHT ) & 0x8000 ) m_SpriteState.Transform.Position.x++;
	if ( GetAsyncKeyState( 'Z'		) & 0x0001 ) CBulletManager::Shot( m_SpriteState.Transform.Position, "Normal" );
	if ( GetAsyncKeyState( 'X'		) & 0x0001 ) CBulletManager::Shot( m_SpriteState.Transform.Position, "PotionRest" );
	if ( GetAsyncKeyState( 'C'		) & 0x0001 ) CBulletManager::Shot( m_SpriteState.Transform.Position, "SpecialSpill" );
	if ( GetAsyncKeyState( 'V'		) & 0x0001 ) CBulletManager::Shot( m_SpriteState.Transform.Position, "SpecialSharp" );
	if ( GetAsyncKeyState( 'B'		) & 0x0001 ) CBulletManager::Shot( m_SpriteState.Transform.Position, "SpecialPowerUp" );
	if ( GetAsyncKeyState( 'N'		) & 0x0001 ) CBulletManager::Shot( m_SpriteState.Transform.Position, "SpecialBigPotion" );

	Action();			// 行動処理.
	CoolTimeUpdate();	// クールタイムの更新処理.

	for ( auto& s : m_HpBarStateList ) {
		s.Transform.Position.x = m_SpriteState.Transform.Position.x - m_pSprite->GetSpriteState().Disp.w / 2.0f;
		s.Transform.Position.y = m_SpriteState.Transform.Position.y - m_pSprite->GetSpriteState().Disp.h / 2.0f - 10.0f;
	}
	m_HpBarStateList[Gauge].Color				= Color4::Red;
	m_HpBarStateList[Gauge].Transform.Scale.x	= static_cast<float>( m_Hp ) / m_HpMax;

	// トランスフォームの更新.
	m_Transform = m_SpriteState.Transform;

	UpdateCollision();
	CActorCollisionManager::PushObject( this, ECollObjType::Both );
}

//---------------------------.
// 描画.
//---------------------------.
void CEnemy::Render()
{
	if ( m_IsDisp == false ) return;

	m_pSprite->RenderUI( &m_SpriteState );
	for ( int i = 0; i < HpBar_Max; ++i ) {
		m_pHpBarList[i]->RenderUI( &m_HpBarStateList[i] );
	}

	CCollisionRender::PushSphere2D( m_pCollisions->GetCollision<CSphere2D>() );
}

//---------------------------.
// 当たり判定関数.
//---------------------------.
void CEnemy::Collision( CActor* pActor )
{
	if ( m_IsDisp == false ) return;

	const EObjectTag Tag = pActor->GetObjectTag();
	if ( Tag != EObjectTag::Bullet &&
		 Tag != EObjectTag::ItemBullet &&
		 Tag != EObjectTag::SpecialBullet ) return;

	// ダメージを受けるかの当たり判定.
	if ( Tag == EObjectTag::SpecialBullet && m_StrongDamageCoolTimeCnt != 0 ) return;
	DamageCollision( pActor );
}

//---------------------------.
// スポーンさせる.
//---------------------------.
void CEnemy::Spown( const D3DXPOSITION3& Pos, const SEnemyState State )
{
	// 敵の情報を保存.
	m_StartEnemyState	= State;
	m_EnemyState		= State;

	// HPの初期化.
	m_HpMax				= 100;
	m_Hp				= m_HpMax;

	// 初期化.
	Init();

	// 表示位置の設定.
	m_SpriteState.Transform.Position	= Pos;
	m_IsDisp							= true;
}

//---------------------------.
// 当たり判定の初期化.
//---------------------------.
void CEnemy::InitCollision()
{
	m_pCollisions = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollType::Sphere2D );
	m_pCollisions->GetCollision<CSphere2D>()->SetSprite( m_pSprite, &m_SpriteState );
}

//---------------------------.
// 当たり判定の更新.
//---------------------------.
void CEnemy::UpdateCollision()
{
}

//---------------------------.
// ダメージを受けるかの当たり判定.
//---------------------------.
void CEnemy::DamageCollision( CActor* pActor )
{
	// 弾を取得.
	CBulletBase* pBullet = dynamic_cast<CBulletBase*>( pActor );

	// ダメージを受けるかの当たり判定.
	if ( pBullet->GetIsDamageBullet() == false ) return;
	if ( Coll2D::IsSphere2DToSphere2D(
		m_pCollisions->GetCollision<CSphere2D>(),
		pBullet->GetCollision<CSphere2D>() ) == false ) return;

	// ダメージを受ける.
	DamageHp( pBullet->GetAttack() );

	// SEの再生.
	CSoundManager::PlaySE( "Damage" );

	// 弾の削除.
	pBullet->Delete( m_ObjectTag );

	// 強い弾の場合クールタイムを儲ける.
	if ( pBullet->GetObjectTag() == EObjectTag::Bullet ) return;
	m_StrongDamageCoolTimeCnt = CONSTANT.COOL_TIME;
}

//---------------------------.
// クールタイムの更新.
//---------------------------.
void CEnemy::CoolTimeUpdate()
{
	m_StrongDamageCoolTimeCnt -= m_DeltaTime;

	// クールタイムを終了させる.
	if ( m_StrongDamageCoolTimeCnt > 0.0f ) return;
	m_StrongDamageCoolTimeCnt = 0.0f;
}

//---------------------------.
// 敵の行動.
//---------------------------.
void CEnemy::Action()
{
	SEnemyActionState* pState = m_EnemyState.GetNowAction();

	if (		pState->ActionName == "Attack"	) Attack();
	else if (	pState->ActionName == "Wait"	) Wait();
	else if (	pState->ActionName == "Move"	) Move();
	else if (	pState->ActionName == "Loop"	) ActionLoop();

	// 現在の行動を元に戻す.
	if ( m_EnemyState.NowActionNo < m_EnemyState.ActionSize ) return;
	m_EnemyState.NowActionNo = 0;
}

//---------------------------.
// 攻撃.
//---------------------------.
void CEnemy::Attack()
{
	if ( AttackCheck() == false ) return;

	SEnemyActionState* pState = m_EnemyState.GetNowAction();

	// 攻撃した回数を増やす.
	pState->AttackNum++;

	// 弾を発射する.
	CBulletManager::Shot( m_SpriteState.Transform.Position, pState->BulletName );

	// 次の行動に変更.
	if ( pState->AttackNum < pState->ShotNum ) return;
	pState->AttackNum = 0;
	m_EnemyState.NowActionNo++;
}

//---------------------------.
// 攻撃を行うか確認.
//---------------------------.
bool CEnemy::AttackCheck()
{
	SEnemyActionState* pState = m_EnemyState.GetNowAction();

	// 攻撃の待ち時間を増やす.
	pState->AttackTimeCnt += m_DeltaTime;

	// 攻撃を行う時間を取得.
	const float AttackTime = pState->Interval + pState->BulletAddTime * CBulletManager::GetBulletNum();

	// 攻撃を行うか調べる.
	if ( pState->AttackTimeCnt < AttackTime ) return false;

	// 攻撃の待ち時間を初期化する.
	pState->AttackTimeCnt = INIT_FLOAT;
	return true;
}

//---------------------------.
// 待機.
//---------------------------.
void CEnemy::Wait()
{
	SEnemyActionState* pState = m_EnemyState.GetNowAction();

	// 待機時間を増やす.
	pState->WaitTimeCnt += m_DeltaTime;

	// 終了時間を取得.
	const float EndTime = pState->WaitTime + pState->BulletAddTime * CBulletManager::GetBulletNum();

	// 待機を終了するか.
	if ( pState->WaitTimeCnt < EndTime ) return;

	// 次の行動に変更.
	pState->WaitTimeCnt = 0;
	m_EnemyState.NowActionNo++;
}

//---------------------------.
// 移動.
//---------------------------.
void CEnemy::Move()
{
/*
	{
	  "Type": "Move",
	  "Speed": 1.0,
	  "MoveType": "Step",
	  "IsActionnStop": true
	},
*/
}

//---------------------------.
// 行動のループ.
//---------------------------.
void CEnemy::ActionLoop()
{
	SEnemyActionState* pState = m_EnemyState.GetNowAction();

	// 指定したアクションに変更.
	if ( pState->LoopEndNum <= 0 ||
		 pState->NowLoopNum < pState->LoopEndNum )
	{
		m_EnemyState.NowActionNo = pState->GoActionNo;

		// ループ回数を追加.
		if ( pState->LoopEndNum <= 0 ) return;
		pState->NowLoopNum++;
		return;
	}

	// 次の行動に変更.
	pState->NowLoopNum = 0;
	m_EnemyState.NowActionNo++;
}
