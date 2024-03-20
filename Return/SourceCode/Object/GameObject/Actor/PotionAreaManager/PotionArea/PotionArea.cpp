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
// ������.
//---------------------------.
bool CPotionArea::Init()
{
	// �G�t�F�N�g�̎擾.
	m_pEffectSprite = CSpriteResource::GetSprite( "Potion", &m_EffectStateBase );

	// �G�t�F�N�g�̐ݒ�.
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
// �X�V.
//---------------------------.
void CPotionArea::Update( const float& DeltaTime )
{
	m_DeltaTime = DeltaTime;

	for ( auto& e : m_EffectStateList ) {
		if ( e.IsLock ) continue;
		e.Transform.Position.y -= e.UIAnimState.Speed;
	}

	// �g�p���Ă��Ȃ��G�t�F�N�g�̍폜.
	EffectDelete();
	if ( m_PlayerCoolTime > 0.0f ) {
		m_PlayerCoolTime -= DeltaTime;
		if ( m_PlayerCoolTime <= 0.0f ) m_PlayerCoolTime = 0.0f;
	}

	// �|�[�V�����G���A�����S�ɏI��������.
	if ( EndCheck()			) m_IsEnd = true;
	if ( m_IsDisp == false	) return;

	EffectTimeUpdate();		// �G�t�F�N�g�̕t�^���Ԃ̍X�V.
	PotionEffectUpdate();	// �|�[�V�����G�t�F�N�g�̍X�V.
	PotionAreaEndCheck();	// �|�[�V�����G���A���I���������m�F.

	UpdateCollision();
	CActorCollisionManager::PushObject( this, ECollObjType::BeAHit );
}

//---------------------------.
// �����蔻��I����Ăяo�����X�V.
//---------------------------.
void CPotionArea::LateUpdate( const float& DeltaTime )
{
	// �G�t�F�N�g�̕t�^���Ԃ̃��Z�b�g.
	EffectTimeReset();

	// �����蔻����s������.
	if ( m_IsHit == false ) return;
	m_IsHit = false;

	// �g�p�ł���񐔂�0�ȉ��̎��͍s��Ȃ�.
	if ( m_StartAreaState.UseNum <= 0	) return;

	// �|�[�V�����G���A�̎g�p�ł���񐔂����炷.
	m_AreaState.UseNum--;
	if ( m_AreaState.UseNum > 0 ) return;

	// �|�[�V�����G���A���I��������.
	m_IsDisp = false;
}

//---------------------------.
// �`��.
//---------------------------.
void CPotionArea::Render()
{
	for ( auto& e : m_EffectStateList ) m_pEffectSprite->RenderUI( &e );

	CCollisionRender::PushSphere2D( m_pCollisions->GetCollision<CSphere2D>() );
}

//---------------------------.
// �����蔻��֐�.
//---------------------------.
void CPotionArea::Collision( CActor* pActor )
{
	if ( m_AreaState.EffectTime > 0.0f ) return;

	const EObjectTag Tag = pActor->GetObjectTag();
	if ( Tag != EObjectTag::Player &&
		 Tag != EObjectTag::Enemy ) return;
	if ( Tag == EObjectTag::Player && m_PlayerCoolTime > 0.0f ) return;

	// �G���A���ɂ��邩�̓����蔻��.
	CCharacter* pCharacter = dynamic_cast<CCharacter*>( pActor );
	if ( Coll2D::IsSphere2DToSphere2D(
		m_pCollisions->GetCollision<CSphere2D>(),
		pCharacter->GetCollision<CSphere2D>() ) == false ) return;

	// �|�[�V�������ʂɉ����ăX�e�[�^�X��ύX������.
	switch ( m_AreaState.PotionType ) {
	case PotionNo::Heal:	pCharacter->HealHp(		 m_AreaState.EffectValue ); break;
	case PotionNo::Poison:	pCharacter->DamageHp(	 m_AreaState.EffectValue ); break;
	case PotionNo::Rest:	pCharacter->AddStamina(  m_AreaState.EffectValue ); break;
	default: break;
	}

	// ��������.
	m_IsHit = true;

	// �N�[���^�C����݂���.
	if ( Tag != EObjectTag::Player ) return;
	m_PlayerCoolTime = 1.0f;
}

//---------------------------.
// �|�[�V�����G���A�̍쐬.
//---------------------------.
void CPotionArea::CreateArea( const D3DXPOSITION3& Pos, const float Radius, const SPotionAreaState& State )
{
	// �|�[�V�����G���A�̏���ۑ�.
	m_Transform.Position	= Pos;
	m_StartAreaState		= State;
	m_AreaState				= State;
	m_CollRadius			= Radius;

	// ������.
	Init();

	// ���a�ɂ������ʒu�ɃG�t�F�N�g��\������.
	const float AddAng = Math::RADIAN_MAX / CONSTANT.START_EFFECT_NUM;
	for ( int i = 0; i < CONSTANT.START_EFFECT_NUM; ++i ) {
		// �ǉ�������W�����߂�.
		D3DXPOSITION3 AddPos = INIT_FLOAT3;
		AddPos.x = cosf( AddAng * i ) * m_CollRadius;
		AddPos.y = sinf( AddAng * i ) * m_CollRadius;

		// ���W�����߂�.
		const D3DXPOSITION3& CenterPos	= m_Transform.Position;
		const D3DXPOSITION3& PlayPos	= CenterPos + AddPos;

		// �G�t�F�N�g�̍Đ�.
		EffectPlay( PlayPos, true );
	}
}

//---------------------------.
// �����蔻��̏�����.
//---------------------------.
void CPotionArea::InitCollision()
{
	// �_���[�W����̓����蔻��̍쐬.
	m_pCollisions = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollType::Sphere2D );
	m_pCollisions->GetCollision<CSphere2D>()->SetPosition(	m_Transform.Get2DPosition() );
	m_pCollisions->GetCollision<CSphere2D>()->SetRadius(	m_CollRadius );
}

//---------------------------.
// �����蔻��̍X�V.
//---------------------------.
void CPotionArea::UpdateCollision()
{
}

//---------------------------.
// �|�[�V�����G�t�F�N�g�̍X�V.
//---------------------------.
void CPotionArea::PotionEffectUpdate()
{
	const float Rate = 1.0f - m_AreaState.DeleteTime / m_StartAreaState.DeleteTime;

	// �|�[�V�����G�t�F�N�g�̍Đ��̑҂����Ԃ𑝂₷.
	m_PotionEffectTimeCnt += m_DeltaTime;
	if ( m_PotionEffectTimeCnt < CONSTANT.EFFECT_PLAY_TIME + CONSTANT.EFFECT_PLAY_TIME * Rate ) return;
	m_PotionEffectTimeCnt = 0.0f;

	const int Max = CRandom::GetRand( CONSTANT.EFFECT_PLAY_NUM_MIN, CONSTANT.EFFECT_PLAY_NUM_MAX );
	for ( int i = 0; i < Max; ++i ) {
		// �ʒu�������_���Ō��߂�.
		const float Ang		= CRandom::GetRand( Math::RADIAN_MIN, Math::RADIAN_MAX );
		const float Radius	= CRandom::GetRand( 0.0f, m_CollRadius );

		// ��������̒ǉ����W�����߂�.
		D3DXPOSITION3 AddPos = INIT_FLOAT3;
		AddPos.x = cosf( Ang ) * Radius;
		AddPos.y = sinf( Ang ) * Radius;

		// ���W�����߂�.
		const D3DXPOSITION3& CenterPos	= m_Transform.Position;
		const D3DXPOSITION3& PlayPos	= CenterPos + AddPos;

		// �G�t�F�N�g�̍Đ�.
		EffectPlay( PlayPos );
	}
}

//---------------------------.
// �G�t�F�N�g�̕t�^���Ԃ̍X�V.
//---------------------------.
void CPotionArea::EffectTimeUpdate()
{
	if ( m_AreaState.EffectTime <= 0.0f ) return;

	m_AreaState.EffectTime -= m_DeltaTime;
	if ( m_AreaState.EffectTime <= 0.0f ) m_AreaState.EffectTime = 0.0f;
}

//---------------------------.
// �G�t�F�N�g�̕t�^���Ԃ̃��Z�b�g.
//---------------------------.
void CPotionArea::EffectTimeReset()
{
	if ( m_AreaState.EffectTime > 0.0f ) return;

	m_AreaState.EffectTime = m_StartAreaState.EffectTime;
}

//---------------------------.
// �G�t�F�N�g�̍Đ�.
//---------------------------.
void CPotionArea::EffectPlay( const D3DXPOSITION3& Pos, const bool IsStart )
{
	// �g�p���Ă��Ȃ��̂�����Ύg�p����.
	for ( auto& e : m_EffectStateList ) {
		if ( e.AnimState.IsAnimation ) continue;

		// �G�t�F�N�g�̍Đ�.
		e = m_EffectStateBase;
		e.Transform.Position = Pos;

		// �ړ����֎~������.
		if ( IsStart ) m_EffectStateList.back().IsLock = true;
		// ���x�̐ݒ�.
		else m_EffectStateList.back().UIAnimState.Speed = CRandom::GetRand( CONSTANT.EFFECT_SPEED_MIN, CONSTANT.EFFECT_SPEED_MAX );
		return;
	}

	// �V�����쐬.
	m_EffectStateList.emplace_back( m_EffectStateBase );

	// �G�t�F�N�g�̍Đ�.
	m_EffectStateList.back().Transform.Position = Pos;

	// �ړ����֎~������.
	if ( IsStart ) m_EffectStateList.back().IsLock = true;
	// ���x�̐ݒ�.
	else m_EffectStateList.back().UIAnimState.Speed = CRandom::GetRand( CONSTANT.EFFECT_SPEED_MIN, CONSTANT.EFFECT_SPEED_MAX );
}

//---------------------------.
// �g�p���Ă��Ȃ��G�t�F�N�g�̍폜.
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
// �G�t�F�N�g�����S�ɏI���������m�F.
//---------------------------.
bool CPotionArea::EndCheck()
{
	for ( auto& e : m_EffectStateList ) {
		if ( e.AnimState.IsAnimation ) return false;
	}
	return true;
}

//---------------------------.
// �|�[�V�����G���A���I���������m�F.
//---------------------------.
void CPotionArea::PotionAreaEndCheck()
{
	// �폜�܂ł̎��Ԃ����炵�Ă���.
	m_AreaState.DeleteTime -= m_DeltaTime;
	if ( m_AreaState.DeleteTime > 0.0f ) return;

	// �|�[�V�����G���A���I��������.
	m_IsDisp = false;
}
