#pragma once
#include "EnemyStruct.h"
#include "..\..\Character.h"

/**************************************************
*	GNX.
*		๚ฑcFP.
**/
class CEnemy final
	: public CCharacter
{
public:
	CEnemy();
	virtual ~CEnemy();

	// ๚ป.
	virtual bool Init() override;

	// XV.
	virtual void Update( const float& DeltaTime ) override;

	// `ๆ.
	virtual void Render() override;

	// ฝ่ป่ึ.
	virtual void Collision( CActor* pActor ) override;

	// X|[ณน้.
	void Spown( const D3DXPOSITION3& Pos, const SEnemyState State );

protected:
	// ฝ่ป่ฬ๚ป.
	//	Initึฬลใษฤิ.
	virtual void InitCollision() override;
	// ฝ่ป่ฬXV.
	//	Updateึฬลใษฤิ.
	virtual void UpdateCollision() override;

private:
	// _[W๐๓ฏ้ฉฬฝ่ป่.
	void DamageCollision( CActor* pActor );

	// N[^CฬXV.
	void CoolTimeUpdate();

	// Gฬsฎ.
	void Action();

	// U.
	void Attack();
	// U๐sคฉmF.
	bool AttackCheck();
	
	// า@.
	void Wait();

	// ฺฎ.
	void Move();

	// sฎฬ[v.
	void ActionLoop();

private:
	std::vector<CSprite*>			m_pHpBarList;				// HPo[ฬๆXg.
	std::vector<SSpriteRenderState>	m_HpBarStateList;			// HPo[ฬ๎๑Xg.
	SEnemyState						m_StartEnemyState;			// JnฬGฬ๎๑.
	SEnemyState						m_EnemyState;				// Gฬ๎๑.
	float							m_StrongDamageCoolTimeCnt;	// ญขepฬN[^C.

private:
	// Of[^ฬ่.
	const ConstantStruct::SEnemy& CONSTANT = CConstant::GetEnemy();
};
