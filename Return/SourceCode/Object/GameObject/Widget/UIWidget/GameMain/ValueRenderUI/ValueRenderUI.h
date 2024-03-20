#pragma once
#include "..\..\..\..\..\..\Common\Font\Font.h"

// 値の種類.
enum class enValueType : unsigned char {
	Heal,
	Damage
} typedef EValueType;

/************************************************
*	ダメージや回復数の表示のUIクラス.
**/
class CValueRenderUI final
{
public:
	CValueRenderUI();
	~CValueRenderUI();

	// インスタンスの取得.
	static CValueRenderUI* GetInstance();

	// 初期化.
	static bool Init();

	// 更新.
	static void Update( const float& DeltaTime );

	// 描画.
	static void Render();

	// 値UIの追加.
	static void PushValue( const D3DXPOSITION3 Pos, const int Value, const EValueType Type );

private:
	// 値UIの更新.
	static void ValueUpdate( const float& DeltaTime );
	// 使用していない値UIの削除.
	static void ValueDelete();

private:
	CFont*							m_pFont;			// フォント.
	SFontRenderState				m_FontStateBase;	// フォントの情報ベース.
	std::vector<SFontRenderState>	m_FontStateList;	// フォントの情報リスト.
};