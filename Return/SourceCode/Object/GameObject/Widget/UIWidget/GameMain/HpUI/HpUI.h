#pragma once
#include "..\..\UIWidget.h"

class CPlayer;

/************************************************
*	HPのUIクラス.
**/
class CHpUI final
	: public CUIWidget
{
public:
	CHpUI();
	virtual ~CHpUI();

	// 初期化.
	virtual bool Init() override;

	// 更新.
	virtual void Update( const float& DeltaTime ) override;

	// 描画.
	virtual void Render() override;

	// プレイヤーの設定.
	void SetPlayer( CPlayer* pPlayer ) { m_pPlayer = pPlayer; }

private:
	CPlayer*			m_pPlayer;		// プレイヤー.
	CSprite*			m_pHpBack;		// HPの背景.
	CSprite*			m_pHpFrame;		// HPのフレーム.
	SSpriteRenderState	m_HpBackState;	// HPの背景情報.
	SSpriteRenderState	m_HpFrameState;	// HPのフレーム情報.
	float				m_AnimAng;		// アニメーション用の角度(度).

private:
	// 外部データの定数.
	const ConstantStruct::SPlayer& CONSTANT = CConstant::GetPlayer();
};