#pragma once
#include "..\..\Actor.h"
#include "Enemy\EnemyStruct.h"

class CEnemy;

/************************************************
*	エネミーマネージャー.
**/
class CEnemyManager
{
public:
	CEnemyManager();
	virtual ~CEnemyManager();

	// インスタンスの取得.
	static CEnemyManager* GetInstance();

	// 初期化.
	static bool Init();

	// 更新.
	static void Update( const float& DeltaTime );

	// 描画.
	static void Render();
	// 描画リストの取得.
	static std::vector<std::pair<float, CActor*>> GetRenderList();

	// スポーンさせる.
	static void Spown( const D3DXPOSITION3& Pos, const std::string& Name );

private:
	// 弾の情報の読み込み.
	static HRESULT StateLood();
	// 弾の情報があるか調べる.
	static void LoadStateCheck( const std::string& Name );

	// 描画リストの更新.
	static void RenderListUpdate();

	// 使用していない敵の削除.
	static void EnemyDelete();

private:
	std::vector<std::shared_ptr<CEnemy>>			m_pEnemyList;	// 敵リスト.
	std::vector<std::pair<float, CActor*>>			m_pRenderList;	// 描画リスト.
	std::unordered_map<std::string, SEnemyState>	m_StateList;	// 敵の情報リスト.
};