#pragma once
#include "..\..\Global.h"

class CNormalFade;
class CUFOFade;

// フェードタイプ.
enum class enFadeType : unsigned char
{
	None,
	
	NormalFade,	// 普通のフェード.
	UFOFade,	// UFO型に切り抜かれたフェード.
} typedef EFadeType;

/************************************************
*	フェードマネージャークラス.
*		�ｱ田友輝.
**/
class CFadeManager final
{
public:
	CFadeManager();
	~CFadeManager();

	// インスタンスの取得.
	static CFadeManager* GetInstance();

	// 初期化.
	static bool Init();
	// 描画.
	static void Render();

	// フェードイン.
	static bool FadeIn( const EFadeType Type, const float Speed );
	// フェードアウト.
	static bool FadeOut( const EFadeType Type, const float Speed );

	// フェード中か取得.
	static bool GetIsFade() { return GetInstance()->m_NowFadeType != EFadeType::None; }

private:
	EFadeType						m_NowFadeType;	// 現在のフェードタイプ.
	std::unique_ptr<CNormalFade>	m_pNormalFade;	// 普通のフェード.
	std::unique_ptr<CUFOFade>		m_pUFOFade;		// UFO型に切り抜かれたフェード.
};