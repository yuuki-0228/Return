#pragma once
#include "..\..\Global.h"

// ラスタライザステート.
enum class enRS_STATE
{
	None,	// 正背面描画.
	Back,	// 背面を描画しない.
	Front,	// 正面を描画しない.
	Wire,	// ワイヤーフレーム描画.

	Max,
} typedef ERS_STATE;

/************************************************
*	DirectX11 セットアップ.
**/
class CDirectX11 final
{
public:
	CDirectX11();
	~CDirectX11();

	// インスタンスの取得.
	static CDirectX11* GetInstance();

	// DirectX11構築.
	static HRESULT Create( HWND hWnd );
	// DirectX11解放.
	static void Release();

	// バックバッファクリア関数.
	static void ClearBackBuffer();
	// 表示.
	static void Present();

	// デバイスを取得.
	static ID3D11Device* GetDevice() { return GetInstance()->m_pDevice11; }
	// デバイスコンテキストを取得.
	static ID3D11DeviceContext* GetContext() { return GetInstance()->m_pContext11; }
	// デプスステンシルビューの取得.
	static ID3D11DepthStencilView* GetDepthSV() { return GetInstance()->m_pBackBuffer_DSTexDSV; }
	// ウィンドウサイズの取得.
	static UINT GetWndWidth()	{ return GetInstance()->m_WndWidth;		}
	static UINT GetWndHeight()	{ return GetInstance()->m_WndHeight;	}
	// 背景色の取得.
	static D3DXCOLOR4 GetBackColor() { return GetInstance()->m_BackColor; }
	// 深度(Z)テストが有効か取得.
	static bool GetDepth() { return GetInstance()->m_IsDepth; }
	// アルファブレンドが有効か取得.
	static bool GetAlphaBlend() { return GetInstance()->m_IsAlphaBlend; }
	// アルファトゥカバレージが有効か取得.
	static bool GetAlphaToCoverage() { return GetInstance()->m_IsAlphaToCoverage; }

	// 背景色の設定.
	static void SetBackColor( const D3DXCOLOR4& Color ) { GetInstance()->m_BackColor = Color; }
	// 深度(Z)テストON/OFF切り替え.
	static void SetDepth( bool flag );
	// アルファブレンドON/OFF切り替え.
	static void SetAlphaBlend( bool flag );
	// アルファトゥカバレージON/OFF切り替え.
	static void SetAlphaToCoverage( bool flag );
	// ラスタライザステート設定.
	static void SetRasterizerState( const ERS_STATE& RsState );
	// マウスカーソルを表示するかの設定.
	static void SetIsDispMouseCursor( const bool Flag ) { GetInstance()->m_IsDispMouseCursor = Flag; }

	// フルスクリーンの設定.
	static void SetFullScreen();
	// フルスクリーン状態か取得.
	static bool IsFullScreen();

	// アクティブウィンドウか確認.
	static void CheckActiveWindow();
	// ウィンドウがアクティブか取得.
	static bool IsWindowActive() { return GetInstance()->m_IsWindowActive; };

private:
	// デバイスとスワップチェイン作成.
	HRESULT CreateDeviceAndSwapChain();

	// バックバッファ作成：カラー用レンダーターゲットビュー作成.
	HRESULT CreateColorBackBufferRTV();

	// バックバッファ作成：デプスステンシル用レンダーターゲットビュー作成.
	HRESULT CreateDepthStencilBackBufferRTV();

	// ラスタライザ作成.
	HRESULT CreateRasterizer();

	// デプスステンシルステート作成.
	HRESULT CreateDepthStencilState();

	// アルファブレンドステート作成.
	HRESULT CreateAlphaBlendState();

	// ビューポート作成.
	HRESULT CreateViewports();

private:
	HWND						m_hWnd;						// ウィンドウハンドル.

	ID3D11Device*				m_pDevice11;				// デバイスオブジェクト.
	ID3D11DeviceContext*		m_pContext11;				// デバイスコンテキスト.
	IDXGISwapChain*				m_pSwapChain;				// スワップチェーン.
	ID3D11RenderTargetView*		m_pBackBuffer_TexRTV;		// レンダ―ターゲットビュー.
	ID3D11Texture2D*			m_pBackBuffer_DSTex;		// デプスステンシル用テクスチャ.
	ID3D11DepthStencilView*		m_pBackBuffer_DSTexDSV;		// デプスステンシルビュー.

	// 深度(Z)テスト設定.
	ID3D11DepthStencilState*	m_pDepthStencilStateOn;		// 有効設定.
	ID3D11DepthStencilState*	m_pDepthStencilStateOff;	// 無効設定.

	// アルファブレンド.
	ID3D11BlendState*			m_pAlphaBlendOn;			// 有効設定.
	ID3D11BlendState*			m_pAlphaBlendOff;			// 無効設定.

	// アルファトゥカバレージ.
	ID3D11BlendState*			m_pAlphaToCoverageOn;		// 有効設定..

	// ラスタライザステート.
	ID3D11RasterizerState*		m_pCullNone;				// 正背面描画.
	ID3D11RasterizerState*		m_pCullBack;				// 背面を描画しない.
	ID3D11RasterizerState*		m_pCullFront;				// 正面を描画しない.
	ID3D11RasterizerState*		m_pWireFrame;				// ワイヤーフレーム描画.

	UINT						m_WndWidth;					// ウィンドウ幅.
	UINT						m_WndHeight;				// ウィンドウ高さ.

	D3DXCOLOR4					m_BackColor;				// 背景色.

	bool						m_IsWindowActive;			// アクティブウィンドウか確認.
	bool						m_IsDepth;					// 深度(Z)テストが有効か.
	bool						m_IsAlphaBlend;				// アルファブレンドが有効か.
	bool						m_IsAlphaToCoverage;		// アルファトゥカバレージが有効か.
	bool						m_IsDispMouseCursor;		// マウスカーソルを表示するか.

private:
	// コピー・ムーブコンストラクタ, 代入演算子の削除.
	CDirectX11( const CDirectX11 & )				= delete;
	CDirectX11& operator = ( const CDirectX11 & )	= delete;
	CDirectX11( CDirectX11 && )						= delete;
	CDirectX11& operator = ( CDirectX11 && )		= delete;
};