#pragma once
#include "..\..\UIWidget.h"

class CPlayer;

/************************************************
*	スタミナUIクラス.
**/
class CStaminaUI final
	: public CUIWidget
{
public:
	CStaminaUI();
	virtual ~CStaminaUI();

	// 初期化.
	virtual bool Init() override;

	// 更新.
	virtual void Update( const float& DeltaTime ) override;

	// 描画.
	virtual void Render() override;

	// プレイヤーの設定.
	void SetPlayer( CPlayer* pPlayer ) { m_pPlayer = pPlayer; }

private:
	CPlayer*			m_pPlayer;				// プレイヤー.
	CSprite*			m_pStaminaBack;			// スタミナの背景.
	CSprite*			m_pStaminaFrame;		// スタミナのフレーム.
	SSpriteRenderState	m_StaminaBackState;		// スタミナの背景情報.
	SSpriteRenderState	m_StaminaGaugeState;	// スタミナのゲージ情報.
	SSpriteRenderState	m_StaminaFrameState;	// スタミナのフレーム情報.

private:
	// 外部データの定数.
	const ConstantStruct::SPlayer& CONSTANT = CConstant::GetPlayer();
};