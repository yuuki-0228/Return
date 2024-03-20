#include "DebugFlagWindow.h"
#include "..\FlagManager\FlagManager.h"
#include "..\..\ImGuiManager\ImGuiManager.h"
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>

CDebugFlagWindow::CDebugFlagWindow()
	: m_FindString			( "" )
	, m_IsDispNameNoneFlag	( false, u8"�����̃t���O��\��/��\��", "" )
{
}

CDebugFlagWindow::~CDebugFlagWindow()
{
}

//----------------------------.
// �C���X�^���X�̎擾.
//----------------------------.
CDebugFlagWindow* CDebugFlagWindow::GetInstance()
{
	static std::unique_ptr<CDebugFlagWindow> pInstance = std::make_unique<CDebugFlagWindow>();
	return pInstance.get();
}

//----------------------------.
// �`��.
//----------------------------.
void CDebugFlagWindow::Render()
{
	CDebugFlagWindow* pI = GetInstance();

	ImGui::Begin( "DebugFlagWindow" );

	// ���b�Z�[�W�����̕\��.
	FlagFind();

	// �e�t���O�̃O���[�v���Ƃ̏������X�g.
	std::unordered_map<std::string, std::vector<CFlagManager::Handle>> GroupFuncList;

	// �t���O��\�����邩���ׂ�.
	ImGui::BeginChild( ImGui::GetID( ( void* )0 ), ImVec2( 0.0f, 0.0f ), ImGuiWindowFlags_NoTitleBar );
	const CFlagManager::Handle Size = CFlagManager::GetFlagNum();
	for ( CFlagManager::Handle i = 0; i < Size; i++ ) {
		std::string Name	= CFlagManager::GetName( i );
		std::string Group	= CFlagManager::GetGroup( i );

		// �\������t���O�����ׂ�.
		if ( pI->m_IsDispNameNoneFlag == false && Name == "" ) continue;
		if ( Name == "" ) {
			// �����̃t���O�ɉ��̖��O������.
			Name	= "NameNoneFlag_" + std::to_string( static_cast<int>( i ) );
			CFlagManager::SetGroup( i, "NameNone" );
		}
		if ( Name.find( pI->m_FindString ) == std::string::npos ) continue;

		// �����̒ǉ�.
		GroupFuncList[Group.c_str()].emplace_back( i );
	}

	// �t���O�̕\��.
	for ( auto& [Group, Handle] : GroupFuncList ) {
		// �O���[�v�Ɏw�肪�Ȃ��ꍇ���̂܂ܕ\������.
		if ( Group == "" ) {
			for ( auto& h : Handle ) {
				bool				Flag = CFlagManager::IsBitFlag( h );
				const std::string	Name = CFlagManager::GetName( h );
				CFlagManager::SetBitFlag( h, CImGuiManager::CheckBox( Name.c_str(), &Flag ) );
			}
			continue;
		}

		// �O���[�v�Ɏw�肪����O���[�v���쐬���\������.
		if ( ImGui::TreeNodeEx( Group.c_str(), ImGuiTreeNodeFlags_Framed ) ) {
			for ( auto& h : Handle ) {
				bool		Flag = CFlagManager::IsBitFlag( h );
				std::string	Name = CFlagManager::GetName( h );
				if ( Group == "NameNone" ) Name = "NameNoneFlag_" + std::to_string( static_cast<int>( h ) );
				CFlagManager::SetBitFlag( h, CImGuiManager::CheckBox( Name.c_str(), &Flag ) );
			}
			ImGui::TreePop();
		}
	}

	ImGui::EndChild();
	ImGui::End();
}

//---------------------.
//	�t���O��������.
//---------------------.
void CDebugFlagWindow::FlagFind()
{
	ImGui::Text( u8"���� :" );
	ImGui::SameLine();
	ImGui::InputText( " ", &GetInstance()->m_FindString );
	ImGui::Separator();
}
