#pragma once
#include "..\..\Global.h"
#include <unordered_map>
#include <string>
#include <memory>
#include <mutex>
#include <filesystem>

class CSkinMesh;
class CStaticMesh;
class CSkyBoxMesh;
class CCollisionMesh;

/************************************************
*	���b�V�����\�[�X�N���X.
*		���c�F�P.
**/
class CMeshResource final
{
	using CollisionMesh_map	= std::unordered_map<std::string, std::unique_ptr<CCollisionMesh>>;
	using SkyBoxMesh_map	= std::unordered_map<std::string, std::unique_ptr<CSkyBoxMesh>>;
	using StaticMesh_map	= std::unordered_map<std::string, std::unique_ptr<CStaticMesh>>;
	using SkinMesh_map		= std::unordered_map<std::string, std::unique_ptr<CSkinMesh>>;
	using Mesh_List			= std::vector<std::string>;

public:
	CMeshResource();
	~CMeshResource();

	// �C���X�^���X�̎擾.
	static CMeshResource* GetInstance();

	// ���b�V���̓ǂݍ���.
	static HRESULT MeshLoad();

	// ���b�V���̎擾.
	static CCollisionMesh*	GetCollision(	const std::string& name );
	static CSkyBoxMesh*		GetSkyBox(		const std::string& name );
	static CStaticMesh*		GetStatic(		const std::string& name );
	static CSkinMesh*		GetSkin(		const std::string& name, LPD3DXANIMATIONCONTROLLER* pOutAC = nullptr );

	// �ۑ����Ă��郁�b�V�����X�g�̎擾.
	static Mesh_List	GetCollisionList()	{ return GetInstance()->m_CollisionMeshNames;	}
	static Mesh_List	GetSkyBoxList()		{ return GetInstance()->m_SkyBoxMeshNames;	}
	static Mesh_List	GetStaticList()		{ return GetInstance()->m_StaticMeshNames;	}
	static Mesh_List	GetSkinList()		{ return GetInstance()->m_SkinMeshNames;	}

private:
	CollisionMesh_map	m_CollisionMeshList;	// �R���W�������b�V���̃��X�g.
	SkyBoxMesh_map		m_SkyBoxMeshList;		// �X�J�C�{�b�N�X���b�V���̃��X�g.
	StaticMesh_map		m_StaticMeshList;		// �X�^�e�B�b�N���b�V���̃��X�g.
	SkinMesh_map		m_SkinMeshList;			// �X�L�����b�V���̃��X�g.
	Mesh_List			m_CollisionMeshNames;	// �R���W�������b�V���̖��O���X�g.
	Mesh_List			m_SkyBoxMeshNames;		// �X�J�C�{�b�N�X���b�V���̖��O���X�g.
	Mesh_List			m_StaticMeshNames;		// �X�^�e�B�b�N���b�V���̖��O���X�g.
	Mesh_List			m_SkinMeshNames;		// �X�L�����b�V���̖��O���X�g.
	std::mutex			m_Mutex;
	bool				m_IsLoadEnd;			// �ǂݍ��݂��I�������.

private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CMeshResource( const CMeshResource& )				= delete;
	CMeshResource& operator= ( const CMeshResource& )	= delete;
	CMeshResource( CMeshResource&& )					= delete;
	CMeshResource& operator= ( CMeshResource&& )		= delete;
};
