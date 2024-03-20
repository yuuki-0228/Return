#include "PotionArea.h"
#include "..\..\Character\Character.h"
#include "..\..\..\..\..\Common\SoundManeger\SoundManeger.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\..\Utility\Math\Math.h"
#include "..\..\..\..\..\Utility\Random\Random.h"

CPotionArea::CPotionArea()
	: m_pEffectSprite		( nullptr )
	, m_EffectStateBase		()
	, m_EffectStateList		()
	, m_StartAreaState		()
	, m_AreaState			()
	, m_pRenderList			()
	, m_CollRadius			( INIT_FLOAT )
	, m_PotionEffectTimeCnt	( INIT_FLOAT )
	, m_IsEnd				( false )
	, m_IsHit				( false )
{
}

CPotionArea::~CPotionArea()
{
}

//---------------------------.
// 初期化.
//---------------------------.
bool CPotionArea::Init()
{
	// エフェクトの取得.
	m_pEffectSprite = CSpriteResource::GetSprite( "Potion", &m_EffectStateBase );

	// エフェクトの設定.
	m_EffectStateBase.IsDisp					= true;
	m_EffectStateBase.Color						= m_AreaState.EffectColor;
	m_EffectStateBase.AnimState.IsAnimation		= true;
	m_EffectStateBase.AnimState.IsOneAnimPlay	= true;
	m_EffectStateBase.AnimState.FrameTime		= CONSTANT.EFFECT_ANIMATION_SPPED;

	m_ObjectTag = EObjectTag::PotionArea;

	InitCollision();
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CPotionArea::Update( const float& DeltaTime )
{
	m_DeltaTime = DeltaTime;

	for ( auto& e : m_EffectStateList ) {
		if ( e.IsLock ) continue;
		e.Transform.Position.y -= e.UIAnimState.Speed;
	}

	// 使用していないエフェクトの削除.
	EffectDelete();
	if ( m_PlayerCoolTime > 0.0f ) {
		m_PlayerCoolTime -= DeltaTime;
		if ( m_PlayerCoolTime <= 0.0f ) m_PlayerCoolTime = 0.0f;
	}

	// ポーションエリアが完全に終了したか.
	if ( EndCheck()			) m_IsEnd = true;
	if ( m_IsDisp == false	) return;

	EffectTimeUpdate();		// エフェクトの付与時間の更新.
	PotionEffectUpdate();	// ポーションエフェクトの更新.
	PotionAreaEndCheck();	// ポーションエリアが終了したか確認.

	UpdateCollision();
	CActorCollisionManager::PushObject( this, ECollObjType::BeAHit );
}

//---------------------------.
// 当たり判定終了後呼び出される更新.
//---------------------------.
void CPotionArea::LateUpdate( const float& DeltaTime )
{
	// エフェクトの付与時間のリセット.
	EffectTimeReset();

	// 当たり判定を行ったか.
	if ( m_IsHit == false ) return;
	m_IsHit = false;

	// 使用できる回数が0以下の時は行わない.
	if ( m_StartAreaState.UseNum <= 0	) return;

	// ポーションエリアの使用できる回数を減らす.
	m_AreaState.UseNum--;
	if ( m_AreaState.UseNum > 0 ) return;

	// ポーションエリアが終了させる.
	m_IsDisp = false;
}

//---------------------------.
// 描画.
//---------------------------.
void CPotionArea::Render()
{
	for ( auto& e : m_EffectStateList ) m_pEffectSprite->RenderUI( &e );

	CCollisionRender::PushSphere2D( m_pCollisions->GetCollision<CSphere2D>() );
}

//---------------------------.
// 当たり判定関数.
//---------------------------.
void CPotionArea::Collision( CActor* pActor )
{
	if ( m_AreaState.EffectTime > 0.0f ) return;

	const EObjectTag Tag = pActor->GetObjectTag();
	if ( Tag != EObjectTag::Player &&
		 Tag != EObjectTag::Enemy ) return;
	if ( Tag == EObjectTag::Player && m_PlayerCoolTime > 0.0f ) return;

	// エリア内にいるかの当たり判定.
	CCharacter* pCharacter = dynamic_cast<CCharacter*>( pActor );
	if ( Coll2D::IsSphere2DToSphere2D(
		m_pCollisions->GetCollision<CSphere2D>(),
		pCharacter->GetCollision<CSphere2D>() ) == false ) return;

	// ポーション効果に応じてステータスを変更させる.
	switch ( m_AreaState.PotionType ) {
	case PotionNo::Heal:	pCharacter->HealHp(		 m_AreaState.EffectValue ); break;
	case PotionNo::Poison:	pCharacter->DamageHp(	 m_AreaState.EffectValue ); break;
	case PotionNo::Rest:	pCharacter->AddStamina(  m_AreaState.EffectValue ); break;
	default: break;
	}

	// 当たった.
	m_IsHit = true;

	// クールタイムを設ける.
	if ( Tag != EObjectTag::Player ) return;
	m_PlayerCoolTime = 1.0f;
}

//---------------------------.
// ポーションエリアの作成.
//---------------------------.
void CPotionArea::CreateArea( const D3DXPOSITION3& Pos, const float Radius, const SPotionAreaState& State )
{
	// ポーションエリアの情報を保存.
	m_Transform.Position	= Pos;
	m_StartAreaState		= State;
	m_AreaState				= State;
	m_CollRadius			= Radius;

	// 初期化.
	Init();

	// 半径にあった位置にエフェクトを表示する.
	const float AddAng = Math::RADIAN_MAX / CONSTANT.START_EFFECT_NUM;
	for ( int i = 0; i < CONSTANT.START_EFFECT_NUM; ++i ) {
		// 追加する座標を求める.
		D3DXPOSITION3 AddPos = INIT_FLOAT3;
		AddPos.x = cosf( AddAng * i ) * m_CollRadius;
		AddPos.y = sinf( AddAng * i ) * m_CollRadius;

		// 座標を求める.
		const D3DXPOSITION3& CenterPos	= m_Transform.Position;
		const D3DXPOSITION3& PlayPos	= CenterPos + AddPos;

		// エフェクトの再生.
		EffectPlay( PlayPos, true );
	}
}

//---------------------------.
// 当たり判定の初期化.
//---------------------------.
void CPotionArea::InitCollision()
{
	// ダメージ判定の当たり判定の作成.
	m_pCollisions = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollType::Sphere2D );
	m_pCollisions->GetCollision<CSphere2D>()->SetPosition(	m_Transform.Get2DPosition() );
	m_pCollisions->GetCollision<CSphere2D>()->SetRadius(	m_CollRadius );
}

//---------------------------.
// 当たり判定の更新.
//---------------------------.
void CPotionArea::UpdateCollision()
{
}

//---------------------------.
// ポーションエフェクトの更新.
//---------------------------.
void CPotionArea::PotionEffectUpdate()
{
	const float Rate = 1.0f - m_AreaState.DeleteTime / m_StartAreaState.DeleteTime;

	// ポーションエフェクトの再生の待ち時間を増やす.
	m_PotionEffectTimeCnt += m_DeltaTime;
	if ( m_PotionEffectTimeCnt < CONSTANT.EFFECT_PLAY_TIME + CONSTANT.EFFECT_PLAY_TIME * Rate ) return;
	m_PotionEffectTimeCnt = 0.0f;

	const int Max = CRandom::GetRand( CONSTANT.EFFECT_PLAY_NUM_MIN, CONSTANT.EFFECT_PLAY_NUM_MAX );
	for ( int i = 0; i < Max; ++i ) {
		// 位置をランダムで決める.
		const float Ang		= CRandom::GetRand( Math::RADIAN_MIN, Math::RADIAN_MAX );
		const float Radius	= CRandom::GetRand( 0.0f, m_CollRadius );

		// 中央からの追加座標を求める.
		D3DXPOSITION3 AddPos = INIT_FLOAT3;
		AddPos.x = cosf( Ang ) * Radius;
		AddPos.y = sinf( Ang ) * Radius;

		// 座標を求める.
		const D3DXPOSITION3& CenterPos	= m_Transform.Position;
		const D3DXPOSITION3& PlayPos	= CenterPos + AddPos;

		// エフェクトの再生.
		EffectPlay( PlayPos );
	}
}

//---------------------------.
// エフェクトの付与時間の更新.
//---------------------------.
void CPotionArea::EffectTimeUpdate()
{
	if ( m_AreaState.EffectTime <= 0.0f ) return;

	m_AreaState.EffectTime -= m_DeltaTime;
	if ( m_AreaState.EffectTime <= 0.0f ) m_AreaState.EffectTime = 0.0f;
}

//---------------------------.
// エフェクトの付与時間のリセット.
//---------------------------.
void CPotionArea::EffectTimeReset()
{
	if ( m_AreaState.EffectTime > 0.0f ) return;

	m_AreaState.EffectTime = m_StartAreaState.EffectTime;
}

//---------------------------.
// エフェクトの再生.
//---------------------------.
void CPotionArea::EffectPlay( const D3DXPOSITION3& Pos, const bool IsStart )
{
	// 使用していないのがあれば使用する.
	for ( auto& e : m_EffectStateList ) {
		if ( e.AnimState.IsAnimation ) continue;

		// エフェクトの再生.
		e = m_EffectStateBase;
		e.Transform.Position = Pos;

		// 移動を禁止させる.
		if ( IsStart ) m_EffectStateList.back().IsLock = true;
		// 速度の設定.
		else m_EffectStateList.back().UIAnimState.Speed = CRandom::GetRand( CONSTANT.EFFECT_SPEED_MIN, CONSTANT.EFFECT_SPEED_MAX );
		return;
	}

	// 新しく作成.
	m_EffectStateList.emplace_back( m_EffectStateBase );

	// エフェクトの再生.
	m_EffectStateList.back().Transform.Position = Pos;

	// 移動を禁止させる.
	if ( IsStart ) m_EffectStateList.back().IsLock = true;
	// 速度の設定.
	else m_EffectStateList.back().UIAnimState.Speed = CRandom::GetRand( CONSTANT.EFFECT_SPEED_MIN, CONSTANT.EFFECT_SPEED_MAX );
}

//---------------------------.
// 使用していないエフェクトの削除.
//---------------------------.
void CPotionArea::EffectDelete()
{
	const int EffectMax = static_cast<int>( m_EffectStateList.size() ) - 1;
	for ( int i = EffectMax; i >= 0; --i ) {
		if ( m_EffectStateList[i].AnimState.IsAnimation ) break;
		m_EffectStateList.pop_back();
	}
}

//---------------------------.
// エフェクトが完全に終了したか確認.
//---------------------------.
bool CPotionArea::EndCheck()
{
	for ( auto& e : m_EffectStateList ) {
		if ( e.AnimState.IsAnimation ) return false;
	}
	return true;
}

//---------------------------.
// ポーションエリアが終了したか確認.
//---------------------------.
void CPotionArea::PotionAreaEndCheck()
{
	// 削除までの時間を減らしていく.
	m_AreaState.DeleteTime -= m_DeltaTime;
	if ( m_AreaState.DeleteTime > 0.0f ) return;

	// ポーションエリアが終了させる.
	m_IsDisp = false;
}
