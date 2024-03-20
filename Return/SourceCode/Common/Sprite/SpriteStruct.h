#pragma once
#include "..\..\Global.h"

// ローカル座標の番号.
enum class enLocalPosition : unsigned char
{
	LeftUp,		// 0:左上.
	Left,		// 1:左.
	LeftDown,	// 2:左下.
	Down,		// 3:下.
	RightDown,	// 4:右下.
	Right,		// 5:右.
	RightUp,	// 6:右上.
	Up,			// 7:上.
	Center,		// 8:中央.

	Max,
	None = 0
} typedef ELocalPosition;

// サンプラステート.
enum class enSamplerState : unsigned char
{
	Wrap,		// タイル状に繰り返される.
	Mirror,		// 反転しながら繰り返される.
	Clamp,		// 0~1に設定される.
	Border,		// 0~1の範囲外はhlslで指定した色になる.
	MirrorOne,	// 0を中心に反転される.

	Max,
	None = 0
} typedef ESamplerState;

// コンスタントバッファのアプリ側の定義.
//	※シェーダ内のコンスタントバッファと一致している必要あり.
struct stSpriteShaderConstantBuffer
{
	ALIGN16 D3DXMATRIX	mWVP;				// ワールド,ビュー,プロジェクションの合成変換行列.	
	ALIGN16 D3DXCOLOR4	vColor;				// カラー(RGBAの型に合わせる).
	ALIGN16 D3DXVECTOR4	vUV;				// UV座標(x,yのみ使用).
	ALIGN16 D3DXVECTOR4	vRenderArea;		// 描画するエリア.
	ALIGN16 float		fViewPortWidth;		// ビューポート幅.
	ALIGN16 float		fViewPortHeight;	// ビューポート高さ.
	ALIGN16 D3DXVECTOR4 vDitherFlag;		// ディザ抜きを使用するか(xが1以上なら使用する).
	ALIGN16 D3DXVECTOR4 vAlphaBlockFlag;	// アルファブロックを使用するか(xが1以上なら使用する).
} typedef SSpriteShaderConstantBuffer;

// 幅高さ構造体.
struct stSize
{
	float w;
	float h;

	stSize()
		: stSize ( 0.0f, 0.0f )
	{
	}
	stSize( const float size[2] )
		: w	( size[0] )
		, h	( size[1] )
	{
	}
	stSize( const float w, const float h )
		: w	( w )
		, h	( h )
	{
	}
} typedef SSize;

// 頂点の構造体.
struct stVertex
{
	D3DXPOSITION3	Pos;	// 頂点座標.
	D3DXVECTOR2		Tex;	// テクスチャ座標.
} typedef SVertex;

//スプライト構造体.
struct stSpriteState
{
	SSize									Disp;				// 表示幅高さ.
	SSize									Base;				// 元画像幅高さ.
	SSize									Stride;				// 1コマ当たりの幅高さ.
	D3DXVECTOR2								OffSet;				// 補正値.
	ELocalPosition							LocalPosNo;			// ローカル座標の番号.
	std::vector<std::string>				DispSceneList;		// 表示するシーン名.
	std::unordered_map<std::string, int>	SceneDispNum;		// シーンに表示する個数.
	std::string								FilePath;			// ファイルパス.
} typedef SSpriteState;

// UIアニメーション情報構造体.
struct stUIAnimState
{
	std::string		UIAnimWhen;			// いつ行うか.
	std::string		UIAnimWhat;			// 何を行うか.
	std::string		EndMessage;			// 再生終了時のメッセージ名.
	STransform		StartTransform;		// 初期のトランスフォーム.
	D3DXCOLOR4		StartColor;			// 初期の色.
	bool			Flag_x;				// x方向のフラグ.
	bool			Flag_y;				// y方向のフラグ.
	bool			IsLoop;				// ループさせるか.
	bool			IsAnimPlay;			// アニメーションをするか.
	float			Cnt_x;				// x方向のカウント.
	float			Cnt_y;				// x方向のカウント.
	float			Speed;				// 速度.
	int				PlayCnt;			// 再生カウント.
	int				PlayMax;			// 再生上限.

	stUIAnimState()
		: UIAnimWhen		( "None" )
		, UIAnimWhat		( "None" )
		, EndMessage		( "" )
		, StartTransform	()
		, StartColor		( Color4::White )
		, Flag_x			( false )
		, Flag_y			( false )
		, IsLoop			( true )
		, IsAnimPlay		( false )
		, Cnt_x				( 0.0f )
		, Cnt_y				( 0.0f )
		, Speed				( 0.0f )
		, PlayCnt			( 0 )
		, PlayMax			( 0 )
	{}
} typedef SUIAnimState;

// スプライトアニメーション情報構造体.
struct stSpriteAnimState
{
	POINT			PatternNo;				// パターン番号.
	POINT			PatternMax;				// パターン最大数.
	D3DXVECTOR2		UV;						// テクスチャUV座標.
	D3DXVECTOR2		Scroll;					// UVスクロール.
	D3DXVECTOR2		ScrollSpeed;			// テクスチャUV座標.
	int				AnimNumber;				// アニメーション番号.
	int				AnimNumberMax;			// アニメーション番号の最大数.
	int				FrameCount;				// フレームカウント.
	int				FrameCountSpeed;		// フレームカウント速度.
	int				FrameTime;				// フレームタイム.
	bool			IsOneAnimPlay;			// アニメーションを一回だけ行うか.
	bool			IsSetAnimNumber;		// アニメーション番号を設定するか.
	bool			IsAnimation;			// アニメーションするか.
	bool			IsAnimationX;			// Xのみアニメーションするか.
	bool			IsAnimationY;			// Yのみアニメーションするか.
	bool			IsUVScrollX;			// X方向にUVスクロールをするか.
	bool			IsUVScrollY;			// Y方向にUVスクロールをするか.

	stSpriteAnimState()
		: PatternNo			( { 0, 0 } )
		, PatternMax		( { 1, 1 } )
		, UV				( 0.0f, 0.0f )
		, ScrollSpeed		( 0.0f, 0.0f )
		, AnimNumber		( 0 )
		, AnimNumberMax		( 1 )
		, FrameCount		( 0 )
		, FrameCountSpeed	( 1 )
		, FrameTime			( 10 )
		, IsOneAnimPlay		( false )
		, IsSetAnimNumber	( false )
		, IsAnimation		( false )
		, IsAnimationX		( false )
		, IsAnimationY		( false )
		, IsUVScrollX		( false )
		, IsUVScrollY		( false )
	{}
	
	//--------------------------.
	// UV座標に変換.
	//--------------------------.
	D3DXVECTOR2 ConvertUV() {
		return {
			static_cast<float>( PatternNo.x ) / static_cast<float>( PatternMax.x ),
			static_cast<float>( PatternNo.y ) / static_cast<float>( PatternMax.y )
		};
	}

	//--------------------------.
	// アニメーション番号の設定.
	//	アニメーション番号を設定をしない場合は処理は行わない.
	//--------------------------.
	void SetAnimNumber() {
		if ( IsSetAnimNumber == false ) return;

		// アニメーションをしない.
		IsAnimation = false;

		// アニメーション番号を算出.
		PatternNo.x = AnimNumber % PatternMax.x;
		PatternNo.y = AnimNumber / PatternMax.x;

		// UV座標に変換.
		UV = ConvertUV();
	}

	//--------------------------.
	// アニメーションの更新.
	//	アニメーションしない場合は処理は行わない.
	//--------------------------.
	void AnimUpdate() {
		if( IsAnimation  == false &&
			IsAnimationX == false &&
			IsAnimationY == false ) return;

		FrameCount += FrameCountSpeed;
		if ( FrameCount % FrameTime != 0 ) return;
		if ( IsAnimation ) {
			// アニメーション番号を算出.
			PatternNo.x = AnimNumber % PatternMax.x;
			PatternNo.y = AnimNumber / PatternMax.x;
			AnimNumber++;	// アニメーション番号を加算.

			// アニメーション番号が最大アニメーション数より多ければ初期化する.
			if ( AnimNumber >= AnimNumberMax ) {
				FrameCount = 0;
				AnimNumber = 0;

				// アニメーションを終了する.
				if ( IsOneAnimPlay ) IsAnimation = false;
			}

			// UV座標に変換.
			UV = ConvertUV();
			return;
		}

		if ( IsAnimationX ) {
			// アニメーション番号を加算.
			PatternNo.x++;
			AnimNumber++;

			// アニメーション番号が最大アニメーション数より多ければ初期化する.
			if ( PatternNo.x >= PatternMax.x ) {
				FrameCount	= 0;
				AnimNumber	= 0;
				PatternNo.x = 0;

				// アニメーションを終了する.
				if ( IsOneAnimPlay ) IsAnimation = false;
			}

			// UV座標に変換.
			UV.x = ConvertUV().x;
			return;
		}

		if ( IsAnimationY ) {
			// アニメーション番号を加算.
			PatternNo.y++;
			AnimNumber++;

			// アニメーション番号が最大アニメーション数より多ければ初期化する.
			if ( PatternNo.y >= PatternMax.y ) {
				FrameCount	= 0;
				AnimNumber	= 0;
				PatternNo.y = 0;

				// アニメーションを終了する.
				if ( IsOneAnimPlay ) IsAnimation = false;
			}

			// UV座標に変換.
			UV.y = ConvertUV().y;
			return;
		}
	}

	//--------------------------.
	// UVスクロールの更新.
	//--------------------------.
	void UVScrollUpdate()
	{
		if ( IsUVScrollX ) {
			UV.x += ScrollSpeed.x;
			if ( UV.x > 1.0f ) UV.x -= 1.0f;
			if ( UV.x < 0.0f ) UV.x += 1.0f;
		}

		if ( IsUVScrollY ) {
			UV.y += ScrollSpeed.y;
			if ( UV.y > 1.0f ) UV.y -= 1.0f;
			if ( UV.y < 0.0f ) UV.y += 1.0f;
		}
	}

} typedef SSpriteAnimState;

// 描画用画像情報構造体.
struct stSpriteRenderState
{
	SSpriteAnimState	AnimState;			// アニメーション情報.
	SUIAnimState		UIAnimState;		// UIアニメーション情報.
	STransform			Transform;			// トランスフォーム.
	ESamplerState		SmaplerNo;			// サンプラ番号.
	D3DXCOLOR4			Color;				// 色.
	D3DXVECTOR4			RenderArea;			// 描画するエリア( 左上x, 左上y, 幅, 高さ ).
	std::string			SceneName;			// どのシーンか.
	std::string			No;					// どの設定の画像か.
	bool				IsDisp;				// 表示するか.
	bool				IsGrab;				// つかまれているか.
	bool				IsLock;				// つかめなくするか.

	stSpriteRenderState()
		: AnimState		()
		, UIAnimState	()
		, Transform		()
		, SmaplerNo		( ESamplerState::Wrap )
		, Color			( Color4::White )
		, RenderArea	( 0, 0, WND_W, WND_H )
		, SceneName		( "" )
		, No			( "" )
		, IsDisp		( true )
		, IsGrab		( false )
		, IsLock		( false )
	{}

} typedef SSpriteRenderState;
