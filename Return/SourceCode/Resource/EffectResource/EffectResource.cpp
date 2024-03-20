#include "EffectResource.h"
#include "..\..\Common\EffectManager\Effect\Effect.h"

namespace {
	constexpr char FILE_PATH[] = "Data\\Effect"; // �G�t�F�N�g�t�@�C���p�X.
}

CEffectResource::CEffectResource()
	: m_EffectList		()
	, m_EffectNames		()
	, m_Mutex			()
	, m_IsLoadEnd		( false )
{
}

CEffectResource::~CEffectResource()
{
	m_EffectList.clear();
}

//---------------------------.
// �C���X�^���X�̎擾.
//---------------------------.
CEffectResource* CEffectResource::GetInstance()
{
	static std::unique_ptr<CEffectResource> pInstance = std::make_unique<CEffectResource>();
	return pInstance.get();
}

//---------------------------.
// �G�t�F�N�g�̓ǂݍ���.
//---------------------------.
HRESULT CEffectResource::EffectLoad()
{
	CEffectResource* pI = GetInstance();
	std::unique_lock<std::mutex> Lock( pI->m_Mutex );

	auto EffectLoad = [&]( const std::filesystem::directory_entry& Entry )
	{
		const std::string Extension = Entry.path().extension().string();	// �g���q.
		const std::string FilePath	= Entry.path().string();				// �t�@�C���p�X.
		const std::string FileName	= Entry.path().stem().string();			// �t�@�C����.

		// �g���q�� ".efk" �ł͂Ȃ��ꍇ�ǂݍ��܂Ȃ�.
		if ( Extension != ".efk" && Extension != ".EFK" ) return;

		// �G�t�F�N�g��ǂݍ��݃��X�g�Ɋi�[����.
		pI->m_EffectList[FileName] = std::make_unique<CEffect>( FilePath );
		pI->m_EffectNames.emplace_back( FileName );
	};

	CLog::PushLog( "------ �G�t�F�N�g�ǂݍ��݊J�n ------" );
	try {
		std::filesystem::recursive_directory_iterator Dir_itr( FILE_PATH ), End_itr;
		std::for_each( Dir_itr, End_itr, EffectLoad );
	}
	catch ( const std::filesystem::filesystem_error& e ) {
		// �G���[���b�Z�[�W��\��.
		ErrorMessage( e.path1().string().c_str() );
		return E_FAIL;
	}
	CLog::PushLog( "------ �G�t�F�N�g�ǂݍ��ݏI�� ------" );

	// �ǂݍ��ݏI��.
	pI->m_IsLoadEnd = true;
	return S_OK;
}

//---------------------------.
// �G�t�F�N�g�擾�֐�.
//---------------------------.
CEffect* CEffectResource::GetEffect( const std::string& FileName )
{
	// �ǂݍ��݂��I����Ă��Ȃ������� null ��Ԃ�.
	if ( GetInstance()->m_IsLoadEnd == false )
		return nullptr;

	// �w�肵���G�t�F�N�g���擾.
	for ( auto& m : GetInstance()->m_EffectList ) {
		if ( m.first == FileName ) return m.second.get();
	}

	ErrorMessage( FileName + " effects not found" );
	return nullptr;
}
