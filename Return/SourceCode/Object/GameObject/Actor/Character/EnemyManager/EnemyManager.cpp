#include "EnemyManager.h"
#include "Enemy\Enemy.h"

namespace {
	constexpr char STATE_FILE_PATH[] = "Data\\Parameter\\Enemy";
}

CEnemyManager::CEnemyManager()
	: m_pEnemyList	()
	, m_pRenderList	()
{
}

CEnemyManager::~CEnemyManager()
{
}

//----------------------------.
// �C���X�^���X�̎擾.
//----------------------------.
CEnemyManager* CEnemyManager::GetInstance()
{
	static std::unique_ptr<CEnemyManager> pInstance = std::make_unique<CEnemyManager>();
	return pInstance.get();
}

//---------------------------.
// ������.
//---------------------------.
bool CEnemyManager::Init()
{
	CEnemyManager* pI = GetInstance();

	pI->m_pEnemyList.clear();
	pI->m_pRenderList.clear();

	// �G�̏���ǂݍ���.
	StateLood();

	// �f�o�b�N�ň�̏���.
	D3DXPOSITION3 SpownPos = INIT_FLOAT3;
	SpownPos.x = FWND_W / 2.0f;
	SpownPos.y = FWND_H / 2.0f;
	Spown( SpownPos, "Normal" );
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CEnemyManager::Update( const float& DeltaTime )
{
	CEnemyManager* pI = GetInstance();

	// �G�̍X�V.
	for( auto& e : pI->m_pEnemyList ) e->Update( DeltaTime );
	EnemyDelete();	// �g�p���Ă��Ȃ��G�̍폜.
}

//---------------------------.
// �`��.
//---------------------------.
void CEnemyManager::Render()
{
}

//---------------------------.
// �`�惊�X�g�̎擾.
//---------------------------.
std::vector<std::pair<float, CActor*>> CEnemyManager::GetRenderList()
{
	// �`�揇�Ԃ̍X�V.
	RenderListUpdate();
	return GetInstance()->m_pRenderList;
}

//---------------------------.
// �X�|�[��������.
//---------------------------.
void CEnemyManager::Spown( const D3DXPOSITION3& Pos, const std::string& Name )
{
	CEnemyManager* pI = GetInstance();

	// �e�̏�񂪓ǂݍ��܂�Ă��邩���ׂ�.
#ifdef _DEBUG
	LoadStateCheck( Name );
#endif

	// �g�p���Ă��Ȃ��̂�����Ύg�p����.
	for ( auto& b : pI->m_pEnemyList ) {
		if ( b->GetIsDisp() ) continue;

		// �G���X�|�[��������.
		b->Spown( Pos, pI->m_StateList[Name] );
		return;
	}

	// �V�����쐬.
	pI->m_pEnemyList.emplace_back( std::make_shared<CEnemy>() );

	// �G���X�|�[��������.
	pI->m_pEnemyList.back()->Spown( Pos, pI->m_StateList[Name] );
}

//---------------------------.
// �e�̏��̓ǂݍ���.
//---------------------------.
HRESULT CEnemyManager::StateLood()
{
	CEnemyManager* pI = GetInstance();

	auto StateLoad = [&]( const std::filesystem::directory_entry& Entry )
	{
		const std::string Extension = Entry.path().extension().string();	// �g���q.
		const std::string FilePath	= Entry.path().string();				// �t�@�C���p�X.
		const std::string FileName	= Entry.path().stem().string();			// �t�@�C����.

		// �g���q�� ".json" �ł͂Ȃ��ꍇ�ǂݍ��܂Ȃ�.
		if ( Extension != ".json" && Extension != ".JSON" ) return;

		// json�t�@�C����ǂݍ���.
		json j = FileManager::JsonLoad( FilePath );

		// �U�����X�g�̍쐬.
		std::vector<SEnemyActionState> ActionList;
		const int ActionSize = static_cast<int>( j["Action"].size() );
		ActionList.resize( ActionSize );
		for ( int i = 0; i < ActionSize; ++i ) {
			ActionList[i].ActionInit( j["Action"][i] );
		}

		// �e�̏������X�g�Ɋi�[����.
		pI->m_StateList[FileName] = SEnemyState(
			j["TypeName"],
			j["HP"],
			ActionList
		);
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
void CEnemyManager::LoadStateCheck( const std::string& Name )
{
	CEnemyManager* pI = GetInstance();

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
// �`�惊�X�g�̍X�V.
//---------------------------.
void CEnemyManager::RenderListUpdate()
{
	CEnemyManager* pI = GetInstance();

	pI->m_pRenderList.clear();

	// Y���W�ƃ|�C���^���擾.
	for ( auto& e : pI->m_pEnemyList ) {
		pI->m_pRenderList.emplace_back( std::make_pair( e->GetPosition().y, e.get() ) );
	}
}

//---------------------------.
// �g�p���Ă��Ȃ��G�̍폜.
//---------------------------.
void CEnemyManager::EnemyDelete()
{
	CEnemyManager* pI = GetInstance();

	// ���ʂ̒e�̍폜.
	const int NormalMax = static_cast<int>( pI->m_pEnemyList.size() ) - 1;
	for ( int i = NormalMax; i >= 0; --i ) {
		if ( pI->m_pEnemyList[i]->GetIsDisp() ) break;
		pI->m_pEnemyList.pop_back();
	}
}