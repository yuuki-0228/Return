#pragma once
#include "..\..\..\Global.h"
#include "..\..\..\Common\Sprite\Sprite.h"

/************************************************
*	���[�h���̕`��N���X.
*		���c�F�P.
**/
class CLoadRender final
{
public:
	CLoadRender();
	~CLoadRender();

	// �ǂݍ���.
	HRESULT Load();

	// �X�V.
	void Update();
	// �`��.
	void Render();

private:
	std::unique_ptr<CSprite>	m_pSpriteBack;	// �w�i�摜.
	std::unique_ptr<CSprite>	m_pSpriteText;	// "Now Loading"�摜.
	std::unique_ptr<CSprite>	m_pSpriteDot;	// "..."�摜.
	std::unique_ptr<CSprite>	m_pSpriteUFO;	// UFO�摜.
	std::unique_ptr<CSprite>	m_pMask;		// �}�X�N�摜.
	SSpriteRenderState			m_DotState;		// "..."�̏��.
	SHORT						m_DotAnimCnt;	// "..."�̃A�j���[�V�����p�J�E���g.
};
