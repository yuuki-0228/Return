#pragma once
#include "..\Actor.h"

class CGimmickManager;
class CEnemyManager;
class CBulletManager;

/************************************************
*	ステージマネージャー.
*		﨑田友輝.
**/
class CStageManager
	: public CActor
{
public:
	CStageManager();
	virtual ~CStageManager();

	// 初期化.
	virtual bool Init() override;

	// 更新.
	virtual void Update( const float& DeltaTime ) override;
	// 当たり判定終了後呼び出される更新.
	virtual void LateUpdate( const float& DeltaTime ) override;

	// 描画.
	virtual void Render() override;

private:
	// 描画リストの更新.
	void RenderListUpdate();

private:
	std::vector<std::pair<float, CActor*>>	m_pRenderList;		// 描画リスト.

private:
	// 外部データの定数.
	ConstantStruct::stStage CONSTANT = CConstant::GetStage();
};