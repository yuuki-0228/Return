#include "PotionBullet.h"
#include "..\..\..\PotionAreaManager\PotionAreaManager.h"
#include "..\..\..\Character\Player\AttackEffect\AttackEffect.h"
#include "..\..\..\..\..\..\Common\SoundManeger\SoundManeger.h"
#include "..\..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\..\..\Utility\Math\Math.h"

CPotionBullet::CPotionBullet()
	: CPotionBullet	( false )
{
}

CPotionBullet::CPotionBullet( const bool IsBadPotion )
	: m_DamageLv	( EffectNo::Lv1 )
	, m_IsBadPotion	( IsBadPotion )
{
}

CPotionBullet::~CPotionBullet()
{
}

//---------------------------.
// ������.
//---------------------------.
bool CPotionBullet::Init()
{
	// �摜�̎擾.
	m_pSprite									= CSpriteResource::GetSprite( "PotionBullet",	&m_SpriteState );
	m_pEffectSpriteList[EffectNo::BulletDelete]	= CSpriteResource::GetSprite( "BulletDelete",	&m_EffectStateList[EffectNo::BulletDelete] );
	m_pEffectSpriteList[EffectNo::StateUp]		= CSpriteResource::GetSprite( "StateUp",		&m_EffectStateList[EffectNo::StateUp] );
	m_pEffectSpriteList[EffectNo::StateDown]	= CSpriteResource::GetSprite( "StateDown",		&m_EffectStateList[EffectNo::StateDown] );
	m_pLvEffectSpriteList[EffectNo::Lv1]		= CSpriteResource::GetSprite( "BulletLv1",		&m_LvEffectStateList[EffectNo::Lv1] );
	m_pLvEffectSpriteList[EffectNo::Lv2]		= CSpriteResource::GetSprite( "BulletLv2",		&m_LvEffectStateList[EffectNo::Lv2] );
	m_pLvEffectSpriteList[EffectNo::Lv3]		= CSpriteResource::GetSprite( "BulletLv3",		&m_LvEffectStateList[EffectNo::Lv3] );
	m_pLvEffectSpriteList[EffectNo::Lv4]		= CSpriteResource::GetSprite( "BulletLv4",		&m_LvEffectStateList[EffectNo::Lv4] );

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
		e.AnimState.IsAnimation		= true;
		e.AnimState.FrameTime		= CONSTANT.EFFECT_ANIMATION_SPEED;
	}

	m_ObjectTag = EObjectTag::ItemBullet;
	InitCollision();
	return true;
}

//---------------------------.
// �����蔻��̏�����.
//---------------------------.
void CPotionBullet::InitCollision()
{
	m_pCollisions = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollType::Sphere2D );
	m_pCollisions->GetCollision<CSphere2D>()->SetSprite( m_pSprite, &m_SpriteState );
}

//---------------------------.
// �e�̏�Ԃ̏�����.
//---------------------------.
void CPotionBullet::InitBulletState( const SBulletState& State )
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
bool CPotionBullet::DeleteCheck( const EObjectTag DeleteTag )
{
	// ���a�̎擾.
	const float Radius = m_pCollisions->GetCollision<CSphere2D>()->GetRadius();

	// �|�[�V�����G���A�̍쐬.
	CPotionAreaManager::PushArea(
		m_SpriteState.Transform.Position,
		Radius * CConstant::GetPotionArea().AREA_RADIUS,
		m_BulletState.SkillName
	);
	return true;
}

//---------------------------.
// �X�e�[�W����]����悤�ɑł��Ԃ��邩�̊m�F.
//---------------------------.
bool CPotionBullet::RotationHitBackCheck( const int AttackNo )
{
	if ( AttackNo == AttackNo::Heavy ) return true;

	// �e�̑ϋv�l�����炷.
	m_BulletState.Shield -= AttackNo - 1;

	// SE�̍Đ�.
	CSoundManager::PlaySE( "ShieldAttack" );

	// �ϋv�l��0�ɂȂ�����e����.
	if ( m_BulletState.Shield > 0 ) return true;
	Delete( EObjectTag::None );

	// SE�̍Đ�.
	CSoundManager::PlaySE( "ShieldBreak" );
	return false;
}

//---------------------------.
// �}�E�X�J�[�\���̕����ɑł��Ԃ�.
//---------------------------.
bool CPotionBullet::AttackHitBackCheck( const int AttackNo )
{
	// �e�̑ϋv�l�����炷.
	if ( m_IsBadPotion ) {
		if ( AttackNo != AttackNo::Heavy ) {
			m_BulletState.Shield -= AttackNo;
		}
	}
	else {
		m_BulletState.Shield -= AttackNo - 1;
		if ( AttackNo == AttackNo::Heavy ) {
			m_BulletState.Shield = 0;
		}
	}

	// �ϋv�l��0�ɂȂ�����e����.
	if ( m_BulletState.Shield > 0 ) return true;
	Delete( EObjectTag::None );

	// SE�̍Đ�.
	CSoundManager::PlaySE( "ShieldBreak" );
	return true;
}

//---------------------------.
// �e�̃X�L���̍X�V.
//---------------------------.
void CPotionBullet::SkillUpdate()
{
	// �ϋv�l���ǂ������Ă��邩.
	const float Rate = static_cast<float>( m_BulletState.Shield ) / m_StartBulletState.Shield;

	// ���݂̒e�̃��x���𒲂ׂ�.
	const int OldLv = m_DamageLv;
	if (		Rate > 0.75f		) m_DamageLv = EffectNo::Lv1;
	else if (	Rate > 0.50f		) m_DamageLv = EffectNo::Lv2;
	else if (	Rate > 0.25f		) m_DamageLv = EffectNo::Lv3;
	else							  m_DamageLv = EffectNo::Lv4;
	if ( m_BulletState.Shield == 1	) m_DamageLv = EffectNo::Lv4;

	// ���x�����オ�����ꍇ�����ڂ�ύX����.
	if ( OldLv == m_DamageLv ) return;
	m_SpriteState.AnimState.AnimNumber = m_BulletState.ImageNo + m_SpriteState.AnimState.PatternMax.x * m_DamageLv;
}

//---------------------------.
// ���x���G�t�F�N�g�̍X�V.
//---------------------------.
void CPotionBullet::LvEffectUpdate()
{
	const float Rate = m_BulletState.Attack / m_StartBulletState.Attack;

	// �ꎞ�I�ɑS�Ẵ��x���G�t�F�N�g���\���ɂ���.
	for ( auto& e : m_LvEffectStateList ) {
		e.IsDisp = false;
	}

	// ���݂̒e�̃��x���𒲂ׂ�.
	const int OldLv = m_BulletLv;
	if (		Rate >= CONSTANT.LEVEL_POWER[EffectNo::Lv4] ) m_BulletLv = EffectNo::Lv4;
	else if (	Rate >= CONSTANT.LEVEL_POWER[EffectNo::Lv3] ) m_BulletLv = EffectNo::Lv3;
	else if (	Rate >= CONSTANT.LEVEL_POWER[EffectNo::Lv2] ) m_BulletLv = EffectNo::Lv2;
	else if (	Rate >  CONSTANT.LEVEL_POWER[EffectNo::Lv1] ) m_BulletLv = EffectNo::Lv1;

	// �Ή��������x���G�t�F�N�g��\������.
	if ( m_BulletLv == EffectNo::LvEffect_None ) return;
	m_LvEffectStateList[m_BulletLv].IsDisp = true;

	// ���x�����オ�����ꍇSE���Đ�����.
	if ( OldLv == m_BulletLv ) return;
	CSoundManager::PlaySE( "LvUp" );
}