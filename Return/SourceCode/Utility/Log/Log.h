#pragma once
#include <Windows.h>
#include <memory>
#include <string>
#include <time.h>

/************************************************
*	���O�N���X.
*		���c�F�P.
**/
class CLog final
{
public:
	CLog();
	~CLog();

	// �C���X�^���X�̎擾.
	static CLog* GetInstance();

	// ���O�e�L�X�g���쐬.
	//	�A�v���N�����ɍ쐬����.
	static HRESULT OpenLogText();

	// ���O�e�L�X�g�����.
	static HRESULT CloseLogText();

	// ���O�t�@�C���̔j��.
	static HRESULT DeleteLogText();

	// ���O�̓���.
	static HRESULT PushLog( const char*			Log );
	static HRESULT PushLog( const std::string&	Log );

private:
	bool m_Stop;	// ���O���~���邩.

private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CLog( const CLog& )					= delete;
	CLog& operator = ( const CLog& )	= delete;
	CLog( CLog&& )						= delete;
	CLog& operator = ( CLog&& )			= delete;
};
