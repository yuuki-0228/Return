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
// インスタンスの取得.
//----------------------------.
CBulletManager* CBulletManager::GetInstance()
{
	static std::unique_ptr<CBulletManager> pInstance = std::make_unique<CBulletManager>();
	return pInstance.get();
}

//---------------------------.
// 初期化.
//---------------------------.
bool CBulletManager::Init()
{
	CBulletManager* pI = GetInstance();

	pI->m_pBulletList.clear();
	pI->m_pRenderList.clear();

	// 弾の情報を読み込む.
	StateLood();
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CBulletManager::Update( const float& DeltaTime )
{
	CBulletManager* pI = GetInstance();

	for ( int i = 0; i < pI->m_pBulletList.size(); ++i ) {
		pI->m_pBulletList[i]->Update( DeltaTime );
	}

	BulletDelete();			// 使用していない弾の削除.
	SameBulletDelete();		// 同じ弾通し重なっている場合削除する.
	PushTimingBar();		// タイミングバーに情報を渡す.
}

//---------------------------.
// 当たり判定終了後呼び出される更新.
//---------------------------.
void CBulletManager::LateUpdate( const float& DeltaTime )
{
	CBulletManager* pI = GetInstance();

	for ( int i = 0; i < pI->m_pBulletList.size(); ++i ) {
		pI->m_pBulletList[i]->LateUpdate( DeltaTime );
	}
}

//---------------------------.
// 描画.
//---------------------------.
void CBulletManager::Render()
{
}

//---------------------------.
// 描画リストの取得.
//---------------------------.
std::vector<std::pair<float, CActor*>> CBulletManager::GetRenderList()
{
	// 描画順番の更新.
	RenderListUpdate();
	return GetInstance()->m_pRenderList;
}

//---------------------------.
// 弾を全て削除.
//---------------------------.
void CBulletManager::AllDelete()
{
	CBulletManager* pI = GetInstance();

	for ( auto& b : pI->m_pBulletList ) b->Delete( EObjectTag::None, true );
}

//---------------------------.
// 弾の発射.
//---------------------------.
void CBulletManager::Shot( const D3DXPOSITION3& Pos, const std::string& Name )
{
	CBulletManager* pI = GetInstance();

	// 弾の情報が読み込まれているか調べる.
#ifdef _DEBUG
	LoadStateCheck( Name );
#endif

	// 使用していないのがあれば使用する.
	for ( auto& b : pI->m_pBulletList ) {
		if ( b->GetIsEnd() == false ) continue;

		// 弾の発射.
		b = CreateBullet( pI->m_StateList[Name].BulletTag );
		b->NormalShot( Pos, pI->m_StateList[Name] );
		return;
	}

	// 新しく作成.
	pI->m_pBulletList.emplace_back( CreateBullet( pI->m_StateList[Name].BulletTag ) );

	// 弾を撃つ.
	pI->m_pBulletList.back()->NormalShot( Pos, pI->m_StateList[Name] );
}

//---------------------------.
// 落下する弾の発射.
//---------------------------.
void CBulletManager::DropShot( const D3DXPOSITION3& Pos, const std::string& Name )
{
	CBulletManager* pI = GetInstance();

	// 弾の情報が読み込まれているか調べる.
#ifdef _DEBUG
	LoadStateCheck( Name );
#endif

	// 使用していないのがあれば使用する.
	for ( auto& b : pI->m_pBulletList ) {
		if ( b->GetIsEnd() == false ) continue;

		// 弾の発射.
		b = CreateBullet( pI->m_StateList[Name].BulletTag );
		b->DropShot( Pos, pI->m_StateList[Name] );
		return;
	}

	// 新しく作成.
	pI->m_pBulletList.emplace_back( CreateBullet( pI->m_StateList[Name].BulletTag ) );

	// 弾を撃つ.
	pI->m_pBulletList.back()->DropShot( Pos, pI->m_StateList[Name] );
}

//---------------------------.
// 角度にあった方向に弾を発射する( 上が0度 ).
//---------------------------.
void CBulletManager::DirectionShot( const D3DXPOSITION3& Pos, const float Ang, const std::string& Name )
{
	CBulletManager* pI = GetInstance();

	// 弾の情報が読み込まれているか調べる.
#ifdef _DEBUG
	LoadStateCheck( Name );
#endif

	// 使用していないのがあれば使用する.
	for ( auto& b : pI->m_pBulletList ) {
		if ( b->GetIsEnd() == false ) continue;

		// 弾の発射.
		b = CreateBullet( pI->m_StateList[Name].BulletTag );
		b->DirectionShot( Pos, Ang, pI->m_StateList[Name] );
		return;
	}

	// 新しく作成.
	pI->m_pBulletList.emplace_back( CreateBullet( pI->m_StateList[Name].BulletTag ) );

	// 弾を撃つ.
	pI->m_pBulletList.back()->DirectionShot( Pos, Ang, pI->m_StateList[Name] );
}

//---------------------------.
// 全方向に弾を発射.
//---------------------------.
void CBulletManager::AllDirectionShot( const D3DXPOSITION3& Pos, const int Num, const std::string& Name )
{
	for ( int i = 0; i < Num; ++i ) {
		const float Ang = ( Math::DEGREE_MAX / Num ) * i;
		DirectionShot( Pos, Ang, Name );
	}
}

//---------------------------.
// 弾の変更.
//---------------------------.
void CBulletManager::BulletChange( CBulletBase* pOldBullet, const std::string& Name, const bool IsEffectPlay )
{
	CBulletManager* pI = GetInstance();

	// 弾の情報が読み込まれているか調べる.
#ifdef _DEBUG
	LoadStateCheck( Name );
#endif

	// 変更前の弾の情報の取得.
	const D3DXPOSITION3 Pos			= pOldBullet->GetPosition();
	const bool			IsRotMove	= pOldBullet->GetIsRotationMove();

	// 弾の削除.
	pOldBullet->Delete( EObjectTag::None, true, IsEffectPlay );

	// 使用していないのがあれば使用する.
	for ( auto& b : pI->m_pBulletList ) {
		if ( b->GetIsEnd() == false ) continue;

		// 弾の発射.
		b = CreateBullet( pI->m_StateList[Name].BulletTag );
		if ( IsRotMove ) {
			b->StageRotationShot( Pos, pI->m_StateList[Name] );
			return;
		}
		b->DropShot( Pos, pI->m_StateList[Name] );
		return;
	}

	// 新しく作成.
	pI->m_pBulletList.emplace_back( CreateBullet( pI->m_StateList[Name].BulletTag ) );

	// 弾を撃つ.
	if ( IsRotMove ) {
		pI->m_pBulletList.back()->StageRotationShot( Pos, pI->m_StateList[Name] );
		return;
	}
	pI->m_pBulletList.back()->DropShot( Pos, pI->m_StateList[Name] );
}

//---------------------------.
// 弾の数を取得.
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
// 弾の情報の読み込み.
//---------------------------.
HRESULT CBulletManager::StateLood()
{
	CBulletManager* pI = GetInstance();

	auto StateLoad = [&]( const std::filesystem::directory_entry& Entry )
	{
		const std::string Extension = Entry.path().extension().string();	// 拡張子.
		const std::string FilePath	= Entry.path().string();				// ファイルパス.
		const std::string FileName	= Entry.path().stem().string();			// ファイル名.

		// 拡張子が ".json" ではない場合読み込まない.
		if ( Extension != ".json" && Extension != ".JSON" ) return;

		// jsonファイルを読み込む.
		json j = FileManager::JsonLoad( FilePath );

		// 弾の情報をリストに格納する.
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

	CLog::PushLog( "------ エフェクト読み込み開始 ------" );
	try {
		std::filesystem::recursive_directory_iterator Dir_itr( STATE_FILE_PATH ), End_itr;
		std::for_each( Dir_itr, End_itr, StateLoad );
	}
	catch ( const std::filesystem::filesystem_error& e ) {
		// エラーメッセージを表示.
		ErrorMessage( e.path1().string().c_str() );
		return E_FAIL;
	}
	CLog::PushLog( "------ エフェクト読み込み終了 ------" );

	// 読み込み終了.
	return S_OK;
}

//---------------------------.
// タグの取得.
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
// 弾の情報が読み込まれているか調べる.
//---------------------------.
void CBulletManager::LoadStateCheck( const std::string& Name )
{
	CBulletManager* pI = GetInstance();

	// 調べる.
	bool IsLoad = false;
	for ( auto& [Key, State] : pI->m_StateList ) {
		if ( Key != Name ) continue;
		IsLoad = true;
		break;
	}

	// 読み込まれている.
	if ( IsLoad ) return;

	// 読み込まれていない.
	ErrorMessage( "「" + Name + "」の弾の情報がありません" );
}

//---------------------------.
// 弾の作成.
//---------------------------.
std::shared_ptr<CBulletBase> CBulletManager::CreateBullet( const EObjectTag Tag )
{
	// タグに対応したインスタンス作成.
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

	// 初期化しておく.
	Out->Init();
	return Out;
}

//---------------------------.
// 描画リストの更新.
//---------------------------.
void CBulletManager::RenderListUpdate()
{
	CBulletManager* pI = GetInstance();

	pI->m_pRenderList.clear();

	// Y座標とポインタを取得.
	for ( auto& b : pI->m_pBulletList ) {
		pI->m_pRenderList.emplace_back( std::make_pair( b->GetPosition().y, b.get() ) );
	}
}

//---------------------------.
// 同じ弾通し重なっている場合削除する.
//---------------------------.
void CBulletManager::SameBulletDelete()
{
	CBulletManager* pI = GetInstance();

	// 同じ状態の弾通し重なっている場合削除する.
	for ( auto& a : pI->m_pBulletList ) {
		if ( a->GetIsDisp() == false ) continue;
		for ( auto& b : pI->m_pBulletList ) {
			if ( a == b											) continue;
			if ( b->GetIsDisp()			== false				) continue;
			if ( b->GetIsRotationMove() == false				) continue;
			if ( a->GetBulletState()	!= b->GetBulletState()	) continue;
	
			// 弾通しの距離を求める.
			const D3DXVECTOR3&	Vector	 = a->GetPosition() - b->GetPosition();
			const float			Distance = D3DXVec3Length( &Vector );
			if ( Distance <= pI->CONSTANT.SAME_BULLET_DELETE_DISTANCE ) b->Delete( EObjectTag::None, false, false );
		}
	}
}

//---------------------------.
// 使用していない弾の削除.
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
// タイミングバーに情報を渡す.
//---------------------------.
void CBulletManager::PushTimingBar()
{
	CBulletManager* pI = GetInstance();

	// 普通の弾を渡す.
	for ( auto& b : pI->m_pBulletList ) {
		if ( b->GetIsDisp() == false ) continue;
		if ( b->GetIsCollisionStop() ) continue;

		// プレイヤーとの距離を求める.
		const D3DXVECTOR3&	Vector	 = CConstant::GetPlayer().POSITION - b->GetPosition();
		const float			Distance = D3DXVec3Length( &Vector );
		const bool			IsStrong = b->GetObjectTag() == EObjectTag::SpecialBullet;
		const bool			IsItem	 = b->GetObjectTag() == EObjectTag::ItemBullet;
		if ( Distance > pI->CONSTANT.TIMINGBAR_DISTANCE ) continue;
		
		// プレイヤーの方向に飛んでいるか.
		D3DXVECTOR3	DireVector = Vector;
		D3DXVec3Normalize( &DireVector, &DireVector );
		const D3DXVECTOR3&	MoveVector	= b->GetMoveVecotr();
		const D3DXVECTOR3&	XVector		= { DireVector.x, INIT_FLOAT, INIT_FLOAT };
		const D3DXVECTOR3&	YVector		= { INIT_FLOAT, DireVector.y, INIT_FLOAT };
		const D3DXVECTOR3&	AddXVector	= { DireVector.x + MoveVector.x, INIT_FLOAT, INIT_FLOAT };
		const D3DXVECTOR3&	AddYVector	= { INIT_FLOAT, DireVector.y + MoveVector.y, INIT_FLOAT };
		const bool			IsXMove		= D3DXVec3Length( &AddXVector ) >= D3DXVec3Length( &XVector );
		const bool			IsYMove		= D3DXVec3Length( &AddYVector ) >= D3DXVec3Length( &YVector );

		// タイミングバーにアイコンを表示する.
		if ( IsXMove == false || IsYMove == false ) continue;
		CTimingBarUI::PushIcon( Distance, IsStrong, IsItem );
	}
}
