#include "CrackGimmick.h"
#include "..\..\..\BulletManager\BulletManager.h"
#include "..\..\..\..\..\..\Common\SoundManeger\SoundManeger.h"
#include "..\..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\..\..\Utility\Math\Math.h"

namespace {
	constexpr int CRACKED_NO = 1;	// �������Ă���摜No.
	constexpr int BROKEN_NO	 = 2;	// ���Ă���摜No.
}

CCrackGimmick::CCrackGimmick()
{
}

CCrackGimmick::~CCrackGimmick()
{
}

//---------------------------.
// ������.
//---------------------------.
bool CCrackGimmick::Init()
{
	// �摜�̎擾.
	m_pSprite = CSpriteResource::GetSprite( "Barrel", &m_SpriteState );

	// �����ڂ�ύX�ł���悤�ɕύX.
	m_SpriteState.AnimState.IsSetAnimNumber = true;

	m_ObjectTag = EObjectTag::Gimmick;
	InitCollision();
	return true;
}

//---------------------------.
// �����蔻��֐�.
//---------------------------.
void CCrackGimmick::Collision( CActor* pActor )
{
	if ( m_GimmickState.Hp <= 0 ) return;
	const EObjectTag Tag = pActor->GetObjectTag();
	if ( Tag != EObjectTag::Bullet &&
		 Tag != EObjectTag::ItemBullet &&
		 Tag != EObjectTag::SpecialBullet ) return;

	// �e���擾.
	CBulletBase* pBullet = dynamic_cast<CBulletBase*>(pActor);

	// �_���[�W���󂯂邩�̓����蔻��.
	if ( Coll2D::IsSphere2DToSphere2D(
		m_pCollisions->GetCollision<CSphere2D>(),
		pBullet->GetCollision<CSphere2D>() ) == false ) return;

	// �e���폜����.
	pBullet->Delete( m_ObjectTag );

	// �ϋv�͂����炷.
	if ( Tag == EObjectTag::Bullet ) m_GimmickState.Hp--;
	else							 m_GimmickState.Hp = 0;

	// �A�C�e���𗎂Ƃ�.
	if ( m_GimmickState.Hp > 0 ) return;
	m_GimmickState.Hp = 0;
	CBulletManager::DropShot( m_SpriteState.Transform.Position, m_GimmickState.DropBulletName );
}

//---------------------------.
// �����蔻��̏�����.
//---------------------------.
void CCrackGimmick::InitCollision()
{
	m_pCollisions = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollType::Sphere2D );
	m_pCollisions->GetCollision<CSphere2D>()->SetSprite( m_pSprite, &m_SpriteState );
}

//---------------------------.
// �����蔻��̍X�V.
//---------------------------.
void CCrackGimmick::UpdateCollision()
{
}

//---------------------------.
// �M�~�b�N�̍X�V.
//---------------------------.
void CCrackGimmick::GimmickUpdate()
{
	// �ϋv�͂�0�Ȃ��ꂽ�����ڂɕύX.
	if ( m_GimmickState.Hp <= 0 ) {
		m_SpriteState.AnimState.AnimNumber = BROKEN_NO;
		return;
	}

	// �ϋv�͂������ȉ��Ȃ��ꂩ���̌����ڂɕύX.
	const float Rate = static_cast<float>( m_GimmickState.Hp ) / m_StartGimmickState.Hp;
	if ( Rate > 0.5 ) return;
	m_SpriteState.AnimState.AnimNumber = CRACKED_NO;
}
