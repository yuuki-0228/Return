#pragma once

// オブジェクトタグリスト.
enum class enObjectTagList : unsigned int
{
	None,

	Player,				// プレイヤー.
	Enemy,				// 敵.
	Gimmick,			// ステージギミック.
	PotionArea,			// ポーションエリア.
	Bullet,				// 弾.
	ItemBullet,			// アイテムの弾.
	SpecialBullet,		// 強いの弾.
	SharpBullet,		// 貫通弾.
	SpillBullet,		// インク弾をこぼしながら飛ぶ弾.
	PowerUpBullet,		// 弾のレベルを上げる弾.
	PotionBullet,		// ポーション弾.
	GoodPotionBullet,	// 良いポーション弾.
	BadPotionBullet,	// 悪いポーション弾.
	BigPotionBullet,	// 大きいポーション弾.
	HealPotionBullet,	// 回復ポーション弾.
	PoisonPotionBullet,	// 毒ポーション弾.
	RestPotionBullet,	// スタミナ回復ポーション弾.

	Max,

} typedef EObjectTag;
