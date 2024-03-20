#pragma once
#include "..\..\Global.h"

/************************************************
*	DirectX9 �Z�b�g�A�b�v.
**/
class CDirectX9 final
{
public:
	CDirectX9();
	~CDirectX9();

	// �C���X�^���X�̎擾.
	static CDirectX9* GetInstance();

	// DirectX9�\�z.
	static HRESULT Create( HWND hWnd );
	// DirectX9���.
	static void Release();

	// �f�o�C�X�I�u�W�F�N�g���擾.
	static LPDIRECT3DDEVICE9 GetDevice() { return GetInstance()->m_pDevice9; }

private:
	LPDIRECT3DDEVICE9	m_pDevice9;	// �f�o�C�X�I�u�W�F�N�g.

private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CDirectX9( const CDirectX9 & ) = delete;
	CDirectX9& operator = ( const CDirectX9 & ) = delete;
	CDirectX9( CDirectX9 && ) = delete;
	CDirectX9& operator = ( CDirectX9 && ) = delete;
};