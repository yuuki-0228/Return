#pragma once

// �I�u�W�F�N�g�^�O���X�g.
enum class enObjectTagList : unsigned int
{
	None,

	Player,				// �v���C���[.
	Enemy,				// �G.
	Gimmick,			// �X�e�[�W�M�~�b�N.
	PotionArea,			// �|�[�V�����G���A.
	Bullet,				// �e.
	ItemBullet,			// �A�C�e���̒e.
	SpecialBullet,		// �����̒e.
	SharpBullet,		// �ђʒe.
	SpillBullet,		// �C���N�e�����ڂ��Ȃ����Ԓe.
	PowerUpBullet,		// �e�̃��x�����グ��e.
	PotionBullet,		// �|�[�V�����e.
	GoodPotionBullet,	// �ǂ��|�[�V�����e.
	BadPotionBullet,	// �����|�[�V�����e.
	BigPotionBullet,	// �傫���|�[�V�����e.
	HealPotionBullet,	// �񕜃|�[�V�����e.
	PoisonPotionBullet,	// �Ń|�[�V�����e.
	RestPotionBullet,	// �X�^�~�i�񕜃|�[�V�����e.

	Max,

} typedef EObjectTag;
