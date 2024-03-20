#include "StageEditor.h"
#include "..\..\Object\GameObject\Actor\StageManager\StageManager.h"
#include "..\..\Utility\ImGuiManager\MessageWindow\MessageWindow.h"
#include "..\..\Utility\Input\Input.h"

namespace {
	constexpr char STAGE_FILE_PATH[] = "Data\\Parameter\\Stage.json";
}

CStageEditor::CStageEditor()
	: m_pStage			( nullptr )
	, m_SelectObject	( "ATypeApartment" )
{
}

CStageEditor::~CStageEditor()
{
}

//---------------------------.
// ������.
//---------------------------.
bool CStageEditor::Init()
{
	// �X�e�[�W�̏�����.
	m_pStage = std::make_unique<CStageManager>();
	m_pStage->Init();

	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CStageEditor::Update( const float& DeltaTime )
{
	m_pStage->Update( DeltaTime );
	m_pStage->Update( DeltaTime );
}

//---------------------------.
// ImGui�`��.
//---------------------------.
void CStageEditor::ImGuiRender()
{
	ImGui::Begin( "StageEditor" );
	if ( ImGui::BeginTabBar( "TabBar", ImGuiTabBarFlags_FittingPolicyScroll | ImGuiTabBarFlags_TabListPopupButton ) ) {
		// �V�[���^�u.
		if ( ImGui::BeginTabItem( "Scene" ) ) {
			ImGui::BeginChild( ImGui::GetID( (void*) 0 ), ImVec2( 0.0f, 0.0f ), ImGuiWindowFlags_NoTitleBar );

			// �X�e�[�W�̐ݒ�.
			m_pStage->StageEditorImGuiRender();

			// �ۑ�.
			if ( CImGuiManager::Button( "Save" ) || CKeyInput::IsANDKeyDown( VK_CONTROL, 'S' ) ) {
				FileManager::JsonSave( STAGE_FILE_PATH, m_pStage->GetStageData() );
				CMessageWindow::PushMessage( u8"�X�e�[�W��ۑ����܂���", Color4::Magenta );
			}
			ImGui::EndChild();
			ImGui::EndTabItem();
		}
		// �ǉ��^�u.
		if ( ImGui::BeginTabItem( "NewObject" ) ) {
			// �ǉ�����I�u�W�F�N�g�̃R���{�{�b�N�X.
			CImGuiManager::Combo( u8"UI���X�g", &m_SelectObject, { "ATypeApartment", "BTypeApartment", "Island" } );
			ImGui::SameLine();

			// �X�v���C�g�̒ǉ��{�^��.
			if ( CImGuiManager::Button( "Add" ) ) {
				m_pStage->AddObject( m_SelectObject );
			}

			// �X�e�[�W�̏��̕\��.
			ImGui::BeginChild( ImGui::GetID( (void*) 0 ), ImVec2( 0.0f, 0.0f ), ImGuiWindowFlags_NoTitleBar );
			m_pStage->StageStateImGuiRender();
			ImGui::EndChild();
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::End();
}

//---------------------------.
// ���f���̕`��.
//---------------------------.
void CStageEditor::ModelRender()
{
	m_pStage->Sprite3DRender_B();
	m_pStage->Render();
	m_pStage->Sprite3DRender_A();
}

//---------------------------.
// UI�̕`��.
//---------------------------.
void CStageEditor::UIRender()
{
}
