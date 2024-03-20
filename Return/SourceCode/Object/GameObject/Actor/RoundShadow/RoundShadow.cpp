#include "RoundShadow.h"
#include "..\..\..\..\Common\DirectX\DirectX11.h"
#include "..\..\..\..\Common\Mesh\StaticMesh\StaticMesh.h"
#include "..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\Utility\Math\Math.h"

namespace {
	// �e�̈ʒu(Y).
	constexpr float SHADOW_POS_Y = -4.0f;
}

CRoundShadow::CRoundShadow( CActor* pObj, CStaticMesh* pModel, float vScale )
	: m_pSpriteShadow	( nullptr )
	, m_ShadowState		()
	, m_pObject			( pObj )
{
	// �摜�̎擾.
	m_pSpriteShadow = CSpriteResource::GetSprite( "RoundShadow", &m_ShadowState );
	m_ShadowState.Transform.Rotation = { Math::ToRadian( 90.0f ), 0.0f, 0.0f };

	// �e�̃T�C�Y���擾.
	m_ShadowState.Transform.Scale = GetShadowScale( pModel, vScale );
}

CRoundShadow::~CRoundShadow()
{
}

//---------------------------.
// ������.
//---------------------------.
bool CRoundShadow::Init()
{
	return true;
}

//---------------------------.
// �X�V.
//	�e�̉摜�� null / �I�u�W�F�N�g�� null ��
//	�ꍇ�͏����͍s��Ȃ�.
//---------------------------.
void CRoundShadow::Update( const float& DeltaTime )
{
}

//---------------------------.
// �`��.
//	�e�̉摜�� null / �I�u�W�F�N�g�� null ��
//	�ꍇ�͏����͍s��Ȃ�.
//---------------------------.
void CRoundShadow::Render()
{
	if ( m_pSpriteShadow	== nullptr ) return;
	if ( m_pObject			== nullptr ) return;

	// ���W���X�V.
	const D3DXPOSITION3& ObjPos		= m_pObject->GetPosition();
	STransform*			 ShadowTra	= &m_ShadowState.Transform;
	ShadowTra->Position	= { ObjPos.x, SHADOW_POS_Y, ObjPos.z };

	// �X�v���C�g�̕`�揈��.
	CDirectX11::SetAlphaToCoverage( true );
	m_pSpriteShadow->Render3D( &m_ShadowState );
	CDirectX11::SetAlphaToCoverage( false );
}

//---------------------------.
// �����̉e�̃T�C�Y���擾.
//---------------------------.
D3DXSCALE3 CRoundShadow::GetShadowScale( CStaticMesh* pModel, float vScale )
{
	LPDIRECT3DVERTEXBUFFER9 pVB				= nullptr;	// ���_�o�b�t�@.
	void*					pVertices		= nullptr;	// ���_.
	D3DXPOSITION3			Center( 0.0f, 0.0f, 0.0f );	// ���S���W.
	float					ShadowRadius	= 32.0f;	// �e�̔��a.
	float					ModelRadius		= 0.0f;		// ���f���̔��a.

	// ���_�o�b�t�@���擾.
	if ( FAILED(
		pModel->GetMesh()->GetVertexBuffer( &pVB ) ) )
	{
		return D3DXSCALE3( 0.0f, 0.0f, 0.0f );
	}

	// ���b�V���̒��_�o�b�t�@�����b�N����.
	if ( FAILED( pVB->Lock( 0, 0, &pVertices, 0 ) ) )
	{
		SAFE_RELEASE( pVB );
		return D3DXSCALE3( 0.0f, 0.0f, 0.0f );
	}

	// ���b�V���̊O�ډ~�̒��S�Ɣ��a���v�Z����.
	D3DXComputeBoundingSphere(
		static_cast< D3DXVECTOR3* >( pVertices ),
		pModel->GetMesh()->GetNumVertices(),					// ���_�̐�.
		D3DXGetFVFVertexSize( pModel->GetMesh()->GetFVF() ),	// ���_�̏��.
		&Center,												// (out)���S���W.
		&ModelRadius );											// (out)���a.

	// �{�����v�Z.
	float Scale = ( ModelRadius * vScale ) / ShadowRadius;
	return D3DXSCALE3( Scale, Scale, Scale );
}
