#pragma once
#include "..\Actor.h"
#include "..\Actor.h"
#include "GimmickBase\GimmickBase.h"

/************************************************
*	�X�e�[�W�M�~�b�N�}�l�[�W���[.
**/
class CGimmickManager
{
public:
	CGimmickManager();
	virtual ~CGimmickManager();

	// �C���X�^���X�̎擾.
	static CGimmickManager* GetInstance();

	// ������.
	static bool Init();

	// �X�V.
	static void Update( const float& DeltaTime );

	// �`��.
	static void Render();
	// �`�惊�X�g�̎擾.
	static std::vector<std::pair<float, CActor*>> GetRenderList();

	// �M�~�b�N��z�u����.
	static void Arrangement( const D3DXPOSITION3& Pos, const std::string& Name );

private:
	// �e�̏��̓ǂݍ���.
	static HRESULT StateLood();
	// �e�̏�񂪂��邩���ׂ�.
	static void LoadStateCheck( const std::string& Name );

	// �M�~�b�N�̍쐬.
	static std::shared_ptr<CGimmickBase> CreateGimmick( const std::string Type );

	// �`�惊�X�g�̍X�V.
	static void RenderListUpdate();

private:
	std::vector<std::shared_ptr<CGimmickBase>>		m_pGimmickList;	// �X�e�[�W�M�~�b�N���X�g.
	std::vector<std::pair<float, CActor*>>			m_pRenderList;	// �`�惊�X�g.
	std::unordered_map<std::string, SGimmickState>	m_StateList;	// �M�~�b�N�̏�񃊃X�g.
};