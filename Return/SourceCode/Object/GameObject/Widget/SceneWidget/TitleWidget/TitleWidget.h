#pragma once
#include "..\SceneWidget.h"

/************************************************
*	�^�C�g��UI�N���X.
*		���c�F�P.
**/
class CTitleWidget final
	: public CSceneWidget
{
public:
	CTitleWidget();
	virtual ~CTitleWidget();

	// ������.
	virtual bool Init() override;

	// �X�V.
	virtual void Update( const float& DeltaTime ) override;

	// �`��.
	virtual void Render() override;
};
