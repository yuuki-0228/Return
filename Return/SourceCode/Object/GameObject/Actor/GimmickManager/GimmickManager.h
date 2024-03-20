#pragma once
#include "..\Actor.h"
#include "..\Actor.h"
#include "GimmickBase\GimmickBase.h"

/************************************************
*	ステージギミックマネージャー.
**/
class CGimmickManager
{
public:
	CGimmickManager();
	virtual ~CGimmickManager();

	// インスタンスの取得.
	static CGimmickManager* GetInstance();

	// 初期化.
	static bool Init();

	// 更新.
	static void Update( const float& DeltaTime );

	// 描画.
	static void Render();
	// 描画リストの取得.
	static std::vector<std::pair<float, CActor*>> GetRenderList();

	// ギミックを配置する.
	static void Arrangement( const D3DXPOSITION3& Pos, const std::string& Name );

private:
	// 弾の情報の読み込み.
	static HRESULT StateLood();
	// 弾の情報があるか調べる.
	static void LoadStateCheck( const std::string& Name );

	// ギミックの作成.
	static std::shared_ptr<CGimmickBase> CreateGimmick( const std::string Type );

	// 描画リストの更新.
	static void RenderListUpdate();

private:
	std::vector<std::shared_ptr<CGimmickBase>>		m_pGimmickList;	// ステージギミックリスト.
	std::vector<std::pair<float, CActor*>>			m_pRenderList;	// 描画リスト.
	std::unordered_map<std::string, SGimmickState>	m_StateList;	// ギミックの情報リスト.
};