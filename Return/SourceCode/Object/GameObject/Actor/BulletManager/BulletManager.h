#pragma once
#include "..\Actor.h"
#include "BulletBase\BulletBase.h"

/************************************************
*	�e�}�l�[�W���[.
*		���c�F�P.
**/
class CBulletManager
{
public:
	CBulletManager();
	virtual ~CBulletManager();

	// �C���X�^���X�̎擾.
	static CBulletManager* GetInstance();

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

	// �e��S�č폜.
	static void AllDelete();

	// �e�̔���.
	static void Shot( const D3DXPOSITION3& Pos, const std::string& Name );
	// ��������e�̔���.
	static void DropShot( const D3DXPOSITION3& Pos, const std::string& Name );
	// �p�x�ɂ����������ɒe�𔭎˂���( �オ0�x ).
	static void DirectionShot( const D3DXPOSITION3& Pos, const float Ang, const std::string& Name );
	// �S�����ɒe�𔭎�.
	static void AllDirectionShot( const D3DXPOSITION3& Pos, const int Num, const std::string& Name );

	// �e�̕ύX.
	static void BulletChange( CBulletBase* pOldBullet, const std::string& Name, const bool IsEffectPlay = false );

	// �e�̐����擾.
	static int GetBulletNum();

private:
	// �e�̏��̓ǂݍ���.
	static HRESULT StateLood();
	// �^�O�̎擾.
	static EObjectTag GetBulletTag( const std::string& TagName );
	// �e�̏�񂪂��邩���ׂ�.
	static void LoadStateCheck( const std::string& Name );

	// �e�̍쐬.
	static std::shared_ptr<CBulletBase> CreateBullet( const EObjectTag Tag );

	// �`�惊�X�g�̍X�V.
	static void RenderListUpdate();


	// �����e�ʂ��d�Ȃ��Ă���ꍇ�폜����.
	static void SameBulletDelete();
	// �g�p���Ă��Ȃ��e�̍폜.
	static void BulletDelete();

	// �^�C�~���O�o�[�ɏ���n��.
	static void PushTimingBar();

private:
	std::vector<std::shared_ptr<CBulletBase>>		m_pBulletList;			// �e���X�g.
	std::vector<std::pair<float, CActor*>>			m_pRenderList;			// �`�惊�X�g.
	std::unordered_map<std::string, SBulletState>	m_StateList;			// �e�̏�񃊃X�g.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SBullet& CONSTANT = CConstant::GetBullet();
};