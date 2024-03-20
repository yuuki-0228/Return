#include "CameraManager.h"
#include "..\FreeCamera\FreeCamera.h"
#include "..\..\..\Scene\SceneManager\SceneManager.h"
#include "..\..\..\Utility\Input\Input.h"
#include "..\..\..\Utility\ImGuiManager\DebugWindow\DebugWindow.h"
#include "..\..\..\Utility\Math\Math.h"
#include "..\..\..\Utility\Random\Random.h"

#include "..\..\..\Utility\FrameRate\FrameRate.h"

namespace {
	// �J�����̐U���̎��.
	enum enShakingType : unsigned char {
		None,

		Pitch,		// �c�h��.
		Rolling,	// ���h��.
		Mix,		// �~�b�N�X.
	} typedef EShakingType;
}

CCameraManager::CCameraManager()
	: m_pNowCamera			( nullptr )
	, m_pTmpCamera			( nullptr )
	, m_pFreeCamera			( nullptr )
	, m_pRay				( nullptr )
	, m_FreeCameraState		()
	, m_TmpCameraState		()
	, m_DireVec				( INIT_FLOAT3 )
	, m_HitStopEndProc		( {} )
	, m_HitStopSlowProc		( {} )
	, m_ShakingNo			( EShakingType::None )
	, m_ShakingNum			( INIT_INT )
	, m_DeltaTime			( INIT_FLOAT )
	, m_ShakingTimeCnt		( INIT_FLOAT )
	, m_ShakingTimeMax		( INIT_FLOAT )
	, m_ShakingMax			( INIT_FLOAT )
	, m_ShakingMin			( INIT_FLOAT )
	, m_IsHitStop			( false )
	, m_IsHitStopSlow		( false )
	, m_IsFreeCamera		( false )
#ifdef _DEBUG
	, m_IsCameraChangeLock	( false )
#else
	, m_IsCameraChangeLock	( true )
#endif
{
	m_pFreeCamera	= std::make_unique<CFreeCamera>();
	m_pRay			= std::make_unique<CRay>();
}

CCameraManager::~CCameraManager()
{
}

//---------------------------.
// �C���X�^���X�̎擾.
//---------------------------.
CCameraManager* CCameraManager::GetInstance()
{
	static std::unique_ptr<CCameraManager> pInstance = std::make_unique<CCameraManager>();
	return pInstance.get();
}

//---------------------------.
// �X�V
//	�J�������ݒ肳��Ă��Ȃ��ꍇ�͏����͍s��Ȃ�.
//---------------------------.
void CCameraManager::Update( float DeltaTime )
{
	CCameraManager* pI = GetInstance();

	pI->m_DeltaTime = DeltaTime;

	if ( pI->m_pNowCamera == nullptr ) return;

	// �J�����̐؂�ւ�.
	if ( pI->m_IsCameraChangeLock == false ) {
		if ( CDebugKeyInput::IsANDKeyDown( 'C', 'M' ) ) {
			if ( pI->m_IsFreeCamera )	ChangeSetCamera();
			else						ChangeFreeCamera();
		}
	}

	// ���݂̃J�����̍X�V.
	ShakingUpdate();
	if ( CSceneManager::GetIsSceneStop() == false ) {
		pI->m_pNowCamera->Update( DeltaTime );
	}
	pI->m_pNowCamera->ViewProjUpdate();

	// �q�b�g�X�g�b�v�̒�~.
	if ( pI->m_IsHitStop && pI->m_ShakingNo == EShakingType::None ) {
		if ( pI->m_IsHitStopSlow ) {
			// �V�[���̒�~����������.
			CSceneManager::SetIsSceneStop( false );

			// �X���[�ɂ�����.
			CFrameRate::SetSlow( 5 );
			SetMixShaking( pI->m_ShakingTimeMax, 5, 0.2f, 0.05f );
			pI->m_IsHitStopSlow = false;

			// �X���[�ɕύX���ɍs���֐����Ăяo��.
			pI->m_HitStopSlowProc();
			pI->m_HitStopSlowProc = {};
		}
		else {
			// �V�[���̒�~����������.
			CSceneManager::SetIsSceneStop( false );

			// �q�b�g�X�g�b�v�̒�~.
			pI->m_IsHitStop = false;
			CFrameRate::ResetSpeed();

			// �q�b�g�X�g�b�v�I�����ɍs���֐����Ăяo��.
			pI->m_HitStopEndProc();
			pI->m_HitStopEndProc = {};
		}
	}

	// �����x�N�g���̍X�V.
	const D3DXROTATION3& CamRot = pI->m_pNowCamera->GetRotation();
	pI->m_DireVec.x = sinf( Math::ToRadian( CamRot.y ) );
	pI->m_DireVec.y = sinf( Math::ToRadian( CamRot.x ) );
	pI->m_DireVec.z = cosf( Math::ToRadian( CamRot.y ) );

	// �J�����̌����Ă�������̃x�N�g�������߂�.
	const D3DXPOSITION3& Pos	= CCameraManager::GetPosition();
	const D3DXPOSITION3& Look	= CCameraManager::GetLookPosition();
	D3DXVECTOR3 Vec				= Look - Pos;

	// ���C�̍X�V.
	pI->m_pRay->SetVector( Vec );
	pI->m_pRay->SetPosition( Pos );

#ifdef _DEBUG
	CDebugWindow::PushProc( "Camera", [&]() {
		const D3DXPOSITION3 Pos		= CCameraManager::GetPosition();
		const D3DXPOSITION3 Look	= CCameraManager::GetLookPosition();
		const D3DXROTATION3 Rot		= CCameraManager::GetRotation();

		ImGui::Text( "------------------------------" );
		ImGui::Text( "'C + M' Change Camera( SetCamera <-> FreeCamera )" );
		ImGui::Text( "FreeCameraMove 'Ctrl + '" );
		ImGui::Text( "'A or D'         : x Move" );
		ImGui::Text( "'SPACE or SHIFT' : y Move" );
		ImGui::Text( "'W or S'         : z Move " );
		ImGui::Text( "'R or F'         : x Rotation" );
		ImGui::Text( "'Q or E'         : y Rotation" );
		ImGui::Text( "'Z or X'         : Zoom" );
		ImGui::Text( "------------------------------" );
		ImGui::Text( "Position         : x[%.3f], y[%.3f], z[%.3f]", Pos.x,  Pos.y,  Pos.z );
		ImGui::Text( "LookPosition     : x[%.3f], y[%.3f], z[%.3f]", Look.x, Look.y, Look.z );
		ImGui::Text( "Rotation         : x[%.3f], y[%.3f], z[%.3f]", Rot.x,  Rot.y,  Rot.z );
		ImGui::Text( "IsFreeCamera     : %s", GetInstance()->m_IsFreeCamera ? "true" : "false" );
		ImGui::Text( "------------------------------" );
	} );
#endif	// #ifdef _DEBUG.
}

//---------------------------.
// �J�����̐ݒ�.
//	�ݒ肷��J������ null �̏ꍇ�͏����͍s��Ȃ�.
//---------------------------.
void CCameraManager::SetCamera( CCameraBase* pCamera )
{
	CCameraManager* pI = GetInstance();

	if ( pCamera == nullptr ) return;

	// ���݃t���[�J������ݒ肳��Ă���ꍇ�O��.
	if ( pI->m_IsFreeCamera ){
		pI->m_FreeCameraState	= pI->m_pNowCamera->GetCameraState();
		pI->m_IsFreeCamera		= false;
	}

	// �J�����̐ݒ�.
	pI->m_pNowCamera = pCamera;
}

//---------------------------.
// �J�����̐U��(�c�h��)�̐ݒ�.
//---------------------------.
void CCameraManager::SetPitch( const float Time, const int Num, const float MaxValue, const float MinValue )
{
	CCameraManager* pI = GetInstance();

	// �h��̐ݒ�.
	pI->m_ShakingNo			= EShakingType::Pitch;
	pI->m_ShakingNum		= Num		< 1		? 1			: Num; 
	pI->m_ShakingMax		= MaxValue	< 0.0f	? 0.0f		: MaxValue;
	pI->m_ShakingMin		= MinValue	< 0.0f	? MaxValue	: MinValue;
	pI->m_ShakingTimeMax	= Time;
	pI->m_ShakingTimeCnt	= INIT_FLOAT;
}

//---------------------------.
// �J�����̐U��(���h��)�̐ݒ�.
//---------------------------.
void CCameraManager::SetRolling( const float Time, const int Num, const float MaxValue, const float MinValue )
{
	CCameraManager* pI = GetInstance();

	// �h��̐ݒ�.
	pI->m_ShakingNo			= EShakingType::Rolling;
	pI->m_ShakingNum		= Num		< 1		? 1			: Num; 
	pI->m_ShakingMax		= MaxValue	< 0.0f	? 0.0f		: MaxValue;
	pI->m_ShakingMin		= MinValue	< 0.0f	? MaxValue	: MinValue;
	pI->m_ShakingTimeMax	= Time;
	pI->m_ShakingTimeCnt	= INIT_FLOAT;
}

//---------------------------.
// �J�����̐U��(�~�b�N�X)�̐ݒ�.
//---------------------------.
void CCameraManager::SetMixShaking( const float Time, const int Num, const float MaxValue, const float MinValue )
{
	CCameraManager* pI = GetInstance();

	// �h��̐ݒ�.
	pI->m_ShakingNo			= EShakingType::Mix;
	pI->m_ShakingNum		= Num		< 1		? 1			: Num; 
	pI->m_ShakingMax		= MaxValue	< 0.0f	? 0.0f		: MaxValue;
	pI->m_ShakingMin		= MinValue	< 0.0f	? MaxValue	: MinValue;
	pI->m_ShakingTimeMax	= Time;
	pI->m_ShakingTimeCnt	= INIT_FLOAT;
}

//---------------------------.
// �q�b�g�X�g�b�v�̐ݒ�.
//---------------------------.
void CCameraManager::HitStop( const float Time, const bool IsSlow, const std::function<void()>& EndProc, const std::function<void()>& SlowProc )
{
	CCameraManager* pI = GetInstance();

	// �q�b�g�X�g�b�v���s���悤�ɐݒ�.
	pI->m_IsHitStop		= true;
	pI->m_IsHitStopSlow = IsSlow;

	// �֐���ۑ�����.
	pI->m_HitStopEndProc	= EndProc;
	pI->m_HitStopSlowProc	= SlowProc;

	// ��ʂ̗h��̐ݒ�.
	SetMixShaking( Time, 5 * static_cast<int>( Time ), 0.2f, 0.05f );

	// �V�[�����~������.
	CSceneManager::SetIsSceneStop( true );
}

//---------------------------.
// �t���[�J�����ɕύX.
//	�t���[�J������ null / ���݃t���[�J���� �̏ꍇ�����͍s��Ȃ�.
//---------------------------.
void CCameraManager::ChangeFreeCamera()
{
	CCameraManager* pI = GetInstance();

	if ( pI->m_pFreeCamera  == nullptr	) return;
	if ( pI->m_IsFreeCamera == true		) return;

	// ���ݐݒ肳��Ă���J������ۊǂ���.
	pI->m_pTmpCamera		= pI->m_pNowCamera;
	pI->m_TmpCameraState	= pI->m_pNowCamera->GetCameraState();

	// �t���[�J�����ɐݒ�.
	pI->m_pNowCamera = pI->m_pFreeCamera.get();
	pI->m_pNowCamera->SetCameraState( pI->m_FreeCameraState );

	pI->m_IsFreeCamera = true;
}

//---------------------------.
// �ݒ肳��Ă���J�����ɕύX.
//	�ۊǒ��̃J������ null / ���ݐݒ蒆�̃J���� �̏ꍇ�����͍s��Ȃ�.
//---------------------------.
void CCameraManager::ChangeSetCamera()
{
	CCameraManager* pI = GetInstance();

	if ( pI->m_pFreeCamera  == nullptr	) return;
	if ( pI->m_IsFreeCamera == false	) return;

	// �t���[�J�����̏�Ԃ�ۊǂ���.
	pI->m_FreeCameraState	= pI->m_pNowCamera->GetCameraState();

	// �ݒ肳�ꂽ�J�����ɐݒ�.
	pI->m_pNowCamera = pI->m_pTmpCamera;
	pI->m_pNowCamera->SetCameraState( pI->m_TmpCameraState );

	pI->m_IsFreeCamera = false;
}

//---------------------------.
// �h��̍X�V.
//	�h�ꂳ��Ȃ��ꍇ�͏������s��Ȃ�.
//---------------------------.
void CCameraManager::ShakingUpdate()
{
	CCameraManager* pI = GetInstance();

	if ( pI->m_ShakingNo == EShakingType::None ) return;

	// �h��̃^�C�v�ɉ������X�V���s��.
	D3DXVECTOR3 ShakingAddPos;
	switch ( pI->m_ShakingNo ) {
	case EShakingType::Pitch:
		ShakingAddPos = PitchUpdate();
		break;
	case EShakingType::Rolling:
		ShakingAddPos = RollingUpdate();
		break;
	case EShakingType::Mix:
	{
		D3DXVECTOR3 PitchAddPos;
		D3DXVECTOR3 RollingAddPos;

		PitchAddPos		= PitchUpdate();
		RollingAddPos	= RollingUpdate();

		ShakingAddPos = PitchAddPos + RollingAddPos;
		break;
	}
	default:
		break;
	}

	// �h��ɂ��ǉ��̍��W��ݒ�.
	pI->m_pNowCamera->SetShaking( ShakingAddPos );
}

//---------------------------.
// �c�h��̍X�V.
//---------------------------.
D3DXPOSITION3 CCameraManager::PitchUpdate()
{
	CCameraManager* pI = GetInstance();

	// �h���ݒ�.
	D3DXPOSITION3	ShakingAddPos	= INIT_FLOAT3;
	const float		ShakingValue	= CRandom::GetRand( pI->m_ShakingMin, pI->m_ShakingMax );
	ShakingAddPos.y = sinf( Math::RADIAN_MAX * ( pI->m_ShakingTimeCnt / pI->m_ShakingTimeMax ) * pI->m_ShakingNum ) * ShakingValue;

	// �h��̎��Ԃ𑝂₷.
	pI->m_ShakingTimeCnt += pI->m_DeltaTime;
	if ( pI->m_ShakingTimeCnt >= pI->m_ShakingTimeMax ) {
		// �h����~������.
		pI->m_ShakingNo			= EShakingType::None;
		pI->m_ShakingTimeCnt	= INIT_FLOAT;
		ShakingAddPos			= INIT_FLOAT3;
	};
	return ShakingAddPos;
}

//---------------------------.
// ���h��̍X�V.
//---------------------------.
D3DXPOSITION3 CCameraManager::RollingUpdate()
{
	CCameraManager* pI = GetInstance();

	// �ړ��x�N�g���̍쐬.
	const D3DXVECTOR3&	LookVector = pI->m_pNowCamera->GetLookVector();
	D3DXVECTOR3			MoveVector = Math::X_AXIS;
	D3DXMATRIX			mRot;
	D3DXMatrixRotationYawPitchRoll( &mRot, atan2f( LookVector.x, LookVector.z ), 0.0f, 0.0f );
	D3DXVec3TransformCoord( &MoveVector, &MoveVector, &mRot );

	// �h���ݒ�.
	D3DXPOSITION3	ShakingAddPos	= INIT_FLOAT3;
	const float		ShakingValue	= CRandom::GetRand( pI->m_ShakingMin, pI->m_ShakingMax );
	ShakingAddPos = MoveVector * ( sinf( Math::RADIAN_MAX * ( pI->m_ShakingTimeCnt / pI->m_ShakingTimeMax ) * pI->m_ShakingNum ) * ShakingValue );

	// �h��̎��Ԃ𑝂₷.
	pI->m_ShakingTimeCnt += pI->m_DeltaTime;
	if ( pI->m_ShakingTimeCnt >= pI->m_ShakingTimeMax ) {
		// �h����~������.
		pI->m_ShakingNo			= EShakingType::None;
		pI->m_ShakingTimeCnt	= INIT_FLOAT;
		ShakingAddPos			= INIT_FLOAT3;
	};
	return ShakingAddPos;
}
