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
// �C���X�^���X�̎擾.
//---------------------------.
CConstant* CConstant::GetInstance()
{
	static std::unique_ptr<CConstant> pInstance = std::make_unique<CConstant>();
	return pInstance.get();
}

//---------------------------.
// �^�C�g���Ŏg�p����萔�̓ǂݍ���.
//---------------------------.
void CConstant::TitleLoad()
{
#ifdef _DEBUG
	CConstant* pI = GetInstance();
	CLog::PushLog( "------ �^�C�g���Ŏg�p����萔�̓ǂݍ��݊J�n -------" );

	CLog::PushLog( "------ �^�C�g���Ŏg�p����萔�̓ǂݍ��ݏI�� -------" );
#endif
}

//---------------------------.
// �Q�[�����C���Ŏg�p����萔�̓ǂݍ���.
//---------------------------.
void CConstant::GameMainLoad()
{
#ifdef _DEBUG
	CConstant* pI = GetInstance();
	CLog::PushLog( "------ �Q�[�����C���Ŏg�p����萔�̓ǂݍ��݊J�n -------" );

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

	CLog::PushLog( "------ �Q�[�����C���Ŏg�p����萔�̓ǂݍ��ݏI�� -------" );
#endif
}

//---------------------------.
// �Q�[���N���A�Ŏg�p����萔�̓ǂݍ���.
//---------------------------.
void CConstant::GameClearLoad()
{
#ifdef _DEBUG
	CConstant* pI = GetInstance();
	CLog::PushLog( "------ �Q�[���N���A�Ŏg�p����萔�̓ǂݍ��݊J�n -------" );

	CLog::PushLog( "------ �Q�[���N���A�Ŏg�p����萔�̓ǂݍ��ݏI�� -------" );
#endif
}