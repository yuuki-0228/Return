#include "SpillBullet.h"
#include "..\..\BulletManager.h"
#include "..\..\..\Character\Player\AttackEffect\AttackEffect.h"
#include "..\..\..\..\..\..\Common\SoundManeger\SoundManeger.h"
#include "..\..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\..\..\Utility\Math\Math.h"

CSpillBullet::CSpillBullet()
	: SpownTimeCnt		( INIT_FLOAT )
{
}

CSpillBullet::~CSpillBullet()
{
}

//---------------------------.
// ������.
//---------------------------.
bool CSpillBullet::Init()
{
	// �摜�̎擾.
	m_pSprite									= CSpriteResource::GetSprite( "SpecialBullet",	&m_SpriteState );
	m_pEffectSpriteList[EffectNo::BulletDelete]	= CSpriteResource::GetSprite( "BulletDelete",	&m_EffectStateList[EffectNo::BulletDelete] );
	m_pEffectSpriteList[EffectNo::StateUp]		= CSpriteResource::GetSprite( "StateUp",		&m_EffectStateList[EffectNo::StateUp] );
	m_pEffectSpriteList[EffectNo::StateDown]	= CSpriteResource::GetSprite( "StateDown",		&m_EffectStateList[EffectNo::StateDown] );
	m_pLvEffectSpriteList[EffectNo::Lv1]		= CSpriteResource::GetSprite( "ShieldLv1",		&m_LvEffectStateList[EffectNo::Lv1] );
	m_pLvEffectSpriteList[EffectNo::Lv2]		= CSpriteResource::GetSprite( "ShieldLv2",		&m_LvEffectStateList[EffectNo::Lv2] );
	m_pLvEffectSpriteList[EffectNo::Lv3]		= CSpriteResource::GetSprite( "ShieldLv3",		&m_LvEffectStateList[EffectNo::Lv3] );
	m_pLvEffectSpriteList[EffectNo::Lv4]		= CSpriteResource::GetSprite( "ShieldLv4",		&m_LvEffectStateList[EffectNo::Lv4] );

	// �A�j���[�V�����ԍ���ݒ�ł���悤�ɐݒ�.
	m_SpriteState.AnimState.IsSetAnimNumber = true;

	// �G�t�F�N�g�̐ݒ�.
	for ( auto& e : m_EffectStateList ) {
		e.IsDisp					= false;
		e.AnimState.IsAnimation		= true;
		e.AnimState.IsOneAnimPlay	= true;
		e.AnimState.FrameTime		= CONSTANT.EFFECT_ANIMATION_SPEED;
	}

	// ���x���G�t�F�N�g�̐ݒ�.
	for ( auto& e : m_LvEffectStateList ) {
		e.IsDisp					= false;
		e.AnimState.IsAnimation		= false;
		e.AnimState.IsOneAnimPlay	= true;
		e.AnimState.FrameTime		= CONSTANT.EFFECT_ANIMATION_SPEED;
		e.Color.w					= CONSTANT.SHIELD_ALPHA;
	}

	// ��]�����𔽎��v���ɕύX.
	m_RotationDire = EDirection::Right;

	m_ObjectTag = EObjectTag::SpecialBullet;
	InitCollision();
	return true;
}

//---------------------------.
// �����蔻��֐�.
//---------------------------.
void CSpillBullet::Collision( CActor* pActor )
{
	if ( pActor->GetObjectTag() != EObjectTag::Bullet ) return;

	// �e���擾.
	CBulletBase* pBullet = dynamic_cast<CBulletBase*>(pActor);;

	if ( pBullet->GetIsDamageBullet() == false ) return;
	if ( Coll2D::IsSphere2DToSphere2D(
		m_pCollisions->GetCollision<CSphere2D>(),
		pBullet->GetCollision<CSphere2D>() ) == false ) return;

	// �e�̍폜.
	pBullet->Delete( m_ObjectTag );

	// �T�C�Y��傫�����Ă���.
	if ( m_HitBackCnt == 0 ) return;
	m_BulletState.MoveSpeed			/= SKILL_CONSTANT.DELETE_SPEED_DOWN;
	m_SpriteState.Transform.Scale	+= { SKILL_CONSTANT.GET_ADD_SIZE, SKILL_CONSTANT.GET_ADD_SIZE, SKILL_CONSTANT.GET_ADD_SIZE };
}

//---------------------------.
// �����蔻��̏�����.
//---------------------------.
void CSpillBullet::InitCollision()
{
	m_pCollisions = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollType::Sphere2D );
	m_pCollisions->GetCollision<CSphere2D>()->SetSprite( m_pSprite, &m_SpriteState );
}

//---------------------------.
// �����蔻��̍X�V.
//---------------------------.
void CSpillBullet::UpdateCollision()
{
}

//---------------------------.
// �e�̏�Ԃ̏�����.
//---------------------------.
void CSpillBullet::InitBulletState( const SBulletState& State )
{
	// ��Ԃ̏�����.
	m_BulletLv														= EffectNo::LvEffect_None;
	m_HitBackCnt													= INIT_INT;
	m_IsRotationMove												= false;
	m_IsCollisionStop												= false;
	m_EffectStateList[EffectNo::BulletDelete].IsDisp				= false;
	m_EffectStateList[EffectNo::BulletDelete].AnimState.IsAnimation	= true;
	m_EffectStateList[EffectNo::BulletDelete].Color					= Color4::RGBA( State.DeleteColor );

	// �e�̏���ݒ肷��.
	m_StartBulletState	= State;
	m_BulletState		= State;

	// �e�̌����ڂ�ύX����.
	m_SpriteState.Transform.Scale		= STransform::NORMAL_SCALE_VEC3;
	m_SpriteState.AnimState.AnimNumber	= State.ImageNo;

	// �G�t�F�N�g��S�Ĕ�\���ɂ���.
	for ( auto& e : m_EffectStateList	) e.IsDisp = false;
	for ( auto& e : m_LvEffectStateList ) e.IsDisp = false;
}

//---------------------------.
// �e���폜���邩�m�F.
//---------------------------.
bool CSpillBullet::DeleteCheck( const EObjectTag DeleteTag )
{
	if ( DeleteTag != EObjectTag::Enemy ) return true;

	// �C���N�e���㉽�o���邩���ׂ�.
	const int Num = static_cast<int>( ( m_SpriteState.Transform.Scale.x - SKILL_CONSTANT.DELETE_SPOWN_DEC_SIZE ) / SKILL_CONSTANT.SPOWN_DEC_SIZE * 2.0f );
	if ( Num < SKILL_CONSTANT.DELETE_SPOWN_MIN ) return true;

	// ��o����C���N�e��S�����ɔ�΂�.
	CBulletManager::AllDirectionShot( m_SpriteState.Transform.Position, Num, "NormalInk" );
	return true;
}

//---------------------------.
// �X�e�[�W����]����悤�ɑł��Ԃ��邩�̊m�F.
//---------------------------.
bool CSpillBullet::RotationHitBackCheck( const int AttackNo )
{
	// ��]���������Ƃ̂���e�͑ł��Ԃ��Ȃ�.
	if ( m_HitBackCnt != 0 ) {
		if ( m_BulletState.MoveSpeed > CONSTANT.SPEED_DONW_MAX ) m_BulletState.MoveSpeed /= CONSTANT.SHIELD_ATTACK_SPEED_DOWN;
		return false;
	}

	// �V�[���h�����炷.
	m_BulletState.Shield -= AttackNo::Max - AttackNo;
	if ( m_BulletState.MoveSpeed > CONSTANT.SPEED_DONW_MAX ) m_BulletState.MoveSpeed /= CONSTANT.SHIELD_ATTACK_SPEED_DOWN;
	CSoundManager::PlaySE( "ShieldAttack" );

	// �V�[���h����ꂽ�ꍇ���̑��x�ɖ߂�.
	if ( m_BulletState.Shield > 0 ) return false;
	m_BulletState.MoveSpeed = m_StartBulletState.MoveSpeed;
	CSoundManager::PlaySE( "ShieldBreak" );

	// �o���A�G�t�F�N�g��S�Ĕ�\���ɂ���.
	for ( auto& e : m_LvEffectStateList ) {
		e.IsDisp = false;
	}
	return true;
}

//---------------------------.
// �}�E�X�J�[�\���̕����ɑł��Ԃ�.
//---------------------------.
bool CSpillBullet::AttackHitBackCheck( const int AttackNo )
{
	m_BulletState.MoveSpeed = m_StartBulletState.MoveSpeed;
	return true;
}

//---------------------------.
// �e�̃X�L���̍X�V.
//---------------------------.
void CSpillBullet::SkillUpdate()
{
	if ( m_HitBackCnt == 0 ) return;

	// �C���N�e���o���҂����Ԃ𑝂₷
	SpownTimeCnt += m_DeltaTime;
	if ( SpownTimeCnt <= SKILL_CONSTANT.SPOWN_TIME ) return;
	SpownTimeCnt = 0.0f;

	// �C���N�e����������.
	CBulletManager::DropShot( m_SpriteState.Transform.Position, "NormalInk" );

	// �e�̃T�C�Y������������.
	m_SpriteState.Transform.Scale -= { SKILL_CONSTANT.SPOWN_DEC_SIZE, SKILL_CONSTANT.SPOWN_DEC_SIZE, SKILL_CONSTANT.SPOWN_DEC_SIZE };
	m_BulletState.MoveSpeed *= SKILL_CONSTANT.DELETE_SPEED_DOWN;

	// ����̃T�C�Y��菬�����Ȃ����ꍇ�폜����.
	if ( m_SpriteState.Transform.Scale.x <= SKILL_CONSTANT.DELETE_SIZE ) Delete( EObjectTag::None, true );
}

//---------------------------.
// ���x���G�t�F�N�g�̍X�V.
//---------------------------.
void CSpillBullet::LvEffectUpdate()
{
	if ( m_HitBackCnt != 0 ) return;
	const float Rate = static_cast<float>( m_BulletState.Shield ) / m_StartBulletState.Shield;

	// �ꎞ�I�ɑS�Ẵ��x���G�t�F�N�g���\���ɂ���.
	for ( auto& e : m_LvEffectStateList ) {
		e.IsDisp = false;
	}

	// ���݂̒e�̃��x���𒲂ׂ�.
	const int OldLv = m_BulletLv;
	if (		Rate <= 0.25f ) m_BulletLv = EffectNo::Lv1;
	else if (	Rate <= 0.5f  ) m_BulletLv = EffectNo::Lv2;
	else if (	Rate <= 0.75f ) m_BulletLv = EffectNo::Lv3;
	else						m_BulletLv = EffectNo::Lv4;

	// �Ή��������x���G�t�F�N�g��\������.
	if ( m_BulletLv == EffectNo::LvEffect_None ) return;
	m_LvEffectStateList[m_BulletLv].IsDisp = true;

	// ���x�����オ�����ꍇSE���Đ�����.
	if ( OldLv == m_BulletLv ) return;
	CSoundManager::PlaySE( "ShieldLvDown" );
}
