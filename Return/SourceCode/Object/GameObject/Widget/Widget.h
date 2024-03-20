#pragma once
#include "..\GameObject.h"
#include "..\..\..\Common\Sprite\Sprite.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"

namespace {
	// �I�����񋓑�.
	enum class enChoice : unsigned char
	{
		StartChoice,	// �ŏ��̑I����.
		EndChoice,		// �Ō�̑I����.

		Max,			// �ő�v�f��.
	} typedef EChoice;

	// �g�k�Ǘ��񋓑�.
	enum class enScaling : unsigned char
	{
		None,			// �ҋ@��.
		Expand,			// �g�咆.
		Shrink,			// �k����.
	} typedef EScaling;
}

/************************************************
*	�Q�[������UI�x�[�X�N���X.
*		���c�F�P.
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
	const float m_Wnd_W;		// �E�B���h�E�̕�.
	const float m_Wnd_H;		// �E�B���h�E�̍���.
};
