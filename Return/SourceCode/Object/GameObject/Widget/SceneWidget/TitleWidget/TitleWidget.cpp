#include "TitleWidget.h"
#include "..\..\..\..\..\Common\SoundManeger\SoundManeger.h"
#include "..\..\..\..\..\Utility\Input\Input.h"

CTitleWidget::CTitleWidget()
{
}

CTitleWidget::~CTitleWidget()
{
}

//---------------------------.
// 初期化.
//---------------------------.
bool CTitleWidget::Init()
{
	SetSceneName( ESceneList::Title );
	GetSpriteList();

	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CTitleWidget::Update( const float & DeltaTime )
{
}

//---------------------------.
// 描画.
//---------------------------.
void CTitleWidget::Render()
{
}
