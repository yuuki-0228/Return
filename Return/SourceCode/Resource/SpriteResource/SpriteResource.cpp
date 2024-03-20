#include "SpriteResource.h"
#include "..\..\Common\Sprite\Sprite.h"
#include "..\..\Utility\FileManager\FileManager.h"

namespace {
	constexpr char FILE_PATH[] = "Data\\Sprite"; // �X�v���C�g�t�@�C���p�X.
}

CSpriteResource::CSpriteResource()
	: m_SpriteList		()
	, m_SpriteNames		()
	, m_Mutex			()
	, m_IsLoadEnd		( false )
{
}

CSpriteResource::~CSpriteResource()
{
	m_SpriteList.clear();
}

//---------------------------.
// �C���X�^���X�̎擾.
//---------------------------.
CSpriteResource* CSpriteResource::GetInstance()
{
	static std::unique_ptr<CSpriteResource> pInstance = std::make_unique<CSpriteResource>();
	return pInstance.get();
}

//---------------------------.
// �X�v���C�g�̓ǂݍ���.
//---------------------------.
HRESULT CSpriteResource::SpriteLoad()
{
	CSpriteResource* pI = GetInstance();
	std::unique_lock<std::mutex> Lock( pI->m_Mutex );

	auto SpriteLoad = [&]( const std::filesystem::directory_entry& Entry )
	{
		const std::string Extension = Entry.path().extension().string();	// �g���q.
		const std::string FilePath	= Entry.path().string();				// �t�@�C���p�X.
		const std::string FileName	= Entry.path().stem().string();			// �t�@�C����.

		// �g���q�� ".midi" / ".mp3" / ".wav" �ł͂Ȃ��ꍇ�ǂݍ��܂Ȃ�.
		if ( Extension != ".png" && Extension != ".PNG" &&
			 Extension != ".bmp" && Extension != ".BMP" &&
			 Extension != ".jpg" && Extension != ".JPG" ) return;

		// �X�v���C�g�t�@�C���̓ǂݍ���.
		pI->m_SpriteList[FileName] = std::make_unique<CSprite>();
		if ( FAILED( pI->m_SpriteList[FileName]->Init( FilePath ) ) ) throw E_FAIL;
		pI->m_SpriteNames.emplace_back( FileName );
	};

	CLog::PushLog( "------ �X�v���C�g�ǂݍ��݊J�n ------" );
	try {
		std::filesystem::recursive_directory_iterator Dir_itr( FILE_PATH ), End_itr;
		std::for_each( Dir_itr, End_itr, SpriteLoad );
	}
	catch ( const std::filesystem::filesystem_error& e ) {
		// �G���[���b�Z�[�W��\��.
		ErrorMessage( e.path1().string().c_str() );
		return E_FAIL;
	}
	CLog::PushLog( "------ �X�v���C�g�ǂݍ��ݏI�� ------" );

	// �ǂݍ��ݏI��.
	pI->m_IsLoadEnd = true;
	return S_OK;
}

//---------------------------.
// �X�v���C�g�擾�֐�.
//---------------------------.
CSprite* CSpriteResource::GetSprite( const std::string& FileName, stSpriteRenderState* pState )
{
	// �ǂݍ��݂��I����Ă��Ȃ������� null ��Ԃ�.
	if ( GetInstance()->m_IsLoadEnd == false )
		return nullptr;

	// �w�肵���G�t�F�N�g���擾.
	for ( auto& s : GetInstance()->m_SpriteList ) {
		if ( s.first != FileName	) continue;
		if ( pState  != nullptr		) *pState = s.second->GetRenderState();
		return s.second.get();
	}

	ErrorMessage( FileName + " sounds not found" );
	return nullptr;
}
