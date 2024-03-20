#include "ConstantStruct.h"
#include "..\FileManager\FileManager.h"

// �ϐ����𕶎���Ŏ擾�}�N��.
#define TO_STRING( VariableName ) # VariableName

namespace {
	constexpr int _COMMENT	= 0; // �R�����g�̈ʒu.
	constexpr int _DATA		= 1; // �f�[�^�̈ʒu.
	constexpr int _FIRST	= 1; // 1�ڂ̈ʒu.
	constexpr int _SECOND	= 2; // 2�ڂ̈ʒu.
	constexpr int _X		= 1; // x�̈ʒu.
	constexpr int _Y		= 2; // y�̈ʒu.
	constexpr int _Z		= 3; // z�̈ʒu.
	constexpr int _W		= 3; // w�̈ʒu.
	constexpr int _R		= 1; // �Ԃ̈ʒu.
	constexpr int _G		= 2; // �΂̈ʒu.
	constexpr int _B		= 3; // �̈ʒu.
	constexpr int _A		= 4; // �A���t�@�l�̈ʒu.

	// �t�@�C���p�X�̎擾.
	std::string GetFilePath( const std::string& Name ) {
		const std::string& FilePath = "Data\\Parameter\\Constant\\" + Name + ".json";
		CLog::PushLog( FilePath + " �ǂݍ��� �F ����");
		return FilePath;
	}

	// �z��̃T�C�Y���擾.
	int GetSize( const json& j ) {
		// ��ԍŏ��ɃR�����g�����邽�ߗv�f����1���炵�Ă���.
		return static_cast<int>( j.size() ) - 1;
	}

	// �����������Ȃ��悤��std::string�ɕϊ����Ď擾.
	std::string GetString( const std::string& string ) {
		// json��UTF8�Ȃ��߈�x�������ϊ�����.
		std::wstring wString = StringConversion::to_wString( string, ECodePage::UTF8 );
		return StringConversion::to_String( wString );
	}
}

//---------------------------.
// �t���[�J�����萔�̓ǂݍ��݊֐�.
//---------------------------.
void ConstantStruct::stFreeCamera::Load()
{
	json j = FileManager::JsonLoad( GetFilePath ( "CameraFree" ) );
	MOVE_SPEED				= j[TO_STRING( MOVE_SPEED				)][_DATA];
	ROTATION_SPEED			= j[TO_STRING( ROTATION_SPEED			)][_DATA];
}

//---------------------------.
// �X�e�[�W�̒萔�̓ǂݍ��݊֐�.
//---------------------------.
void ConstantStruct::stStage::Load()
{
	json j = FileManager::JsonLoad( GetFilePath( "Stage" ) );
	STAGE_RADIUS			= j[TO_STRING( STAGE_RADIUS				)][_DATA];
	STAGE_CENTER.x			= j[TO_STRING( STAGE_CENTER				)][_X];
	STAGE_CENTER.y			= j[TO_STRING( STAGE_CENTER				)][_Y];
	STAGE_CENTER3.x			= j[TO_STRING( STAGE_CENTER				)][_X];
	STAGE_CENTER3.y			= j[TO_STRING( STAGE_CENTER				)][_Y];
	STAGE_CENTER3.z			= INIT_FLOAT;
}

//---------------------------.
// �v���C���[�̒萔�̓ǂݍ��݊֐�.
//---------------------------.
void ConstantStruct::stPlayer::Load()
{
	json j = FileManager::JsonLoad( GetFilePath( "Player" ) );
	DAMAGE_COLL_SIZE			= j[TO_STRING( DAMAGE_COLL_SIZE			)][_DATA];
	HEAVY_COLL_SIZE				= j[TO_STRING( HEAVY_COLL_SIZE			)][_DATA];
	NORMAL_COLL_SIZE			= j[TO_STRING( NORMAL_COLL_SIZE			)][_DATA];
	WEAK_COLL_SIZE				= j[TO_STRING( WEAK_COLL_SIZE			)][_DATA];
	HP_UI_POS_Y					= j[TO_STRING( HP_UI_POS_Y				)][_DATA];
	STAMINA_UI_POS_Y			= j[TO_STRING( STAMINA_UI_POS_Y			)][_DATA];
	HP_UV_Y						= j[TO_STRING( HP_UV_Y					)][_DATA];
	ATTACK_STAMINA				= j[TO_STRING( ATTACK_STAMINA			)][_DATA];
	ATTACK_TYPE_DOWN_STAMINA	= j[TO_STRING( ATTACK_TYPE_DOWN_STAMINA )][_DATA];
	RECOVERY_STAMINA			= j[TO_STRING( RECOVERY_STAMINA			)][_DATA];
	STAMINA_RECOVERY_SPEED		= j[TO_STRING( STAMINA_RECOVERY_SPEED	)][_DATA];
	STAMINA_MAX					= j[TO_STRING( STAMINA_MAX				)][_DATA];
	HP_MAX						= j[TO_STRING( HP_MAX					)][_DATA];
	POSITION.x					= j[TO_STRING( POSITION					)][_X];
	POSITION.y					= j[TO_STRING( POSITION					)][_Y];
	POSITION.z					= j[TO_STRING( POSITION					)][_Z];
}

//---------------------------.
// �G�̒萔�̓ǂݍ��݊֐�.
//---------------------------.
void ConstantStruct::stEnemy::Load()
{
	json j = FileManager::JsonLoad( GetFilePath( "Enemy" ) );
	COOL_TIME				= j[TO_STRING( COOL_TIME				)][_DATA];
}

//---------------------------.
// �|�[�V�����G���A�̒萔�̓ǂݍ��݊֐�.
//---------------------------.
void ConstantStruct::stPotionArea::Load()
{
	json j = FileManager::JsonLoad( GetFilePath( "PotionArea" ) );
	AREA_RADIUS						= j[TO_STRING( AREA_RADIUS					)][_DATA];
	EFFECT_PLAY_TIME				= j[TO_STRING( EFFECT_PLAY_TIME				)][_DATA];
	EFFECT_SPEED_MIN				= j["EFFECT_SPEED"							 ][_FIRST];
	EFFECT_SPEED_MAX				= j["EFFECT_SPEED"							 ][_SECOND];
	EFFECT_PLAY_NUM_MIN				= j["EFFECT_PLAY_NUM"						 ][_FIRST];
	EFFECT_PLAY_NUM_MAX				= j["EFFECT_PLAY_NUM"						 ][_SECOND];
	START_EFFECT_NUM				= j[TO_STRING( START_EFFECT_NUM				)][_DATA];
	EFFECT_ANIMATION_SPPED			= j[TO_STRING( EFFECT_ANIMATION_SPPED		)][_DATA];
}

//---------------------------.
// �e�̒萔�̓ǂݍ��݊֐�.
//---------------------------.
void ConstantStruct::stBullet::Load()
{
	json j = FileManager::JsonLoad( GetFilePath( "Bullet" ) );
	ROTATION_SPEED					= j[TO_STRING( ROTATION_SPEED				)][_DATA];
	NORMAL_STAGE_RADIUS_DISTANCE	= j[TO_STRING( NORMAL_STAGE_RADIUS_DISTANCE )][_DATA];
	STRONG_STAGE_RADIUS_DISTANCE	= j[TO_STRING( STRONG_STAGE_RADIUS_DISTANCE )][_DATA];
	KEEP_AWAY_SPEED					= j[TO_STRING( KEEP_AWAY_SPEED				)][_DATA];
	ROTATION_MOVE_SPEED_USERATE		= j[TO_STRING( ROTATION_MOVE_SPEED_USERATE	)][_DATA];
	TIMINGBAR_DISTANCE				= j[TO_STRING( TIMINGBAR_DISTANCE			)][_DATA];
	SAME_BULLET_DELETE_DISTANCE		= j[TO_STRING( SAME_BULLET_DELETE_DISTANCE	)][_DATA];
	SHIELD_ALPHA					= j[TO_STRING( SHIELD_ALPHA					)][_DATA];
	SHIELD_ATTACK_SPEED_DOWN		= j[TO_STRING( SHIELD_ATTACK_SPEED_DOWN		)][_DATA];
	SPEED_DONW_MAX					= j[TO_STRING( SPEED_DONW_MAX				)][_DATA];
	EFFECT_ANIMATION_SPEED			= j[TO_STRING( EFFECT_ANIMATION_SPEED		)][_DATA];
	WEAK_ATTACK_DELETE_CNT			= j[TO_STRING( WEAK_ATTACK_DELETE_CNT		)][_DATA];
	const int LvSize				= GetSize( j[TO_STRING( LEVEL_POWER			)] );
	LEVEL_POWER.resize( LvSize );
	for ( int i = 0; i < LvSize; ++i ) {
		const int No				= i + 1;
		LEVEL_POWER[i]				= j[TO_STRING( LEVEL_POWER					)][No];
	}
	const int SpeedSize				= GetSize( j[TO_STRING( SPEED_MAGNIFICATION )] );
	SPEED_MAGNIFICATION.resize( SpeedSize );
	for ( int i = 0; i < SpeedSize; ++i ) {
		const int No				= i + 1;
		SPEED_MAGNIFICATION[i]		= j[TO_STRING( SPEED_MAGNIFICATION			)][No];
	}
	const int AttackSize			= GetSize( j[TO_STRING( ATTACK_MAGNIFICATION )] );
	ATTACK_MAGNIFICATION.resize( AttackSize );
	for ( int i = 0; i < AttackSize; ++i ) {
		const int No				= i + 1;
		ATTACK_MAGNIFICATION[i]		= j[TO_STRING( ATTACK_MAGNIFICATION			)][No];
	}
}

//---------------------------.
// �e����������e�̒萔�̓ǂݍ��݊֐�.
//---------------------------.
void ConstantStruct::stPowerUpBullet::Load()
{
	json j = FileManager::JsonLoad( GetFilePath( "BulletPowerUp" ) );
	POWER_UP_SPEED_DOWN				= j[TO_STRING( POWER_UP_SPEED_DOWN			)][_DATA];
	COOL_TIME						= j[TO_STRING( COOL_TIME					)][_DATA];
}

//---------------------------.
// �ђʒe�̒萔�̓ǂݍ��݊֐�.
//---------------------------.
void ConstantStruct::stSharpBullet::Load()
{
	json j = FileManager::JsonLoad( GetFilePath( "BulletSharp" ) );
	GET_ADD_ATTACK					= j[TO_STRING( GET_ADD_ATTACK				)][_DATA];
	DELETE_SPEED_UP					= j[TO_STRING( DELETE_SPEED_UP				)][_DATA];
	DELERE_ADD_SCALE				= j[TO_STRING( DELERE_ADD_SCALE				)][_DATA];
	HIT_DEC_SCALE					= j[TO_STRING( HIT_DEC_SCALE				)][_DATA];
	HIT_DEC_ATTACK					= j[TO_STRING( HIT_DEC_ATTACK				)][_DATA];
	DEC_ATTACK_MAX					= j[TO_STRING( DEC_ATTACK_MAX				)][_DATA];
	DELETE_SIZE						= j[TO_STRING( DELETE_SIZE					)][_DATA];
}

//---------------------------.
// �e����������e�̒萔�̓ǂݍ��݊֐�.
//---------------------------.
void ConstantStruct::stSpillBullet::Load()
{
	json j = FileManager::JsonLoad( GetFilePath( "BulletSpill" ) );
	GET_ADD_SIZE					= j[TO_STRING( GET_ADD_SIZE					)][_DATA];
	DELETE_SPEED_DOWN				= j[TO_STRING( DELETE_SPEED_DOWN			)][_DATA];
	SPOWN_TIME						= j[TO_STRING( SPOWN_TIME					)][_DATA];
	SPOWN_DEC_SIZE					= j[TO_STRING( SPOWN_DEC_SIZE				)][_DATA];
	DELETE_SPOWN_DEC_SIZE			= j[TO_STRING( DELETE_SPOWN_DEC_SIZE		)][_DATA];
	DELETE_SIZE						= j[TO_STRING( DELETE_SIZE					)][_DATA];
	DELETE_SPOWN_MIN				= j[TO_STRING( DELETE_SPOWN_MIN				)][_DATA];
}

//---------------------------.
// �傫���|�[�V�����̒萔�\����.
//---------------------------.
void ConstantStruct::stBigPotionBullet::Load()
{
	json j = FileManager::JsonLoad( GetFilePath( "BulletBigPotion" ) );
	POTION_TIME_MIN					= j[TO_STRING( POTION_TIME_MIN				)][_DATA];
	POTION_TIME_MAX					= j[TO_STRING( POTION_TIME_MAX				)][_DATA];
	DELETE_ADD_TIME					= j[TO_STRING( DELETE_ADD_TIME				)][_DATA];

}
