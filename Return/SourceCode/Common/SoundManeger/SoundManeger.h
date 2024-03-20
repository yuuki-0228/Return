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
*	サウンドマネージャークラス.
*		﨑田友輝.
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

	// インスタンスの取得.
	static CSoundManager* GetInstance();

	// サウンドの読み込み.
	static HRESULT SoundLoad( HWND hWnd );

	// リスナーを設定.
	static void	SetListener( CObject* pObj ) { GetInstance()->m_pListener = pObj; }

	// 再生.
	static void	PlaySE(				std::string Name,						 float	Interval	= 0.0f, bool IsStopPlay = false, bool IsMsgStop = false ); // SEを再生する.
	static void	PlaySE3D(			std::string Name, D3DXPOSITION3 PlayPos, float	Interval	= 0.0f, bool IsStopPlay = false, bool IsMsgStop = false ); // 3DでSEを再生する.
	static void	PlayBGM(			std::string Name,						 bool	OldBGMStop	= true,							 bool IsMsgStop = false ); // BGMを再生する.
	static void	PlayFadeBGM(		std::string Name,						 int	FadeSpeed	= 1,					 		 bool IsMsgStop = false ); // BGMをフェードで切り替えて再生する.
	static void	PlayBGM3D(			std::string Name, D3DXPOSITION3 PlayPos, bool	OldBGMStop	= true, 						 bool IsMsgStop = false ); // 3DでBGMを再生する.

	// 逆再生.
	static void	ReversePlaySE(		std::string Name,						 float	Interval	= 0.0f, bool IsStopPlay = false, bool IsMsgStop = false ); // SEを再生する.
	static void	ReversePlaySE3D(	std::string Name, D3DXPOSITION3 PlayPos, float	Interval	= 0.0f, bool IsStopPlay = false, bool IsMsgStop = false ); // 3DでSEを再生する.
	static void	ReversePlayBGM(		std::string Name,						 bool	OldBGMStop	= true,							 bool IsMsgStop = false ); // BGMを再生する.
	static void	ReversePlayFadeBGM(	std::string Name,						 int	FadeSpeed	= 1,					 		 bool IsMsgStop = false ); // BGMをフェードで切り替えて再生する.
	static void	ReversePlayBGM3D(	std::string Name, D3DXPOSITION3 PlayPos, bool	OldBGMStop	= true, 						 bool IsMsgStop = false ); // 3DでBGMを再生する.

	// 停止.
	static void	StopSE(		std::string Name,	bool IsMsgStop = false ); // 指定のSEを停止させる.
	static void	StopBGM(	std::string Name,	bool IsMsgStop = false ); // 指定のBGMを停止させる.
	static void	BGMAllStop(						bool IsMsgStop = false ); // BGMを全て停止させる.
	static void	isLoopStop( bool Flag,			bool IsMsgStop = false ); // ループ再生を停止させるか.

	// 一時停止.
	static void	PauseSE(	std::string Name,	bool IsMsgStop = false ); // 指定のSEを一時停止させる.
	static void	PauseBGM(	std::string Name,	bool IsMsgStop = false ); // 指定のBGMを一時停止させる.
	static void	BGMAllPause(					bool IsMsgStop = false ); // BGMを全て一時停止させる.

	// 音量の設定.
	static void ResetSEVolume(	std::string Name );
	static void SetSEVolume(	std::string Name, int Volume );
	static void AddSEVolume(	std::string Name, int Volume );

	// 再生速度の設定.
	static void ResetSEPlaySpeed( std::string Name );
	static void SetSEPlaySpeed( std::string Name, int Speed );
	static void AddSEPlaySpeed( std::string Name, int Speed );

private:
	// サウンドファイルを開く.
	static void OpenSound(
		std::vector<std::shared_ptr<CSound>>* List,	const HWND&			hWnd,
		const int			PlayMax,				const int			MaxVolume,
		const std::string&	FileName,				const std::wstring& wFilePath );

private:
	CObject*		m_pListener;	// リスナー(3D再生で使用する).
	Sound_map		m_BGMList;		// サウンド(BGM)リスト.
	Sound_map		m_SEList;		// サウンド(SE)リスト.
	Sound_Time_map	m_PlayTimeList;	// 前回再生してからどんだけ経ったか.
	Sound_List		m_BGMNames;		// BGM名前リスト.
	Sound_List		m_SENames;		// SE名前リスト.
	std::string		m_NowBGMName;	// 現在のBGM名.
	std::mutex		m_Mutex;
	bool			m_IsLoadEnd;	// 読み込みが終わったか.
	bool			m_LoopStopFlag;	// ループの停止させる.

private:
	// コピー・ムーブコンストラクタ, 代入演算子の削除.
	CSoundManager( const CSoundManager& )				= delete;
	CSoundManager& operator = ( const CSoundManager& )	= delete;
	CSoundManager( CSoundManager&& )					= delete;
	CSoundManager& operator = ( CSoundManager&& )		= delete;
};

