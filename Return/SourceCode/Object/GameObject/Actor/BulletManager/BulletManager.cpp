#include "BulletManager.h"
#include "..\BulletManager\BulletBase\NormalBullet\NormalBullet.h"
#include "..\BulletManager\BulletBase\SharpBullet\SharpBullet.h"
#include "..\BulletManager\BulletBase\SpillBullet\SpillBullet.h"
#include "..\BulletManager\BulletBase\PowerUpBullet\PowerUpBullet.h"
#include "..\BulletManager\BulletBase\PotionBullet\PotionBullet.h"
#include "..\BulletManager\BulletBase\BigPotionBullet\BigPotionBullet.h"
#include "..\..\Widget\UIWidget\GameMain\TimingBarUI\TimingBarUI.h"
#include "..\..\Widget\UIWidget\GameMain\TimingBarUI\TimingBarUI.h"

namespace {
	constexpr char STATE_FILE_PATH[] = "Data\\Parameter\\Bullet";
}

CBulletManager::CBulletManager()
	: m_pBulletList			()
	, m_pRenderList			()
	, m_StateList			()
{
}

CBulletManager::~CBulletManager()
{
}

//----------------------------.
// �C���X�^���X�̎擾.
//----------------------------.
CBulletManager* CBulletManager::GetInstance()
{
	static std::unique_ptr<CBulletManager> pInstance = std::make_unique<CBulletManager>();
	return pInstance.get();
}

//---------------------------.
// ������.
//---------------------------.
bool CBulletManager::Init()
{
	CBulletManager* pI = GetInstance();

	pI->m_pBulletList.clear();
	pI->m_pRenderList.clear();

	// �e�̏���ǂݍ���.
	StateLood();
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CBulletManager::Update( const float& DeltaTime )
{
	CBulletManager* pI = GetInstance();

	for ( int i = 0; i < pI->m_pBulletList.size(); ++i ) {
		pI->m_pBulletList[i]->Update( DeltaTime );
	}

	BulletDelete();			// �g�p���Ă��Ȃ��e�̍폜.
	SameBulletDelete();		// �����e�ʂ��d�Ȃ��Ă���ꍇ�폜����.
	PushTimingBar();		// �^�C�~���O�o�[�ɏ���n��.
}

//---------------------------.
// �����蔻��I����Ăяo�����X�V.
//---------------------------.
void CBulletManager::LateUpdate( const float& DeltaTime )
{
	CBulletManager* pI = GetInstance();

	for ( int i = 0; i < pI->m_pBulletList.size(); ++i ) {
		pI->m_pBulletList[i]->LateUpdate( DeltaTime );
	}
}

//---------------------------.
// �`��.
//---------------------------.
void CBulletManager::Render()
{
}

//---------------------------.
// �`�惊�X�g�̎擾.
//---------------------------.
std::vector<std::pair<float, CActor*>> CBulletManager::GetRenderList()
{
	// �`�揇�Ԃ̍X�V.
	RenderListUpdate();
	return GetInstance()->m_pRenderList;
}

//---------------------------.
// �e��S�č폜.
//---------------------------.
void CBulletManager::AllDelete()
{
	CBulletManager* pI = GetInstance();

	for ( auto& b : pI->m_pBulletList ) b->Delete( EObjectTag::None, true );
}

//---------------------------.
// �e�̔���.
//---------------------------.
void CBulletManager::Shot( const D3DXPOSITION3& Pos, const std::string& Name )
{
	CBulletManager* pI = GetInstance();

	// �e�̏�񂪓ǂݍ��܂�Ă��邩���ׂ�.
#ifdef _DEBUG
	LoadStateCheck( Name );
#endif

	// �g�p���Ă��Ȃ��̂�����Ύg�p����.
	for ( auto& b : pI->m_pBulletList ) {
		if ( b->GetIsEnd() == false ) continue;

		// �e�̔���.
		b = CreateBullet( pI->m_StateList[Name].BulletTag );
		b->NormalShot( Pos, pI->m_StateList[Name] );
		return;
	}

	// �V�����쐬.
	pI->m_pBulletList.emplace_back( CreateBullet( pI->m_StateList[Name].BulletTag ) );

	// �e������.
	pI->m_pBulletList.back()->NormalShot( Pos, pI->m_StateList[Name] );
}

//---------------------------.
// ��������e�̔���.
//---------------------------.
void CBulletManager::DropShot( const D3DXPOSITION3& Pos, const std::string& Name )
{
	CBulletManager* pI = GetInstance();

	// �e�̏�񂪓ǂݍ��܂�Ă��邩���ׂ�.
#ifdef _DEBUG
	LoadStateCheck( Name );
#endif

	// �g�p���Ă��Ȃ��̂�����Ύg�p����.
	for ( auto& b : pI->m_pBulletList ) {
		if ( b->GetIsEnd() == false ) continue;

		// �e�̔���.
		b = CreateBullet( pI->m_StateList[Name].BulletTag );
		b->DropShot( Pos, pI->m_StateList[Name] );
		return;
	}

	// �V�����쐬.
	pI->m_pBulletList.emplace_back( CreateBullet( pI->m_StateList[Name].BulletTag ) );

	// �e������.
	pI->m_pBulletList.back()->DropShot( Pos, pI->m_StateList[Name] );
}

//---------------------------.
// �p�x�ɂ����������ɒe�𔭎˂���( �オ0�x ).
//---------------------------.
void CBulletManager::DirectionShot( const D3DXPOSITION3& Pos, const float Ang, const std::string& Name )
{
	CBulletManager* pI = GetInstance();

	// �e�̏�񂪓ǂݍ��܂�Ă��邩���ׂ�.
#ifdef _DEBUG
	LoadStateCheck( Name );
#endif

	// �g�p���Ă��Ȃ��̂�����Ύg�p����.
	for ( auto& b : pI->m_pBulletList ) {
		if ( b->GetIsEnd() == false ) continue;

		// �e�̔���.
		b = CreateBullet( pI->m_StateList[Name].BulletTag );
		b->DirectionShot( Pos, Ang, pI->m_StateList[Name] );
		return;
	}

	// �V�����쐬.
	pI->m_pBulletList.emplace_back( CreateBullet( pI->m_StateList[Name].BulletTag ) );

	// �e������.
	pI->m_pBulletList.back()->DirectionShot( Pos, Ang, pI->m_StateList[Name] );
}

//---------------------------.
// �S�����ɒe�𔭎�.
//---------------------------.
void CBulletManager::AllDirectionShot( const D3DXPOSITION3& Pos, const int Num, const std::string& Name )
{
	for ( int i = 0; i < Num; ++i ) {
		const float Ang = ( Math::DEGREE_MAX / Num ) * i;
		DirectionShot( Pos, Ang, Name );
	}
}

//---------------------------.
// �e�̕ύX.
//---------------------------.
void CBulletManager::BulletChange( CBulletBase* pOldBullet, const std::string& Name, const bool IsEffectPlay )
{
	CBulletManager* pI = GetInstance();

	// �e�̏�񂪓ǂݍ��܂�Ă��邩���ׂ�.
#ifdef _DEBUG
	LoadStateCheck( Name );
#endif

	// �ύX�O�̒e�̏��̎擾.
	const D3DXPOSITION3 Pos			= pOldBullet->GetPosition();
	const bool			IsRotMove	= pOldBullet->GetIsRotationMove();

	// �e�̍폜.
	pOldBullet->Delete( EObjectTag::None, true, IsEffectPlay );

	// �g�p���Ă��Ȃ��̂�����Ύg�p����.
	for ( auto& b : pI->m_pBulletList ) {
		if ( b->GetIsEnd() == false ) continue;

		// �e�̔���.
		b = CreateBullet( pI->m_StateList[Name].BulletTag );
		if ( IsRotMove ) {
			b->StageRotationShot( Pos, pI->m_StateList[Name] );
			return;
		}
		b->DropShot( Pos, pI->m_StateList[Name] );
		return;
	}

	// �V�����쐬.
	pI->m_pBulletList.emplace_back( CreateBullet( pI->m_StateList[Name].BulletTag ) );

	// �e������.
	if ( IsRotMove ) {
		pI->m_pBulletList.back()->StageRotationShot( Pos, pI->m_StateList[Name] );
		return;
	}
	pI->m_pBulletList.back()->DropShot( Pos, pI->m_StateList[Name] );
}

//---------------------------.
// �e�̐����擾.
//---------------------------.
int CBulletManager::GetBulletNum()
{
	CBulletManager* pI = GetInstance();

	int Num = 0;
	for ( auto& b : pI->m_pBulletList ) {
		if ( b->GetIsDisp() == false ) continue;
		Num++;
	}
    return Num;
}

//---------------------------.
// �e�̏��̓ǂݍ���.
//---------------------------.
HRESULT CBulletManager::StateLood()
{
	CBulletManager* pI = GetInstance();

	auto StateLoad = [&]( const std::filesystem::directory_entry& Entry )
	{
		const std::string Extension = Entry.path().extension().string();	// �g���q.
		const std::string FilePath	= Entry.path().string();				// �t�@�C���p�X.
		const std::string FileName	= Entry.path().stem().string();			// �t�@�C����.

		// �g���q�� ".json" �ł͂Ȃ��ꍇ�ǂݍ��܂Ȃ�.
		if ( Extension != ".json" && Extension != ".JSON" ) return;

		// json�t�@�C����ǂݍ���.
		json j = FileManager::JsonLoad( FilePath );

		// �e�̏������X�g�Ɋi�[����.
		const D3DXCOLOR3&	Color		= { j["DeleteColor"]["R"], j["DeleteColor"]["G"], j["DeleteColor"]["B"] };
		const std::string&	SkillName	= j["SkillName"].empty() ? "" : j["SkillName"];
		const int			Shield		= j["Shield"].empty() ? 0 : static_cast<int>( j["Shield"] );
		pI->m_StateList[FileName] = SBulletState(
			GetBulletTag( j["BulletTag"] ),
			Color,
			SkillName,
			j["MoveSpeed"],
			j["Attack"],
			j["PlayerDamage"],
			Shield,
			j["ImageNo"]
		);
	};

	CLog::PushLog( "------ �G�t�F�N�g�ǂݍ��݊J�n ------" );
	try {
		std::filesystem::recursive_directory_iterator Dir_itr( STATE_FILE_PATH ), End_itr;
		std::for_each( Dir_itr, End_itr, StateLoad );
	}
	catch ( const std::filesystem::filesystem_error& e ) {
		// �G���[���b�Z�[�W��\��.
		ErrorMessage( e.path1().string().c_str() );
		return E_FAIL;
	}
	CLog::PushLog( "------ �G�t�F�N�g�ǂݍ��ݏI�� ------" );

	// �ǂݍ��ݏI��.
	return S_OK;
}

//---------------------------.
// �^�O�̎擾.
//---------------------------.
EObjectTag CBulletManager::GetBulletTag( const std::string& TagName )
{
	if ( TagName == "Normal"		) return EObjectTag::Bullet;
	if ( TagName == "Sharp"			) return EObjectTag::SharpBullet;
	if ( TagName == "Spill"			) return EObjectTag::SpillBullet;
	if ( TagName == "PowerUp"		) return EObjectTag::PowerUpBullet;
	if ( TagName == "GoodPotion"	) return EObjectTag::GoodPotionBullet;
	if ( TagName == "BadPotion"		) return EObjectTag::BadPotionBullet;
	if ( TagName == "BigPotion"		) return EObjectTag::BigPotionBullet;
	return EObjectTag::None;
}

//---------------------------.
// �e�̏�񂪓ǂݍ��܂�Ă��邩���ׂ�.
//---------------------------.
void CBulletManager::LoadStateCheck( const std::string& Name )
{
	CBulletManager* pI = GetInstance();

	// ���ׂ�.
	bool IsLoad = false;
	for ( auto& [Key, State] : pI->m_StateList ) {
		if ( Key != Name ) continue;
		IsLoad = true;
		break;
	}

	// �ǂݍ��܂�Ă���.
	if ( IsLoad ) return;

	// �ǂݍ��܂�Ă��Ȃ�.
	ErrorMessage( "�u" + Name + "�v�̒e�̏�񂪂���܂���" );
}

//---------------------------.
// �e�̍쐬.
//---------------------------.
std::shared_ptr<CBulletBase> CBulletManager::CreateBullet( const EObjectTag Tag )
{
	// �^�O�ɑΉ������C���X�^���X�쐬.
	std::shared_ptr<CBulletBase> Out;
	switch ( Tag ) {
	case EObjectTag::Bullet:			Out = std::make_shared<CNormalBullet>();		break;
	case EObjectTag::SharpBullet:		Out = std::make_shared<CSharpBullet>();			break;
	case EObjectTag::SpillBullet:		Out = std::make_shared<CSpillBullet>();			break;
	case EObjectTag::PowerUpBullet:		Out = std::make_shared<CPowerUpBullet>();		break;
	case EObjectTag::GoodPotionBullet:	Out = std::make_shared<CPotionBullet>( false );	break;
	case EObjectTag::BadPotionBullet:	Out = std::make_shared<CPotionBullet>( true  );	break;
	case EObjectTag::BigPotionBullet:	Out = std::make_shared<CBigPotionBullet>();		break;
	default: return nullptr;
	}

	// ���������Ă���.
	Out->Init();
	return Out;
}

//---------------------------.
// �`�惊�X�g�̍X�V.
//---------------------------.
void CBulletManager::RenderListUpdate()
{
	CBulletManager* pI = GetInstance();

	pI->m_pRenderList.clear();

	// Y���W�ƃ|�C���^���擾.
	for ( auto& b : pI->m_pBulletList ) {
		pI->m_pRenderList.emplace_back( std::make_pair( b->GetPosition().y, b.get() ) );
	}
}

//---------------------------.
// �����e�ʂ��d�Ȃ��Ă���ꍇ�폜����.
//---------------------------.
void CBulletManager::SameBulletDelete()
{
	CBulletManager* pI = GetInstance();

	// ������Ԃ̒e�ʂ��d�Ȃ��Ă���ꍇ�폜����.
	for ( auto& a : pI->m_pBulletList ) {
		if ( a->GetIsDisp() == false ) continue;
		for ( auto& b : pI->m_pBulletList ) {
			if ( a == b											) continue;
			if ( b->GetIsDisp()			== false				) continue;
			if ( b->GetIsRotationMove() == false				) continue;
			if ( a->GetBulletState()	!= b->GetBulletState()	) continue;
	
			// �e�ʂ��̋��������߂�.
			const D3DXVECTOR3&	Vector	 = a->GetPosition() - b->GetPosition();
			const float			Distance = D3DXVec3Length( &Vector );
			if ( Distance <= pI->CONSTANT.SAME_BULLET_DELETE_DISTANCE ) b->Delete( EObjectTag::None, false, false );
		}
	}
}

//---------------------------.
// �g�p���Ă��Ȃ��e�̍폜.
//---------------------------.
void CBulletManager::BulletDelete()
{
	CBulletManager* pI = GetInstance();

	const int NormalMax = static_cast<int>( pI->m_pBulletList.size() ) - 1;
	for ( int i = NormalMax; i >= 0; --i ) {
		if ( pI->m_pBulletList[i]->GetIsEnd() == false ) break;
		pI->m_pBulletList.pop_back();
	}
}

//---------------------------.
// �^�C�~���O�o�[�ɏ���n��.
//---------------------------.
void CBulletManager::PushTimingBar()
{
	CBulletManager* pI = GetInstance();

	// ���ʂ̒e��n��.
	for ( auto& b : pI->m_pBulletList ) {
		if ( b->GetIsDisp() == false ) continue;
		if ( b->GetIsCollisionStop() ) continue;

		// �v���C���[�Ƃ̋��������߂�.
		const D3DXVECTOR3&	Vector	 = CConstant::GetPlayer().POSITION - b->GetPosition();
		const float			Distance = D3DXVec3Length( &Vector );
		const bool			IsStrong = b->GetObjectTag() == EObjectTag::SpecialBullet;
		const bool			IsItem	 = b->GetObjectTag() == EObjectTag::ItemBullet;
		if ( Distance > pI->CONSTANT.TIMINGBAR_DISTANCE ) continue;
		
		// �v���C���[�̕����ɔ��ł��邩.
		D3DXVECTOR3	DireVector = Vector;
		D3DXVec3Normalize( &DireVector, &DireVector );
		const D3DXVECTOR3&	MoveVector	= b->GetMoveVecotr();
		const D3DXVECTOR3&	XVector		= { DireVector.x, INIT_FLOAT, INIT_FLOAT };
		const D3DXVECTOR3&	YVector		= { INIT_FLOAT, DireVector.y, INIT_FLOAT };
		const D3DXVECTOR3&	AddXVector	= { DireVector.x + MoveVector.x, INIT_FLOAT, INIT_FLOAT };
		const D3DXVECTOR3&	AddYVector	= { INIT_FLOAT, DireVector.y + MoveVector.y, INIT_FLOAT };
		const bool			IsXMove		= D3DXVec3Length( &AddXVector ) >= D3DXVec3Length( &XVector );
		const bool			IsYMove		= D3DXVec3Length( &AddYVector ) >= D3DXVec3Length( &YVector );

		// �^�C�~���O�o�[�ɃA�C�R����\������.
		if ( IsXMove == false || IsYMove == false ) continue;
		CTimingBarUI::PushIcon( Distance, IsStrong, IsItem );
	}
}
