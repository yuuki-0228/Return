#include "ValueRenderUI.h"
#include "..\..\..\..\..\..\Resource\FontResource\FontResource.h"

CValueRenderUI::CValueRenderUI()
	: m_pFont			( nullptr )
	, m_FontStateBase	()
	, m_FontStateList	()
{
}
CValueRenderUI::~CValueRenderUI()
{
}

//---------------------------.
// インスタンスの取得.
//---------------------------.
CValueRenderUI* CValueRenderUI::GetInstance()
{
	static std::unique_ptr<CValueRenderUI> pInstance = std::make_unique<CValueRenderUI>();
	return pInstance.get();
}

//---------------------------.
// 初期化.
//---------------------------.
bool CValueRenderUI::Init()
{
	CValueRenderUI* pI = GetInstance();
	
	// フォントの取得.
	pI->m_pFont = CFontResource::GetFont( "コーポレート・ロゴ ver2 Medium", &pI->m_FontStateBase );

	// フォントの情報ベースの作成.
	pI->m_FontStateBase.TextAlign		= ETextAlign::Center;
	pI->m_FontStateBase.IsOutLine		= true;
	pI->m_FontStateBase.OutLineColor	= Color4::DarkGray;
	pI->m_FontStateBase.OutLineSize		= 2.0f;
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CValueRenderUI::Update( const float& DeltaTime )
{
	CValueRenderUI* pI = GetInstance();

	ValueUpdate( DeltaTime );	// 値UIの更新.
	ValueDelete();				// 使用していない値UIの削除.
}

//---------------------------.
// 描画.
//---------------------------.
void CValueRenderUI::Render()
{
	CValueRenderUI* pI = GetInstance();

	for ( auto& s : pI->m_FontStateList ) {
		pI->m_pFont->RenderUI( &s );
	}
}

//---------------------------.
// 値の追加.
//---------------------------.
void CValueRenderUI::PushValue( const D3DXPOSITION3 Pos, const int Value, const EValueType Type )
{
	CValueRenderUI* pI = GetInstance();

	// 値の取得.
	const int			iDamage = Value;
	const D3DXCOLOR4	Color	= Type == EValueType::Heal ? Color4::Green : Color4::Red;

	// 使用していないのがあれば使用する.
	for ( auto& s : pI->m_FontStateList ) {
		if ( s.IsDisp ) continue;

		// 文字を設定.
		s.Transform.Position	= Pos;
		s.Color					= Color;
		s.Text					= std::to_string( iDamage );
		s.IsDisp				= true;
		return;
	}

	// 新しく作成.
	pI->m_FontStateList.emplace_back( pI->m_FontStateBase );
	pI->m_FontStateList.back().Transform.Position	= Pos;
	pI->m_FontStateList.back().Color				= Color;
	pI->m_FontStateList.back().Text					= std::to_string( iDamage );
}

//---------------------------.
// 値UIの更新.
//---------------------------.
void CValueRenderUI::ValueUpdate( const float& DeltaTime )
{
	CValueRenderUI* pI = GetInstance();

	// 半透明にしながら上にあげる.
	for ( auto& s : pI->m_FontStateList ) {
		if ( s.IsDisp == false ) continue;
		s.Transform.Position.y	-= 16.0f * DeltaTime;
		s.Color.w				-= 0.50f * DeltaTime;

		// 透明になった場合非表示にする.
		if ( s.Color.w > Color::ALPHA_MIN ) continue;
		s.IsDisp = false;
	}
}

//---------------------------.
// 使用していない値UIの削除.
//---------------------------.
void CValueRenderUI::ValueDelete()
{
	CValueRenderUI* pI = GetInstance();

	const int NormalMax = static_cast< int >( pI->m_FontStateList.size() ) - 1;
	for ( int i = NormalMax; i >= 0; --i ) {
		if ( pI->m_FontStateList[i].IsDisp ) break;
		pI->m_FontStateList.pop_back();
	}
}