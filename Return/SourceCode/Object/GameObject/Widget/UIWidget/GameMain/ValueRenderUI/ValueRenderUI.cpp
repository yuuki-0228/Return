#include "ValueRenderUI.h"
#include "..\..\..\..\..\..\Resource\FontResource\FontResource.h"

CValueRenderUI::CValueRenderUI()
	: m_pFont			( nullptr )
	, m_FontStateBase	()
	, m_FontStateList	()
{
}
CValueRenderUI::~CValueRenderUI()
{
}

//---------------------------.
// �C���X�^���X�̎擾.
//---------------------------.
CValueRenderUI* CValueRenderUI::GetInstance()
{
	static std::unique_ptr<CValueRenderUI> pInstance = std::make_unique<CValueRenderUI>();
	return pInstance.get();
}

//---------------------------.
// ������.
//---------------------------.
bool CValueRenderUI::Init()
{
	CValueRenderUI* pI = GetInstance();
	
	// �t�H���g�̎擾.
	pI->m_pFont = CFontResource::GetFont( "�R�[�|���[�g�E���S ver2 Medium", &pI->m_FontStateBase );

	// �t�H���g�̏��x�[�X�̍쐬.
	pI->m_FontStateBase.TextAlign		= ETextAlign::Center;
	pI->m_FontStateBase.IsOutLine		= true;
	pI->m_FontStateBase.OutLineColor	= Color4::DarkGray;
	pI->m_FontStateBase.OutLineSize		= 2.0f;
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CValueRenderUI::Update( const float& DeltaTime )
{
	CValueRenderUI* pI = GetInstance();

	ValueUpdate( DeltaTime );	// �lUI�̍X�V.
	ValueDelete();				// �g�p���Ă��Ȃ��lUI�̍폜.
}

//---------------------------.
// �`��.
//---------------------------.
void CValueRenderUI::Render()
{
	CValueRenderUI* pI = GetInstance();

	for ( auto& s : pI->m_FontStateList ) {
		pI->m_pFont->RenderUI( &s );
	}
}

//---------------------------.
// �l�̒ǉ�.
//---------------------------.
void CValueRenderUI::PushValue( const D3DXPOSITION3 Pos, const int Value, const EValueType Type )
{
	CValueRenderUI* pI = GetInstance();

	// �l�̎擾.
	const int			iDamage = Value;
	const D3DXCOLOR4	Color	= Type == EValueType::Heal ? Color4::Green : Color4::Red;

	// �g�p���Ă��Ȃ��̂�����Ύg�p����.
	for ( auto& s : pI->m_FontStateList ) {
		if ( s.IsDisp ) continue;

		// ������ݒ�.
		s.Transform.Position	= Pos;
		s.Color					= Color;
		s.Text					= std::to_string( iDamage );
		s.IsDisp				= true;
		return;
	}

	// �V�����쐬.
	pI->m_FontStateList.emplace_back( pI->m_FontStateBase );
	pI->m_FontStateList.back().Transform.Position	= Pos;
	pI->m_FontStateList.back().Color				= Color;
	pI->m_FontStateList.back().Text					= std::to_string( iDamage );
}

//---------------------------.
// �lUI�̍X�V.
//---------------------------.
void CValueRenderUI::ValueUpdate( const float& DeltaTime )
{
	CValueRenderUI* pI = GetInstance();

	// �������ɂ��Ȃ����ɂ�����.
	for ( auto& s : pI->m_FontStateList ) {
		if ( s.IsDisp == false ) continue;
		s.Transform.Position.y	-= 16.0f * DeltaTime;
		s.Color.w				-= 0.50f * DeltaTime;

		// �����ɂȂ����ꍇ��\���ɂ���.
		if ( s.Color.w > Color::ALPHA_MIN ) continue;
		s.IsDisp = false;
	}
}

//---------------------------.
// �g�p���Ă��Ȃ��lUI�̍폜.
//---------------------------.
void CValueRenderUI::ValueDelete()
{
	CValueRenderUI* pI = GetInstance();

	const int NormalMax = static_cast< int >( pI->m_FontStateList.size() ) - 1;
	for ( int i = NormalMax; i >= 0; --i ) {
		if ( pI->m_FontStateList[i].IsDisp ) break;
		pI->m_FontStateList.pop_back();
	}
}