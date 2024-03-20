#pragma once
#include "..\SceneWidget.h"

class CPlayer;
class CHpUI;
class CStaminaUI;

/************************************************
*	�Q�[�����C��UI�N���X.
*		���c�F�P.
**/
class CGameMainWidget final
	: public CSceneWidget
{
public:
	CGameMainWidget();
	virtual ~CGameMainWidget();

	// ������.
	virtual bool Init() override;

	// �X�V.
	virtual void Update( const float& DeltaTime ) override;

	// �`��.
	virtual void Render() override;

	// �v���C���[�̐ݒ�.
	void SetPlayer( CPlayer* pPlayer );

private:
	CSprite*					m_pFrame;		// �g.
	std::unique_ptr<CHpUI>		m_pHpUI;		// HP.
	std::unique_ptr<CStaminaUI>	m_pStaminaUI;	// �X�^�~�i.
};
