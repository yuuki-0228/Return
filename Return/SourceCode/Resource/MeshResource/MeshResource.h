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
*	メッシュリソースクラス.
*		�ｱ田友輝.
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

	// インスタンスの取得.
	static CMeshResource* GetInstance();

	// メッシュの読み込み.
	static HRESULT MeshLoad();

	// メッシュの取得.
	static CCollisionMesh*	GetCollision(	const std::string& name );
	static CSkyBoxMesh*		GetSkyBox(		const std::string& name );
	static CStaticMesh*		GetStatic(		const std::string& name );
	static CSkinMesh*		GetSkin(		const std::string& name, LPD3DXANIMATIONCONTROLLER* pOutAC = nullptr );

	// 保存しているメッシュリストの取得.
	static Mesh_List	GetCollisionList()	{ return GetInstance()->m_CollisionMeshNames;	}
	static Mesh_List	GetSkyBoxList()		{ return GetInstance()->m_SkyBoxMeshNames;	}
	static Mesh_List	GetStaticList()		{ return GetInstance()->m_StaticMeshNames;	}
	static Mesh_List	GetSkinList()		{ return GetInstance()->m_SkinMeshNames;	}

private:
	CollisionMesh_map	m_CollisionMeshList;	// コリジョンメッシュのリスト.
	SkyBoxMesh_map		m_SkyBoxMeshList;		// スカイボックスメッシュのリスト.
	StaticMesh_map		m_StaticMeshList;		// スタティックメッシュのリスト.
	SkinMesh_map		m_SkinMeshList;			// スキンメッシュのリスト.
	Mesh_List			m_CollisionMeshNames;	// コリジョンメッシュの名前リスト.
	Mesh_List			m_SkyBoxMeshNames;		// スカイボックスメッシュの名前リスト.
	Mesh_List			m_StaticMeshNames;		// スタティックメッシュの名前リスト.
	Mesh_List			m_SkinMeshNames;		// スキンメッシュの名前リスト.
	std::mutex			m_Mutex;
	bool				m_IsLoadEnd;			// 読み込みが終わったか.

private:
	// コピー・ムーブコンストラクタ, 代入演算子の削除.
	CMeshResource( const CMeshResource& )				= delete;
	CMeshResource& operator= ( const CMeshResource& )	= delete;
	CMeshResource( CMeshResource&& )					= delete;
	CMeshResource& operator= ( CMeshResource&& )		= delete;
};
