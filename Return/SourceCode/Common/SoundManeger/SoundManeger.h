#pragma once
#include "..\..\Global.h"
#include <unordered_map>
#include <string>
#include <memory>
#include <mutex>
#include <filesystem>

class CSound;
class CObject;

/************************************************
*	�T�E���h�}�l�[�W���[�N���X.
*		���c�F�P.
**/
class CSoundManager final
{
public:
	using Sound_map			= std::unordered_map<std::string, std::vector<std::shared_ptr<CSound>>>;
	using Sound_Time_map	= std::unordered_map<std::string, ULONGLONG>;
	using Sound_List		= std::vector<std::string>;

public:
	CSoundManager();
	~CSoundManager();

	// �C���X�^���X�̎擾.
	static CSoundManager* GetInstance();

	// �T�E���h�̓ǂݍ���.
	static HRESULT SoundLoad( HWND hWnd );

	// ���X�i�[��ݒ�.
	static void	SetListener( CObject* pObj ) { GetInstance()->m_pListener = pObj; }

	// �Đ�.
	static void	PlaySE(				std::string Name,						 float	Interval	= 0.0f, bool IsStopPlay = false, bool IsMsgStop = false ); // SE���Đ�����.
	static void	PlaySE3D(			std::string Name, D3DXPOSITION3 PlayPos, float	Interval	= 0.0f, bool IsStopPlay = false, bool IsMsgStop = false ); // 3D��SE���Đ�����.
	static void	PlayBGM(			std::string Name,						 bool	OldBGMStop	= true,							 bool IsMsgStop = false ); // BGM���Đ�����.
	static void	PlayFadeBGM(		std::string Name,						 int	FadeSpeed	= 1,					 		 bool IsMsgStop = false ); // BGM���t�F�[�h�Ő؂�ւ��čĐ�����.
	static void	PlayBGM3D(			std::string Name, D3DXPOSITION3 PlayPos, bool	OldBGMStop	= true, 						 bool IsMsgStop = false ); // 3D��BGM���Đ�����.

	// �t�Đ�.
	static void	ReversePlaySE(		std::string Name,						 float	Interval	= 0.0f, bool IsStopPlay = false, bool IsMsgStop = false ); // SE���Đ�����.
	static void	ReversePlaySE3D(	std::string Name, D3DXPOSITION3 PlayPos, float	Interval	= 0.0f, bool IsStopPlay = false, bool IsMsgStop = false ); // 3D��SE���Đ�����.
	static void	ReversePlayBGM(		std::string Name,						 bool	OldBGMStop	= true,							 bool IsMsgStop = false ); // BGM���Đ�����.
	static void	ReversePlayFadeBGM(	std::string Name,						 int	FadeSpeed	= 1,					 		 bool IsMsgStop = false ); // BGM���t�F�[�h�Ő؂�ւ��čĐ�����.
	static void	ReversePlayBGM3D(	std::string Name, D3DXPOSITION3 PlayPos, bool	OldBGMStop	= true, 						 bool IsMsgStop = false ); // 3D��BGM���Đ�����.

	// ��~.
	static void	StopSE(		std::string Name,	bool IsMsgStop = false ); // �w���SE���~������.
	static void	StopBGM(	std::string Name,	bool IsMsgStop = false ); // �w���BGM���~������.
	static void	BGMAllStop(						bool IsMsgStop = false ); // BGM��S�Ē�~������.
	static void	isLoopStop( bool Flag,			bool IsMsgStop = false ); // ���[�v�Đ����~�����邩.

	// �ꎞ��~.
	static void	PauseSE(	std::string Name,	bool IsMsgStop = false ); // �w���SE���ꎞ��~������.
	static void	PauseBGM(	std::string Name,	bool IsMsgStop = false ); // �w���BGM���ꎞ��~������.
	static void	BGMAllPause(					bool IsMsgStop = false ); // BGM��S�Ĉꎞ��~������.

	// ���ʂ̐ݒ�.
	static void ResetSEVolume(	std::string Name );
	static void SetSEVolume(	std::string Name, int Volume );
	static void AddSEVolume(	std::string Name, int Volume );

	// �Đ����x�̐ݒ�.
	static void ResetSEPlaySpeed( std::string Name );
	static void SetSEPlaySpeed( std::string Name, int Speed );
	static void AddSEPlaySpeed( std::string Name, int Speed );

private:
	// �T�E���h�t�@�C�����J��.
	static void OpenSound(
		std::vector<std::shared_ptr<CSound>>* List,	const HWND&			hWnd,
		const int			PlayMax,				const int			MaxVolume,
		const std::string&	FileName,				const std::wstring& wFilePath );

private:
	CObject*		m_pListener;	// ���X�i�[(3D�Đ��Ŏg�p����).
	Sound_map		m_BGMList;		// �T�E���h(BGM)���X�g.
	Sound_map		m_SEList;		// �T�E���h(SE)���X�g.
	Sound_Time_map	m_PlayTimeList;	// �O��Đ����Ă���ǂ񂾂��o������.
	Sound_List		m_BGMNames;		// BGM���O���X�g.
	Sound_List		m_SENames;		// SE���O���X�g.
	std::string		m_NowBGMName;	// ���݂�BGM��.
	std::mutex		m_Mutex;
	bool			m_IsLoadEnd;	// �ǂݍ��݂��I�������.
	bool			m_LoopStopFlag;	// ���[�v�̒�~������.

private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CSoundManager( const CSoundManager& )				= delete;
	CSoundManager& operator = ( const CSoundManager& )	= delete;
	CSoundManager( CSoundManager&& )					= delete;
	CSoundManager& operator = ( CSoundManager&& )		= delete;
};

