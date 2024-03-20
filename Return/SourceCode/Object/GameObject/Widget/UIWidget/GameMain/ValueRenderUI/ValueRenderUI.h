#pragma once
#include "..\..\..\..\..\..\Common\Font\Font.h"

// �l�̎��.
enum class enValueType : unsigned char {
	Heal,
	Damage
} typedef EValueType;

/************************************************
*	�_���[�W��񕜐��̕\����UI�N���X.
**/
class CValueRenderUI final
{
public:
	CValueRenderUI();
	~CValueRenderUI();

	// �C���X�^���X�̎擾.
	static CValueRenderUI* GetInstance();

	// ������.
	static bool Init();

	// �X�V.
	static void Update( const float& DeltaTime );

	// �`��.
	static void Render();

	// �lUI�̒ǉ�.
	static void PushValue( const D3DXPOSITION3 Pos, const int Value, const EValueType Type );

private:
	// �lUI�̍X�V.
	static void ValueUpdate( const float& DeltaTime );
	// �g�p���Ă��Ȃ��lUI�̍폜.
	static void ValueDelete();

private:
	CFont*							m_pFont;			// �t�H���g.
	SFontRenderState				m_FontStateBase;	// �t�H���g�̏��x�[�X.
	std::vector<SFontRenderState>	m_FontStateList;	// �t�H���g�̏�񃊃X�g.
};