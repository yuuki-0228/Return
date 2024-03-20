#pragma once
#include "BulletStruct.h"
#include "..\..\Actor.h"
#include "..\..\..\..\..\Common\Sprite\Sprite.h"

/**************************************************
*	弾ベースクラス.
*		﨑田友輝.
**/
class CBulletBase
	: public CActor
{
public:
	CBulletBase();
	virtual ~CBulletBase();

	// 更新.
	virtual void Update( const float& DeltaTime ) override final;

	// 描画.
	virtual void Render() override final;
	
	// 終了したか取得.
	bool GetIsEnd() const { return m_IsEnd; }
	// 敵にダメージを与えることのできる弾か取得.
	bool GetIsDamageBullet() const { return m_HitBackCnt > 0; }
	// 回転して移動しているか取得.
	bool GetIsRotationMove() const { return m_IsRotationMove; }

	// 弾のタグを取得.
	EObjectTag GetBulletTag() const { return m_BulletState.BulletTag; }

	// プレイヤーに対してのダメージを取得.
	int GetPlayerDamage() const { return m_BulletState.PlayerDamage; }
	// 攻撃力の取得.
	float GetAttack() const { return m_BulletState.Attack; }
	// 弾の情報を取得.
	SBulletState GetBulletState() const { return m_BulletState; }
	// 移動ベクトルの取得.
	D3DXVECTOR3 GetMoveVecotr() const { return m_MoveVector; }

	// 弾のレベルの設定.
	bool SetBulletLv( const int Lv );
	// 弾のレベルを上げる.
	bool BulletLvUp();

	// ステージを回転するように発射.
	void StageRotationShot( const D3DXPOSITION3& Pos, const SBulletState& State );
	// 通常の発射.
	void NormalShot( const D3DXPOSITION3& Pos, const SBulletState& State );
	// 落下するように発射.
	void DropShot( const D3DXPOSITION3& Pos, const SBulletState& State );
	// 角度にあった方向に弾を発射する( 上が0度 ).
	void DirectionShot( const D3DXPOSITION3& Pos, float Ang, const SBulletState& State );

	// ステージを回転するように打ち返す.
	void RotationHitBack( const int AttackNo );
	// マウスカーソルの方向に打ち返す.
	void AttackHitBack( const int AttackNo );

	// 弾を消す.
	void Delete( const EObjectTag DeleteTag = EObjectTag::None, const bool CheckSkip = false, const bool IsEffectPlay = true );

protected:
	// 弾の状態の初期化.
	virtual	void InitBulletState( const SBulletState& State ) = 0;

	// 弾を削除するか確認.
	virtual bool DeleteCheck( const EObjectTag DeleteTag ) { return true; }
	// ステージを回転するように打ち返せるかの確認.
	virtual bool RotationHitBackCheck( const int AttackNo ) { return true; }
	// マウスカーソルの方向に打ち返す.
	virtual bool AttackHitBackCheck( const int AttackNo ) { return true; }

	// 弾のスキルの更新.
	virtual void SkillUpdate() {}
	// レベルエフェクトの更新.
	virtual void LvEffectUpdate() = 0;

private:
	// 弾を回転させる.
	void BulletRotation();

	// 通常の移動.
	void NormalMove();
	// 落下する弾の移動.
	void DropMove();
	// ステージを回転する移動( 時計回り ).
	void LeftRotationMove();
	// ステージを回転する移動( 反時計回り ).
	void RightRotationMove();

	// エフェクトの更新.
	void EffectUpdate();

	// ステージ外かチェック.
	void StageOutCheck();
	// 弾の動作が完全に終了したか確認.
	void EndCheck();

protected:
	CSprite*							m_pSprite;				// 弾の画像.
	SSpriteRenderState					m_SpriteState;			// 弾の画像の情報.
	D3DXVECTOR3							m_MoveVector;			// 移動ベクトル.
	std::vector<CSprite*>				m_pEffectSpriteList;	// エフェクトの画像リスト.
	std::vector<CSprite*>				m_pLvEffectSpriteList;	// 弾のレベルエフェクトの画像リスト.
	std::vector<SSpriteRenderState>		m_EffectStateList;		// エフェクトの画像の情報リスト.
	std::vector<SSpriteRenderState>		m_LvEffectStateList;	// 弾のレベルエフェクトの画像の情報リスト.
	SBulletState						m_BulletState;			// 弾の情報.
	SBulletState						m_StartBulletState;		// 開始時の弾の情報.
	EDirection							m_RotationDire;			// 回転方向.
	float								m_RotationAng;			// 回転用の角度.
	float								m_RotationRadius;		// 回転用の半径.
	int									m_HitBackCnt;			// 跳ね返した回数.
	int									m_BulletLv;				// 弾のレベル.
	int									m_WeakAttackCnt;		// 弱攻撃を連続でされた回数.
	bool								m_IsDrop;				// 落下する弾か.
	bool								m_IsRotationMove;		// 回転する移動を行うか.
	bool								m_IsEnd;				// 終了したか.

protected:
	// 外部データの定数.
	const ConstantStruct::SBullet& CONSTANT = CConstant::GetBullet();
};
