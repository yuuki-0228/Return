#pragma once
#include "..\SceneBase.h"

class CSprite;
class CPlayer;
class CEnemyManager;
class CStageManager;
class CGameMainWidget;

/************************************************
*	�Q�[���N���X.
*		���c�F�P.
**/
class CGameMain final
	: public CSceneBase
{
public:
	CGameMain();
	~CGameMain();

	// ������.
	virtual bool Init() override;

	// �X�V.
	virtual void Update( const float& DeltaTime ) override;

	// ���f���̕`��.
	virtual void ModelRender() override;
	// �X�v���C�g(UI)�̕`��.
	virtual void SpriteUIRender() override;
	// �X�v���C�g(3D)/Effect�̕`��.
	//	_A�F��ɕ\������� / _B�F��ɕ\�������.
	virtual void Sprite3DRender_A() override;
	virtual void Sprite3DRender_B() override;

private:
	CSprite*							m_pBack;			// �w�i.
	std::shared_ptr<CPlayer>			m_pPlayer;			// �v���C���[.
	std::shared_ptr<CStageManager>		m_pStageManager;	// �X�e�[�W�}�l�[�W���[.
	std::unique_ptr<CGameMainWidget>	m_GameMainWidget;	// UI.
};