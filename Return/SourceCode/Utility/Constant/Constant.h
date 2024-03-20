#pragma once
#include "..\FileManager\FileManager.h"
#include "ConstantStruct.h"

/************************************************
*	定数クラス.
*		﨑田友輝.
**/
class CConstant final
{
public:
	CConstant();
	~CConstant();

	// インスタンスの取得.
	static CConstant* GetInstance();

	// タイトルで使用する定数の読み込み.
	static void TitleLoad();
	// ゲームメインで使用する定数の読み込み.
	static void GameMainLoad();
	// ゲームクリアで使用する定数の読み込み.
	static void GameClearLoad();

	// 定数の取得.
	static const ConstantStruct::SFreeCamera&		GetFreeCamera()			{ return GetInstance()->m_FreeCameraData; }
	static const ConstantStruct::SStage&			GetStage()				{ return GetInstance()->m_StageData; }
	static const ConstantStruct::SPlayer&			GetPlayer()				{ return GetInstance()->m_PlayerData; }
	static const ConstantStruct::SEnemy&			GetEnemy()				{ return GetInstance()->m_EnemyData; }
	static const ConstantStruct::SPotionArea&		GetPotionArea()			{ return GetInstance()->m_PotionAreaData; }
	static const ConstantStruct::SBullet&			GetBullet()				{ return GetInstance()->m_BulletData; }
	static const ConstantStruct::SPowerUpBullet&	GetPowerUpBullet()		{ return GetInstance()->m_PowerUpBulletData; }
	static const ConstantStruct::SSharpBullet&		GetSharpBullet()		{ return GetInstance()->m_SharpBulletData; }
	static const ConstantStruct::SSpillBullet&		GetSpillBullet()		{ return GetInstance()->m_SpillBulletData; }
	static const ConstantStruct::SBigPotionBullet&	GetBigPotionBullet()	{ return GetInstance()->m_BigPotionBulletData; }

private:
	// 定数データ.
	ConstantStruct::SFreeCamera				m_FreeCameraData;
	ConstantStruct::SStage					m_StageData;
	ConstantStruct::SPlayer					m_PlayerData;
	ConstantStruct::SEnemy					m_EnemyData;
	ConstantStruct::SPotionArea				m_PotionAreaData;
	ConstantStruct::SBullet					m_BulletData;
	ConstantStruct::SPowerUpBullet			m_PowerUpBulletData;
	ConstantStruct::SSharpBullet			m_SharpBulletData;
	ConstantStruct::SSpillBullet			m_SpillBulletData;
	ConstantStruct::SBigPotionBullet		m_BigPotionBulletData;
};
