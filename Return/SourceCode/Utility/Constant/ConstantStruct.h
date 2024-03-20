#pragma once
#include "..\..\Global.h"

/************************************************
*	定数構造体.
**/
namespace ConstantStruct {
	// フリーカメラの定数構造体.
	struct stFreeCamera {
		float	MOVE_SPEED;				// カメラの移動速度.
		float	ROTATION_SPEED;			// カメラの回転速度.

		void Load();
		stFreeCamera() { Load(); }
	} typedef SFreeCamera;

	// ステージの定数構造体.
	struct stStage {
		float			STAGE_RADIUS;		// マップの半径.
		D3DXPOSITION2	STAGE_CENTER;		// マップに中心座標.
		D3DXPOSITION3	STAGE_CENTER3;		// マップに中心座標.

		void Load();
		stStage() { Load(); }
	} typedef SStage;

	// プレイヤーの定数構造体.
	struct stPlayer {
		float			DAMAGE_COLL_SIZE;			// ダメージを受けるかの当たり判定の大きさ.
		float			HEAVY_COLL_SIZE;			// 強攻撃の当たり判定の大きさ.
		float			NORMAL_COLL_SIZE;			// 通常攻撃の当たり判定の大きさ.
		float			WEAK_COLL_SIZE;				// 弱攻撃の当たり判定の大きさ.
		float			HP_UI_POS_Y;				// HPのUIのY座標の位置.
		float			STAMINA_UI_POS_Y;			// スタミナのUIのY座標の位置.
		float			HP_UV_Y;					// HPのUVのY座標の位置.
		float			ATTACK_STAMINA;				// 攻撃で使用するスタミナの量.
		float			ATTACK_TYPE_DOWN_STAMINA;	// 攻撃のタイプに応じて減らすスタミナ.
		float			RECOVERY_STAMINA;			// 打ち返した時の回復するスタミナの量.
		float			STAMINA_RECOVERY_SPEED;		// 1秒で回復するスタミナの量.
		float			STAMINA_MAX;				// スタミナの上限.
		int				HP_MAX;						// HPの上限.
		D3DXPOSITION3	POSITION;					// 表示する場所.

		void Load();
		stPlayer() { Load(); }
	} typedef SPlayer;

	// 敵の定数構造体.
	struct stEnemy {
		float			COOL_TIME;			// 強い弾のクールタイム(秒).

		void Load();
		stEnemy() { Load(); }
	} typedef SEnemy;

	// ポーションエリアの定数構造体.
	struct stPotionArea {
		float				AREA_RADIUS;					// ポーションエリアの半径を弾の何倍の大きさか.
		float				EFFECT_PLAY_TIME;				// エフェクトを再生していく速度.
		float				EFFECT_SPEED_MIN;				// エフェクトを上に上がる速度の最小値.
		float				EFFECT_SPEED_MAX;				// エフェクトを上に上がる速度の最大値.
		int					EFFECT_PLAY_NUM_MIN;			// 一回のエフェクトの再生で再生する個数の最小値.
		int					EFFECT_PLAY_NUM_MAX;			// 一回のエフェクトの再生で再生する個数の最大値.
		int					START_EFFECT_NUM;				// 開始時の円のエフェクトの数.
		int					EFFECT_ANIMATION_SPPED;			// エフェクトのアニメーション速度.

		void Load();
		stPotionArea() { Load(); }
	} typedef SPotionArea;

	// 弾の定数構造体.
	struct stBullet {
		float				ROTATION_SPEED;					// 回転速度.
		float				NORMAL_STAGE_RADIUS_DISTANCE;	// 通常の弾のステージの半径から最大離れる距離.
		float				STRONG_STAGE_RADIUS_DISTANCE;	// 強い弾のステージの半径から最大離れる距離.
		float				KEEP_AWAY_SPEED;				// 離れさせていく速度.
		float				ROTATION_MOVE_SPEED_USERATE;	// 回転移動の時のスピードの使用率( 0.0f ~ 1.0f ).
		float				TIMINGBAR_DISTANCE;				// タイミングバーに表示させる距離.
		float				SAME_BULLET_DELETE_DISTANCE;	// 同じ弾通し重なっていると判定する距離.
		float				SHIELD_ALPHA;					// シールドのアルファ値.
		float				SHIELD_ATTACK_SPEED_DOWN;		// シールドに攻撃時の移動速度の減少割合.
		float				SPEED_DONW_MAX;					// 移動速度の減少の上限.
		int					EFFECT_ANIMATION_SPEED;			// エフェクトのアニメーション速度.
		int					WEAK_ATTACK_DELETE_CNT;			// 弱い攻撃が何回連続でされたら削除するか.
		std::vector<float>	LEVEL_POWER;					// レベルごとの必要な攻撃力の倍率.
		std::vector<float>	SPEED_MAGNIFICATION;			// 移動の増やす倍率リスト.
		std::vector<float>	ATTACK_MAGNIFICATION;			// 攻撃力の増やす倍率リスト.

		void Load();
		stBullet() { Load(); }
	} typedef SBullet;
	
	// 弾を強化する弾の定数構造体.
	struct stPowerUpBullet {
		float				POWER_UP_SPEED_DOWN;			// 弾のレベルをあげた時の移動速度の減少割合.
		float				COOL_TIME;						// 弾のレベルをあげた時のクールタイム(秒).

		void Load();
		stPowerUpBullet() { Load(); }
	} typedef SPowerUpBullet;

	// 貫通弾の定数構造体.
	struct stSharpBullet {
		float				GET_ADD_ATTACK;					// 弾を削除した時にもらう攻撃力の割合.
		float				DELETE_SPEED_UP;				// 弾を削除した時の移動速度の上昇割合.
		float				DELERE_ADD_SCALE;				// 弾を削除した時に追加する拡縮値.
		float				HIT_DEC_SCALE;					// 弾が当たった時に減らす拡縮値.
		float				HIT_DEC_ATTACK;					// 弾が当たった時に減らす攻撃力の割合.
		float				DEC_ATTACK_MAX;					// 攻撃力の減少の上限.
		float				DELETE_SIZE;					// 弾を削除するサイズ.

		void Load();
		stSharpBullet() { Load(); }
	} typedef SSharpBullet;

	// 弾を召喚する弾の定数構造体.
	struct stSpillBullet {
		float				GET_ADD_SIZE;					// 弾を削除した時に大きくするサイズ.
		float				DELETE_SPEED_DOWN;				// 弾を削除した時の移動速度の減少割合.
		float				SPOWN_TIME;						// 弾を召喚する時間(秒).
		float				SPOWN_DEC_SIZE;					// 弾を召喚した時に小さくするサイズ.
		float				DELETE_SPOWN_DEC_SIZE;			// 削除時の弾の召喚数の確認の時に減らすサイズ.
		float				DELETE_SIZE;					// 弾を削除するサイズ.
		int					DELETE_SPOWN_MIN;				// 削除時の弾の最小数.

		void Load();
		stSpillBullet() { Load(); }
	} typedef SSpillBullet;

	// 大きいポーションの定数構造体.
	struct stBigPotionBullet {
		float				POTION_TIME_MIN;				// 壊れた時のポーションの効果時間の最小値(秒).
		float				POTION_TIME_MAX;				// 壊れた時のポーションの効果時間の最大値(秒).
		float				DELETE_ADD_TIME;				// 弾を削除した時にポーションの効果時間を増やす量(秒).

		void Load();
		stBigPotionBullet() { Load(); }
	} typedef SBigPotionBullet;
}
