#include "XInput.h"
#include "..\..\..\FileManager\FileManager.h"

namespace {
	constexpr char WINDOW_SETTING_FILE_PATH[] = "Data\\Parameter\\Config\\WindowSetting.json";	// �E�B���h�E�̐ݒ�̃t�@�C���p�X.
}

// �l��͈͓��Ɏ��߂�֐�.
template<typename T>
T Clamp( T val, T min, T max )
{
	return ( val < min ) ? min : ( max < val ) ? max : val;
}

CXInput::CXInput()
	: m_padId			( 0 )
	, m_state			()
	, m_stateOld		()
	, m_vibration		()
	, m_IsVibration		( false, u8"XInput�̐U�����g�p���邩",		"Input" )
	, m_ButStop			( false, u8"XInput�̃{�^�����~�����邩",	"Input" )
	, m_connect			( false )
	, m_IsNotActiveStop	( false )
{
	// �E�B���h�E�̐ݒ�̎擾.
	json WndSetting		= FileManager::JsonLoad( WINDOW_SETTING_FILE_PATH );
	m_IsNotActiveStop	= WndSetting["IsInputNotActiveStop"];
}

CXInput::~CXInput()
{
	EndProc();
}

//----------------------------.
// �C���X�^���X�̎擾.
//----------------------------.
CXInput* CXInput::GetInstance()
{
	static std::unique_ptr<CXInput> pInstance = std::make_unique<CXInput>();
	return pInstance.get();
}

//----------------------------.
// �L�[���͂̍X�V.
//----------------------------.
bool CXInput::Update()
{
	CXInput* pI = GetInstance();

	// �L�[�����X�V����O�ɑޔ�.
	pI->m_stateOld = pI->m_state;
	if( pI->UpdateStatus() == false ){
		return false;
	}
	return true;
}

//----------------------------.
// �I������.
//----------------------------.
void CXInput::EndProc()
{
	SetVibration( 0, 0 );
}

//----------------------------.
// �L�[�������ꂽ���Ƃ��擾.
//	������:XINPUT_GAMEPAD_A �Ȃ�.
//----------------------------.
bool CXInput::IsKeyPress( WORD GamePad )
{
	CXInput* pI = GetInstance();
	if ( pI->m_IsNotActiveStop && !CDirectX11::IsWindowActive() ) return false;

	if ( pI->m_ButStop.get() ) return false;
	switch ( GamePad ) {
	case XINPUT_LSTICK_U:
	case XINPUT_LSTICK_D:
	case XINPUT_LSTICK_L:
	case XINPUT_LSTICK_R:
	case XINPUT_RSTICK_U:
	case XINPUT_RSTICK_D:
	case XINPUT_RSTICK_L:
	case XINPUT_RSTICK_R:
		return IsStiPressed( GamePad );
	case XINPUT_LTRIGGER:
	case XINPUT_RTRIGGER:
		return IsTriPressed( GamePad );
	default:
		return IsButPressed( GamePad );
	}
}

//----------------------------.
// �������u��.
//	������:XINPUT_GAMEPAD_A �Ȃ�.
//----------------------------.
bool CXInput::IsKeyDown( WORD GamePad )
{
	CXInput* pI = GetInstance();
	if ( pI->m_IsNotActiveStop && !CDirectX11::IsWindowActive() ) return false;

	if ( pI->m_ButStop.get() ) return false;
	switch ( GamePad ) {
	case XINPUT_LSTICK_U:
	case XINPUT_LSTICK_D:
	case XINPUT_LSTICK_L:
	case XINPUT_LSTICK_R:
	case XINPUT_RSTICK_U:
	case XINPUT_RSTICK_D:
	case XINPUT_RSTICK_L:
	case XINPUT_RSTICK_R:
		return IsStiDown( GamePad );
	case XINPUT_LTRIGGER:
	case XINPUT_RTRIGGER:
		return IsTriDown( GamePad );
	default:
		return IsButDown( GamePad );
	}
}

//----------------------------.
// �������u��.
//	������:XINPUT_GAMEPAD_A �Ȃ�.
//----------------------------.
bool CXInput::IsKeyUp( WORD GamePad )
{
	CXInput* pI = GetInstance();
	if ( pI->m_IsNotActiveStop && !CDirectX11::IsWindowActive() ) return false;

	if ( pI->m_ButStop.get() ) return false;
	switch ( GamePad ) {
	case XINPUT_LSTICK_U:
	case XINPUT_LSTICK_D:
	case XINPUT_LSTICK_L:
	case XINPUT_LSTICK_R:
	case XINPUT_RSTICK_U:
	case XINPUT_RSTICK_D:
	case XINPUT_RSTICK_L:
	case XINPUT_RSTICK_R:
		return IsStiUp( GamePad );
	case XINPUT_LTRIGGER:
	case XINPUT_RTRIGGER:
		return IsTriUp( GamePad );
	default:
		return IsButUp( GamePad );
	}
}

//----------------------------.
// ���������Ă���.
//	������:XINPUT_GAMEPAD_A �Ȃ�.
//----------------------------.
bool CXInput::IsKeyRepeat( WORD GamePad )
{
	CXInput* pI = GetInstance();
	if ( pI->m_IsNotActiveStop && !CDirectX11::IsWindowActive() ) return false;

	if ( pI->m_ButStop.get() ) return false;
	switch ( GamePad ) {
		case XINPUT_LSTICK_U:
		case XINPUT_LSTICK_D:
		case XINPUT_LSTICK_L:
		case XINPUT_LSTICK_R:
		case XINPUT_RSTICK_U:
		case XINPUT_RSTICK_D:
		case XINPUT_RSTICK_L:
		case XINPUT_RSTICK_R:
			return IsStiRepeat( GamePad );
		case XINPUT_LTRIGGER:
		case XINPUT_RTRIGGER:
			return IsTriRepeat( GamePad );
		default:
			return IsButRepeat( GamePad );
	}
}

//----------------------------.
// �U���ݒ�.
//----------------------------.
bool CXInput::SetVibration( const float LMotorSpd, const float RMotorSpd)
{
	CXInput* pI = GetInstance();

	if ( pI->m_IsVibration.get() ) {
		pI->m_vibration.wLeftMotorSpeed  =
			static_cast<WORD>( INPUT_VIBRATION_MAX * Clamp( LMotorSpd, 0.0f, 1.0f ) );;
		pI->m_vibration.wRightMotorSpeed =
			static_cast<WORD>( INPUT_VIBRATION_MAX * Clamp( RMotorSpd, 0.0f, 1.0f ) );;

		if ( ERROR_SUCCESS == XInputSetState(
			pI->m_padId, &pI->m_vibration ) ) {
			return true;
		}
	}
	return false;
}

//----------------------------.
// �L�[���͂̍X�V.
//----------------------------.
bool CXInput::UpdateStatus()
{
	m_connect = false;
	if( ERROR_SUCCESS == XInputGetState(
		m_padId,
		&m_state ) )
	{
		m_connect = true;
		return true;
	}
	return false;
}

//----------------------------.
// �{�^���������ꂽ���Ƃ��擾����.
//----------------------------.
bool CXInput::IsButPressed( WORD GamePad )
{
	return	GetInstance()->m_state.Gamepad.wButtons & GamePad;
}

//----------------------------.
// �{�^�����������u��.
//----------------------------.
bool CXInput::IsButDown( WORD GamePad )
{
	CXInput* pI = GetInstance();

	return	( pI->m_stateOld.Gamepad.wButtons & GamePad ) == 0 &&
			( pI->m_state.Gamepad.wButtons & GamePad ) != 0;
}

//----------------------------.
// �{�^���𗣂����u��.
//----------------------------.
bool CXInput::IsButUp( WORD GamePad )
{
	CXInput* pI = GetInstance();

	return	( pI->m_stateOld.Gamepad.wButtons & GamePad ) != 0 &&
			( pI->m_state.Gamepad.wButtons & GamePad ) == 0;
}

//----------------------------.
// �{�^�������������Ă���.
//----------------------------.
bool CXInput::IsButRepeat( WORD GamePad )
{
	CXInput* pI = GetInstance();

	return	( pI->m_stateOld.Gamepad.wButtons & GamePad ) != 0 &&
			( pI->m_state.Gamepad.wButtons & GamePad ) != 0;
}

//----------------------------.
// �X�e�B�b�N���|�������Ƃ��擾����.
//----------------------------.
bool CXInput::IsStiPressed( WORD GamePad )
{
	CXInput* pI = GetInstance();

	if ( GamePad == XINPUT_LSTICK_U ) return pI->m_state.Gamepad.sThumbLY >= INPUT_THUMB_MAX;
	if ( GamePad == XINPUT_LSTICK_D ) return pI->m_state.Gamepad.sThumbLY <= INPUT_THUMB_MIN;
	if ( GamePad == XINPUT_LSTICK_L ) return pI->m_state.Gamepad.sThumbLX <= INPUT_THUMB_MIN;
	if ( GamePad == XINPUT_LSTICK_R ) return pI->m_state.Gamepad.sThumbLX >= INPUT_THUMB_MAX;
	if ( GamePad == XINPUT_RSTICK_U ) return pI->m_state.Gamepad.sThumbRY >= INPUT_THUMB_MAX;
	if ( GamePad == XINPUT_RSTICK_D ) return pI->m_state.Gamepad.sThumbRY <= INPUT_THUMB_MIN;
	if ( GamePad == XINPUT_RSTICK_L ) return pI->m_state.Gamepad.sThumbRX <= INPUT_THUMB_MIN;
	if ( GamePad == XINPUT_RSTICK_R ) return pI->m_state.Gamepad.sThumbRX >= INPUT_THUMB_MAX;
	return false;
}

//----------------------------.
// �X�e�B�b�N��|�����u��.
//----------------------------.
bool CXInput::IsStiDown( WORD GamePad )
{
	CXInput* pI = GetInstance();

	if ( GamePad == XINPUT_LSTICK_U ){
		return	pI->m_stateOld.Gamepad.sThumbLY < INPUT_THUMB_MAX &&
				pI->m_state.Gamepad.sThumbLY >= INPUT_THUMB_MAX;
	}
	if ( GamePad == XINPUT_LSTICK_D ){
		return	pI->m_stateOld.Gamepad.sThumbLY > INPUT_THUMB_MIN &&
				pI->m_state.Gamepad.sThumbLY <= INPUT_THUMB_MIN;
	}
	if ( GamePad == XINPUT_LSTICK_L ){
		return	pI->m_stateOld.Gamepad.sThumbLX > INPUT_THUMB_MIN &&
				pI->m_state.Gamepad.sThumbLX <= INPUT_THUMB_MIN;
	}
	if ( GamePad == XINPUT_LSTICK_R ){
		return	pI->m_stateOld.Gamepad.sThumbLX < INPUT_THUMB_MAX &&
				pI->m_state.Gamepad.sThumbLX >= INPUT_THUMB_MAX;
	}
	if ( GamePad == XINPUT_RSTICK_U ){
		return	pI->m_stateOld.Gamepad.sThumbRY < INPUT_THUMB_MAX &&
				pI->m_state.Gamepad.sThumbRY >= INPUT_THUMB_MAX;
	}
	if ( GamePad == XINPUT_RSTICK_D ){
		return	pI->m_stateOld.Gamepad.sThumbRY > INPUT_THUMB_MIN &&
				pI->m_state.Gamepad.sThumbRY <= INPUT_THUMB_MIN;
	}
	if ( GamePad == XINPUT_RSTICK_L ){
		return	pI->m_stateOld.Gamepad.sThumbRX > INPUT_THUMB_MIN &&
				pI->m_state.Gamepad.sThumbRX <= INPUT_THUMB_MIN;
	}
	if ( GamePad == XINPUT_RSTICK_R ){
		return	pI->m_stateOld.Gamepad.sThumbRX < INPUT_THUMB_MAX &&
				pI->m_state.Gamepad.sThumbRX >= INPUT_THUMB_MAX;
	}
	return false;
}

//----------------------------.
// �X�e�B�b�N��߂����u��.
//----------------------------.
bool CXInput::IsStiUp( WORD GamePad )
{
	CXInput* pI = GetInstance();

	if ( GamePad == XINPUT_LSTICK_U ){
		return	pI->m_stateOld.Gamepad.sThumbLY >= INPUT_THUMB_MAX &&
				pI->m_state.Gamepad.sThumbLY < INPUT_THUMB_MAX;
	}
	if ( GamePad == XINPUT_LSTICK_D ){
		return	pI->m_stateOld.Gamepad.sThumbLY <= INPUT_THUMB_MIN &&
				pI->m_state.Gamepad.sThumbLY > INPUT_THUMB_MIN;
	}
	if ( GamePad == XINPUT_LSTICK_L ){
		return	pI->m_stateOld.Gamepad.sThumbLX <= INPUT_THUMB_MIN &&
				pI->m_state.Gamepad.sThumbLX > INPUT_THUMB_MIN;
	}
	if ( GamePad == XINPUT_LSTICK_R ){
		return	pI->m_stateOld.Gamepad.sThumbLX >= INPUT_THUMB_MAX &&
				pI->m_state.Gamepad.sThumbLX < INPUT_THUMB_MAX;
	}
	if ( GamePad == XINPUT_RSTICK_U ){
		return	pI->m_stateOld.Gamepad.sThumbLY >= INPUT_THUMB_MAX &&
				pI->m_state.Gamepad.sThumbLY < INPUT_THUMB_MAX;
	}
	if ( GamePad == XINPUT_RSTICK_D ){
		return	pI->m_stateOld.Gamepad.sThumbLY <= INPUT_THUMB_MIN &&
				pI->m_state.Gamepad.sThumbLY > INPUT_THUMB_MIN;
	}
	if ( GamePad == XINPUT_RSTICK_L ){
		return	pI->m_stateOld.Gamepad.sThumbLX <= INPUT_THUMB_MIN &&
				pI->m_state.Gamepad.sThumbLX > INPUT_THUMB_MIN;
	}
	if ( GamePad == XINPUT_RSTICK_R ){
		return	pI->m_stateOld.Gamepad.sThumbLX >= INPUT_THUMB_MAX &&
				pI->m_state.Gamepad.sThumbLX < INPUT_THUMB_MAX;
	}
	return false;
}

//----------------------------.
// �X�e�B�b�N��|�������Ă���.
//----------------------------.
bool CXInput::IsStiRepeat( WORD GamePad )
{
	CXInput* pI = GetInstance();

	if ( GamePad == XINPUT_LSTICK_U ){
		return	pI->m_stateOld.Gamepad.sThumbLY >= INPUT_THUMB_MAX &&
				pI->m_state.Gamepad.sThumbLY >= INPUT_THUMB_MAX;
	}
	if ( GamePad == XINPUT_LSTICK_D ){
		return	pI->m_stateOld.Gamepad.sThumbLY <= INPUT_THUMB_MIN &&
				pI->m_state.Gamepad.sThumbLY <= INPUT_THUMB_MIN;
	}
	if ( GamePad == XINPUT_LSTICK_L ){
		return	pI->m_stateOld.Gamepad.sThumbLX <= INPUT_THUMB_MIN &&
				pI->m_state.Gamepad.sThumbLX <= INPUT_THUMB_MIN;
	}
	if ( GamePad == XINPUT_LSTICK_R ){
		return	pI->m_stateOld.Gamepad.sThumbLX >= INPUT_THUMB_MAX &&
				pI->m_state.Gamepad.sThumbLX >= INPUT_THUMB_MAX;
	}
	if ( GamePad == XINPUT_RSTICK_U ){
		return	pI->m_stateOld.Gamepad.sThumbLY >= INPUT_THUMB_MAX &&
				pI->m_state.Gamepad.sThumbLY >= INPUT_THUMB_MAX;
	}
	if ( GamePad == XINPUT_RSTICK_D ){
		return	pI->m_stateOld.Gamepad.sThumbLY <= INPUT_THUMB_MIN &&
				pI->m_state.Gamepad.sThumbLY <= INPUT_THUMB_MIN;
	}
	if ( GamePad == XINPUT_RSTICK_L ){
		return	pI->m_stateOld.Gamepad.sThumbLX <= INPUT_THUMB_MIN &&
				pI->m_state.Gamepad.sThumbLX >= INPUT_THUMB_MIN;
	}
	if ( GamePad == XINPUT_RSTICK_R ){
		return	pI->m_stateOld.Gamepad.sThumbLX >= INPUT_THUMB_MAX &&
				pI->m_state.Gamepad.sThumbLX >= INPUT_THUMB_MAX;
	}
	return false;
}

//----------------------------.
// �g���K�[�������ꂽ���Ƃ��擾����.
//----------------------------.
bool CXInput::IsTriPressed( WORD GamePad )
{
	CXInput* pI = GetInstance();

	if ( GamePad == XINPUT_LTRIGGER ) return pI->m_state.Gamepad.bLeftTrigger >= INPUT_TRIGGER_MAX;
	if ( GamePad == XINPUT_RTRIGGER ) return pI->m_state.Gamepad.bRightTrigger >= INPUT_TRIGGER_MAX;
	return false;
}

//----------------------------.
// �g���K�[���������u��.
//----------------------------.
bool CXInput::IsTriDown( WORD GamePad )
{
	CXInput* pI = GetInstance();

	if ( GamePad == XINPUT_LTRIGGER ){
		return	pI->m_stateOld.Gamepad.bLeftTrigger < INPUT_TRIGGER_MAX &&
				pI->m_state.Gamepad.bLeftTrigger >= INPUT_TRIGGER_MAX;
	}
	if ( GamePad == XINPUT_RTRIGGER ){
		return	pI->m_stateOld.Gamepad.bRightTrigger < INPUT_TRIGGER_MAX &&
				pI->m_state.Gamepad.bRightTrigger >= INPUT_TRIGGER_MAX;
	}
	return false;
}

//----------------------------.
// �g���K�[�𗣂����u��.
//----------------------------.
bool CXInput::IsTriUp( WORD GamePad )
{
	CXInput* pI = GetInstance();

	if ( GamePad == XINPUT_LTRIGGER ){
		return	pI->m_stateOld.Gamepad.bLeftTrigger >= INPUT_TRIGGER_MAX &&
				pI->m_state.Gamepad.bLeftTrigger < INPUT_TRIGGER_MAX;
	}
	if ( GamePad == XINPUT_RTRIGGER ){
		return	pI->m_stateOld.Gamepad.bRightTrigger >= INPUT_TRIGGER_MAX &&
				pI->m_state.Gamepad.bRightTrigger < INPUT_TRIGGER_MAX;
	}
	return false;
}

//----------------------------.
// �g���K�[�����������Ă���.
//----------------------------.
bool CXInput::IsTriRepeat( WORD GamePad )
{
	CXInput* pI = GetInstance();

	if ( GamePad == XINPUT_LTRIGGER ){
		return	pI->m_stateOld.Gamepad.bLeftTrigger >= INPUT_TRIGGER_MAX &&
				pI->m_state.Gamepad.bLeftTrigger >= INPUT_TRIGGER_MAX;
	}
	if ( GamePad == XINPUT_RTRIGGER ){
		return	pI->m_stateOld.Gamepad.bRightTrigger >= INPUT_TRIGGER_MAX &&
				pI->m_state.Gamepad.bRightTrigger >= INPUT_TRIGGER_MAX;
	}
	return false;
}
