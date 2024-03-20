#include "FadeManager.h"
#include "FadeBase\NormalFade\NormalFade.h"
#include "FadeBase\UFOFade\UFOFade.h"

CFadeManager::CFadeManager()
	: m_NowFadeType	( EFadeType::None )
	, m_pNormalFade	( nullptr )
	, m_pUFOFade	( nullptr )
{
	m_pNormalFade	= std::make_unique<CNormalFade>();
	m_pUFOFade		= std::make_unique<CUFOFade>();
}

CFadeManager::~CFadeManager()
{
}

//---------------------------.
// インスタンスの取得.
//---------------------------.
CFadeManager* CFadeManager::GetInstance()
{
	static std::unique_ptr<CFadeManager> pInstance = std::make_unique<CFadeManager>();
	return pInstance.get();
}

//---------------------------.
// 初期化.
//---------------------------.
bool CFadeManager::Init()
{
	CFadeManager* pI = GetInstance();

	if ( pI->m_pNormalFade->Init()	== false ) return false;
	if ( pI->m_pUFOFade->Init()		== false ) return false;
	return true;
}

//---------------------------.
// 描画.
//---------------------------.
void CFadeManager::Render()
{
	CFadeManager* pI = GetInstance();

	if ( pI->m_NowFadeType == EFadeType::None ) return;

	// タイプに応じたフェードの描画.
	switch ( pI->m_NowFadeType ) {
	case EFadeType::NormalFade:
		pI->m_pNormalFade->Render();
		break;
	case EFadeType::UFOFade:
		pI->m_pUFOFade->Render();
		break;
	default:
		break;
	}
}

//---------------------------.
// フェードイン.
//---------------------------.
bool CFadeManager::FadeIn( const EFadeType Type, const float Speed )
{
	CFadeManager* pI = GetInstance();

	// フェードタイプを保存.
	pI->m_NowFadeType = Type;

	// タイプに応じたフェードインを行う.
	switch ( Type ) {
	case EFadeType::NormalFade:
		return pI->m_pNormalFade->FadeIn( Speed );
	case EFadeType::UFOFade:
		return pI->m_pUFOFade->FadeIn( Speed );
	default:
		return true;
	}
}

//---------------------------.
// フェードアウト.
//---------------------------.
bool CFadeManager::FadeOut( const EFadeType Type, const float Speed )
{
	CFadeManager* pI = GetInstance();

	// フェードタイプを保存.
	pI->m_NowFadeType = Type;

	// タイプに応じたフェードアウトを行う.
	bool IsEnd = false;
	switch ( Type ) {
	case EFadeType::NormalFade:
		IsEnd = pI->m_pNormalFade->FadeOut( Speed );
		break;
	case EFadeType::UFOFade:
		IsEnd = pI->m_pUFOFade->FadeOut( Speed );
		break;
	default:
		IsEnd = true;
		break;
	}

	// フェードが終了したか.
	if ( IsEnd == false ) return false;
	pI->m_NowFadeType = EFadeType::None;
	return true;
}