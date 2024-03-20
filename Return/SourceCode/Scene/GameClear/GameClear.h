#pragma once
#include "..\SceneBase.h"
#include "..\..\Common\Sprite\Sprite.h"

class CGameClearWidget;

/************************************************
*	�Q�[���N���A�N���X.
*		���c�F�P.
**/
class CGameClear final
	: public CSceneBase
{
public:
	CGameClear();
	~CGameClear();

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
	std::unique_ptr<CGameClearWidget>	m_pGameClearWidget;	// UI.
};