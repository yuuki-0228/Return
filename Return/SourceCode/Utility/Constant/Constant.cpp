#include "Constant.h"

CConstant::CConstant()
	: m_FreeCameraData		()
	, m_StageData			()
	, m_PlayerData			()
	, m_EnemyData			()
	, m_PotionAreaData		()
	, m_BulletData			()
	, m_PowerUpBulletData	()
	, m_SharpBulletData		()
	, m_SpillBulletData		()
	, m_BigPotionBulletData	()
{
}

CConstant::~CConstant()
{
}

//---------------------------.
// インスタンスの取得.
//---------------------------.
CConstant* CConstant::GetInstance()
{
	static std::unique_ptr<CConstant> pInstance = std::make_unique<CConstant>();
	return pInstance.get();
}

//---------------------------.
// タイトルで使用する定数の読み込み.
//---------------------------.
void CConstant::TitleLoad()
{
#ifdef _DEBUG
	CConstant* pI = GetInstance();
	CLog::PushLog( "------ タイトルで使用する定数の読み込み開始 -------" );

	CLog::PushLog( "------ タイトルで使用する定数の読み込み終了 -------" );
#endif
}

//---------------------------.
// ゲームメインで使用する定数の読み込み.
//---------------------------.
void CConstant::GameMainLoad()
{
#ifdef _DEBUG
	CConstant* pI = GetInstance();
	CLog::PushLog( "------ ゲームメインで使用する定数の読み込み開始 -------" );

	pI->m_FreeCameraData.Load();
	pI->m_StageData.Load();
	pI->m_PlayerData.Load();
	pI->m_EnemyData.Load();
	pI->m_PotionAreaData.Load();
	pI->m_BulletData.Load();
	pI->m_PowerUpBulletData.Load();
	pI->m_SharpBulletData.Load();
	pI->m_SpillBulletData.Load();
	pI->m_BigPotionBulletData.Load();

	CLog::PushLog( "------ ゲームメインで使用する定数の読み込み終了 -------" );
#endif
}

//---------------------------.
// ゲームクリアで使用する定数の読み込み.
//---------------------------.
void CConstant::GameClearLoad()
{
#ifdef _DEBUG
	CConstant* pI = GetInstance();
	CLog::PushLog( "------ ゲームクリアで使用する定数の読み込み開始 -------" );

	CLog::PushLog( "------ ゲームクリアで使用する定数の読み込み終了 -------" );
#endif
}