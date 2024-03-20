#pragma once
#include "..\..\Actor.h"
#include "Enemy\EnemyStruct.h"

class CEnemy;

/************************************************
*	�G�l�~�[�}�l�[�W���[.
**/
class CEnemyManager
{
public:
	CEnemyManager();
	virtual ~CEnemyManager();

	// �C���X�^���X�̎擾.
	static CEnemyManager* GetInstance();

	// ������.
	static bool Init();

	// �X�V.
	static void Update( const float& DeltaTime );

	// �`��.
	static void Render();
	// �`�惊�X�g�̎擾.
	static std::vector<std::pair<float, CActor*>> GetRenderList();

	// �X�|�[��������.
	static void Spown( const D3DXPOSITION3& Pos, const std::string& Name );

private:
	// �e�̏��̓ǂݍ���.
	static HRESULT StateLood();
	// �e�̏�񂪂��邩���ׂ�.
	static void LoadStateCheck( const std::string& Name );

	// �`�惊�X�g�̍X�V.
	static void RenderListUpdate();

	// �g�p���Ă��Ȃ��G�̍폜.
	static void EnemyDelete();

private:
	std::vector<std::shared_ptr<CEnemy>>			m_pEnemyList;	// �G���X�g.
	std::vector<std::pair<float, CActor*>>			m_pRenderList;	// �`�惊�X�g.
	std::unordered_map<std::string, SEnemyState>	m_StateList;	// �G�̏�񃊃X�g.
};