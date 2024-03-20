#pragma once
#include "..\BulletBase.h"

/************************************************
*	ΡΚeNX.
*		ϊ±cFP.
**/
class CSharpBullet final
	: public CBulletBase
{
public:
	CSharpBullet();
	virtual ~CSharpBullet();

	// ϊ».
	virtual bool Init() override;

	// ½θ»θΦ.
	virtual void Collision( CActor* pActor ) override;

protected:
	// ½θ»θΜϊ».
	//	InitΦΜΕγΙΔΤ.
	virtual void InitCollision() override;
	// ½θ»θΜXV.
	//	UpdateΦΜΕγΙΔΤ.
	virtual void UpdateCollision() override;

	// eΜσΤΜϊ».
	virtual	void InitBulletState( const SBulletState& State ) override;

	// eπν·ι©mF.
	virtual bool DeleteCheck( const EObjectTag DeleteTag ) override;
	// Xe[Wπρ]·ιζ€ΙΕΏΤΉι©ΜmF.
	virtual bool RotationHitBackCheck( const int AttackNo ) override;
	// }EXJ[\ΜϋόΙΕΏΤ·.
	virtual bool AttackHitBackCheck( const int AttackNo ) override;

	// xGtFNgΜXV.
	virtual void LvEffectUpdate() override;

private:
	// Of[^Μθ.
	const ConstantStruct::SSharpBullet& SKILL_CONSTANT = CConstant::GetSharpBullet();
};