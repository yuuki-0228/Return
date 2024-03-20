#include "PotionAreaManager.h"

namespace {
	constexpr char STATE_FILE_PATH[] = "Data\\Parameter\\PotionArea";
}

CPotionAreaManager::CPotionAreaManager()
	: m_pAreaList	()
	, m_pRenderList	()
	, m_StateList	()
{
}

CPotionAreaManager::~CPotionAreaManager()
{
}

//---------------------------.
// インスタンスの取得.
//---------------------------.
CPotionAreaManager* CPotionAreaManager::GetInstance()
{
	static std::unique_ptr<CPotionAreaManager> pInstance = std::make_unique<CPotionAreaManager>();
	return pInstance.get();
}

//---------------------------.
// 初期化.
//---------------------------.
bool CPotionAreaManager::Init()
{
	CPotionAreaManager* pI = GetInstance();

	pI->m_pAreaList.clear();
	pI->m_pRenderList.clear();

	// 弾の情報を読み込む.
	StateLood();
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CPotionAreaManager::Update( const float& DeltaTime )
{
	CPotionAreaManager* pI = GetInstance();

	for ( int i = 0; i < pI->m_pAreaList.size(); ++i ) {
		pI->m_pAreaList[i]->Update( DeltaTime );
	}

	AreaDelete();	// 使用していない弾の削除.
}

//---------------------------.
// 当たり判定終了後呼び出される更新.
//---------------------------.
void CPotionAreaManager::LateUpdate( const float& DeltaTime )
{
	CPotionAreaManager* pI = GetInstance();

	for ( int i = 0; i < pI->m_pAreaList.size(); ++i ) {
		pI->m_pAreaList[i]->LateUpdate( DeltaTime );
	}
}

//---------------------------.
// 描画.
//---------------------------.
void CPotionAreaManager::Render()
{
}

//---------------------------.
// 描画リストの取得.
//---------------------------.
std::vector<std::pair<float, CActor*>> CPotionAreaManager::GetRenderList()
{
	// 描画順番の更新.
	RenderListUpdate();
	return GetInstance()->m_pRenderList;
}

//---------------------------.
// エリアの追加.
//---------------------------.
void CPotionAreaManager::PushArea( const D3DXPOSITION3& Pos, const float Radius, const std::string& Name, const float Time )
{
	CPotionAreaManager* pI = GetInstance();

	// 弾の情報が読み込まれているか調べる.
#ifdef _DEBUG
	LoadStateCheck( Name );
#endif

	// エリアの情報の更新.
	SPotionAreaState State = pI->m_StateList[Name];
	if ( Time > INIT_FLOAT ) {
		State.DeleteTime = Time;
	}

	// 使用していないのがあれば使用する.
	for ( auto& b : pI->m_pAreaList ) {
		if ( b->GetIsEnd() == false ) continue;

		// ポーションエリアの作成.
		b->CreateArea( Pos, Radius, State );
		return;
	}

	// 新しく作成.
	pI->m_pAreaList.emplace_back( std::make_shared<CPotionArea>() );

	// ポーションエリアの作成.
	pI->m_pAreaList.back()->CreateArea( Pos, Radius, State );
}

//---------------------------.
// 弾の情報の読み込み.
//---------------------------.
HRESULT CPotionAreaManager::StateLood()
{
	CPotionAreaManager* pI = GetInstance();

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
		const D3DXCOLOR4&	Color	= { j["EffectColor"]["R"], j["EffectColor"]["G"], j["EffectColor"]["B"], 1.0f };
		const int			UseNum	= j["UseNum"].empty() ? 0 : static_cast<int>( j["UseNum"] );
		pI->m_StateList[FileName] = SPotionAreaState(
			GetPotionType( j["PotionType"] ),
			UseNum,
			j["DeleteTime"],
			j["EffectTime"],
			j["EffectValue"],
			Color
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
void CPotionAreaManager::LoadStateCheck( const std::string& Name )
{
	CPotionAreaManager* pI = GetInstance();

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
// ポーションのタイプを取得.
//---------------------------.
int CPotionAreaManager::GetPotionType( const std::string& Type )
{
	if ( Type == "Heal"		) return PotionNo::Heal;
	if ( Type == "Poison"	) return PotionNo::Poison;
	if ( Type == "Rest"		) return PotionNo::Rest;
	return PotionNo::None;
}

//---------------------------.
// 描画リストの更新.
//---------------------------.
void CPotionAreaManager::RenderListUpdate()
{
	CPotionAreaManager* pI = GetInstance();

	pI->m_pRenderList.clear();

	// Y座標とポインタを取得.
	for ( auto& b : pI->m_pAreaList ) {
		pI->m_pRenderList.emplace_back( std::make_pair( b->GetPosition().y, b.get() ) );
	}
}

//---------------------------.
// 使用していない弾の削除.
//---------------------------.
void CPotionAreaManager::AreaDelete()
{
	CPotionAreaManager* pI = GetInstance();

	const int NormalMax = static_cast<int>( pI->m_pAreaList.size() ) - 1;
	for ( int i = NormalMax; i >= 0; --i ) {
		if ( pI->m_pAreaList[i]->GetIsEnd() == false ) break;
		pI->m_pAreaList.pop_back();
	}
}
