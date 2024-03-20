#pragma once
#include "SceneManager\SceneManager.h"
#include "..\Common\SoundManeger\SoundManeger.h"
#include "..\Object\Camera\Cameras\Cameras.h"
#include "..\Object\Camera\CameraManager\CameraManager.h"
#include "..\Utility\Input\Input.h"

class CCameras;

/************************************************
*	�V�[���x�[�X�N���X.
*		���c�F�P.
**/
class CSceneBase
{
public:
	CSceneBase();
	virtual ~CSceneBase();

	// ������.
	virtual bool Init() = 0;

	// �X�V.
	virtual void Update( const float& DeltaTime ) = 0;

	// ���f���̕`��.
	virtual void ModelRender() = 0;
	// �X�v���C�g(UI)�̕`��.
	virtual void SpriteUIRender() = 0;
	// �X�v���C�g(3D)/Effect�̕`��.
	//	_A�F��ɕ\������� / _B�F��ɕ\�������.
	virtual void Sprite3DRender_A() = 0;
	virtual void Sprite3DRender_B() = 0;

protected:
	std::unique_ptr<CCameras>	m_pCameras;		// �����̃J��������������N���X.
	std::string					m_BGMName;		// BGM��.

};