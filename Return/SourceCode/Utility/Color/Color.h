#pragma once
#include "..\..\.\Global.h"
#include <algorithm>
#undef RGB

using D3DXCOLOR3 = D3DXVECTOR3;
using D3DXCOLOR4 = D3DXVECTOR4;

/************************************************
*	�F�ꗗ( R,G,B ).
*		���c�F�P.
**/
namespace Color3 {
	constexpr float COLOR_MAX = 1.0f; // �F�̍ő�l.
	constexpr float COLOR_MIN = 0.0f; // �F�̍ŏ��l.
	constexpr float ALPHA_MAX = 1.0f; // �A���t�@�̍ő�l.
	constexpr float ALPHA_MIN = 0.0f; // �A���t�@�̍ŏ��l.

	const D3DXCOLOR3 Red			= { 1.00f, 0.00f, 0.00f };			// �ԐF.
	const D3DXCOLOR3 Green			= { 0.00f, 1.00f, 0.00f };			// �ΐF.
	const D3DXCOLOR3 Blue			= { 0.00f, 0.00f, 1.00f };			// �F.
	const D3DXCOLOR3 Yellow			= { 1.00f, 1.00f, 0.00f };			// ���F.
	const D3DXCOLOR3 Cyan			= { 0.00f, 1.00f, 1.00f };			// ���F.
	const D3DXCOLOR3 Magenta		= { 1.00f, 0.00f, 1.00f };			// �g�F
	const D3DXCOLOR3 Orange			= { 0.95f, 0.59f, 0.00f };			// ��F.
	const D3DXCOLOR3 Chartreuse		= { 0.85f, 0.89f, 0.40f };			// ���ΐF.
	const D3DXCOLOR3 SpringGreen	= { 0.72f, 0.81f, 0.54f };			// �ΐF.
	const D3DXCOLOR3 Azur			= { 0.00f, 0.50f, 1.00f };			// ��F.
	const D3DXCOLOR3 Purple			= { 0.44f, 0.00f, 1.00f };			// ���F.
	const D3DXCOLOR3 Rose			= { 1.00f, 0.00f, 0.50f };			// �K�N�F.
	const D3DXCOLOR3 White			= { 1.00f, 1.00f, 1.00f };			// ���F.
	const D3DXCOLOR3 LightGray		= { 0.75f, 0.75f, 0.75f };			// �����D�F.
	const D3DXCOLOR3 Gray			= { 0.50f, 0.50f, 0.50f };			// �D�F.
	const D3DXCOLOR3 DarkGray		= { 0.25f, 0.25f, 0.25f };			// �Z���D�F.
	const D3DXCOLOR3 Black			= { 0.00f, 0.00f, 0.00f };			// ���F.

	// �������p.
	const D3DXCOLOR3 INIT = { COLOR_MAX, COLOR_MAX, COLOR_MAX };
	const D3DXCOLOR3 None = { COLOR_MIN, COLOR_MIN, COLOR_MIN };

	// �A���t�@�l��͈͓��Ɏ��߂�.
	inline float AlphaClamp( float  Alpha ) { return std::clamp( Alpha, ALPHA_MIN, ALPHA_MAX ); }
	inline void	 AlphaClamp( float* Alpha ) { *Alpha = AlphaClamp( *Alpha ); }
	
	// �F( R,G,B )���쐬.
	inline D3DXCOLOR3 RGB( const float r,	const float g,	const float b ) { return D3DXCOLOR3( r, g, b ); }
	inline D3DXCOLOR3 RGB( const int r,		const int g,	const int b ) { return D3DXCOLOR3( static_cast<float>( r ) / 255.0f, static_cast<float>( g ) / 255.0f, static_cast<float>( b ) / 255.0f ); }
}
// �F�ꗗ( R,G,B ).
namespace Color = Color3;

/************************************************
*	�F�ꗗ( R,G,B,A ).
*		���c�F�P.
**/
namespace Color4 {
	constexpr float COLOR_MAX = 1.0f; // �F�̍ő�l.
	constexpr float COLOR_MIN = 0.0f; // �F�̍ŏ��l.
	constexpr float ALPHA_MAX = 1.0f; // �A���t�@�̍ő�l.
	constexpr float ALPHA_MIN = 0.0f; // �A���t�@�̍ŏ��l.

	const D3DXCOLOR4 Red			= { 1.00f, 0.00f, 0.00f, ALPHA_MAX };	// �ԐF.
	const D3DXCOLOR4 Green			= { 0.00f, 1.00f, 0.00f, ALPHA_MAX };	// �ΐF.
	const D3DXCOLOR4 Blue			= { 0.00f, 0.00f, 1.00f, ALPHA_MAX };	// �F.
	const D3DXCOLOR4 Yellow			= { 1.00f, 1.00f, 0.00f, ALPHA_MAX };	// ���F.
	const D3DXCOLOR4 Cyan			= { 0.00f, 1.00f, 1.00f, ALPHA_MAX };	// ���F.
	const D3DXCOLOR4 Magenta		= { 1.00f, 0.00f, 1.00f, ALPHA_MAX };	// �g�F
	const D3DXCOLOR4 Orange			= { 0.95f, 0.59f, 1.00f, ALPHA_MAX };	// ��F.
	const D3DXCOLOR4 Chartreuse		= { 0.85f, 0.89f, 0.40f, ALPHA_MAX };	// ���ΐF.
	const D3DXCOLOR4 SpringGreen	= { 0.72f, 0.81f, 0.54f, ALPHA_MAX };	// �ΐF.
	const D3DXCOLOR4 Azur			= { 0.00f, 0.50f, 1.00f, ALPHA_MAX };	// ��F.
	const D3DXCOLOR4 Purple			= { 0.44f, 0.00f, 1.00f, ALPHA_MAX };	// ���F.
	const D3DXCOLOR4 Rose			= { 1.00f, 0.00f, 0.50f, ALPHA_MAX };	// �K�N�F.
	const D3DXCOLOR4 White			= { 1.00f, 1.00f, 1.00f, ALPHA_MAX };	// ���F.
	const D3DXCOLOR4 LightGray		= { 0.75f, 0.75f, 0.75f, ALPHA_MAX };	// �����D�F.
	const D3DXCOLOR4 Gray			= { 0.50f, 0.50f, 0.50f, ALPHA_MAX };	// �D�F.
	const D3DXCOLOR4 DarkGray		= { 0.25f, 0.25f, 0.25f, ALPHA_MAX };	// �Z���D�F.
	const D3DXCOLOR4 Black			= { 0.00f, 0.00f, 0.00f, ALPHA_MAX };	// ���F.

	// �������p.
	const D3DXCOLOR4 INIT = { COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX };
	const D3DXCOLOR4 None = { COLOR_MIN, COLOR_MIN, COLOR_MIN, ALPHA_MIN };

	// �A���t�@�l��͈͓��Ɏ��߂�.
	inline float AlphaClamp( float  Alpha ) { return std::clamp( Alpha, ALPHA_MIN, ALPHA_MAX ); }
	inline void	 AlphaClamp( float* Alpha ) { *Alpha = AlphaClamp( *Alpha ); }

	// �F( R,G,B,A )���쐬.
	inline D3DXCOLOR4 RGBA( const D3DXCOLOR3& Color, const float a = ALPHA_MAX ) { return D3DXCOLOR4( Color, a ); }
	inline D3DXCOLOR4 RGBA( const float r,	const float g,	const float b,	const float a = ALPHA_MAX ) { return D3DXCOLOR4( r, g, b, a ); }
	inline D3DXCOLOR4 RGBA( const int r,	const int g,	const int b,	const float a = ALPHA_MAX ) { return D3DXCOLOR4( static_cast<float>( r ) / 255.0f, static_cast<float>( g ) / 255.0f, static_cast<float>( b ) / 255.0f, a ); }
}
