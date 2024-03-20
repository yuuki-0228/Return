#pragma once
#include "..\..\Global.h"

/************************************************
*	�萔�\����.
**/
namespace ConstantStruct {
	// �t���[�J�����̒萔�\����.
	struct stFreeCamera {
		float	MOVE_SPEED;				// �J�����̈ړ����x.
		float	ROTATION_SPEED;			// �J�����̉�]���x.

		void Load();
		stFreeCamera() { Load(); }
	} typedef SFreeCamera;

	// �X�e�[�W�̒萔�\����.
	struct stStage {
		float			STAGE_RADIUS;		// �}�b�v�̔��a.
		D3DXPOSITION2	STAGE_CENTER;		// �}�b�v�ɒ��S���W.
		D3DXPOSITION3	STAGE_CENTER3;		// �}�b�v�ɒ��S���W.

		void Load();
		stStage() { Load(); }
	} typedef SStage;

	// �v���C���[�̒萔�\����.
	struct stPlayer {
		float			DAMAGE_COLL_SIZE;			// �_���[�W���󂯂邩�̓����蔻��̑傫��.
		float			HEAVY_COLL_SIZE;			// ���U���̓����蔻��̑傫��.
		float			NORMAL_COLL_SIZE;			// �ʏ�U���̓����蔻��̑傫��.
		float			WEAK_COLL_SIZE;				// ��U���̓����蔻��̑傫��.
		float			HP_UI_POS_Y;				// HP��UI��Y���W�̈ʒu.
		float			STAMINA_UI_POS_Y;			// �X�^�~�i��UI��Y���W�̈ʒu.
		float			HP_UV_Y;					// HP��UV��Y���W�̈ʒu.
		float			ATTACK_STAMINA;				// �U���Ŏg�p����X�^�~�i�̗�.
		float			ATTACK_TYPE_DOWN_STAMINA;	// �U���̃^�C�v�ɉ����Č��炷�X�^�~�i.
		float			RECOVERY_STAMINA;			// �ł��Ԃ������̉񕜂���X�^�~�i�̗�.
		float			STAMINA_RECOVERY_SPEED;		// 1�b�ŉ񕜂���X�^�~�i�̗�.
		float			STAMINA_MAX;				// �X�^�~�i�̏��.
		int				HP_MAX;						// HP�̏��.
		D3DXPOSITION3	POSITION;					// �\������ꏊ.

		void Load();
		stPlayer() { Load(); }
	} typedef SPlayer;

	// �G�̒萔�\����.
	struct stEnemy {
		float			COOL_TIME;			// �����e�̃N�[���^�C��(�b).

		void Load();
		stEnemy() { Load(); }
	} typedef SEnemy;

	// �|�[�V�����G���A�̒萔�\����.
	struct stPotionArea {
		float				AREA_RADIUS;					// �|�[�V�����G���A�̔��a��e�̉��{�̑傫����.
		float				EFFECT_PLAY_TIME;				// �G�t�F�N�g���Đ����Ă������x.
		float				EFFECT_SPEED_MIN;				// �G�t�F�N�g����ɏオ�鑬�x�̍ŏ��l.
		float				EFFECT_SPEED_MAX;				// �G�t�F�N�g����ɏオ�鑬�x�̍ő�l.
		int					EFFECT_PLAY_NUM_MIN;			// ���̃G�t�F�N�g�̍Đ��ōĐ�������̍ŏ��l.
		int					EFFECT_PLAY_NUM_MAX;			// ���̃G�t�F�N�g�̍Đ��ōĐ�������̍ő�l.
		int					START_EFFECT_NUM;				// �J�n���̉~�̃G�t�F�N�g�̐�.
		int					EFFECT_ANIMATION_SPPED;			// �G�t�F�N�g�̃A�j���[�V�������x.

		void Load();
		stPotionArea() { Load(); }
	} typedef SPotionArea;

	// �e�̒萔�\����.
	struct stBullet {
		float				ROTATION_SPEED;					// ��]���x.
		float				NORMAL_STAGE_RADIUS_DISTANCE;	// �ʏ�̒e�̃X�e�[�W�̔��a����ő嗣��鋗��.
		float				STRONG_STAGE_RADIUS_DISTANCE;	// �����e�̃X�e�[�W�̔��a����ő嗣��鋗��.
		float				KEEP_AWAY_SPEED;				// ���ꂳ���Ă������x.
		float				ROTATION_MOVE_SPEED_USERATE;	// ��]�ړ��̎��̃X�s�[�h�̎g�p��( 0.0f ~ 1.0f ).
		float				TIMINGBAR_DISTANCE;				// �^�C�~���O�o�[�ɕ\�������鋗��.
		float				SAME_BULLET_DELETE_DISTANCE;	// �����e�ʂ��d�Ȃ��Ă���Ɣ��肷�鋗��.
		float				SHIELD_ALPHA;					// �V�[���h�̃A���t�@�l.
		float				SHIELD_ATTACK_SPEED_DOWN;		// �V�[���h�ɍU�����̈ړ����x�̌�������.
		float				SPEED_DONW_MAX;					// �ړ����x�̌����̏��.
		int					EFFECT_ANIMATION_SPEED;			// �G�t�F�N�g�̃A�j���[�V�������x.
		int					WEAK_ATTACK_DELETE_CNT;			// �ア�U��������A���ł��ꂽ��폜���邩.
		std::vector<float>	LEVEL_POWER;					// ���x�����Ƃ̕K�v�ȍU���͂̔{��.
		std::vector<float>	SPEED_MAGNIFICATION;			// �ړ��̑��₷�{�����X�g.
		std::vector<float>	ATTACK_MAGNIFICATION;			// �U���͂̑��₷�{�����X�g.

		void Load();
		stBullet() { Load(); }
	} typedef SBullet;
	
	// �e����������e�̒萔�\����.
	struct stPowerUpBullet {
		float				POWER_UP_SPEED_DOWN;			// �e�̃��x�������������̈ړ����x�̌�������.
		float				COOL_TIME;						// �e�̃��x�������������̃N�[���^�C��(�b).

		void Load();
		stPowerUpBullet() { Load(); }
	} typedef SPowerUpBullet;

	// �ђʒe�̒萔�\����.
	struct stSharpBullet {
		float				GET_ADD_ATTACK;					// �e���폜�������ɂ��炤�U���͂̊���.
		float				DELETE_SPEED_UP;				// �e���폜�������̈ړ����x�̏㏸����.
		float				DELERE_ADD_SCALE;				// �e���폜�������ɒǉ�����g�k�l.
		float				HIT_DEC_SCALE;					// �e�������������Ɍ��炷�g�k�l.
		float				HIT_DEC_ATTACK;					// �e�������������Ɍ��炷�U���͂̊���.
		float				DEC_ATTACK_MAX;					// �U���͂̌����̏��.
		float				DELETE_SIZE;					// �e���폜����T�C�Y.

		void Load();
		stSharpBullet() { Load(); }
	} typedef SSharpBullet;

	// �e����������e�̒萔�\����.
	struct stSpillBullet {
		float				GET_ADD_SIZE;					// �e���폜�������ɑ傫������T�C�Y.
		float				DELETE_SPEED_DOWN;				// �e���폜�������̈ړ����x�̌�������.
		float				SPOWN_TIME;						// �e���������鎞��(�b).
		float				SPOWN_DEC_SIZE;					// �e�������������ɏ���������T�C�Y.
		float				DELETE_SPOWN_DEC_SIZE;			// �폜���̒e�̏������̊m�F�̎��Ɍ��炷�T�C�Y.
		float				DELETE_SIZE;					// �e���폜����T�C�Y.
		int					DELETE_SPOWN_MIN;				// �폜���̒e�̍ŏ���.

		void Load();
		stSpillBullet() { Load(); }
	} typedef SSpillBullet;

	// �傫���|�[�V�����̒萔�\����.
	struct stBigPotionBullet {
		float				POTION_TIME_MIN;				// ��ꂽ���̃|�[�V�����̌��ʎ��Ԃ̍ŏ��l(�b).
		float				POTION_TIME_MAX;				// ��ꂽ���̃|�[�V�����̌��ʎ��Ԃ̍ő�l(�b).
		float				DELETE_ADD_TIME;				// �e���폜�������Ƀ|�[�V�����̌��ʎ��Ԃ𑝂₷��(�b).

		void Load();
		stBigPotionBullet() { Load(); }
	} typedef SBigPotionBullet;
}
