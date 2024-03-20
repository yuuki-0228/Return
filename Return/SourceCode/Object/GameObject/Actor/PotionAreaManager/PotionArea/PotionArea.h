#pragma once
#include "..\..\Actor.h"
#include "PotionAreaStruct.h"

/************************************************
*	ポーションエリアクラス.
**/
class CPotionArea final
	: public CActor
{
public:
	CPotionArea();
	virtual ~CPotionArea();
	
	// 初期化.
	virtual bool Init() override;

	// 更新.
	virtual void Update( const float& DeltaTime ) override;
	// 当たり判定終了後呼び出される更新.
	virtual void LateUpdate( const float& DeltaTime ) override;

	// 描画.
	virtual void Render() override;

	// 当たり判定関数.
	virtual void Collision( CActor* pActor ) override;

	// ポーションエリアの作成.
	void CreateArea( const D3DXPOSITION3& Pos, const float Radius, const SPotionAreaState& State );

	// 完全に終了したか取得.
	bool GetIsEnd() const { return m_IsEnd; }

protected:
	// 当たり判定の初期化.
	//	Init関数の最後に呼ぶ.
	virtual void InitCollision();
	// 当たり判定の更新.
	//	Update関数の最後に呼ぶ.
	virtual void UpdateCollision();

private:
	// ポーションエフェクトの更新.
	void PotionEffectUpdate();

	// エフェクトの付与時間の更新.
	void EffectTimeUpdate();
	// エフェクトの付与時間のリセット.
	void EffectTimeReset();

	// エフェクトの再生.
	void EffectPlay( const D3DXPOSITION3& Pos, const bool IsStart = false );
	// 使用していないエフェクトの削除.
	void EffectDelete();

	// エフェクトが完全に終了したか確認.
	bool EndCheck();
	// ポーションエリアが終了したか確認.
	void PotionAreaEndCheck();

private:
	CSprite*								m_pEffectSprite;		// エフェクト画像.
	SSpriteRenderState						m_EffectStateBase;		// エフェクトの情報ベース.
	std::vector<SSpriteRenderState>			m_EffectStateList;		// エフェクトの情報リスト.
	SPotionAreaState						m_StartAreaState;		// 開始時のポーションエリアの情報.
	SPotionAreaState						m_AreaState;			// ポーションエリアの情報.
	std::vector<std::pair<float, CActor*>>	m_pRenderList;			// 描画リスト.
	float									m_CollRadius;			// 当たり判定の半径.
	float									m_PotionEffectTimeCnt;	// ポーションエフェクトの待ち時間用カウント.
	float									m_PlayerCoolTime;		// プレイヤーのクールタイム.
	bool									m_IsEnd;				// 完全に終了したか.
	bool									m_IsHit;				// 当たり判定が当たったか.

private:
	// 外部データの定数.
	const ConstantStruct::SPotionArea& CONSTANT = CConstant::GetPotionArea();
};