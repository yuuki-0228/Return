#include "FreeCamera.h"
#include "..\..\..\Utility\Math\Math.h"
#include "..\..\..\Utility\Input\Input.h"

CFreeCamera::CFreeCamera()
	: m_CameraPosition	( 0.0f, 1.0f, 0.0f )
	, m_MoveVec			( INIT_FLOAT3 )
	, m_SideMoveVec		( INIT_FLOAT3 )
{
}

CFreeCamera::~CFreeCamera()
{
}

//---------------------------.
// �X�V.
//---------------------------.
void CFreeCamera::Update( const float& DeltaTime )
{
	// �J�����̑���.
//	if ( CKeyInput::IsKeyPress( VK_CONTROL ) ) {
		if ( CInput::IsKeyPress( "FreeCameraMoveF" ) ) m_CameraPosition			-= m_MoveVec	 * CONSTANT.MOVE_SPEED;
		if ( CInput::IsKeyPress( "FreeCameraMoveB" ) ) m_CameraPosition			+= m_MoveVec	 * CONSTANT.MOVE_SPEED;
		if ( CInput::IsKeyPress( "FreeCameraMoveL" ) ) m_CameraPosition			+= m_SideMoveVec * CONSTANT.MOVE_SPEED;
		if ( CInput::IsKeyPress( "FreeCameraMoveR" ) ) m_CameraPosition			-= m_SideMoveVec * CONSTANT.MOVE_SPEED;
		if ( CInput::IsKeyPress( "FreeCameraMoveU" ) ) m_CameraPosition.y		+= CONSTANT.MOVE_SPEED;
		if ( CInput::IsKeyPress( "FreeCameraMoveD" ) ) m_CameraPosition.y		-= CONSTANT.MOVE_SPEED;
		if ( CInput::IsKeyPress( "FreeCameraRotXU" ) ) m_Transform.Rotation.x	-= CONSTANT.ROTATION_SPEED;
		if ( CInput::IsKeyPress( "FreeCameraRotXD" ) ) m_Transform.Rotation.x	+= CONSTANT.ROTATION_SPEED;
		if ( CInput::IsKeyPress( "FreeCameraRotYL" ) ) m_Transform.Rotation.y	-= CONSTANT.ROTATION_SPEED;
		if ( CInput::IsKeyPress( "FreeCameraRotYR" ) ) m_Transform.Rotation.y	+= CONSTANT.ROTATION_SPEED;
//	}

	// �ړ��x�N�g���̍X�V.
	m_MoveVec.x		= sinf( m_Transform.Rotation.y );
	m_MoveVec.z		= cosf( m_Transform.Rotation.y );
	m_SideMoveVec.x = m_MoveVec.z;
	m_SideMoveVec.z = -m_MoveVec.x;

	// �����_�̍X�V.
	m_LookPosition = m_CameraPosition;

	// ���_�̍X�V.
	m_Transform.Position.x = m_LookPosition.x + ( sinf( m_Transform.Rotation.y ) );
	m_Transform.Position.y = m_LookPosition.y + ( sinf( m_Transform.Rotation.x ) );
	m_Transform.Position.z = m_LookPosition.z + ( cosf( m_Transform.Rotation.y ) );
}
