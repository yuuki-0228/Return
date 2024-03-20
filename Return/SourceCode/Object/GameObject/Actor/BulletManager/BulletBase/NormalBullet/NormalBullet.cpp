#include "NormalBullet.h"
#include "..\..\..\Character\Player\AttackEffect\AttackEffect.h"
#include "..\..\..\..\..\..\Common\SoundManeger\SoundManeger.h"
#include "..\..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\..\..\Utility\Math\Math.h"

CNormalBullet::CNormalBullet()
{
}

CNormalBullet::~CNormalBullet()
{
}

//---------------------------.
// ������.
//---------------------------.
bool CNormalBullet::Init()
{
	// �摜�̎擾.
	m_pSprite									= CSpriteResource::GetSprite( "Bullet",			&m_SpriteState );
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

	m_ObjectTag = EObjectTag::Bullet;
	InitCollision();
	return true;
}

//---------------------------.
// �����蔻��̏�����.
//---------------------------.
void CNormalBullet::InitCollision()
{
	m_pCollisions = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollType::Sphere2D );
	m_pCollisions->GetCollision<CSphere2D>()->SetSprite( m_pSprite, &m_SpriteState );
}

//---------------------------.
// �e�̏�Ԃ̏�����.
//---------------------------.
void CNormalBullet::InitBulletState( const SBulletState& State )
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
// ���x���G�t�F�N�g�̍X�V.
//---------------------------.
void CNormalBullet::LvEffectUpdate()
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