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
// ������.
//---------------------------.
bool CPlayer::Init()
{
	// �U���G�t�F�N�g�̍쐬.
	m_pAttackEffect = std::make_unique<CAttackEffect>();
	m_pAttackEffect->Init();

	// �摜�̎擾.
	m_pSprite = CSpriteResource::GetSprite( "Player", &m_SpriteState );

	// �\���ʒu�̐ݒ�.
	m_SpriteState.Transform.Position	= CONSTANT.POSITION;
	m_Transform.Position				= CONSTANT.POSITION;

	// �A�j���[�V��������悤�ɕύX.
	m_SpriteState.AnimState.IsAnimation = true;

	// HP�̐ݒ�.
	m_HpMax			= CONSTANT.HP_MAX;
	m_Hp			= m_HpMax;
	// �X�^�~�i�̐ݒ�.
	m_StaminaMax	= CONSTANT.STAMINA_MAX;
	m_Stamina		= m_StaminaMax;

	m_ObjectTag		= EObjectTag::Player;
	InitCollision();
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CPlayer::Update( const float& DeltaTime )
{
	m_DeltaTime = DeltaTime;

	m_pAttackEffect->Update( DeltaTime );	// �U���G�t�F�N�g�̍X�V.
	RecoveryStamina();						// �X�^�~�i�̉�.
	CoolTimeEnd();							// �N�[���^�C���̏I������.

	if ( GetAsyncKeyState( 'P' ) & 0x0001 ) m_Hp = 0;
	if ( GetAsyncKeyState( 'O' ) & 0x0001 ) m_Hp = CONSTANT.HP_MAX;
	if ( GetAsyncKeyState( 'I' ) & 0x0001 ) CBulletManager::AllDelete();

	UpdateCollision();
	CActorCollisionManager::PushObject( this, ECollObjType::Both, ECollSort::Near );
}

//---------------------------.
// �����蔻��I����Ăяo�����X�V.
//---------------------------.
void CPlayer::LateUpdate( const float& DeltaTime )
{
	AttackStaminaDown();// �U���ɂ��X�^�~�i�̌���.
	MissAttack();		// ��U��.
	HitCoolTimeEnd();	// �U���𓖂Ă����ꍇ�N�[���^�C���𖳂���.
}

//---------------------------.
// �f�o�b�N�̍X�V.
//---------------------------.
void CPlayer::DebugUpdate( const float& DeltaTime )
{
}

//---------------------------.
// �`��.
//---------------------------.
void CPlayer::Render()
{
	m_pSprite->RenderUI( &m_SpriteState );

	// �U���G�t�F�N�g�̕\��.
	m_pAttackEffect->Render();

	CCollisionRender::PushSphere2D( m_pCollisions->GetCollision<CSphere2D>() );
	for ( int i = 1; i < AttackNo::Max; ++i ) {
		CCollisionRender::PushSphere2D( m_pAttackCollList[i].get() );
	}
}

//---------------------------.
// �����蔻��֐�.
//---------------------------.
void CPlayer::Collision( CActor* pActor )
{
	const EObjectTag Tag = pActor->GetObjectTag();
	if ( Tag != EObjectTag::Bullet &&
		 Tag != EObjectTag::ItemBullet &&
		 Tag != EObjectTag::SpecialBullet	) return;
	if ( pActor->GetIsCollisionStop()		) return;

	// �ł��Ԃ��̓����蔻��.
	AttackCollision( pActor );

	// �_���[�W���󂯂邩�̓����蔻��.
	if ( pActor->GetIsCollisionStop()		) return;
	DamageCollision( pActor );
}

//---------------------------.
// �����蔻��̏�����.
//---------------------------.
void CPlayer::InitCollision()
{
	// �_���[�W����̓����蔻��̍쐬.
	m_pCollisions = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollType::Sphere2D );
	m_pCollisions->GetCollision<CSphere2D>()->SetSprite( m_pSprite, &m_SpriteState );
	m_pCollisions->GetCollision<CSphere2D>()->SetRadius( CONSTANT.DAMAGE_COLL_SIZE );

	// ���U���̓����蔻��̍쐬.
	m_pAttackCollList[AttackNo::Heavy] = std::make_unique<CSphere2D>();
	m_pAttackCollList[AttackNo::Heavy]->SetSprite( m_pSprite, &m_SpriteState );
	m_pAttackCollList[AttackNo::Heavy]->SetRadius( CONSTANT.HEAVY_COLL_SIZE );

	// ���U���̓����蔻��̍쐬.
	m_pAttackCollList[AttackNo::Normal] = std::make_unique<CSphere2D>();
	m_pAttackCollList[AttackNo::Normal]->SetSprite( m_pSprite, &m_SpriteState );
	m_pAttackCollList[AttackNo::Normal]->SetRadius( CONSTANT.NORMAL_COLL_SIZE );

	// ��U���̓����蔻��̍쐬.
	m_pAttackCollList[AttackNo::Weak] = std::make_unique<CSphere2D>();
	m_pAttackCollList[AttackNo::Weak]->SetSprite( m_pSprite, &m_SpriteState );
	m_pAttackCollList[AttackNo::Weak]->SetRadius( CONSTANT.WEAK_COLL_SIZE );
}

//---------------------------.
// �����蔻��̍X�V.
//---------------------------.
void CPlayer::UpdateCollision()
{
}

//---------------------------.
// �_���[�W���󂯂邩�̓����蔻��.
//---------------------------.
void CPlayer::DamageCollision( CActor* pActor )
{
	if ( m_Hp <= 0 ) return;

	// �e���擾.
	CBulletBase* pBullet = dynamic_cast<CBulletBase*>( pActor );

	// �_���[�W���󂯂邩�̓����蔻��.
	if ( Coll2D::IsSphere2DToSphere2D(
		m_pCollisions->GetCollision<CSphere2D>(),
		pBullet->GetCollision<CSphere2D>() ) == false ) return;

	// Hp�����炷.
	DamageHp( pBullet->GetPlayerDamage() );

	// SE�̍Đ�.
	CSoundManager::PlaySE( "PlayerDamage" );

	// �e��S�č폜.
	CBulletManager::AllDelete();
}

//---------------------------.
// �e�̍U���̓����蔻��.
//---------------------------.
void CPlayer::AttackCollision( CActor* pActor )
{
	if ( m_Stamina < CONSTANT.ATTACK_STAMINA ) return;

	// �e���擾.
	CBulletBase* pBullet = dynamic_cast<CBulletBase*>( pActor );

	// �ł��Ԃ����̓����蔻��.
	for ( int i = 1; i < AttackNo::Max; ++i ) {
		if ( m_HitAttackNo != AttackNo::None && m_HitAttackNo != i ) continue;
		if ( Coll2D::IsSphere2DToSphere2D(
			m_pAttackCollList[i].get(),
			pBullet->GetCollision<CSphere2D>() ) == false ) continue;

		// ���N���b�N�������Ă��鎞.
		if ( CKeyInput::IsKeyDown( VK_LBUTTON ) ) {
			// �X�e�[�W����]����悤�ɒe��ł��Ԃ�.
			pBullet->RotationHitBack( i ) ;
			m_HitAttackNo		= i;
			m_pAttackEffect->EffectPlay( i );
			return;
		}

		// �E�N���b�N�������Ă��鎞.
		if ( CKeyInput::IsKeyDown( VK_RBUTTON ) ) {
			// �}�E�X�J�[�\���̕����ɔ�Ԃ悤�ɒe��ł��Ԃ�.
			pBullet->AttackHitBack( i );
			m_HitAttackNo		= i;
			m_IsAttackCoolTime	= true;
			m_pAttackEffect->EffectPlay( i );

			// �X�^�~�i�����炷.
			m_Stamina += CONSTANT.RECOVERY_STAMINA - CONSTANT.ATTACK_TYPE_DOWN_STAMINA * i;
			if ( m_Stamina > CONSTANT.STAMINA_MAX ) m_Stamina = CONSTANT.STAMINA_MAX;
			return;
		}
	}
}

//---------------------------.
// �U���ɂ��X�^�~�i�̌���.
//---------------------------.
void CPlayer::AttackStaminaDown()
{
	if ( m_HitAttackNo						== AttackNo::None	) return;
	if ( m_IsAttackCoolTime					== true				) return;
	if ( CKeyInput::IsKeyDown( VK_LBUTTON )	== false			) return;

	// �X�^�~�i�����炷.
	m_Stamina -= CONSTANT.ATTACK_STAMINA - CONSTANT.ATTACK_TYPE_DOWN_STAMINA * ( AttackNo::Max - m_HitAttackNo );
	m_IsAttackCoolTime = true;
}

//---------------------------.
// �X�^�~�i�̉�.
//---------------------------.
void CPlayer::RecoveryStamina()
{
	if ( m_Stamina >= CONSTANT.STAMINA_MAX ) return;

	m_Stamina += CONSTANT.STAMINA_RECOVERY_SPEED * m_DeltaTime;
	if ( m_Stamina > CONSTANT.STAMINA_MAX ) m_Stamina = CONSTANT.STAMINA_MAX;
}

//---------------------------.
// �N�[���^�C���̏I��.
//---------------------------.
void CPlayer::CoolTimeEnd()
{
	if ( m_IsAttackCoolTime == false ) return;
		
	// �Ō�ɍĐ������G�t�F�N�g���I�����Ă��邩.
	if ( m_pAttackEffect->GetIsEffectPlay() ) return;

	// �N�[���^�C�����I��������.
	m_IsAttackCoolTime = false;
}

//---------------------------.
// �U���𓖂Ă����̃N�[���^�C���̏I������.
//---------------------------.
void CPlayer::HitCoolTimeEnd()
{
	if ( m_HitAttackNo == AttackNo::None ) return;

	// �U���̃N�[���^�C���𖳂���.
	m_IsAttackCoolTime	= false;
	m_HitAttackNo		= AttackNo::None;
}

//---------------------------.
// ��U��.
//---------------------------.
void CPlayer::MissAttack()
{
	if ( m_Stamina											<  CONSTANT.ATTACK_STAMINA	) return;
	if ( m_HitAttackNo										!= AttackNo::None			) return;
	if ( m_IsAttackCoolTime									== true						) return;
	if ( CKeyInput::IsORKeyDown( VK_LBUTTON, VK_RBUTTON )	== false					) return;
	
	// ��U��̃G�t�F�N�g�̍Đ�.
	m_IsAttackCoolTime	= true;
	m_HitAttackNo		= AttackNo::None;
	m_pAttackEffect->EffectPlay( AttackNo::None );

	// �X�^�~�i�����炷.
	m_Stamina -= CONSTANT.ATTACK_STAMINA;
}
