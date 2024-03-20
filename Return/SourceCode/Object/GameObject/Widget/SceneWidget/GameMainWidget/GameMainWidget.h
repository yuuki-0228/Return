#pragma once
#include "..\SceneWidget.h"

class CPlayer;
class CHpUI;
class CStaminaUI;

/************************************************
*	ゲームメインUIクラス.
*		﨑田友輝.
**/
class CGameMainWidget final
	: public CSceneWidget
{
public:
	CGameMainWidget();
	virtual ~CGameMainWidget();

	// 初期化.
	virtual bool Init() override;

	// 更新.
	virtual void Update( const float& DeltaTime ) override;

	// 描画.
	virtual void Render() override;

	// プレイヤーの設定.
	void SetPlayer( CPlayer* pPlayer );

private:
	CSprite*					m_pFrame;		// 枠.
	std::unique_ptr<CHpUI>		m_pHpUI;		// HP.
	std::unique_ptr<CStaminaUI>	m_pStaminaUI;	// スタミナ.
};
