#pragma once
#include "..\..\..\..\..\..\Common\Sprite\Sprite.h"
#include "..\..\..\..\..\..\Utility\Constant\Constant.h"

/************************************************
*	タイミングバーのUIクラス.
*		﨑田友輝.
**/
class CTimingBarUI final
{
public:
	using RenderStateList		= std::vector<SSpriteRenderState>;
	using DeleteIconStateList	= std::vector<std::pair<SSpriteRenderState, int>>;

public:
	CTimingBarUI();
	virtual ~CTimingBarUI();

	// インスタンスの取得.
	static CTimingBarUI* GetInstance();

	// 初期化.
	static bool Init();

	// 更新.
	static void Update( const float& DeltaTime );

	// 描画.
	static void Render();

	// 移動するアイコンの追加.
	static void PushIcon( const float Distance, const bool IsStrong = false, const bool IsItem = false );
	// 削除アイコンの追加.
	static void PushDeleteIcon( const int TypeNo, const float Distance );

private:
	// 削除アイコンの作成.
	static void CreateDeleteIcon( const int TypeNo, const float x );

protected:
	CSprite*				m_pBarSprite;			// タイミングバーの画像.
	CSprite*				m_pMoveIconSprite;		// 移動するアイコンの画像.
	std::vector<CSprite*>	m_pDeleteIconList;		// 削除アイコン.
	SSpriteRenderState		m_BarState;				// タイミングバーの情報.
	SSpriteRenderState		m_MoveIconStateBase;	// 移動するアイコンの情報ベース.
	SSpriteRenderState		m_DeleteIconStateBase;	// 削除アイコンの情報ベース.
	DeleteIconStateList		m_DeleteIconStateList;	// 削除アイコンの情報.
	RenderStateList			m_LeftMoveIconState;	// 移動するアイコンの情報.
	RenderStateList			m_RightMoveIconState;	// 移動するアイコンの情報.

private:
	// 外部データの定数.
	const ConstantStruct::SPlayer& CONSTANT = CConstant::GetPlayer();
};
