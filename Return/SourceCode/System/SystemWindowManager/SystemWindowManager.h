#pragma once
#include "..\..\Global.h"

class CInputSetting;
class CWindowSetting;
class CBugReport;
class CSprite;

/************************************************
*	�V�X�e���ݒ������E�B���h�E�}�l�[�W���[.
*		���c�F�P.
**/
class CSystemWindowManager final
{
public:
	CSystemWindowManager();
	~CSystemWindowManager();

	// �C���X�^���X�̎擾.
	static CSystemWindowManager* GetInstance();

	// �X�V.
	static void Update( const float& DeltaTime );
	// �`��.
	static void Render();

private:
	std::unique_ptr<CInputSetting>	m_pInputSetting;	// ���͐ݒ���s���E�B���h�E.
	std::unique_ptr<CWindowSetting> m_pWindowSetting;	// �E�B���h�E�ݒ���s���E�B���h�E.
	std::unique_ptr<CBugReport>		m_pBugReport;		// �o�O�񍐂��s���E�B���h�E.
	CSprite*						m_pButton;			// �{�^��.

private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CSystemWindowManager( const CSystemWindowManager & )				= delete;
	CSystemWindowManager& operator = ( const CSystemWindowManager & )	= delete;
	CSystemWindowManager( CSystemWindowManager && )						= delete;
	CSystemWindowManager& operator = ( CSystemWindowManager && )		= delete;
};
