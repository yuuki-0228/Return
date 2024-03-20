#pragma once
#include "..\..\..\Actor.h"

namespace AttackNo{
	// UÌÔ.
	enum enAttackNo : unsigned char {
		None,

		Heavy,	// ­U.
		Normal,	// ÊíU.
		Weak,	// ãU.

		Max,
	} typedef EAttackNo;
}

/************************************************
*	UGtFNgNX.
*		ú±cFP.
**/
class CAttackEffect final
	: public CActor
{
public:
	CAttackEffect();
	~CAttackEffect();

	// ú».
	virtual bool Init() override;

	// XV.
	virtual void Update( const float& DeltaTime ) override;

	// `æ.
	virtual void Render() override;

	// GtFNgÌÄ¶.
	void EffectPlay( const int No );

	// ÅãÉÄ¶µ½GtFNgªI¹µ½©æ¾.
	bool GetIsEffectPlay() const { return *m_IsNowEffectPlay; }

private:
	// gpµÄ¢È¢GtFNgÌí.
	void EffectDelete();

private:
	std::vector<CSprite*>							m_pAttackSpriteList;	// UGtFNgXg.
	std::vector<SSpriteRenderState>					m_AttackStateBaseList;	// UGtFNgÌîñx[XXg.
	std::vector<std::vector<SSpriteRenderState>>	m_AttackStateList;		// UGtFNgÌîñXg.
	std::vector<std::string>						m_AttackSEList;			// UÌSEXg.
	bool*											m_IsNowEffectPlay;		// »ÝÌGtFNgªÄ¶©.
};
