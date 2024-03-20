#pragma once
#include "..\..\..\Global.h"

/************************************************
*	���b�Z�[�W�E�B���h�E�N���X.
*		���c�F�P.
**/
class CDebugFlagWindow final
{
public:
	CDebugFlagWindow();
	~CDebugFlagWindow();

	// �C���X�^���X�̎擾.
	static CDebugFlagWindow* GetInstance();

	// �`��.
	static void Render();

private:
	static void FlagFind();			// �t���O��������.

private:
	std::string	m_FindString;			// �t���O�E�B���h�E�̌������镶��.
	CBool		m_IsDispNameNoneFlag;	// �����̃t���O��\�������邩.

private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CDebugFlagWindow( const CDebugFlagWindow & )				= delete;
	CDebugFlagWindow& operator = ( const CDebugFlagWindow & )	= delete;
	CDebugFlagWindow( CDebugFlagWindow && )						= delete;
	CDebugFlagWindow& operator = ( CDebugFlagWindow && )		= delete;
};
