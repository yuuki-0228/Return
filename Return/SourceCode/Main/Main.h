#pragma once
#include "..\Global.h"

class CFrameRate;
class CLoadManager;
class CLoadRender;

/************************************************
*	���C���N���X.
**/
class CMain final
{
public:
	CMain();
	~CMain();

	void	Update( const float& DeltaTime );	// �X�V����.
	void	LoadUpdate();						// ���[�h���̍X�V����.
	HRESULT Create();							// �\�z����.
	void	Loop();								// ���C�����[�v.

	// �E�B���h�E�������֐�.
	HRESULT InitWindow( HINSTANCE hInstance );

private:
	// �E�B���h�E�֐�(���b�Z�[�W���̏���).
	static LRESULT CALLBACK MsgProc(
		HWND hWnd, UINT uMsg,
		WPARAM wParam, LPARAM lParam );
	
	// FPS�̕`��.
	void FPSRender();

private:
	HWND							m_hWnd;			// �E�B���h�E�n���h��.
	std::unique_ptr<CFrameRate>		m_pFrameRate;	// �t���[�����[�g.
	std::unique_ptr<CLoadManager>	m_pLoadManager;	// ���[�h�}�l�[�W���[.
	std::unique_ptr<CLoadRender>	m_pLoadRender;	// ���[�h���̕`��.
	bool							m_IsGameLoad;	// �ǂݍ��݂��I��������.
	bool							m_IsFPSRender;	// FPS��\�������邩.
};