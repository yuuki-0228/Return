#pragma once
#include "..\GameObject.h"
#include "..\..\..\Common\Sprite\Sprite.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"

namespace {
	// 選択肢列挙体.
	enum class enChoice : unsigned char
	{
		StartChoice,	// 最初の選択肢.
		EndChoice,		// 最後の選択肢.

		Max,			// 最大要素数.
	} typedef EChoice;

	// 拡縮管理列挙体.
	enum class enScaling : unsigned char
	{
		None,			// 待機中.
		Expand,			// 拡大中.
		Shrink,			// 縮小中.
	} typedef EScaling;
}

/************************************************
*	ゲーム内のUIベースクラス.
*		﨑田友輝.
**/
class CWidget
	: public CGameObject
{
public:
	using Sprite_map	= std::unordered_map<std::string, std::vector<CSprite*>>;
	using State_map		= std::unordered_map<std::string, std::vector<SSpriteRenderState>>;

public:
	CWidget();
	virtual ~CWidget();

protected:
	const float m_Wnd_W;		// ウィンドウの幅.
	const float m_Wnd_H;		// ウィンドウの高さ.
};
