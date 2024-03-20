#pragma once
#include "PotionArea\PotionArea.h"

/************************************************
*	ポーションエリアマネージャークラス.
**/
class CPotionAreaManager final
{
public:
	CPotionAreaManager();
	~CPotionAreaManager();

	// インスタンスの取得.
	static CPotionAreaManager* GetInstance();

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

	// エリアの追加.
	static void PushArea( const D3DXPOSITION3& Pos, const float Radius, const std::string& Name, const float Time = INIT_FLOAT );

private:
	// 弾の情報の読み込み.
	static HRESULT StateLood();
	// 弾の情報があるか調べる.
	static void LoadStateCheck( const std::string& Name );

	// ポーションのタイプを取得.
	static int GetPotionType( const std::string& Type );
	// 描画リストの更新.
	static void RenderListUpdate();

	// 使用していない弾の削除.
	static void AreaDelete();

private:
	std::vector<std::shared_ptr<CPotionArea>>			m_pAreaList;	// エリアリスト.
	std::vector<std::pair<float, CActor*>>				m_pRenderList;	// 描画リスト.
	std::unordered_map<std::string, SPotionAreaState>	m_StateList;	// エリアの情報リスト.

private:
	// 外部データの定数.
	ConstantStruct::stStage CONSTANT = CConstant::GetStage();
};