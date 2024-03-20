#pragma once
#include "PotionArea\PotionArea.h"

/************************************************
*	�|�[�V�����G���A�}�l�[�W���[�N���X.
**/
class CPotionAreaManager final
{
public:
	CPotionAreaManager();
	~CPotionAreaManager();

	// �C���X�^���X�̎擾.
	static CPotionAreaManager* GetInstance();

	// ������.
	static bool Init();

	// �X�V.
	static void Update( const float& DeltaTime );
	// �����蔻��I����Ăяo�����X�V.
	static void LateUpdate( const float& DeltaTime );

	// �`��.
	static void Render();
	// �`�惊�X�g�̎擾.
	static std::vector<std::pair<float, CActor*>> GetRenderList();

	// �G���A�̒ǉ�.
	static void PushArea( const D3DXPOSITION3& Pos, const float Radius, const std::string& Name, const float Time = INIT_FLOAT );

private:
	// �e�̏��̓ǂݍ���.
	static HRESULT StateLood();
	// �e�̏�񂪂��邩���ׂ�.
	static void LoadStateCheck( const std::string& Name );

	// �|�[�V�����̃^�C�v���擾.
	static int GetPotionType( const std::string& Type );
	// �`�惊�X�g�̍X�V.
	static void RenderListUpdate();

	// �g�p���Ă��Ȃ��e�̍폜.
	static void AreaDelete();

private:
	std::vector<std::shared_ptr<CPotionArea>>			m_pAreaList;	// �G���A���X�g.
	std::vector<std::pair<float, CActor*>>				m_pRenderList;	// �`�惊�X�g.
	std::unordered_map<std::string, SPotionAreaState>	m_StateList;	// �G���A�̏�񃊃X�g.

private:
	// �O���f�[�^�̒萔.
	ConstantStruct::stStage CONSTANT = CConstant::GetStage();
};