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
// �X�V.
//---------------------------.
void CBulletBase::Update( const float& DeltaTime )
{
	if ( m_IsEnd			) return;

	EffectUpdate();		// �G�t�F�N�g�̍X�V.
	EndCheck();			// �e�̓��삪���S�ɏI���������m�F.

	if ( m_IsDisp == false	) return;
	m_DeltaTime = DeltaTime;

	LvEffectUpdate();	// ���x���G�t�F�N�g�̍X�V.
	SkillUpdate();		// �X�L���̍X�V.
	NormalMove();		// �ʏ�̒e�̈ړ�.
	DropMove();			// ��������e�̈ړ�.
	LeftRotationMove();	// �X�e�[�W����]����ړ�( ���v��� ).
	RightRotationMove();// �X�e�[�W����]����ړ�( �����v��� ).
	BulletRotation();	// �e����]������.
	StageOutCheck();	// �X�e�[�W�O�����ׂ�.

	// �g�����X�t�H�[���̍X�V.
	m_Transform = m_SpriteState.Transform;

	UpdateCollision();
	ECollObjType Type = m_ObjectTag == EObjectTag::SpecialBullet ? ECollObjType::Both : ECollObjType::Hit;
	CActorCollisionManager::PushObject( this, Type );
}

//---------------------------.
// �`��.
//---------------------------.
void CBulletBase::Render()
{
	if ( m_IsEnd  ) return;
	if ( m_IsDisp ) {
		// �����e�̕`��.
		if ( m_ObjectTag == EObjectTag::SpecialBullet ) m_pSprite->RenderUI( &m_SpriteState );

		// ���x���G�t�F�N�g�̕`��.
		for ( int i = 0; i < EffectNo::LvEffect_Max; ++i ) {
			m_LvEffectStateList[i].Transform.Position = m_SpriteState.Transform.Position;
			m_pLvEffectSpriteList[i]->RenderUI( &m_LvEffectStateList[i] );
		}

		// ���ʂ̒e�̕`��.
		if ( m_ObjectTag != EObjectTag::SpecialBullet ) m_pSprite->RenderUI( &m_SpriteState );
	}

	// �G�t�F�N�g�̕`��.
	for ( int i = 0; i < EffectNo::Effect_Max; ++i ) {
		m_EffectStateList[i].Transform.Position = m_SpriteState.Transform.Position;
		m_pEffectSpriteList[i]->RenderUI( &m_EffectStateList[i] );
	}

	CCollisionRender::PushSphere2D( m_pCollisions->GetCollision<CSphere2D>() );
}

//---------------------------.
// �e�̃��x���̐ݒ�
//---------------------------.
bool CBulletBase::SetBulletLv( const int Lv )
{
	if ( m_IsEnd				) return false;
	if ( m_IsDisp	== false	) return false;
	if ( m_BulletLv == Lv		) return false;

	// �G�t�F�N�g�̍Đ�.
	EffectNo::EEffectNo EffectNo = EffectNo::StateDown;
	if ( m_BulletLv == EffectNo::LvEffect_None || Lv > m_BulletLv ) EffectNo = EffectNo::StateUp;
	m_EffectStateList[EffectNo].IsDisp					= true;
	m_EffectStateList[EffectNo].AnimState.IsAnimation	= true;
	m_EffectStateList[EffectNo].Transform.Position		= m_SpriteState.Transform.Position;
	m_EffectStateList[EffectNo].AnimState.AnimNumber	= 0;
	m_EffectStateList[EffectNo].AnimState.UV.x			= 0.0f;
	m_EffectStateList[EffectNo].AnimState.UV.y			= 0.0f;

	// ���x���̐ݒ�.
	m_BulletLv = Lv;

	// �U���͂̐ݒ�.
	//	Lv1�̎����� "1�{" �Ȃ��� "0.1�{" ���₷
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

	// SE�̍Đ�.
	CSoundManager::PlaySE( "LvUp" );
	return true;
}

//---------------------------.
// �e�̃��x�����グ��.
//---------------------------.
bool CBulletBase::BulletLvUp()
{
	if ( m_IsEnd						) return false;
	if ( m_IsDisp	== false			) return false;
	if ( m_BulletLv == EffectNo::Lv4	) return false;

	// ���x�����グ��.
	const int NextLv = m_BulletLv == EffectNo::LvEffect_None ? EffectNo::Lv1 : m_BulletLv + 1;
	if ( SetBulletLv( NextLv ) == false ) return false;
	return true;
}

//---------------------------.
// �X�e�[�W����]����悤�ɔ���.
//---------------------------.
void CBulletBase::StageRotationShot( const D3DXPOSITION3& Pos, const SBulletState& State )
{
	// ���ˈʒu�̕ۑ�.
	m_SpriteState.Transform.Position = Pos;

	// �e�̏�Ԃ̏�����.
	InitBulletState( State );

	// ��]����e�̈ړ��ɕύX.
	m_IsRotationMove = true;

	// ���a�̐ݒ�.
	const D3DXPOSITION3& CenterPos	= CConstant::GetStage().STAGE_CENTER3;
	D3DXVECTOR3			 Vector		= Pos - CenterPos;
	m_RotationRadius = D3DXVec3Length( &Vector );

	// ���݂̊p�x�̐ݒ�.
	m_RotationAng = atan2f( Pos.y - CenterPos.y, Pos.x - CenterPos.x );

	// �e��\������.
	m_HitBackCnt	= 1;
	m_IsDisp		= true;
	m_IsEnd			= false;
}

//---------------------------.
// �ʏ�̔���.
//---------------------------.
void CBulletBase::NormalShot( const D3DXPOSITION3& Pos, const SBulletState& State )
{
	// ���ˈʒu�̕ۑ�.
	m_SpriteState.Transform.Position = Pos;

	// �ړ��x�N�g���̍쐬.
	m_MoveVector = CConstant::GetPlayer().POSITION - Pos;
	D3DXVec3Normalize( &m_MoveVector, &m_MoveVector );

	// �e�̏�Ԃ̏�����.
	InitBulletState( State );

	// �e��\������.
	m_HitBackCnt	= 0;
	m_IsDisp		= true;
	m_IsEnd			= false;
}

//---------------------------.
// ��������悤�ɔ���.
//---------------------------.
void CBulletBase::DropShot( const D3DXPOSITION3& Pos, const SBulletState& State )
{
	// ���ˈʒu�̕ۑ�.
	m_SpriteState.Transform.Position = Pos;

	// �ړ��x�N�g���̍쐬.
	m_MoveVector = Math::Y_VECTOR;

	// �e�̏�Ԃ̏�����.
	InitBulletState( State );

	// ��������悤�ɐݒ�.
	m_IsDrop = true;

	// �e��\������.
	m_HitBackCnt	= INIT_INT;
	m_IsDisp		= true;
	m_IsEnd			= false;
}

//---------------------------.
// �p�x�ɂ����������ɒe�𔭎˂���( �オ0�x ).
//---------------------------.
void CBulletBase::DirectionShot( const D3DXPOSITION3& Pos, float Ang, const SBulletState& State )
{
	// ���ˈʒu�̕ۑ�.
	m_SpriteState.Transform.Position = Pos;

	// �ړ��x�N�g���̍쐬.
	m_MoveVector = INIT_FLOAT3;
	m_MoveVector.x =  sinf( Math::ToRadian( Ang ) );
	m_MoveVector.y = -cosf( Math::ToRadian( Ang ) );

	// �e�̏�Ԃ̏�����.
	InitBulletState( State );

	// �e��\������.
	m_HitBackCnt	= 1;
	m_IsDisp		= true;
	m_IsEnd			= false;
}

//---------------------------.
// ��]������悤�ɑł��Ԃ�.
//---------------------------.
void CBulletBase::RotationHitBack( const int AttackNo )
{
	if ( m_IsEnd			) return;
	if ( m_IsDisp == false	) return;

	// �ł��Ԃ��邩�m�F.
	if ( RotationHitBackCheck( AttackNo ) == false ) return;

	// �^�C�~���O�o�[�ɍ폜�A�C�R����\��������.
	const D3DXVECTOR3&	PlayerVector = m_SpriteState.Transform.Position - CConstant::GetPlayer().POSITION;
	const float			Distance	 = D3DXVec3Length( &PlayerVector );
	CTimingBarUI::PushDeleteIcon( AttackNo, Distance );

	// �ア�U���Ȃ�񐔂𑝂₷.
	if ( AttackNo == AttackNo::Weak ) {
		m_WeakAttackCnt++;

		// �e���폜����.
		if ( m_WeakAttackCnt >= CONSTANT.WEAK_ATTACK_DELETE_CNT ) {
			Delete( EObjectTag::None, true );
			return;
		}
	}
	else m_WeakAttackCnt = INIT_INT;

	// ��]����e�̈ړ��ɕύX.
	m_IsRotationMove		= true;
	// �J�n�ʒu�������̏ꍇ�����蔻��𖳂���.
	if ( m_SpriteState.Transform.Position.x <= FWND_W / 2.0f ) {
		m_IsCollisionStop	= true;
	}

	// ���a�̐ݒ�.
	const D3DXPOSITION2&	Pos			= m_SpriteState.Transform.Get2DPosition();
	const D3DXPOSITION2&	CenterPos	= CConstant::GetStage().STAGE_CENTER;
	D3DXVECTOR2				Vector		= Pos - CenterPos;
	m_RotationRadius	= D3DXVec2Length( &Vector );

	// ���݂̊p�x�̐ݒ�.
	m_RotationAng		= atan2f( Pos.y - CenterPos.y, Pos.x - CenterPos.x );

	// �ł��Ԃ����񐔂𑝂₷.
	m_HitBackCnt++;

	// �e����Ԃ�ύX����.
	m_BulletState.MoveSpeed *= CONSTANT.SPEED_MAGNIFICATION[AttackNo];
	m_BulletState.Attack	*= CONSTANT.ATTACK_MAGNIFICATION[AttackNo];

	// �G�t�F�N�g�̍Đ�.
	const EffectNo::EEffectNo EffectNo = AttackNo == AttackNo::Weak ? EffectNo::StateDown : EffectNo::StateUp;
	m_EffectStateList[EffectNo].IsDisp					= true;
	m_EffectStateList[EffectNo].AnimState.IsAnimation	= true;
	m_EffectStateList[EffectNo].Transform.Position		= m_SpriteState.Transform.Position;
	m_EffectStateList[EffectNo].AnimState.AnimNumber	= INIT_INT;
	m_EffectStateList[EffectNo].AnimState.UV.x			= INIT_FLOAT;
	m_EffectStateList[EffectNo].AnimState.UV.y			= INIT_FLOAT;
}

//---------------------------.
// �}�E�X�J�[�\���̕����ɑł��Ԃ�.
//---------------------------.
void CBulletBase::AttackHitBack( const int AttackNo )
{
	if ( m_IsEnd			) return;
	if ( m_IsDisp == false	) return;

	// �ł��Ԃ��邩�m�F.
	if ( AttackHitBackCheck( AttackNo ) == false ) return;

	// �^�C�~���O�o�[�ɍ폜�A�C�R����\��������.
	const D3DXVECTOR3&	PlayerVector = m_SpriteState.Transform.Position - CConstant::GetPlayer().POSITION;
	const float			Distance	 = D3DXVec3Length( &PlayerVector );
	CTimingBarUI::PushDeleteIcon( AttackNo, Distance );

	// �ł��Ԃ������Ƃ������e�̏ꍇ���ł�����.
	if ( m_HitBackCnt == INIT_INT ) {
		Delete( EObjectTag::Player, true );
		return;
	}

	// ���ʂ̒e�̈ړ��ɖ߂�..
	m_IsRotationMove	= false;
	m_IsCollisionStop	= true;

	// �}�E�X�J�[�\���ɔ�Ԃ悤�Ƀx�N�g�����쐬.
	const D3DXPOSITION3& Pos		= m_SpriteState.Transform.Position;
	const D3DXPOSITION2& MousePos	= CInput::GetMousePosition();
	m_MoveVector = D3DXPOSITION3( MousePos.x, MousePos.y, INIT_FLOAT ) - Pos;
	D3DXVec3Normalize( &m_MoveVector, &m_MoveVector );

	// �e����Ԃ�ύX����.
	m_BulletState.MoveSpeed *= CONSTANT.SPEED_MAGNIFICATION[AttackNo] * 2.0f;
	m_BulletState.Attack	*= CONSTANT.ATTACK_MAGNIFICATION[AttackNo] * 2.0f;

	// �G�t�F�N�g�̍Đ�.
	const EffectNo::EEffectNo EffectNo = AttackNo == AttackNo::Weak ? EffectNo::StateDown : EffectNo::StateUp;
	m_EffectStateList[EffectNo].IsDisp					= true;
	m_EffectStateList[EffectNo].AnimState.IsAnimation	= true;
	m_EffectStateList[EffectNo].Transform.Position		= m_SpriteState.Transform.Position;
	m_EffectStateList[EffectNo].AnimState.AnimNumber	= 0;
	m_EffectStateList[EffectNo].AnimState.UV.x			= 0.0f;
	m_EffectStateList[EffectNo].AnimState.UV.y			= 0.0f;
}

//---------------------------.
// �e������.
//---------------------------.
void CBulletBase::Delete( const EObjectTag DeleteTag, const bool CheckSkip, const bool IsEffectPlay )
{
	if ( m_IsEnd			) return;
	if ( m_IsDisp == false	) return;
	if ( DeleteCheck( DeleteTag ) == false && CheckSkip == false ) return;

	// �e���\���ɂ���.
	m_IsDisp = false;

	// �G�t�F�N�g���Đ������I��������.
	if ( IsEffectPlay == false ) {
		m_IsEnd = true;
		return;
	}

	// �G�t�F�N�g���Đ�����.
	m_EffectStateList[EffectNo::BulletDelete].IsDisp					= true;
	m_EffectStateList[EffectNo::BulletDelete].AnimState.IsAnimation		= true;
	m_EffectStateList[EffectNo::BulletDelete].Transform.Position		= m_SpriteState.Transform.Position;
	m_EffectStateList[EffectNo::BulletDelete].AnimState.AnimNumber		= INIT_INT;
	m_EffectStateList[EffectNo::BulletDelete].AnimState.UV.x			= INIT_FLOAT;
	m_EffectStateList[EffectNo::BulletDelete].AnimState.UV.y			= INIT_FLOAT;
}

//---------------------------.
// �e����]������.
//---------------------------.
void CBulletBase::BulletRotation()
{
	// �e�̉�]�����ɑΉ�������].
	if ( m_RotationDire == EDirection::Left ) {
		m_SpriteState.Transform.Rotation.z -= Math::ToRadian( CONSTANT.ROTATION_SPEED * m_BulletState.MoveSpeed );
	}
	else {
		m_SpriteState.Transform.Rotation.z += Math::ToRadian( CONSTANT.ROTATION_SPEED * m_BulletState.MoveSpeed );
	}

	// �p�x��͈͓��Ɏ��߂�.
	m_SpriteState.Transform.RotationClamp();
}

//---------------------------.
// �ʏ�̈ړ�.
//---------------------------.
void CBulletBase::NormalMove()
{
	if ( m_IsDrop			) return;
	if ( m_IsRotationMove	) return;

	m_SpriteState.Transform.Position += m_MoveVector * m_BulletState.MoveSpeed;
}

//---------------------------.
// ��������e�̈ړ�.
//---------------------------.
void CBulletBase::DropMove()
{
	if ( m_IsDrop == false ) return;

	m_SpriteState.Transform.Position += m_MoveVector * m_BulletState.MoveSpeed;

	// �X�e�[�W�̒��S���W���擾.
	const D3DXPOSITION2& CenterPos = CConstant::GetStage().STAGE_CENTER;

	// ���S����̈ʒu���擾.
	const D3DXPOSITION3& Pos = m_SpriteState.Transform.Position;
	const float x = Pos.x - CenterPos.x;
	const float y = Pos.y - CenterPos.y;

	// ���a�̎擾.
	const float Distance = sqrtf( x * x + y * y );

	// �}�b�v�̒[�Ɉړ������ꍇ��]����ړ��ɕύX.
	const float ChangeRadius = CConstant::GetStage().STAGE_RADIUS - CONSTANT.NORMAL_STAGE_RADIUS_DISTANCE;
	if ( Distance <= ChangeRadius ) return;
		
	// ��]����e�̈ړ��ɕύX.
	m_IsDrop			= false;
	m_IsRotationMove	= true;

	// ���a�̐ݒ�.
	m_RotationRadius	= Distance;

	// ���݂̊p�x�̐ݒ�.
	m_RotationAng		= atan2f( Pos.y - CenterPos.y, Pos.x - CenterPos.x );

	// �ł��Ԃ����񐔂𑝂₷.
	m_HitBackCnt++;

	// �����n�_�������̏ꍇ�����蔻��𖳂���.
	if ( Pos.x <= FWND_W / 2.0f ) {
		m_IsCollisionStop = true;
	}
}

//---------------------------.
// �X�e�[�W����]����ړ�( ���v��� ).
//---------------------------.
void CBulletBase::LeftRotationMove()
{
	if ( m_IsRotationMove	== false			) return;
	if ( m_RotationDire		!= EDirection::Left ) return;

	// �ǉ�������W�����߂�.
	D3DXPOSITION3 AddPos = INIT_FLOAT3;
	AddPos.x = cosf( m_RotationAng ) * m_RotationRadius;
	AddPos.y = sinf( m_RotationAng ) * m_RotationRadius;

	// �V�������W�����߂�.
	const D3DXPOSITION3& CenterPos		= CConstant::GetStage().STAGE_CENTER3;
	const D3DXPOSITION3& NewPos			= CenterPos + AddPos;

	// �ړ��x�N�g�����쐬.
	m_MoveVector = NewPos - m_SpriteState.Transform.Position;
	D3DXVec3Normalize( &m_MoveVector, &m_MoveVector );

	// �ړ�( ��] ).
	m_SpriteState.Transform.Position	= NewPos;
		
	// �p�x�𑝂₷.
	m_RotationAng += Math::ToRadian( m_BulletState.MoveSpeed * CONSTANT.ROTATION_MOVE_SPEED_USERATE );

	// ���������.
	if ( m_RotationAng >= Math::RADIAN_MAX ) {
		m_RotationAng		-= Math::RADIAN_MAX;
		m_IsCollisionStop	 = false;
	}

	// ���a���ő�ȉ��̎����������ꂳ���Ă���.
	const float Max = CConstant::GetStage().STAGE_RADIUS - CONSTANT.NORMAL_STAGE_RADIUS_DISTANCE;
	if ( m_RotationRadius < Max ) {
		m_RotationRadius += CONSTANT.KEEP_AWAY_SPEED * m_BulletState.MoveSpeed;
		if ( m_RotationRadius >= Max ) m_RotationRadius = Max;
	}
}

//---------------------------.
// �X�e�[�W����]����ړ�( �����v��� ).
//---------------------------.
void CBulletBase::RightRotationMove()
{
	if ( m_IsRotationMove	== false			 ) return;
	if ( m_RotationDire		!= EDirection::Right ) return;

	// �ǉ�������W�����߂�.
	D3DXPOSITION3 AddPos = INIT_FLOAT3;
	AddPos.x = cosf( m_RotationAng ) * m_RotationRadius;
	AddPos.y = sinf( m_RotationAng ) * m_RotationRadius;

	// �V�������W�����߂�.
	const D3DXPOSITION3& CenterPos		= CConstant::GetStage().STAGE_CENTER3;
	const D3DXPOSITION3& NewPos			= CenterPos + AddPos;

	// �ړ��x�N�g�����쐬.
	m_MoveVector = NewPos - m_SpriteState.Transform.Position;
	D3DXVec3Normalize( &m_MoveVector, &m_MoveVector );

	// �ړ�( ��] ).
	m_SpriteState.Transform.Position	= NewPos;
		
	// �p�x�𑝂₷.
	m_RotationAng -= Math::ToRadian( m_BulletState.MoveSpeed * CONSTANT.ROTATION_MOVE_SPEED_USERATE );

	// ���������.
	if ( m_RotationAng <= Math::RADIAN_MIN ) {
		m_RotationAng		+= Math::RADIAN_MAX;
		m_IsCollisionStop	 = false;
	}

	// ���a���ő�ȉ��̎����������ꂳ���Ă���.
	const float Max = CConstant::GetStage().STAGE_RADIUS - CONSTANT.NORMAL_STAGE_RADIUS_DISTANCE;
	if ( m_RotationRadius < Max ) {
		m_RotationRadius += CONSTANT.KEEP_AWAY_SPEED * m_BulletState.MoveSpeed;
		if ( m_RotationRadius >= Max ) m_RotationRadius = Max;
	}
}

//---------------------------.
// �G�t�F�N�g�̍X�V.
//---------------------------.
void CBulletBase::EffectUpdate()
{
	// �G�t�F�N�g�̍X�V.
	for ( auto& e : m_EffectStateList ) {
		// �G�t�F�N�g�̈ʒu��e�Ɠ����ɂ���.
		e.Transform.Position = m_SpriteState.Transform.Position;

		// �G�t�F�N�g���I�����Ă���ꍇ��\���ɂ���.
		if ( e.AnimState.IsAnimation ) continue;
		e.IsDisp = false;
	}

	// ���x���G�t�F�N�g�̍X�V.
	for ( auto& e : m_LvEffectStateList ) {
		// �G�t�F�N�g�̈ʒu��e�Ɠ����ɂ���.
		e.Transform.Position = m_SpriteState.Transform.Position;
	}
}

//---------------------------.
// �X�e�[�W�O���`�F�b�N.
//---------------------------.
void CBulletBase::StageOutCheck()
{
	// �X�e�[�W�̒��S���W���擾.
	const D3DXPOSITION2& CenterPos = CConstant::GetStage().STAGE_CENTER;

	// ���S����̈ʒu���擾.
	const D3DXPOSITION3& Pos = m_SpriteState.Transform.Position;
	const float x = Pos.x - CenterPos.x;
	const float y = Pos.y - CenterPos.y;

	// ���a�̎擾.
	const float Distance = sqrtf( x * x + y * y );

	// �X�e�[�W�O�̏ꍇ��\���ɂ���.
	if ( Distance > CConstant::GetStage().STAGE_RADIUS ) {
		Delete( EObjectTag::None, true );
	}
}

//---------------------------.
// �e�̓��삪���S�ɏI���������m�F.
//---------------------------.
void CBulletBase::EndCheck()
{
	if ( m_EffectStateList[EffectNo::BulletDelete].AnimState.IsAnimation ) return;

	// �e�̍폜�G�t�F�N�g�̍Đ����I�����Ă��邽�ߓ��삪���S�ɏI�����Ă���.
	m_IsEnd = true;
}
