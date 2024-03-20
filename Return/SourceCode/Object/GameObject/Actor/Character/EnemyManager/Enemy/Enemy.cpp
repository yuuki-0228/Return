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
		Back,	// �w�i.
		Gauge,	// �Q�[�W.
		Frame,	// �t���[��.

		HpBar_Max
	} typedef EHpBarNo;

	// �X�v���C�g�̃t�@�C�������X�g.
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
// ������.
//---------------------------.
bool CEnemy::Init()
{
	// �摜�̎擾.
	m_pSprite		= CSpriteResource::GetSprite( m_EnemyState.TypeName, &m_SpriteState );
	for( int i = 0; i < HpBar_Max; ++i ){
		m_pHpBarList[i] = CSpriteResource::GetSprite( SpriteNameList[i], &m_HpBarStateList[i] );
	}

	// �A�j���[�V��������悤�ɕύX.
	m_SpriteState.AnimState.IsAnimationX = true;

	m_ObjectTag = EObjectTag::Enemy;
	InitCollision();
	return true;
}

//---------------------------.
// �X�V.
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

	Action();			// �s������.
	CoolTimeUpdate();	// �N�[���^�C���̍X�V����.

	for ( auto& s : m_HpBarStateList ) {
		s.Transform.Position.x = m_SpriteState.Transform.Position.x - m_pSprite->GetSpriteState().Disp.w / 2.0f;
		s.Transform.Position.y = m_SpriteState.Transform.Position.y - m_pSprite->GetSpriteState().Disp.h / 2.0f - 10.0f;
	}
	m_HpBarStateList[Gauge].Color				= Color4::Red;
	m_HpBarStateList[Gauge].Transform.Scale.x	= static_cast<float>( m_Hp ) / m_HpMax;

	// �g�����X�t�H�[���̍X�V.
	m_Transform = m_SpriteState.Transform;

	UpdateCollision();
	CActorCollisionManager::PushObject( this, ECollObjType::Both );
}

//---------------------------.
// �`��.
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
// �����蔻��֐�.
//---------------------------.
void CEnemy::Collision( CActor* pActor )
{
	if ( m_IsDisp == false ) return;

	const EObjectTag Tag = pActor->GetObjectTag();
	if ( Tag != EObjectTag::Bullet &&
		 Tag != EObjectTag::ItemBullet &&
		 Tag != EObjectTag::SpecialBullet ) return;

	// �_���[�W���󂯂邩�̓����蔻��.
	if ( Tag == EObjectTag::SpecialBullet && m_StrongDamageCoolTimeCnt != 0 ) return;
	DamageCollision( pActor );
}

//---------------------------.
// �X�|�[��������.
//---------------------------.
void CEnemy::Spown( const D3DXPOSITION3& Pos, const SEnemyState State )
{
	// �G�̏���ۑ�.
	m_StartEnemyState	= State;
	m_EnemyState		= State;

	// HP�̏�����.
	m_HpMax				= 100;
	m_Hp				= m_HpMax;

	// ������.
	Init();

	// �\���ʒu�̐ݒ�.
	m_SpriteState.Transform.Position	= Pos;
	m_IsDisp							= true;
}

//---------------------------.
// �����蔻��̏�����.
//---------------------------.
void CEnemy::InitCollision()
{
	m_pCollisions = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollType::Sphere2D );
	m_pCollisions->GetCollision<CSphere2D>()->SetSprite( m_pSprite, &m_SpriteState );
}

//---------------------------.
// �����蔻��̍X�V.
//---------------------------.
void CEnemy::UpdateCollision()
{
}

//---------------------------.
// �_���[�W���󂯂邩�̓����蔻��.
//---------------------------.
void CEnemy::DamageCollision( CActor* pActor )
{
	// �e���擾.
	CBulletBase* pBullet = dynamic_cast<CBulletBase*>( pActor );

	// �_���[�W���󂯂邩�̓����蔻��.
	if ( pBullet->GetIsDamageBullet() == false ) return;
	if ( Coll2D::IsSphere2DToSphere2D(
		m_pCollisions->GetCollision<CSphere2D>(),
		pBullet->GetCollision<CSphere2D>() ) == false ) return;

	// �_���[�W���󂯂�.
	DamageHp( pBullet->GetAttack() );

	// SE�̍Đ�.
	CSoundManager::PlaySE( "Damage" );

	// �e�̍폜.
	pBullet->Delete( m_ObjectTag );

	// �����e�̏ꍇ�N�[���^�C����ׂ���.
	if ( pBullet->GetObjectTag() == EObjectTag::Bullet ) return;
	m_StrongDamageCoolTimeCnt = CONSTANT.COOL_TIME;
}

//---------------------------.
// �N�[���^�C���̍X�V.
//---------------------------.
void CEnemy::CoolTimeUpdate()
{
	m_StrongDamageCoolTimeCnt -= m_DeltaTime;

	// �N�[���^�C�����I��������.
	if ( m_StrongDamageCoolTimeCnt > 0.0f ) return;
	m_StrongDamageCoolTimeCnt = 0.0f;
}

//---------------------------.
// �G�̍s��.
//---------------------------.
void CEnemy::Action()
{
	SEnemyActionState* pState = m_EnemyState.GetNowAction();

	if (		pState->ActionName == "Attack"	) Attack();
	else if (	pState->ActionName == "Wait"	) Wait();
	else if (	pState->ActionName == "Move"	) Move();
	else if (	pState->ActionName == "Loop"	) ActionLoop();

	// ���݂̍s�������ɖ߂�.
	if ( m_EnemyState.NowActionNo < m_EnemyState.ActionSize ) return;
	m_EnemyState.NowActionNo = 0;
}

//---------------------------.
// �U��.
//---------------------------.
void CEnemy::Attack()
{
	if ( AttackCheck() == false ) return;

	SEnemyActionState* pState = m_EnemyState.GetNowAction();

	// �U�������񐔂𑝂₷.
	pState->AttackNum++;

	// �e�𔭎˂���.
	CBulletManager::Shot( m_SpriteState.Transform.Position, pState->BulletName );

	// ���̍s���ɕύX.
	if ( pState->AttackNum < pState->ShotNum ) return;
	pState->AttackNum = 0;
	m_EnemyState.NowActionNo++;
}

//---------------------------.
// �U�����s�����m�F.
//---------------------------.
bool CEnemy::AttackCheck()
{
	SEnemyActionState* pState = m_EnemyState.GetNowAction();

	// �U���̑҂����Ԃ𑝂₷.
	pState->AttackTimeCnt += m_DeltaTime;

	// �U�����s�����Ԃ��擾.
	const float AttackTime = pState->Interval + pState->BulletAddTime * CBulletManager::GetBulletNum();

	// �U�����s�������ׂ�.
	if ( pState->AttackTimeCnt < AttackTime ) return false;

	// �U���̑҂����Ԃ�����������.
	pState->AttackTimeCnt = INIT_FLOAT;
	return true;
}

//---------------------------.
// �ҋ@.
//---------------------------.
void CEnemy::Wait()
{
	SEnemyActionState* pState = m_EnemyState.GetNowAction();

	// �ҋ@���Ԃ𑝂₷.
	pState->WaitTimeCnt += m_DeltaTime;

	// �I�����Ԃ��擾.
	const float EndTime = pState->WaitTime + pState->BulletAddTime * CBulletManager::GetBulletNum();

	// �ҋ@���I�����邩.
	if ( pState->WaitTimeCnt < EndTime ) return;

	// ���̍s���ɕύX.
	pState->WaitTimeCnt = 0;
	m_EnemyState.NowActionNo++;
}

//---------------------------.
// �ړ�.
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
// �s���̃��[�v.
//---------------------------.
void CEnemy::ActionLoop()
{
	SEnemyActionState* pState = m_EnemyState.GetNowAction();

	// �w�肵���A�N�V�����ɕύX.
	if ( pState->LoopEndNum <= 0 ||
		 pState->NowLoopNum < pState->LoopEndNum )
	{
		m_EnemyState.NowActionNo = pState->GoActionNo;

		// ���[�v�񐔂�ǉ�.
		if ( pState->LoopEndNum <= 0 ) return;
		pState->NowLoopNum++;
		return;
	}

	// ���̍s���ɕύX.
	pState->NowLoopNum = 0;
	m_EnemyState.NowActionNo++;
}
