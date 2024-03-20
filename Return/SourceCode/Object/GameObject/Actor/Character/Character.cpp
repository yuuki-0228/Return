#include "Character.h"
#include "..\..\Widget\UIWidget\GameMain\ValueRenderUI\ValueRenderUI.h"

CCharacter::CCharacter()
	: m_pSprite		( nullptr )
	, m_SpriteState	()
	, m_Hp			( INIT_INT )
	, m_HpMax		( INIT_INT )
	, m_Stamina		( INIT_FLOAT )
	, m_StaminaMax	( INIT_FLOAT )
{
}

CCharacter::~CCharacter()
{
}

//---------------------------.
// HPの設定.
//---------------------------.
void CCharacter::SetHp( const int HP )
{
	if ( m_Hp == 0 ) return;

	m_Hp = HP;
	if ( m_Hp < 0		) m_Hp = 0;
	if ( m_Hp > m_HpMax ) m_Hp = m_HpMax;
}

//---------------------------.
// スタミナの設定.
//---------------------------.
void CCharacter::SetStamina( const float Stamina )
{
	m_Stamina = Stamina;
	if ( m_Stamina < 0				) m_Stamina = 0;
	if ( m_Stamina > m_StaminaMax	) m_Stamina = m_StaminaMax;
}

//---------------------------.
// 回復する.
//---------------------------.
void CCharacter::HealHp( const float Heal )
{
	HealHp( static_cast<int>( std::ceil( Heal ) ) );
}
void CCharacter::HealHp( const int Heal )
{
	if ( m_Hp == 0 ) return;

	// HPを増やす.
	m_Hp += Heal;
	if ( m_Hp < 0		) m_Hp = 0;
	if ( m_Hp > m_HpMax ) m_Hp = m_HpMax;

	// 回復量の表示.
	D3DXPOSITION3 DamagePos = m_SpriteState.Transform.Position;
	DamagePos.y -= m_pSprite->GetSpriteState().Disp.h;
	CValueRenderUI::PushValue( DamagePos, Heal, EValueType::Heal );
}

//---------------------------.
// ダメージを受ける.
//---------------------------.
void CCharacter::DamageHp( const float Damage )
{
	DamageHp( static_cast<int>( std::ceil( Damage ) ) );
}
void CCharacter::DamageHp( const int Damage )
{
	if ( m_Hp == 0 ) return;

	// HPを減らす.
	m_Hp -= Damage;
	if ( m_Hp < 0		) m_Hp = 0;
	if ( m_Hp > m_HpMax ) m_Hp = m_HpMax;

	// ダメージ量の表示.
	D3DXPOSITION3 DamagePos = m_SpriteState.Transform.Position;
	DamagePos.y -= m_pSprite->GetSpriteState().Disp.h;
	CValueRenderUI::PushValue( DamagePos, Damage, EValueType::Damage );
}

//---------------------------.
// スタミナの追加.
//---------------------------.
void CCharacter::AddStamina( const float Stamina )
{
	m_Stamina += Stamina;
	if ( m_Stamina < 0				) m_Stamina = 0;
	if ( m_Stamina > m_StaminaMax	) m_Stamina = m_StaminaMax;
}
