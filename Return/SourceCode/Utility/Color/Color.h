#pragma once
#include "..\..\.\Global.h"
#include <algorithm>
#undef RGB

using D3DXCOLOR3 = D3DXVECTOR3;
using D3DXCOLOR4 = D3DXVECTOR4;

/************************************************
*	色一覧( R,G,B ).
*		﨑田友輝.
**/
namespace Color3 {
	constexpr float COLOR_MAX = 1.0f; // 色の最大値.
	constexpr float COLOR_MIN = 0.0f; // 色の最小値.
	constexpr float ALPHA_MAX = 1.0f; // アルファの最大値.
	constexpr float ALPHA_MIN = 0.0f; // アルファの最小値.

	const D3DXCOLOR3 Red			= { 1.00f, 0.00f, 0.00f };			// 赤色.
	const D3DXCOLOR3 Green			= { 0.00f, 1.00f, 0.00f };			// 緑色.
	const D3DXCOLOR3 Blue			= { 0.00f, 0.00f, 1.00f };			// 青色.
	const D3DXCOLOR3 Yellow			= { 1.00f, 1.00f, 0.00f };			// 黄色.
	const D3DXCOLOR3 Cyan			= { 0.00f, 1.00f, 1.00f };			// 水色.
	const D3DXCOLOR3 Magenta		= { 1.00f, 0.00f, 1.00f };			// 紅色
	const D3DXCOLOR3 Orange			= { 0.95f, 0.59f, 0.00f };			// 橙色.
	const D3DXCOLOR3 Chartreuse		= { 0.85f, 0.89f, 0.40f };			// 黄緑色.
	const D3DXCOLOR3 SpringGreen	= { 0.72f, 0.81f, 0.54f };			// 青緑色.
	const D3DXCOLOR3 Azur			= { 0.00f, 0.50f, 1.00f };			// 空色.
	const D3DXCOLOR3 Purple			= { 0.44f, 0.00f, 1.00f };			// 紫色.
	const D3DXCOLOR3 Rose			= { 1.00f, 0.00f, 0.50f };			// 薔薇色.
	const D3DXCOLOR3 White			= { 1.00f, 1.00f, 1.00f };			// 白色.
	const D3DXCOLOR3 LightGray		= { 0.75f, 0.75f, 0.75f };			// 薄い灰色.
	const D3DXCOLOR3 Gray			= { 0.50f, 0.50f, 0.50f };			// 灰色.
	const D3DXCOLOR3 DarkGray		= { 0.25f, 0.25f, 0.25f };			// 濃い灰色.
	const D3DXCOLOR3 Black			= { 0.00f, 0.00f, 0.00f };			// 黒色.

	// 初期化用.
	const D3DXCOLOR3 INIT = { COLOR_MAX, COLOR_MAX, COLOR_MAX };
	const D3DXCOLOR3 None = { COLOR_MIN, COLOR_MIN, COLOR_MIN };

	// アルファ値を範囲内に収める.
	inline float AlphaClamp( float  Alpha ) { return std::clamp( Alpha, ALPHA_MIN, ALPHA_MAX ); }
	inline void	 AlphaClamp( float* Alpha ) { *Alpha = AlphaClamp( *Alpha ); }
	
	// 色( R,G,B )を作成.
	inline D3DXCOLOR3 RGB( const float r,	const float g,	const float b ) { return D3DXCOLOR3( r, g, b ); }
	inline D3DXCOLOR3 RGB( const int r,		const int g,	const int b ) { return D3DXCOLOR3( static_cast<float>( r ) / 255.0f, static_cast<float>( g ) / 255.0f, static_cast<float>( b ) / 255.0f ); }
}
// 色一覧( R,G,B ).
namespace Color = Color3;

/************************************************
*	色一覧( R,G,B,A ).
*		﨑田友輝.
**/
namespace Color4 {
	constexpr float COLOR_MAX = 1.0f; // 色の最大値.
	constexpr float COLOR_MIN = 0.0f; // 色の最小値.
	constexpr float ALPHA_MAX = 1.0f; // アルファの最大値.
	constexpr float ALPHA_MIN = 0.0f; // アルファの最小値.

	const D3DXCOLOR4 Red			= { 1.00f, 0.00f, 0.00f, ALPHA_MAX };	// 赤色.
	const D3DXCOLOR4 Green			= { 0.00f, 1.00f, 0.00f, ALPHA_MAX };	// 緑色.
	const D3DXCOLOR4 Blue			= { 0.00f, 0.00f, 1.00f, ALPHA_MAX };	// 青色.
	const D3DXCOLOR4 Yellow			= { 1.00f, 1.00f, 0.00f, ALPHA_MAX };	// 黄色.
	const D3DXCOLOR4 Cyan			= { 0.00f, 1.00f, 1.00f, ALPHA_MAX };	// 水色.
	const D3DXCOLOR4 Magenta		= { 1.00f, 0.00f, 1.00f, ALPHA_MAX };	// 紅色
	const D3DXCOLOR4 Orange			= { 0.95f, 0.59f, 1.00f, ALPHA_MAX };	// 橙色.
	const D3DXCOLOR4 Chartreuse		= { 0.85f, 0.89f, 0.40f, ALPHA_MAX };	// 黄緑色.
	const D3DXCOLOR4 SpringGreen	= { 0.72f, 0.81f, 0.54f, ALPHA_MAX };	// 青緑色.
	const D3DXCOLOR4 Azur			= { 0.00f, 0.50f, 1.00f, ALPHA_MAX };	// 空色.
	const D3DXCOLOR4 Purple			= { 0.44f, 0.00f, 1.00f, ALPHA_MAX };	// 紫色.
	const D3DXCOLOR4 Rose			= { 1.00f, 0.00f, 0.50f, ALPHA_MAX };	// 薔薇色.
	const D3DXCOLOR4 White			= { 1.00f, 1.00f, 1.00f, ALPHA_MAX };	// 白色.
	const D3DXCOLOR4 LightGray		= { 0.75f, 0.75f, 0.75f, ALPHA_MAX };	// 薄い灰色.
	const D3DXCOLOR4 Gray			= { 0.50f, 0.50f, 0.50f, ALPHA_MAX };	// 灰色.
	const D3DXCOLOR4 DarkGray		= { 0.25f, 0.25f, 0.25f, ALPHA_MAX };	// 濃い灰色.
	const D3DXCOLOR4 Black			= { 0.00f, 0.00f, 0.00f, ALPHA_MAX };	// 黒色.

	// 初期化用.
	const D3DXCOLOR4 INIT = { COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX };
	const D3DXCOLOR4 None = { COLOR_MIN, COLOR_MIN, COLOR_MIN, ALPHA_MIN };

	// アルファ値を範囲内に収める.
	inline float AlphaClamp( float  Alpha ) { return std::clamp( Alpha, ALPHA_MIN, ALPHA_MAX ); }
	inline void	 AlphaClamp( float* Alpha ) { *Alpha = AlphaClamp( *Alpha ); }

	// 色( R,G,B,A )を作成.
	inline D3DXCOLOR4 RGBA( const D3DXCOLOR3& Color, const float a = ALPHA_MAX ) { return D3DXCOLOR4( Color, a ); }
	inline D3DXCOLOR4 RGBA( const float r,	const float g,	const float b,	const float a = ALPHA_MAX ) { return D3DXCOLOR4( r, g, b, a ); }
	inline D3DXCOLOR4 RGBA( const int r,	const int g,	const int b,	const float a = ALPHA_MAX ) { return D3DXCOLOR4( static_cast<float>( r ) / 255.0f, static_cast<float>( g ) / 255.0f, static_cast<float>( b ) / 255.0f, a ); }
}
