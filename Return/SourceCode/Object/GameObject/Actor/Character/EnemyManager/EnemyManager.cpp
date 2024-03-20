#include "EnemyManager.h"
#include "Enemy\Enemy.h"

namespace {
	constexpr char STATE_FILE_PATH[] = "Data\\Parameter\\Enemy";
}

CEnemyManager::CEnemyManager()
	: m_pEnemyList	()
	, m_pRenderList	()
{
}

CEnemyManager::~CEnemyManager()
{
}

//----------------------------.
// インスタンスの取得.
//----------------------------.
CEnemyManager* CEnemyManager::GetInstance()
{
	static std::unique_ptr<CEnemyManager> pInstance = std::make_unique<CEnemyManager>();
	return pInstance.get();
}

//---------------------------.
// 初期化.
//---------------------------.
bool CEnemyManager::Init()
{
	CEnemyManager* pI = GetInstance();

	pI->m_pEnemyList.clear();
	pI->m_pRenderList.clear();

	// 敵の情報を読み込む.
	StateLood();

	// デバックで一体召喚.
	D3DXPOSITION3 SpownPos = INIT_FLOAT3;
	SpownPos.x = FWND_W / 2.0f;
	SpownPos.y = FWND_H / 2.0f;
	Spown( SpownPos, "Normal" );
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CEnemyManager::Update( const float& DeltaTime )
{
	CEnemyManager* pI = GetInstance();

	// 敵の更新.
	for( auto& e : pI->m_pEnemyList ) e->Update( DeltaTime );
	EnemyDelete();	// 使用していない敵の削除.
}

//---------------------------.
// 描画.
//---------------------------.
void CEnemyManager::Render()
{
}

//---------------------------.
// 描画リストの取得.
//---------------------------.
std::vector<std::pair<float, CActor*>> CEnemyManager::GetRenderList()
{
	// 描画順番の更新.
	RenderListUpdate();
	return GetInstance()->m_pRenderList;
}

//---------------------------.
// スポーンさせる.
//---------------------------.
void CEnemyManager::Spown( const D3DXPOSITION3& Pos, const std::string& Name )
{
	CEnemyManager* pI = GetInstance();

	// 弾の情報が読み込まれているか調べる.
#ifdef _DEBUG
	LoadStateCheck( Name );
#endif

	// 使用していないのがあれば使用する.
	for ( auto& b : pI->m_pEnemyList ) {
		if ( b->GetIsDisp() ) continue;

		// 敵をスポーンさせる.
		b->Spown( Pos, pI->m_StateList[Name] );
		return;
	}

	// 新しく作成.
	pI->m_pEnemyList.emplace_back( std::make_shared<CEnemy>() );

	// 敵をスポーンさせる.
	pI->m_pEnemyList.back()->Spown( Pos, pI->m_StateList[Name] );
}

//---------------------------.
// 弾の情報の読み込み.
//---------------------------.
HRESULT CEnemyManager::StateLood()
{
	CEnemyManager* pI = GetInstance();

	auto StateLoad = [&]( const std::filesystem::directory_entry& Entry )
	{
		const std::string Extension = Entry.path().extension().string();	// 拡張子.
		const std::string FilePath	= Entry.path().string();				// ファイルパス.
		const std::string FileName	= Entry.path().stem().string();			// ファイル名.

		// 拡張子が ".json" ではない場合読み込まない.
		if ( Extension != ".json" && Extension != ".JSON" ) return;

		// jsonファイルを読み込む.
		json j = FileManager::JsonLoad( FilePath );

		// 攻撃リストの作成.
		std::vector<SEnemyActionState> ActionList;
		const int ActionSize = static_cast<int>( j["Action"].size() );
		ActionList.resize( ActionSize );
		for ( int i = 0; i < ActionSize; ++i ) {
			ActionList[i].ActionInit( j["Action"][i] );
		}

		// 弾の情報をリストに格納する.
		pI->m_StateList[FileName] = SEnemyState(
			j["TypeName"],
			j["HP"],
			ActionList
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
// 弾の情報があるか調べる.
//---------------------------.
void CEnemyManager::LoadStateCheck( const std::string& Name )
{
	CEnemyManager* pI = GetInstance();

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
// 描画リストの更新.
//---------------------------.
void CEnemyManager::RenderListUpdate()
{
	CEnemyManager* pI = GetInstance();

	pI->m_pRenderList.clear();

	// Y座標とポインタを取得.
	for ( auto& e : pI->m_pEnemyList ) {
		pI->m_pRenderList.emplace_back( std::make_pair( e->GetPosition().y, e.get() ) );
	}
}

//---------------------------.
// 使用していない敵の削除.
//---------------------------.
void CEnemyManager::EnemyDelete()
{
	CEnemyManager* pI = GetInstance();

	// 普通の弾の削除.
	const int NormalMax = static_cast<int>( pI->m_pEnemyList.size() ) - 1;
	for ( int i = NormalMax; i >= 0; --i ) {
		if ( pI->m_pEnemyList[i]->GetIsDisp() ) break;
		pI->m_pEnemyList.pop_back();
	}
}