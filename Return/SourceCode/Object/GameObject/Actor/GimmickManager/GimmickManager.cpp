#include "GimmickManager.h"
#include "GimmickBase\CrackGimmick\CrackGimmick.h"

namespace {
	constexpr char STATE_FILE_PATH[] = "Data\\Parameter\\Gimmick";
}

CGimmickManager::CGimmickManager()
	: m_pGimmickList	()
	, m_pRenderList		()
	, m_StateList		()
{
}

CGimmickManager::~CGimmickManager()
{
}

//----------------------------.
// インスタンスの取得.
//----------------------------.
CGimmickManager* CGimmickManager::GetInstance()
{
	static std::unique_ptr<CGimmickManager> pInstance = std::make_unique<CGimmickManager>();
	return pInstance.get();
}

//---------------------------.
// 初期化.
//---------------------------.
bool CGimmickManager::Init()
{
	CGimmickManager* pI = GetInstance();

	pI->m_pGimmickList.clear();
	pI->m_pRenderList.clear();

	// 敵の情報を読み込む.
	StateLood();

	// デバックで一つ配置.
	D3DXPOSITION3 SpownPos = INIT_FLOAT3;
	SpownPos.x = FWND_W / 2.0f - 100.0f;
	SpownPos.y = 128.0f;
	Arrangement( SpownPos, "Barrel" );
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CGimmickManager::Update( const float& DeltaTime )
{
	CGimmickManager* pI = GetInstance();

	// ステージギミックの更新.
	for ( auto& g : pI->m_pGimmickList ) g->Update( DeltaTime );
}

//---------------------------.
// 描画.
//---------------------------.
void CGimmickManager::Render()
{
}

//---------------------------.
// 描画リストの取得.
//---------------------------.
std::vector<std::pair<float, CActor*>> CGimmickManager::GetRenderList()
{
	// 描画順番の更新.
	RenderListUpdate();
	return GetInstance()->m_pRenderList;
}

//---------------------------.
// ギミックを配置する.
//---------------------------.
void CGimmickManager::Arrangement( const D3DXPOSITION3& Pos, const std::string& Name )
{
	CGimmickManager* pI = GetInstance();

	// 弾の情報が読み込まれているか調べる.
#ifdef _DEBUG
	LoadStateCheck( Name );
#endif
	// 新しく作成.
	pI->m_pGimmickList.emplace_back( CreateGimmick( pI->m_StateList[Name].GimmickName ) );

	// 敵をスポーンさせる.
	pI->m_pGimmickList.back()->Arrangement( Pos, pI->m_StateList[Name] );
}

//---------------------------.
// 弾の情報の読み込み.
//---------------------------.
HRESULT CGimmickManager::StateLood()
{
	CGimmickManager* pI = GetInstance();

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
		pI->m_StateList[FileName].ActionInit( j );
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
// 弾の情報があるか調べる.
//---------------------------.
void CGimmickManager::LoadStateCheck( const std::string& Name )
{
	CGimmickManager* pI = GetInstance();

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
// ギミックの作成.
//---------------------------.
std::shared_ptr<CGimmickBase> CGimmickManager::CreateGimmick( const std::string Type )
{
	// タグに対応したインスタンス作成.
	std::shared_ptr<CGimmickBase> Out;
	if (		Type == "Crack"	) Out = std::make_shared<CCrackGimmick>();
	else return nullptr;

	// 初期化しておく.
	Out->Init();
	return Out;
}

//---------------------------.
// 描画リストの更新.
//---------------------------.
void CGimmickManager::RenderListUpdate()
{
	CGimmickManager* pI = GetInstance();

	pI->m_pRenderList.clear();

	// Y座標とポインタを取得.
	for ( auto& g : pI->m_pGimmickList ) {
		pI->m_pRenderList.emplace_back( std::make_pair( g->GetPosition().y, g.get() ) );
	}
}

