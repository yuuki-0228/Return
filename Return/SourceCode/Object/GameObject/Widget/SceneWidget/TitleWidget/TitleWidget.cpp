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
// ‰Šú‰».
//---------------------------.
bool CTitleWidget::Init()
{
	SetSceneName( ESceneList::Title );
	GetSpriteList();

	return true;
}

//---------------------------.
// XV.
//---------------------------.
void CTitleWidget::Update( const float & DeltaTime )
{
}

//---------------------------.
// •`‰æ.
//---------------------------.
void CTitleWidget::Render()
{
}
