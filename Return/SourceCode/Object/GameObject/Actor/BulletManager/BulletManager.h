#pragma once
#include "..\Actor.h"
#include "BulletBase\BulletBase.h"

/************************************************
*	弾マネージャー.
*		﨑田友輝.
**/
class CBulletManager
{
public:
	CBulletManager();
	virtual ~CBulletManager();

	// インスタンスの取得.
	static CBulletManager* GetInstance();

	// 初期化.
	static bool Init();

	// 更新.
	static void Update( const float& DeltaTime );
	// 当たり判定終了後呼び出される更新.
	static void LateUpdate( const float& DeltaTime );

	// 描画.
	static void Render();
	// 描画リストの取得.
	static std::vector<std::pair<float, CActor*>> GetRenderList();

	// 弾を全て削除.
	static void AllDelete();

	// 弾の発射.
	static void Shot( const D3DXPOSITION3& Pos, const std::string& Name );
	// 落下する弾の発射.
	static void DropShot( const D3DXPOSITION3& Pos, const std::string& Name );
	// 角度にあった方向に弾を発射する( 上が0度 ).
	static void DirectionShot( const D3DXPOSITION3& Pos, const float Ang, const std::string& Name );
	// 全方向に弾を発射.
	static void AllDirectionShot( const D3DXPOSITION3& Pos, const int Num, const std::string& Name );

	// 弾の変更.
	static void BulletChange( CBulletBase* pOldBullet, const std::string& Name, const bool IsEffectPlay = false );

	// 弾の数を取得.
	static int GetBulletNum();

private:
	// 弾の情報の読み込み.
	static HRESULT StateLood();
	// タグの取得.
	static EObjectTag GetBulletTag( const std::string& TagName );
	// 弾の情報があるか調べる.
	static void LoadStateCheck( const std::string& Name );

	// 弾の作成.
	static std::shared_ptr<CBulletBase> CreateBullet( const EObjectTag Tag );

	// 描画リストの更新.
	static void RenderListUpdate();


	// 同じ弾通し重なっている場合削除する.
	static void SameBulletDelete();
	// 使用していない弾の削除.
	static void BulletDelete();

	// タイミングバーに情報を渡す.
	static void PushTimingBar();

private:
	std::vector<std::shared_ptr<CBulletBase>>		m_pBulletList;			// 弾リスト.
	std::vector<std::pair<float, CActor*>>			m_pRenderList;			// 描画リスト.
	std::unordered_map<std::string, SBulletState>	m_StateList;			// 弾の情報リスト.

private:
	// 外部データの定数.
	const ConstantStruct::SBullet& CONSTANT = CConstant::GetBullet();
};