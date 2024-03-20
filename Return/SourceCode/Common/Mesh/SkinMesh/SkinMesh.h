/***************************************************************************************************
*	SkinMeshCode Version 2.10
*	LastUpdate	: 2020/10/06.
*		Unicode対応.
**/
#pragma once

#include "..\..\Common.h"
#include "Parser\SkinMeshParser.h"

class CFog;

/************************************************
*	スキンメッシュクラス
**/
class CSkinMesh final
	: public CCommon
{
public:
	// コンスタントバッファのアプリ側の定義.
	//	※シェーダ内のコンスタントバッファと一致している必要あり.
	struct CBUFFER_PER_MESH
	{
		D3DXMATRIX	mW;				// ワールド行列.
		D3DXMATRIX	mWVP;			// ワールドから射影までの変換行列.
		D3DXCOLOR4	vColor;			// カラー.

	};

	// マテリアル単位.
	struct CBUFFER_PER_MATERIAL
	{
		D3DXVECTOR4 vAmbient;		// アンビエント光.
		D3DXVECTOR4 vDiffuse;		// ディフューズ色.
		D3DXVECTOR4 vSpecular;		// 鏡面反射.
	};

	// シェーダーに渡す値.
	struct CBUFFER_PER_FRAME
	{
		D3DXPOSITION4	vCameraPos;			// カメラ位置.
		D3DXVECTOR4		vLightDir;			// ライト方向.
		D3DXVECTOR4		vLightIntensity;	// ライトの強弱.
		D3DXVECTOR4		vFlag;				// x：ﾃﾞｲｻﾞ抜きを使用するか、	y：ｱﾙﾌｧﾌﾞﾛｯｸを使用するか、	z：距離で透明にするか(1以上なら使用する).
		D3DXVECTOR4		vAlphaDistance;		// x：開始する距離(以下)、	y：開始する距離(以上)、	z：透明にしていく間隔.
	};

	// ボーン単位.
	struct CBUFFER_PER_BONES
	{
		D3DXMATRIX mBone[D3DXPARSER::MAX_BONES];
		CBUFFER_PER_BONES()
		{
			for ( int i = 0; i < D3DXPARSER::MAX_BONES; i++ )
			{
				D3DXMatrixIdentity( &mBone[i] );
			}
		}
	};


	// 頂点構造体.
	struct VERTEX
	{
		D3DXPOSITION3	vPos;		// 頂点位置.
		D3DXVECTOR3		vNorm;		// 頂点法線.
		D3DXVECTOR2		vTex;		// UV座標.
		UINT bBoneIndex[4];			// ボーン 番号.
		float bBoneWeight[4];		// ボーン 重み.
		VERTEX()
			: vPos			()
			, vNorm			()
			, vTex			()
			, bBoneIndex	()
			, bBoneWeight	()
		{}
	};

public:
	CSkinMesh();
	~CSkinMesh();

	HRESULT Init( LPCTSTR fileName );

	// 解放関数.
	HRESULT Release();

	// 描画関数.
	void Render( STransform* pTransform = nullptr, const LPD3DXANIMATIONCONTROLLER pAC = nullptr, D3DXMATRIX* pRot = nullptr );

	double GetAnimSpeed()				{ return m_dAnimSpeed;		}
	void SetAnimSpeed( double dSpeed )	{ m_dAnimSpeed = dSpeed;	}

	double GetAnimTime()				{ return m_dAnimTime;		}
	void SetAnimTime( double dTime )	{ m_dAnimTime = dTime;		}

	// パーサークラスからアニメーションコントローラを取得する.
	LPD3DXANIMATIONCONTROLLER GetAnimationController() { return m_pD3dxMesh->m_pAnimController; }

	// アニメーションセットの切り替え.
	void ChangeAnimSet( int index, LPD3DXANIMATIONCONTROLLER pAC=nullptr );
	// アニメーションセットの切り替え(開始フレーム指定可能版).
	void ChangeAnimSet_StartPos( int index, double dStartFramePos, LPD3DXANIMATIONCONTROLLER pAC=nullptr );

	// アニメーション停止時間を取得.
	double GetAnimPeriod( int index );
	// アニメーション数を取得.
	int GetAnimMax( LPD3DXANIMATIONCONTROLLER pAC=nullptr );
	// メッシュを取得.
	LPD3DXMESH GetMesh() const { return m_pMeshForRay; }

	// 指定したボーン情報(座標・行列)を取得する関数.
	D3DXMATRIX GetMatrixFromBone( LPCSTR sBoneName );
	bool GetMatrixFromBone( LPCSTR sBoneName, D3DXMATRIX* pOutMat );
	D3DXVECTOR3 GetPosFromBone( LPCSTR sBoneName );
	bool GetPosFromBone( LPCSTR sBoneName, D3DXVECTOR3* pOutPos);
	D3DXVECTOR3 GetDeviaPosFromBone( LPCSTR sBoneName, D3DXVECTOR3 vSpecifiedPos = { 0.0f, 0.0f, 0.0f });
	bool GetDeviaPosFromBone( LPCSTR sBoneName, D3DXVECTOR3* pOutPos, D3DXVECTOR3 vSpecifiedPos = { 0.0f, 0.0f, 0.0f });

	// フォグの有効/無効.
	inline void SetIsFog( const bool& Flag ) { m_IsFog = Flag; }
	// ディザ抜きを使用するか.
	inline void SetDither( const bool& Flag ) { m_DitherFlag = Flag; }
	// アルファブロックを使用するか.
	inline void SetAlphaBlock( const bool& Flag ) { m_AlphaBlockFlag = Flag; }
	// アルファブレンドを使用するか.
	inline void SetAlphaBlend( const bool& Flag ) { CDirectX11::SetAlphaBlend( Flag ); }
	// 距離で透明にするか(以下).
	inline void SetDistanceAlphaOrLess( const bool Flag, const float StartDistance = 0.0f, const float AlphaDistance = 0.0f ) {
		m_OrLessDistanceAlphaFlag	= Flag;
		m_OrLessStartDistance		= StartDistance;
		m_AlphaDistance				= AlphaDistance;
	}
	// 距離で透明にするか(以上).
	inline void SetDistanceAlphaOrMore( const bool Flag, const float StartDistance = 0.0f, const float AlphaDistance = 0.0f ) {
		m_OrMoreDistanceAlphaFlag	= Flag;
		m_OrMoreStartDistance		= StartDistance;
		m_AlphaDistance				= AlphaDistance;
	}

	// テクスチャの変更.
	void ChangeTexture( const int No );

private:
	// Xファイルからスキンメッシュを作成する.
	HRESULT LoadXMesh( LPCTSTR lpFileName );

	// シェーダを作成する.
	HRESULT CreateShader();
	// インデックスバッファを作成する.
	HRESULT CreateIndexBuffer( DWORD dwSize, int* pIndex, ID3D11Buffer** ppIndexBuffer );
//	void RecursiveSetNewPoseMatrices( BONE* pBone,D3DXMATRIX* pmParent );

	// メッシュを作成する.
	HRESULT CreateAppMeshFromD3DXMesh( LPD3DXFRAME pFrame );

	// コンスタントバッファ作成する.
	HRESULT CreateCBuffer( ID3D11Buffer** pConstantBuffer, UINT size );
	// サンプラを作成する.
	HRESULT CreateSampler( ID3D11SamplerState** pSampler );

	// 全てのメッシュを作成する.
	void BuildAllMesh( D3DXFRAME* pFrame );

	// Xファイルからスキン関連の情報を読み出す関数.
	HRESULT ReadSkinInfo( MYMESHCONTAINER* pContainer, VERTEX* pvVB, SKIN_PARTS_MESH* pParts );

	// ボーンを次のポーズ位置にセットする関数.
	void SetNewPoseMatrices( SKIN_PARTS_MESH* pParts, int frame, MYMESHCONTAINER* pContainer );
	// 次の(現在の)ポーズ行列を返す関数.
	D3DXMATRIX GetCurrentPoseMatrix( SKIN_PARTS_MESH* pParts, int index );

	// フレーム描画.
	void DrawFrame( STransform* pTransform, LPD3DXFRAME pFrame );
	// パーツ描画.
	void DrawPartsMesh( STransform* pTransform, SKIN_PARTS_MESH* p, D3DXMATRIX World, MYMESHCONTAINER* pContainer );
//	void DrawPartsMeshStatic( SKIN_PARTS_MESH* pMesh, D3DXMATRIX World, MYMESHCONTAINER* pContainer );

	// 全てのメッシュを削除.
	void DestroyAllMesh( D3DXFRAME* pFrame );
	HRESULT DestroyAppMeshFromD3DXMesh( LPD3DXFRAME p );

	// マルチバイト文字をUnicode文字に変換する.
	void ConvertCharaMultiByteToUnicode( WCHAR* Dest, size_t DestArraySize, const CHAR* str );

	// テクスチャの作成.
	ID3D11ShaderResourceView* CreateTexture( const std::string& FilePath );
	// テクスチャの破棄.
	void DetachTexture( LPD3DXFRAME p );
	// テクスチャの設定.
	void AttachTexture( LPD3DXFRAME p, ID3D11ShaderResourceView* tex );

	// 差し替え用のテクスチャを作成.
	HRESULT CreateChangeTexture( const std::string& FilePath );

private:
	ID3D11SamplerState*		m_pSampleLinear;
	ID3D11VertexShader*		m_pVertexShader;
	ID3D11PixelShader*		m_pPixelShader;
	ID3D11InputLayout*		m_pVertexLayout;
	ID3D11Buffer*			m_pCBufferPerMesh;		// コンスタントバッファ(メッシュ毎).
	ID3D11Buffer*			m_pCBufferPerMaterial;	// コンスタントバッファ(マテリアル毎).
	ID3D11Buffer*			m_pCBufferPerFrame;		// コンスタントバッファ(フレーム毎).
	ID3D11Buffer*			m_pCBufferPerBone;		// コンスタントバッファ(ボーン毎).

	D3DXMATRIX				m_mWorld;
	D3DXMATRIX				m_mRotation;

	D3DXMATRIX				m_mView;
	D3DXMATRIX				m_mProj;

	D3DXMATRIX*				m_pRotMatrix;

	D3DXVECTOR3				m_vLight;
	D3DXPOSITION3			m_vCamPos;

	double					m_dAnimSpeed;					// アニメーション速度.
	double					m_dAnimTime;					// アニメーション速度.

	D3DXPARSER*				m_pD3dxMesh;					// メッシュ.
	LPD3DXMESH				m_pMeshForRay;					// ﾚｲとﾒｯｼｭ用.

	TCHAR					m_FilePath[256];				// XFileのパス.

	int						m_iFrame;						// アニメーションフレーム.

	std::unique_ptr<CFog>	m_pFog;							// フォグ.
	bool					m_IsFog;						// フォグを有効にするか.
	bool					m_DitherFlag;					// ディザ抜きを使用するか.
	bool					m_AlphaBlockFlag;				// アルファブロックを使用するか.
	bool					m_OrLessDistanceAlphaFlag;		// 距離で透明にするか(以下).
	bool					m_OrMoreDistanceAlphaFlag;		// 距離で透明にするか(以上).

	float					m_OrLessStartDistance;			// 開始する距離(以下).
	float					m_OrMoreStartDistance;			// 開始する距離(以上).
	float					m_AlphaDistance;				// 透明にしていく間隔.

	std::vector<std::string> m_TexturePathList;				// 差し替えテクスチャパスリスト.
};
