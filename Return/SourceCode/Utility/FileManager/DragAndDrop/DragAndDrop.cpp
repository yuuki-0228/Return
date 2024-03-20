#include "DragAndDrop.h"

CDragAndDrop::CDragAndDrop()
	: m_hWnd			()
	, m_FilePathList	()
{
}

CDragAndDrop::~CDragAndDrop()
{
}

//---------------------------.
// �C���X�^���X�̎擾.
//---------------------------.
CDragAndDrop* CDragAndDrop::GetInstance()
{
	static std::unique_ptr<CDragAndDrop> pInstance = std::make_unique<CDragAndDrop>();
	return pInstance.get();
}

//---------------------------.
// �h���b�N&�h���b�v���g�p����.
//---------------------------.
void CDragAndDrop::Open()
{
	// �h���b�N&�h���b�v���g�p����.
	DragAcceptFiles( GetInstance()->m_hWnd, TRUE );
}

//---------------------------.
// �h���b�N&�h���b�v���g�p���Ȃ�.
//---------------------------.
void CDragAndDrop::Close()
{
	CDragAndDrop* pI = GetInstance();

	// �h���b�N&�h���b�v���g�p���Ȃ�.
	DragAcceptFiles( pI->m_hWnd, FALSE );

	// �ۑ����Ă���t�@�C���p�X���폜����.
	pI->m_FilePathList.clear();
}
