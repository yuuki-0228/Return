#pragma once
#include "..\EditorBase.h"

class CStageManager;

/************************************************
*	�X�e�[�W�G�f�B�^.
*		���c�F�P.
**/
class CStageEditor
	: public CEditorBase
{
public:
	CStageEditor();
	~CStageEditor();

	// ������.
	virtual bool Init() override;

	// �X�V.
	virtual void Update( const float& DeltaTime ) override;

	// ImGui�`��.
	virtual void ImGuiRender() override;
	// ���f���̕`��.
	virtual void ModelRender() override;
	// UI�̕`��.
	virtual void UIRender() override;
	// �G�t�F�N�g�`��.
	virtual void EffectRneder() override {};

private:
	std::unique_ptr<CStageManager>	m_pStage;		// �X�e�[�W.
	std::string						m_SelectObject;	// �I�����Ă���I�u�W�F�N�g.
};