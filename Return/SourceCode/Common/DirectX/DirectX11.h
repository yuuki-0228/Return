#pragma once
#include "..\..\Global.h"

// ���X�^���C�U�X�e�[�g.
enum class enRS_STATE
{
	None,	// ���w�ʕ`��.
	Back,	// �w�ʂ�`�悵�Ȃ�.
	Front,	// ���ʂ�`�悵�Ȃ�.
	Wire,	// ���C���[�t���[���`��.

	Max,
} typedef ERS_STATE;

/************************************************
*	DirectX11 �Z�b�g�A�b�v.
**/
class CDirectX11 final
{
public:
	CDirectX11();
	~CDirectX11();

	// �C���X�^���X�̎擾.
	static CDirectX11* GetInstance();

	// DirectX11�\�z.
	static HRESULT Create( HWND hWnd );
	// DirectX11���.
	static void Release();

	// �o�b�N�o�b�t�@�N���A�֐�.
	static void ClearBackBuffer();
	// �\��.
	static void Present();

	// �f�o�C�X���擾.
	static ID3D11Device* GetDevice() { return GetInstance()->m_pDevice11; }
	// �f�o�C�X�R���e�L�X�g���擾.
	static ID3D11DeviceContext* GetContext() { return GetInstance()->m_pContext11; }
	// �f�v�X�X�e���V���r���[�̎擾.
	static ID3D11DepthStencilView* GetDepthSV() { return GetInstance()->m_pBackBuffer_DSTexDSV; }
	// �E�B���h�E�T�C�Y�̎擾.
	static UINT GetWndWidth()	{ return GetInstance()->m_WndWidth;		}
	static UINT GetWndHeight()	{ return GetInstance()->m_WndHeight;	}
	// �w�i�F�̎擾.
	static D3DXCOLOR4 GetBackColor() { return GetInstance()->m_BackColor; }
	// �[�x(Z)�e�X�g���L�����擾.
	static bool GetDepth() { return GetInstance()->m_IsDepth; }
	// �A���t�@�u�����h���L�����擾.
	static bool GetAlphaBlend() { return GetInstance()->m_IsAlphaBlend; }
	// �A���t�@�g�D�J�o���[�W���L�����擾.
	static bool GetAlphaToCoverage() { return GetInstance()->m_IsAlphaToCoverage; }

	// �w�i�F�̐ݒ�.
	static void SetBackColor( const D3DXCOLOR4& Color ) { GetInstance()->m_BackColor = Color; }
	// �[�x(Z)�e�X�gON/OFF�؂�ւ�.
	static void SetDepth( bool flag );
	// �A���t�@�u�����hON/OFF�؂�ւ�.
	static void SetAlphaBlend( bool flag );
	// �A���t�@�g�D�J�o���[�WON/OFF�؂�ւ�.
	static void SetAlphaToCoverage( bool flag );
	// ���X�^���C�U�X�e�[�g�ݒ�.
	static void SetRasterizerState( const ERS_STATE& RsState );
	// �}�E�X�J�[�\����\�����邩�̐ݒ�.
	static void SetIsDispMouseCursor( const bool Flag ) { GetInstance()->m_IsDispMouseCursor = Flag; }

	// �t���X�N���[���̐ݒ�.
	static void SetFullScreen();
	// �t���X�N���[����Ԃ��擾.
	static bool IsFullScreen();

	// �A�N�e�B�u�E�B���h�E���m�F.
	static void CheckActiveWindow();
	// �E�B���h�E���A�N�e�B�u���擾.
	static bool IsWindowActive() { return GetInstance()->m_IsWindowActive; };

private:
	// �f�o�C�X�ƃX���b�v�`�F�C���쐬.
	HRESULT CreateDeviceAndSwapChain();

	// �o�b�N�o�b�t�@�쐬�F�J���[�p�����_�[�^�[�Q�b�g�r���[�쐬.
	HRESULT CreateColorBackBufferRTV();

	// �o�b�N�o�b�t�@�쐬�F�f�v�X�X�e���V���p�����_�[�^�[�Q�b�g�r���[�쐬.
	HRESULT CreateDepthStencilBackBufferRTV();

	// ���X�^���C�U�쐬.
	HRESULT CreateRasterizer();

	// �f�v�X�X�e���V���X�e�[�g�쐬.
	HRESULT CreateDepthStencilState();

	// �A���t�@�u�����h�X�e�[�g�쐬.
	HRESULT CreateAlphaBlendState();

	// �r���[�|�[�g�쐬.
	HRESULT CreateViewports();

private:
	HWND						m_hWnd;						// �E�B���h�E�n���h��.

	ID3D11Device*				m_pDevice11;				// �f�o�C�X�I�u�W�F�N�g.
	ID3D11DeviceContext*		m_pContext11;				// �f�o�C�X�R���e�L�X�g.
	IDXGISwapChain*				m_pSwapChain;				// �X���b�v�`�F�[��.
	ID3D11RenderTargetView*		m_pBackBuffer_TexRTV;		// �����_�\�^�[�Q�b�g�r���[.
	ID3D11Texture2D*			m_pBackBuffer_DSTex;		// �f�v�X�X�e���V���p�e�N�X�`��.
	ID3D11DepthStencilView*		m_pBackBuffer_DSTexDSV;		// �f�v�X�X�e���V���r���[.

	// �[�x(Z)�e�X�g�ݒ�.
	ID3D11DepthStencilState*	m_pDepthStencilStateOn;		// �L���ݒ�.
	ID3D11DepthStencilState*	m_pDepthStencilStateOff;	// �����ݒ�.

	// �A���t�@�u�����h.
	ID3D11BlendState*			m_pAlphaBlendOn;			// �L���ݒ�.
	ID3D11BlendState*			m_pAlphaBlendOff;			// �����ݒ�.

	// �A���t�@�g�D�J�o���[�W.
	ID3D11BlendState*			m_pAlphaToCoverageOn;		// �L���ݒ�..

	// ���X�^���C�U�X�e�[�g.
	ID3D11RasterizerState*		m_pCullNone;				// ���w�ʕ`��.
	ID3D11RasterizerState*		m_pCullBack;				// �w�ʂ�`�悵�Ȃ�.
	ID3D11RasterizerState*		m_pCullFront;				// ���ʂ�`�悵�Ȃ�.
	ID3D11RasterizerState*		m_pWireFrame;				// ���C���[�t���[���`��.

	UINT						m_WndWidth;					// �E�B���h�E��.
	UINT						m_WndHeight;				// �E�B���h�E����.

	D3DXCOLOR4					m_BackColor;				// �w�i�F.

	bool						m_IsWindowActive;			// �A�N�e�B�u�E�B���h�E���m�F.
	bool						m_IsDepth;					// �[�x(Z)�e�X�g���L����.
	bool						m_IsAlphaBlend;				// �A���t�@�u�����h���L����.
	bool						m_IsAlphaToCoverage;		// �A���t�@�g�D�J�o���[�W���L����.
	bool						m_IsDispMouseCursor;		// �}�E�X�J�[�\����\�����邩.

private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CDirectX11( const CDirectX11 & )				= delete;
	CDirectX11& operator = ( const CDirectX11 & )	= delete;
	CDirectX11( CDirectX11 && )						= delete;
	CDirectX11& operator = ( CDirectX11 && )		= delete;
};