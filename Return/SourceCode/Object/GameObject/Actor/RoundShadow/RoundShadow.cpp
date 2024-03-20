#include "RoundShadow.h"
#include "..\..\..\..\Common\DirectX\DirectX11.h"
#include "..\..\..\..\Common\Mesh\StaticMesh\StaticMesh.h"
#include "..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\Utility\Math\Math.h"

namespace {
	// 影の位置(Y).
	constexpr float SHADOW_POS_Y = -4.0f;
}

CRoundShadow::CRoundShadow( CActor* pObj, CStaticMesh* pModel, float vScale )
	: m_pSpriteShadow	( nullptr )
	, m_ShadowState		()
	, m_pObject			( pObj )
{
	// 画像の取得.
	m_pSpriteShadow = CSpriteResource::GetSprite( "RoundShadow", &m_ShadowState );
	m_ShadowState.Transform.Rotation = { Math::ToRadian( 90.0f ), 0.0f, 0.0f };

	// 影のサイズを取得.
	m_ShadowState.Transform.Scale = GetShadowScale( pModel, vScale );
}

CRoundShadow::~CRoundShadow()
{
}

//---------------------------.
// 初期化.
//---------------------------.
bool CRoundShadow::Init()
{
	return true;
}

//---------------------------.
// 更新.
//	影の画像が null / オブジェクトが null の
//	場合は処理は行わない.
//---------------------------.
void CRoundShadow::Update( const float& DeltaTime )
{
}

//---------------------------.
// 描画.
//	影の画像が null / オブジェクトが null の
//	場合は処理は行わない.
//---------------------------.
void CRoundShadow::Render()
{
	if ( m_pSpriteShadow	== nullptr ) return;
	if ( m_pObject			== nullptr ) return;

	// 座標を更新.
	const D3DXPOSITION3& ObjPos		= m_pObject->GetPosition();
	STransform*			 ShadowTra	= &m_ShadowState.Transform;
	ShadowTra->Position	= { ObjPos.x, SHADOW_POS_Y, ObjPos.z };

	// スプライトの描画処理.
	CDirectX11::SetAlphaToCoverage( true );
	m_pSpriteShadow->Render3D( &m_ShadowState );
	CDirectX11::SetAlphaToCoverage( false );
}

//---------------------------.
// 初期の影のサイズを取得.
//---------------------------.
D3DXSCALE3 CRoundShadow::GetShadowScale( CStaticMesh* pModel, float vScale )
{
	LPDIRECT3DVERTEXBUFFER9 pVB				= nullptr;	// 頂点バッファ.
	void*					pVertices		= nullptr;	// 頂点.
	D3DXPOSITION3			Center( 0.0f, 0.0f, 0.0f );	// 中心座標.
	float					ShadowRadius	= 32.0f;	// 影の半径.
	float					ModelRadius		= 0.0f;		// モデルの半径.

	// 頂点バッファを取得.
	if ( FAILED(
		pModel->GetMesh()->GetVertexBuffer( &pVB ) ) )
	{
		return D3DXSCALE3( 0.0f, 0.0f, 0.0f );
	}

	// メッシュの頂点バッファをロックする.
	if ( FAILED( pVB->Lock( 0, 0, &pVertices, 0 ) ) )
	{
		SAFE_RELEASE( pVB );
		return D3DXSCALE3( 0.0f, 0.0f, 0.0f );
	}

	// メッシュの外接円の中心と半径を計算する.
	D3DXComputeBoundingSphere(
		static_cast< D3DXVECTOR3* >( pVertices ),
		pModel->GetMesh()->GetNumVertices(),					// 頂点の数.
		D3DXGetFVFVertexSize( pModel->GetMesh()->GetFVF() ),	// 頂点の情報.
		&Center,												// (out)中心座標.
		&ModelRadius );											// (out)半径.

	// 倍率を計算.
	float Scale = ( ModelRadius * vScale ) / ShadowRadius;
	return D3DXSCALE3( Scale, Scale, Scale );
}
