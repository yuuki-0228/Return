#include "BulletBase.h"
#include "..\..\Character\Player\AttackEffect\AttackEffect.h"
#include "..\..\..\Widget\UIWidget\GameMain\TimingBarUI\TimingBarUI.h"
#include "..\..\..\..\..\Common\SoundManeger\SoundManeger.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\..\Utility\Input\Input.h"
#include "..\..\..\..\..\Utility\Math\Math.h"

CBulletBase::CBulletBase()
	: m_pSprite				( nullptr )
	, m_SpriteState			()
	, m_MoveVector			( INIT_FLOAT3 )
	, m_pEffectSpriteList	( EffectNo::Effect_Max )
	, m_pLvEffectSpriteList	( EffectNo::LvEffect_Max )
	, m_EffectStateList		( EffectNo::Effect_Max )
	, m_LvEffectStateList	( EffectNo::LvEffect_Max )
	, m_BulletState			()
	, m_StartBulletState	()
	, m_RotationDire		( EDirection::Left )
	, m_RotationAng			( INIT_FLOAT )
	, m_RotationRadius		( INIT_FLOAT )
	, m_HitBackCnt			( INIT_INT )
	, m_BulletLv			( EffectNo::LvEffect_None )
	, m_WeakAttackCnt		( INIT_INT )
	, m_IsDrop				( false )
	, m_IsRotationMove		( false )
	, m_IsEnd				( false )
{
}

CBulletBase::~CBulletBase()
{
}

//---------------------------.
// 更新.
//---------------------------.
void CBulletBase::Update( const float& DeltaTime )
{
	if ( m_IsEnd			) return;

	EffectUpdate();		// エフェクトの更新.
	EndCheck();			// 弾の動作が完全に終了したか確認.

	if ( m_IsDisp == false	) return;
	m_DeltaTime = DeltaTime;

	LvEffectUpdate();	// レベルエフェクトの更新.
	SkillUpdate();		// スキルの更新.
	NormalMove();		// 通常の弾の移動.
	DropMove();			// 落下する弾の移動.
	LeftRotationMove();	// ステージを回転する移動( 時計回り ).
	RightRotationMove();// ステージを回転する移動( 反時計回り ).
	BulletRotation();	// 弾を回転させる.
	StageOutCheck();	// ステージ外か調べる.

	// トランスフォームの更新.
	m_Transform = m_SpriteState.Transform;

	UpdateCollision();
	ECollObjType Type = m_ObjectTag == EObjectTag::SpecialBullet ? ECollObjType::Both : ECollObjType::Hit;
	CActorCollisionManager::PushObject( this, Type );
}

//---------------------------.
// 描画.
//---------------------------.
void CBulletBase::Render()
{
	if ( m_IsEnd  ) return;
	if ( m_IsDisp ) {
		// 強い弾の描画.
		if ( m_ObjectTag == EObjectTag::SpecialBullet ) m_pSprite->RenderUI( &m_SpriteState );

		// レベルエフェクトの描画.
		for ( int i = 0; i < EffectNo::LvEffect_Max; ++i ) {
			m_LvEffectStateList[i].Transform.Position = m_SpriteState.Transform.Position;
			m_pLvEffectSpriteList[i]->RenderUI( &m_LvEffectStateList[i] );
		}

		// 普通の弾の描画.
		if ( m_ObjectTag != EObjectTag::SpecialBullet ) m_pSprite->RenderUI( &m_SpriteState );
	}

	// エフェクトの描画.
	for ( int i = 0; i < EffectNo::Effect_Max; ++i ) {
		m_EffectStateList[i].Transform.Position = m_SpriteState.Transform.Position;
		m_pEffectSpriteList[i]->RenderUI( &m_EffectStateList[i] );
	}

	CCollisionRender::PushSphere2D( m_pCollisions->GetCollision<CSphere2D>() );
}

//---------------------------.
// 弾のレベルの設定
//---------------------------.
bool CBulletBase::SetBulletLv( const int Lv )
{
	if ( m_IsEnd				) return false;
	if ( m_IsDisp	== false	) return false;
	if ( m_BulletLv == Lv		) return false;

	// エフェクトの再生.
	EffectNo::EEffectNo EffectNo = EffectNo::StateDown;
	if ( m_BulletLv == EffectNo::LvEffect_None || Lv > m_BulletLv ) EffectNo = EffectNo::StateUp;
	m_EffectStateList[EffectNo].IsDisp					= true;
	m_EffectStateList[EffectNo].AnimState.IsAnimation	= true;
	m_EffectStateList[EffectNo].Transform.Position		= m_SpriteState.Transform.Position;
	m_EffectStateList[EffectNo].AnimState.AnimNumber	= 0;
	m_EffectStateList[EffectNo].AnimState.UV.x			= 0.0f;
	m_EffectStateList[EffectNo].AnimState.UV.y			= 0.0f;

	// レベルの設定.
	m_BulletLv = Lv;

	// 攻撃力の設定.
	//	Lv1の時だけ "1倍" なため "0.1倍" 増やす
	switch ( m_BulletLv ) {
	case EffectNo::Lv1:
		m_BulletState.Attack = m_StartBulletState.Attack * CONSTANT.LEVEL_POWER[EffectNo::Lv1] + 0.1f;
		break;
	case EffectNo::Lv2:
		m_BulletState.Attack = m_StartBulletState.Attack * CONSTANT.LEVEL_POWER[EffectNo::Lv2];
		break;
	case EffectNo::Lv3:
		m_BulletState.Attack = m_StartBulletState.Attack * CONSTANT.LEVEL_POWER[EffectNo::Lv3];
		break;
	case EffectNo::Lv4:
		m_BulletState.Attack = m_StartBulletState.Attack * CONSTANT.LEVEL_POWER[EffectNo::Lv4];
		break;
	default:
		m_BulletState.Attack = m_StartBulletState.Attack;
		break;
	}

	// SEの再生.
	CSoundManager::PlaySE( "LvUp" );
	return true;
}

//---------------------------.
// 弾のレベルを上げる.
//---------------------------.
bool CBulletBase::BulletLvUp()
{
	if ( m_IsEnd						) return false;
	if ( m_IsDisp	== false			) return false;
	if ( m_BulletLv == EffectNo::Lv4	) return false;

	// レベルを上げる.
	const int NextLv = m_BulletLv == EffectNo::LvEffect_None ? EffectNo::Lv1 : m_BulletLv + 1;
	if ( SetBulletLv( NextLv ) == false ) return false;
	return true;
}

//---------------------------.
// ステージを回転するように発射.
//---------------------------.
void CBulletBase::StageRotationShot( const D3DXPOSITION3& Pos, const SBulletState& State )
{
	// 発射位置の保存.
	m_SpriteState.Transform.Position = Pos;

	// 弾の状態の初期化.
	InitBulletState( State );

	// 回転する弾の移動に変更.
	m_IsRotationMove = true;

	// 半径の設定.
	const D3DXPOSITION3& CenterPos	= CConstant::GetStage().STAGE_CENTER3;
	D3DXVECTOR3			 Vector		= Pos - CenterPos;
	m_RotationRadius = D3DXVec3Length( &Vector );

	// 現在の角度の設定.
	m_RotationAng = atan2f( Pos.y - CenterPos.y, Pos.x - CenterPos.x );

	// 弾を表示する.
	m_HitBackCnt	= 1;
	m_IsDisp		= true;
	m_IsEnd			= false;
}

//---------------------------.
// 通常の発射.
//---------------------------.
void CBulletBase::NormalShot( const D3DXPOSITION3& Pos, const SBulletState& State )
{
	// 発射位置の保存.
	m_SpriteState.Transform.Position = Pos;

	// 移動ベクトルの作成.
	m_MoveVector = CConstant::GetPlayer().POSITION - Pos;
	D3DXVec3Normalize( &m_MoveVector, &m_MoveVector );

	// 弾の状態の初期化.
	InitBulletState( State );

	// 弾を表示する.
	m_HitBackCnt	= 0;
	m_IsDisp		= true;
	m_IsEnd			= false;
}

//---------------------------.
// 落下するように発射.
//---------------------------.
void CBulletBase::DropShot( const D3DXPOSITION3& Pos, const SBulletState& State )
{
	// 発射位置の保存.
	m_SpriteState.Transform.Position = Pos;

	// 移動ベクトルの作成.
	m_MoveVector = Math::Y_VECTOR;

	// 弾の状態の初期化.
	InitBulletState( State );

	// 落下するように設定.
	m_IsDrop = true;

	// 弾を表示する.
	m_HitBackCnt	= INIT_INT;
	m_IsDisp		= true;
	m_IsEnd			= false;
}

//---------------------------.
// 角度にあった方向に弾を発射する( 上が0度 ).
//---------------------------.
void CBulletBase::DirectionShot( const D3DXPOSITION3& Pos, float Ang, const SBulletState& State )
{
	// 発射位置の保存.
	m_SpriteState.Transform.Position = Pos;

	// 移動ベクトルの作成.
	m_MoveVector = INIT_FLOAT3;
	m_MoveVector.x =  sinf( Math::ToRadian( Ang ) );
	m_MoveVector.y = -cosf( Math::ToRadian( Ang ) );

	// 弾の状態の初期化.
	InitBulletState( State );

	// 弾を表示する.
	m_HitBackCnt	= 1;
	m_IsDisp		= true;
	m_IsEnd			= false;
}

//---------------------------.
// 回転させるように打ち返す.
//---------------------------.
void CBulletBase::RotationHitBack( const int AttackNo )
{
	if ( m_IsEnd			) return;
	if ( m_IsDisp == false	) return;

	// 打ち返せるか確認.
	if ( RotationHitBackCheck( AttackNo ) == false ) return;

	// タイミングバーに削除アイコンを表示させる.
	const D3DXVECTOR3&	PlayerVector = m_SpriteState.Transform.Position - CConstant::GetPlayer().POSITION;
	const float			Distance	 = D3DXVec3Length( &PlayerVector );
	CTimingBarUI::PushDeleteIcon( AttackNo, Distance );

	// 弱い攻撃なら回数を増やす.
	if ( AttackNo == AttackNo::Weak ) {
		m_WeakAttackCnt++;

		// 弾を削除する.
		if ( m_WeakAttackCnt >= CONSTANT.WEAK_ATTACK_DELETE_CNT ) {
			Delete( EObjectTag::None, true );
			return;
		}
	}
	else m_WeakAttackCnt = INIT_INT;

	// 回転する弾の移動に変更.
	m_IsRotationMove		= true;
	// 開始位置が左側の場合当たり判定を無くす.
	if ( m_SpriteState.Transform.Position.x <= FWND_W / 2.0f ) {
		m_IsCollisionStop	= true;
	}

	// 半径の設定.
	const D3DXPOSITION2&	Pos			= m_SpriteState.Transform.Get2DPosition();
	const D3DXPOSITION2&	CenterPos	= CConstant::GetStage().STAGE_CENTER;
	D3DXVECTOR2				Vector		= Pos - CenterPos;
	m_RotationRadius	= D3DXVec2Length( &Vector );

	// 現在の角度の設定.
	m_RotationAng		= atan2f( Pos.y - CenterPos.y, Pos.x - CenterPos.x );

	// 打ち返した回数を増やす.
	m_HitBackCnt++;

	// 弾を状態を変更する.
	m_BulletState.MoveSpeed *= CONSTANT.SPEED_MAGNIFICATION[AttackNo];
	m_BulletState.Attack	*= CONSTANT.ATTACK_MAGNIFICATION[AttackNo];

	// エフェクトの再生.
	const EffectNo::EEffectNo EffectNo = AttackNo == AttackNo::Weak ? EffectNo::StateDown : EffectNo::StateUp;
	m_EffectStateList[EffectNo].IsDisp					= true;
	m_EffectStateList[EffectNo].AnimState.IsAnimation	= true;
	m_EffectStateList[EffectNo].Transform.Position		= m_SpriteState.Transform.Position;
	m_EffectStateList[EffectNo].AnimState.AnimNumber	= INIT_INT;
	m_EffectStateList[EffectNo].AnimState.UV.x			= INIT_FLOAT;
	m_EffectStateList[EffectNo].AnimState.UV.y			= INIT_FLOAT;
}

//---------------------------.
// マウスカーソルの方向に打ち返す.
//---------------------------.
void CBulletBase::AttackHitBack( const int AttackNo )
{
	if ( m_IsEnd			) return;
	if ( m_IsDisp == false	) return;

	// 打ち返せるか確認.
	if ( AttackHitBackCheck( AttackNo ) == false ) return;

	// タイミングバーに削除アイコンを表示させる.
	const D3DXVECTOR3&	PlayerVector = m_SpriteState.Transform.Position - CConstant::GetPlayer().POSITION;
	const float			Distance	 = D3DXVec3Length( &PlayerVector );
	CTimingBarUI::PushDeleteIcon( AttackNo, Distance );

	// 打ち返したことが無い弾の場合消滅させる.
	if ( m_HitBackCnt == INIT_INT ) {
		Delete( EObjectTag::Player, true );
		return;
	}

	// 普通の弾の移動に戻す..
	m_IsRotationMove	= false;
	m_IsCollisionStop	= true;

	// マウスカーソルに飛ぶようにベクトルを作成.
	const D3DXPOSITION3& Pos		= m_SpriteState.Transform.Position;
	const D3DXPOSITION2& MousePos	= CInput::GetMousePosition();
	m_MoveVector = D3DXPOSITION3( MousePos.x, MousePos.y, INIT_FLOAT ) - Pos;
	D3DXVec3Normalize( &m_MoveVector, &m_MoveVector );

	// 弾を状態を変更する.
	m_BulletState.MoveSpeed *= CONSTANT.SPEED_MAGNIFICATION[AttackNo] * 2.0f;
	m_BulletState.Attack	*= CONSTANT.ATTACK_MAGNIFICATION[AttackNo] * 2.0f;

	// エフェクトの再生.
	const EffectNo::EEffectNo EffectNo = AttackNo == AttackNo::Weak ? EffectNo::StateDown : EffectNo::StateUp;
	m_EffectStateList[EffectNo].IsDisp					= true;
	m_EffectStateList[EffectNo].AnimState.IsAnimation	= true;
	m_EffectStateList[EffectNo].Transform.Position		= m_SpriteState.Transform.Position;
	m_EffectStateList[EffectNo].AnimState.AnimNumber	= 0;
	m_EffectStateList[EffectNo].AnimState.UV.x			= 0.0f;
	m_EffectStateList[EffectNo].AnimState.UV.y			= 0.0f;
}

//---------------------------.
// 弾を消す.
//---------------------------.
void CBulletBase::Delete( const EObjectTag DeleteTag, const bool CheckSkip, const bool IsEffectPlay )
{
	if ( m_IsEnd			) return;
	if ( m_IsDisp == false	) return;
	if ( DeleteCheck( DeleteTag ) == false && CheckSkip == false ) return;

	// 弾を非表示にする.
	m_IsDisp = false;

	// エフェクトを再生せず終了させる.
	if ( IsEffectPlay == false ) {
		m_IsEnd = true;
		return;
	}

	// エフェクトを再生する.
	m_EffectStateList[EffectNo::BulletDelete].IsDisp					= true;
	m_EffectStateList[EffectNo::BulletDelete].AnimState.IsAnimation		= true;
	m_EffectStateList[EffectNo::BulletDelete].Transform.Position		= m_SpriteState.Transform.Position;
	m_EffectStateList[EffectNo::BulletDelete].AnimState.AnimNumber		= INIT_INT;
	m_EffectStateList[EffectNo::BulletDelete].AnimState.UV.x			= INIT_FLOAT;
	m_EffectStateList[EffectNo::BulletDelete].AnimState.UV.y			= INIT_FLOAT;
}

//---------------------------.
// 弾を回転させる.
//---------------------------.
void CBulletBase::BulletRotation()
{
	// 弾の回転方向に対応した回転.
	if ( m_RotationDire == EDirection::Left ) {
		m_SpriteState.Transform.Rotation.z -= Math::ToRadian( CONSTANT.ROTATION_SPEED * m_BulletState.MoveSpeed );
	}
	else {
		m_SpriteState.Transform.Rotation.z += Math::ToRadian( CONSTANT.ROTATION_SPEED * m_BulletState.MoveSpeed );
	}

	// 角度を範囲内に収める.
	m_SpriteState.Transform.RotationClamp();
}

//---------------------------.
// 通常の移動.
//---------------------------.
void CBulletBase::NormalMove()
{
	if ( m_IsDrop			) return;
	if ( m_IsRotationMove	) return;

	m_SpriteState.Transform.Position += m_MoveVector * m_BulletState.MoveSpeed;
}

//---------------------------.
// 落下する弾の移動.
//---------------------------.
void CBulletBase::DropMove()
{
	if ( m_IsDrop == false ) return;

	m_SpriteState.Transform.Position += m_MoveVector * m_BulletState.MoveSpeed;

	// ステージの中心座標を取得.
	const D3DXPOSITION2& CenterPos = CConstant::GetStage().STAGE_CENTER;

	// 中心からの位置を取得.
	const D3DXPOSITION3& Pos = m_SpriteState.Transform.Position;
	const float x = Pos.x - CenterPos.x;
	const float y = Pos.y - CenterPos.y;

	// 半径の取得.
	const float Distance = sqrtf( x * x + y * y );

	// マップの端に移動した場合回転する移動に変更.
	const float ChangeRadius = CConstant::GetStage().STAGE_RADIUS - CONSTANT.NORMAL_STAGE_RADIUS_DISTANCE;
	if ( Distance <= ChangeRadius ) return;
		
	// 回転する弾の移動に変更.
	m_IsDrop			= false;
	m_IsRotationMove	= true;

	// 半径の設定.
	m_RotationRadius	= Distance;

	// 現在の角度の設定.
	m_RotationAng		= atan2f( Pos.y - CenterPos.y, Pos.x - CenterPos.x );

	// 打ち返した回数を増やす.
	m_HitBackCnt++;

	// 落下地点が左側の場合当たり判定を無くす.
	if ( Pos.x <= FWND_W / 2.0f ) {
		m_IsCollisionStop = true;
	}
}

//---------------------------.
// ステージを回転する移動( 時計回り ).
//---------------------------.
void CBulletBase::LeftRotationMove()
{
	if ( m_IsRotationMove	== false			) return;
	if ( m_RotationDire		!= EDirection::Left ) return;

	// 追加する座標を求める.
	D3DXPOSITION3 AddPos = INIT_FLOAT3;
	AddPos.x = cosf( m_RotationAng ) * m_RotationRadius;
	AddPos.y = sinf( m_RotationAng ) * m_RotationRadius;

	// 新しい座標を求める.
	const D3DXPOSITION3& CenterPos		= CConstant::GetStage().STAGE_CENTER3;
	const D3DXPOSITION3& NewPos			= CenterPos + AddPos;

	// 移動ベクトルを作成.
	m_MoveVector = NewPos - m_SpriteState.Transform.Position;
	D3DXVec3Normalize( &m_MoveVector, &m_MoveVector );

	// 移動( 回転 ).
	m_SpriteState.Transform.Position	= NewPos;
		
	// 角度を増やす.
	m_RotationAng += Math::ToRadian( m_BulletState.MoveSpeed * CONSTANT.ROTATION_MOVE_SPEED_USERATE );

	// 一周したか.
	if ( m_RotationAng >= Math::RADIAN_MAX ) {
		m_RotationAng		-= Math::RADIAN_MAX;
		m_IsCollisionStop	 = false;
	}

	// 半径が最大以下の時少しずつ離れさせていく.
	const float Max = CConstant::GetStage().STAGE_RADIUS - CONSTANT.NORMAL_STAGE_RADIUS_DISTANCE;
	if ( m_RotationRadius < Max ) {
		m_RotationRadius += CONSTANT.KEEP_AWAY_SPEED * m_BulletState.MoveSpeed;
		if ( m_RotationRadius >= Max ) m_RotationRadius = Max;
	}
}

//---------------------------.
// ステージを回転する移動( 反時計回り ).
//---------------------------.
void CBulletBase::RightRotationMove()
{
	if ( m_IsRotationMove	== false			 ) return;
	if ( m_RotationDire		!= EDirection::Right ) return;

	// 追加する座標を求める.
	D3DXPOSITION3 AddPos = INIT_FLOAT3;
	AddPos.x = cosf( m_RotationAng ) * m_RotationRadius;
	AddPos.y = sinf( m_RotationAng ) * m_RotationRadius;

	// 新しい座標を求める.
	const D3DXPOSITION3& CenterPos		= CConstant::GetStage().STAGE_CENTER3;
	const D3DXPOSITION3& NewPos			= CenterPos + AddPos;

	// 移動ベクトルを作成.
	m_MoveVector = NewPos - m_SpriteState.Transform.Position;
	D3DXVec3Normalize( &m_MoveVector, &m_MoveVector );

	// 移動( 回転 ).
	m_SpriteState.Transform.Position	= NewPos;
		
	// 角度を増やす.
	m_RotationAng -= Math::ToRadian( m_BulletState.MoveSpeed * CONSTANT.ROTATION_MOVE_SPEED_USERATE );

	// 一周したか.
	if ( m_RotationAng <= Math::RADIAN_MIN ) {
		m_RotationAng		+= Math::RADIAN_MAX;
		m_IsCollisionStop	 = false;
	}

	// 半径が最大以下の時少しずつ離れさせていく.
	const float Max = CConstant::GetStage().STAGE_RADIUS - CONSTANT.NORMAL_STAGE_RADIUS_DISTANCE;
	if ( m_RotationRadius < Max ) {
		m_RotationRadius += CONSTANT.KEEP_AWAY_SPEED * m_BulletState.MoveSpeed;
		if ( m_RotationRadius >= Max ) m_RotationRadius = Max;
	}
}

//---------------------------.
// エフェクトの更新.
//---------------------------.
void CBulletBase::EffectUpdate()
{
	// エフェクトの更新.
	for ( auto& e : m_EffectStateList ) {
		// エフェクトの位置を弾と同じにする.
		e.Transform.Position = m_SpriteState.Transform.Position;

		// エフェクトが終了している場合非表示にする.
		if ( e.AnimState.IsAnimation ) continue;
		e.IsDisp = false;
	}

	// レベルエフェクトの更新.
	for ( auto& e : m_LvEffectStateList ) {
		// エフェクトの位置を弾と同じにする.
		e.Transform.Position = m_SpriteState.Transform.Position;
	}
}

//---------------------------.
// ステージ外かチェック.
//---------------------------.
void CBulletBase::StageOutCheck()
{
	// ステージの中心座標を取得.
	const D3DXPOSITION2& CenterPos = CConstant::GetStage().STAGE_CENTER;

	// 中心からの位置を取得.
	const D3DXPOSITION3& Pos = m_SpriteState.Transform.Position;
	const float x = Pos.x - CenterPos.x;
	const float y = Pos.y - CenterPos.y;

	// 半径の取得.
	const float Distance = sqrtf( x * x + y * y );

	// ステージ外の場合非表示にする.
	if ( Distance > CConstant::GetStage().STAGE_RADIUS ) {
		Delete( EObjectTag::None, true );
	}
}

//---------------------------.
// 弾の動作が完全に終了したか確認.
//---------------------------.
void CBulletBase::EndCheck()
{
	if ( m_EffectStateList[EffectNo::BulletDelete].AnimState.IsAnimation ) return;

	// 弾の削除エフェクトの再生が終了しているため動作が完全に終了している.
	m_IsEnd = true;
}
