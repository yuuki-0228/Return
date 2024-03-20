#include "BigPotionBullet.h"
#include "..\..\BulletManager.h"
#include "..\..\..\PotionAreaManager\PotionAreaManager.h"
#include "..\..\..\Character\Player\AttackEffect\AttackEffect.h"
#include "..\..\..\..\..\..\Common\SoundManeger\SoundManeger.h"
#include "..\..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\..\..\Utility\Math\Math.h"
#include "..\..\..\..\..\..\Utility\Random\Random.h"

CBigPotionBullet::CBigPotionBullet()
	: m_PotionTiem	( INIT_FLOAT )
{
}

CBigPotionBullet::~CBigPotionBullet()
{
}

//---------------------------.
// ������.
//---------------------------.
bool CBigPotionBullet::Init()
{
	// �摜�̎擾.
	m_pSprite									= CSpriteResource::GetSprite( "SpecialBullet",	&m_SpriteState );
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

	// �|�[�V�����̌��ʎ��Ԃ̏�����.
	m_PotionTiem = SKILL_CONSTANT.POTION_TIME_MIN;

	// ��]�����𔽎��v���ɕύX.
	m_RotationDire = EDirection::Right;

	m_ObjectTag = EObjectTag::SpecialBullet;
	InitCollision();
	return true;
}

//---------------------------.
// �����蔻��֐�.
//---------------------------.
void CBigPotionBullet::Collision( CActor* pActor )
{
	if ( pActor->GetObjectTag() != EObjectTag::Bullet ) return;

	// �e���擾.
	CBulletBase* pBullet = dynamic_cast<CBulletBase*>(pActor);;

	if ( Coll2D::IsSphere2DToSphere2D(
		m_pCollisions->GetCollision<CSphere2D>(),
		pBullet->GetCollision<CSphere2D>() ) == false ) return;

	// �Ń|�[�V�����ɕϐg������.
	CBulletManager::BulletChange( pBullet, "PoisonPotion" );

	// �|�[�V�����̌��ʎ��Ԃ𑝂₷.
	if ( m_PotionTiem >= SKILL_CONSTANT.POTION_TIME_MAX ) return;
	m_PotionTiem += SKILL_CONSTANT.DELETE_ADD_TIME;
	if ( m_PotionTiem >= SKILL_CONSTANT.POTION_TIME_MAX ) m_PotionTiem = SKILL_CONSTANT.POTION_TIME_MAX;
}

//---------------------------.
// �����蔻��̏�����.
//---------------------------.
void CBigPotionBullet::InitCollision()
{
	m_pCollisions = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollType::Sphere2D );
	m_pCollisions->GetCollision<CSphere2D>()->SetSprite( m_pSprite, &m_SpriteState );
}

//---------------------------.
// �����蔻��̍X�V.
//---------------------------.
void CBigPotionBullet::UpdateCollision()
{
}

//---------------------------.
// �e�̏�Ԃ̏�����.
//---------------------------.
void CBigPotionBullet::InitBulletState( const SBulletState& State )
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
	m_SpriteState.AnimState.AnimNumber = State.ImageNo;

	// �G�t�F�N�g��S�Ĕ�\���ɂ���.
	for ( auto& e : m_EffectStateList	) e.IsDisp = false;
	for ( auto& e : m_LvEffectStateList ) e.IsDisp = false;
}

//---------------------------.
// �e���폜���邩�m�F.
//---------------------------.
bool CBigPotionBullet::DeleteCheck( const EObjectTag DeleteTag )
{
	// ���a�̎擾.
	const float Radius = m_pCollisions->GetCollision<CSphere2D>()->GetRadius();

	// �|�[�V�����G���A�̍쐬.
	CPotionAreaManager::PushArea(
		m_SpriteState.Transform.Position,
		Radius * CConstant::GetPotionArea().AREA_RADIUS,
		m_BulletState.SkillName,
		m_PotionTiem
	);
	return true;
}

//---------------------------.
// �X�e�[�W����]����悤�ɑł��Ԃ��邩�̊m�F.
//---------------------------.
bool CBigPotionBullet::RotationHitBackCheck( const int AttackNo )
{
	// ��]���������Ƃ̂���e�͑ł��Ԃ��Ȃ�.
	if ( m_HitBackCnt != 0 ) {
		Delete( EObjectTag::None );

		// SE�̍Đ�.
		CSoundManager::PlaySE( "ShieldBreak" );
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
bool CBigPotionBullet::AttackHitBackCheck( const int AttackNo )
{
	m_BulletState.MoveSpeed = m_StartBulletState.MoveSpeed;
	return true;
}

//---------------------------.
// ���x���G�t�F�N�g�̍X�V.
//---------------------------.
void CBigPotionBullet::LvEffectUpdate()
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