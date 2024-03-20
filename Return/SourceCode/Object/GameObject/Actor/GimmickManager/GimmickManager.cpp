#include "GimmickManager.h"
#include "GimmickBase\CrackGimmick\CrackGimmick.h"

namespace {
	constexpr char STATE_FILE_PATH[] = "Data\\Parameter\\Gimmick";
}

CGimmickManager::CGimmickManager()
	: m_pGimmickList	()
	, m_pRenderList		()
	, m_StateList		()
{
}

CGimmickManager::~CGimmickManager()
{
}

//----------------------------.
// �C���X�^���X�̎擾.
//----------------------------.
CGimmickManager* CGimmickManager::GetInstance()
{
	static std::unique_ptr<CGimmickManager> pInstance = std::make_unique<CGimmickManager>();
	return pInstance.get();
}

//---------------------------.
// ������.
//---------------------------.
bool CGimmickManager::Init()
{
	CGimmickManager* pI = GetInstance();

	pI->m_pGimmickList.clear();
	pI->m_pRenderList.clear();

	// �G�̏���ǂݍ���.
	StateLood();

	// �f�o�b�N�ň�z�u.
	D3DXPOSITION3 SpownPos = INIT_FLOAT3;
	SpownPos.x = FWND_W / 2.0f - 100.0f;
	SpownPos.y = 128.0f;
	Arrangement( SpownPos, "Barrel" );
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CGimmickManager::Update( const float& DeltaTime )
{
	CGimmickManager* pI = GetInstance();

	// �X�e�[�W�M�~�b�N�̍X�V.
	for ( auto& g : pI->m_pGimmickList ) g->Update( DeltaTime );
}

//---------------------------.
// �`��.
//---------------------------.
void CGimmickManager::Render()
{
}

//---------------------------.
// �`�惊�X�g�̎擾.
//---------------------------.
std::vector<std::pair<float, CActor*>> CGimmickManager::GetRenderList()
{
	// �`�揇�Ԃ̍X�V.
	RenderListUpdate();
	return GetInstance()->m_pRenderList;
}

//---------------------------.
// �M�~�b�N��z�u����.
//---------------------------.
void CGimmickManager::Arrangement( const D3DXPOSITION3& Pos, const std::string& Name )
{
	CGimmickManager* pI = GetInstance();

	// �e�̏�񂪓ǂݍ��܂�Ă��邩���ׂ�.
#ifdef _DEBUG
	LoadStateCheck( Name );
#endif
	// �V�����쐬.
	pI->m_pGimmickList.emplace_back( CreateGimmick( pI->m_StateList[Name].GimmickName ) );

	// �G���X�|�[��������.
	pI->m_pGimmickList.back()->Arrangement( Pos, pI->m_StateList[Name] );
}

//---------------------------.
// �e�̏��̓ǂݍ���.
//---------------------------.
HRESULT CGimmickManager::StateLood()
{
	CGimmickManager* pI = GetInstance();

	auto StateLoad = [&]( const std::filesystem::directory_entry& Entry )
	{
		const std::string Extension = Entry.path().extension().string();	// �g���q.
		const std::string FilePath	= Entry.path().string();				// �t�@�C���p�X.
		const std::string FileName	= Entry.path().stem().string();			// �t�@�C����.

		// �g���q�� ".json" �ł͂Ȃ��ꍇ�ǂݍ��܂Ȃ�.
		if ( Extension != ".json" && Extension != ".JSON" ) return;

		// json�t�@�C����ǂݍ���.
		json j = FileManager::JsonLoad( FilePath );

		// �e�̏������X�g�Ɋi�[����.
		pI->m_StateList[FileName].ActionInit( j );
	};

	CLog::PushLog( "------ �G�t�F�N�g�ǂݍ��݊J�n ------" );
	try {
		std::filesystem::recursive_directory_iterator Dir_itr( STATE_FILE_PATH ), End_itr;
		std::for_each( Dir_itr, End_itr, StateLoad );
	}
	catch ( const std::filesystem::filesystem_error& e ) {
		// �G���[���b�Z�[�W��\��.
		ErrorMessage( e.path1().string().c_str() );
		return E_FAIL;
	}
	CLog::PushLog( "------ �G�t�F�N�g�ǂݍ��ݏI�� ------" );

	// �ǂݍ��ݏI��.
	return S_OK;
}

//---------------------------.
// �e�̏�񂪂��邩���ׂ�.
//---------------------------.
void CGimmickManager::LoadStateCheck( const std::string& Name )
{
	CGimmickManager* pI = GetInstance();

	// ���ׂ�.
	bool IsLoad = false;
	for ( auto& [Key, State] : pI->m_StateList ) {
		if ( Key != Name ) continue;
		IsLoad = true;
		break;
	}

	// �ǂݍ��܂�Ă���.
	if ( IsLoad ) return;

	// �ǂݍ��܂�Ă��Ȃ�.
	ErrorMessage( "�u" + Name + "�v�̒e�̏�񂪂���܂���" );
}

//---------------------------.
// �M�~�b�N�̍쐬.
//---------------------------.
std::shared_ptr<CGimmickBase> CGimmickManager::CreateGimmick( const std::string Type )
{
	// �^�O�ɑΉ������C���X�^���X�쐬.
	std::shared_ptr<CGimmickBase> Out;
	if (		Type == "Crack"	) Out = std::make_shared<CCrackGimmick>();
	else return nullptr;

	// ���������Ă���.
	Out->Init();
	return Out;
}

//---------------------------.
// �`�惊�X�g�̍X�V.
//---------------------------.
void CGimmickManager::RenderListUpdate()
{
	CGimmickManager* pI = GetInstance();

	pI->m_pRenderList.clear();

	// Y���W�ƃ|�C���^���擾.
	for ( auto& g : pI->m_pGimmickList ) {
		pI->m_pRenderList.emplace_back( std::make_pair( g->GetPosition().y, g.get() ) );
	}
}

