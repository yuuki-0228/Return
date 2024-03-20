#pragma once
#include "..\GameObject.h"
#include "..\..\..\Common\Sprite\Sprite.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"

namespace {
	// IðñÌ.
	enum class enChoice : unsigned char
	{
		StartChoice,	// ÅÌIð.
		EndChoice,		// ÅãÌIð.

		Max,			// Ååvf.
	} typedef EChoice;

	// gkÇñÌ.
	enum class enScaling : unsigned char
	{
		None,			// Ò@.
		Expand,			// gå.
		Shrink,			// k¬.
	} typedef EScaling;
}

/************************************************
*	Q[àÌUIx[XNX.
*		ú±cFP.
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
	const float m_Wnd_W;		// EBhEÌ.
	const float m_Wnd_H;		// EBhEÌ³.
};
