#include "PotionAreaManager.h"

namespace {
	constexpr char STATE_FILE_PATH[] = "Data\\Parameter\\PotionArea";
}

CPotionAreaManager::CPotionAreaManager()
	: m_pAreaList	()
	, m_pRenderList	()
	, m_StateList	()
{
}

CPotionAreaManager::~CPotionAreaManager()
{
}

//---------------------------.
// �C���X�^���X�̎擾.
//---------------------------.
CPotionAreaManager* CPotionAreaManager::GetInstance()
{
	static std::unique_ptr<CPotionAreaManager> pInstance = std::make_unique<CPotionAreaManager>();
	return pInstance.get();
}

//---------------------------.
// ������.
//---------------------------.
bool CPotionAreaManager::Init()
{
	CPotionAreaManager* pI = GetInstance();

	pI->m_pAreaList.clear();
	pI->m_pRenderList.clear();

	// �e�̏���ǂݍ���.
	StateLood();
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CPotionAreaManager::Update( const float& DeltaTime )
{
	CPotionAreaManager* pI = GetInstance();

	for ( int i = 0; i < pI->m_pAreaList.size(); ++i ) {
		pI->m_pAreaList[i]->Update( DeltaTime );
	}

	AreaDelete();	// �g�p���Ă��Ȃ��e�̍폜.
}

//---------------------------.
// �����蔻��I����Ăяo�����X�V.
//---------------------------.
void CPotionAreaManager::LateUpdate( const float& DeltaTime )
{
	CPotionAreaManager* pI = GetInstance();

	for ( int i = 0; i < pI->m_pAreaList.size(); ++i ) {
		pI->m_pAreaList[i]->LateUpdate( DeltaTime );
	}
}

//---------------------------.
// �`��.
//---------------------------.
void CPotionAreaManager::Render()
{
}

//---------------------------.
// �`�惊�X�g�̎擾.
//---------------------------.
std::vector<std::pair<float, CActor*>> CPotionAreaManager::GetRenderList()
{
	// �`�揇�Ԃ̍X�V.
	RenderListUpdate();
	return GetInstance()->m_pRenderList;
}

//---------------------------.
// �G���A�̒ǉ�.
//---------------------------.
void CPotionAreaManager::PushArea( const D3DXPOSITION3& Pos, const float Radius, const std::string& Name, const float Time )
{
	CPotionAreaManager* pI = GetInstance();

	// �e�̏�񂪓ǂݍ��܂�Ă��邩���ׂ�.
#ifdef _DEBUG
	LoadStateCheck( Name );
#endif

	// �G���A�̏��̍X�V.
	SPotionAreaState State = pI->m_StateList[Name];
	if ( Time > INIT_FLOAT ) {
		State.DeleteTime = Time;
	}

	// �g�p���Ă��Ȃ��̂�����Ύg�p����.
	for ( auto& b : pI->m_pAreaList ) {
		if ( b->GetIsEnd() == false ) continue;

		// �|�[�V�����G���A�̍쐬.
		b->CreateArea( Pos, Radius, State );
		return;
	}

	// �V�����쐬.
	pI->m_pAreaList.emplace_back( std::make_shared<CPotionArea>() );

	// �|�[�V�����G���A�̍쐬.
	pI->m_pAreaList.back()->CreateArea( Pos, Radius, State );
}

//---------------------------.
// �e�̏��̓ǂݍ���.
//---------------------------.
HRESULT CPotionAreaManager::StateLood()
{
	CPotionAreaManager* pI = GetInstance();

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
		const D3DXCOLOR4&	Color	= { j["EffectColor"]["R"], j["EffectColor"]["G"], j["EffectColor"]["B"], 1.0f };
		const int			UseNum	= j["UseNum"].empty() ? 0 : static_cast<int>( j["UseNum"] );
		pI->m_StateList[FileName] = SPotionAreaState(
			GetPotionType( j["PotionType"] ),
			UseNum,
			j["DeleteTime"],
			j["EffectTime"],
			j["EffectValue"],
			Color
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
void CPotionAreaManager::LoadStateCheck( const std::string& Name )
{
	CPotionAreaManager* pI = GetInstance();

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
// �|�[�V�����̃^�C�v���擾.
//---------------------------.
int CPotionAreaManager::GetPotionType( const std::string& Type )
{
	if ( Type == "Heal"		) return PotionNo::Heal;
	if ( Type == "Poison"	) return PotionNo::Poison;
	if ( Type == "Rest"		) return PotionNo::Rest;
	return PotionNo::None;
}

//---------------------------.
// �`�惊�X�g�̍X�V.
//---------------------------.
void CPotionAreaManager::RenderListUpdate()
{
	CPotionAreaManager* pI = GetInstance();

	pI->m_pRenderList.clear();

	// Y���W�ƃ|�C���^���擾.
	for ( auto& b : pI->m_pAreaList ) {
		pI->m_pRenderList.emplace_back( std::make_pair( b->GetPosition().y, b.get() ) );
	}
}

//---------------------------.
// �g�p���Ă��Ȃ��e�̍폜.
//---------------------------.
void CPotionAreaManager::AreaDelete()
{
	CPotionAreaManager* pI = GetInstance();

	const int NormalMax = static_cast<int>( pI->m_pAreaList.size() ) - 1;
	for ( int i = NormalMax; i >= 0; --i ) {
		if ( pI->m_pAreaList[i]->GetIsEnd() == false ) break;
		pI->m_pAreaList.pop_back();
	}
}
