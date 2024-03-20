#pragma once
#include "..\SceneBase.h"

class CTitleWidget;

/************************************************
*	�^�C�g���N���X.
*		���c�F�P.
**/
class CTitle final
	: public CSceneBase
{
public:
	CTitle();
	virtual ~CTitle();

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
	std::unique_ptr<CTitleWidget>			m_pTitleWidget;				// UI.
};