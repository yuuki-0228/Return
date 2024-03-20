#include "Common.h"

CCommon::CCommon()
	: m_pDevice		( nullptr )
	, m_pContext	( nullptr )
	, m_Transform	()
	, m_Color		( Color4::White )
{
	m_pContext	= CDirectX11::GetContext();
	m_pDevice	= CDirectX11::GetDevice();
}

CCommon::~CCommon()
{
	m_pContext	= nullptr;
	m_pDevice	= nullptr;
}