#pragma once
#include "..\..\..\Global.h"

/************************************************
*	�h���b�N���h���b�v�N���X.
*		���c�F�P.
**/
class CDragAndDrop final
{
public:
	CDragAndDrop();
	~CDragAndDrop();

	// �C���X�^���X�̎擾.
	static CDragAndDrop* GetInstance();

	// �h���b�N&�h���b�v���g�p����.
	static void Open();
	// �h���b�N&�h���b�v���g�p���Ȃ�.
	static void Close();
	
	// �t�@�C�����h���b�v���ꂽ��.
	static bool GetIsDrop() { return !GetInstance()->m_FilePathList.empty(); }
	// �t�@�C���p�X���擾.
	static std::vector<std::string> GetFilePath() { return GetInstance()->m_FilePathList; }

	// �t�@�C���p�X��ǉ�.
	static void AddFilePath( const std::string& FilePath ) { GetInstance()->m_FilePathList.emplace_back( FilePath ); }

	// �E�B���h�E�n���h����ݒ�.
	static void SethWnd( const HWND& hWnd ) { GetInstance()->m_hWnd = hWnd; }

private:
	HWND						m_hWnd;			// �E�B���h�E�n���h��.
	std::vector<std::string>	m_FilePathList;	// �t�@�C���p�X���X�g.

private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CDragAndDrop( const CDragAndDrop& )					= delete;
	CDragAndDrop( CDragAndDrop&& )						= delete;
	CDragAndDrop& operator = ( const CDragAndDrop& )	= delete;
	CDragAndDrop& operator = ( CDragAndDrop&& )			= delete;
};