#pragma once
#include "..\SceneWidget.h"

/************************************************
*	�Q�[���N���AUI�N���X.
*		���c�F�P.
**/
class CGameClearWidget final
	: public CSceneWidget
{
public:
	CGameClearWidget();
	virtual ~CGameClearWidget();

	// ������.
	virtual bool Init() override;

	// �X�V.
	virtual void Update( const float& DeltaTime ) override;

	// �`��.
	virtual void Render() override;
};
