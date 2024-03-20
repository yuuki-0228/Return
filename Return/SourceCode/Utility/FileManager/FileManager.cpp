#include "FileManager.h"

//----------------------------.
// �e�L�X�g�t�@�C���̓ǂݍ���.
//	"//" : �R�����g�A�E�g.
//	"#"  : ��؂蕶��(',')�܂ŃR�����g�A�E�g.
//----------------------------.
std::vector<std::string> FileManager::TextLoad(
	const std::string&	FilePath,
	const bool			IsCommentOut,
	const char			Delim )
{
	std::vector<std::string> OutList;

	// �t�@�C�����J��.
	std::fstream i( FilePath );
	if ( !i ) return OutList;

	// ��s�Âǂݍ���.
	std::string Line;
	while ( std::getline( i, Line ) ){
		// �R�����g�A�E�g����.
		if ( IsCommentOut == true ){
			if ( Line.substr( 0, 2 ) == "//" ) continue;
		}

		// �������ꎞ�I�Ɋi�[����.
		std::string Buff = "";
		std::istringstream stream( Line );
		if ( IsCommentOut == false ){
			OutList.emplace_back( Line );
			continue;
		}

		// ��؂蕶��(',')�ŋ�؂��ĕ������擾.
		while ( std::getline( stream, Buff, Delim ) ){
			// �ꕔ���̃R�����g�A�E�g����.
			if ( IsCommentOut == true ){
				if ( Buff.find( '#' ) != std::string::npos ) continue;
			}

			// �ǂݍ��݃��X�g�ɒǉ�.
			OutList.emplace_back( Buff );
		}
	}
	// �t�@�C�������.
	i.close();
	return OutList;
}

//----------------------------.
// �e�L�X�g�t�@�C���ɏ�������.
//----------------------------.
HRESULT FileManager::TextSave( const std::string& FilePath, const std::string& Data )
{
	// �t�@�C�����J��.
	std::ofstream o( FilePath, std::ios::trunc );
	if ( !o ) {
		// �J���Ȃ����߃t�@�C���f�B���N�g�����쐬����.
		FileManager::CreateFileDirectory( FilePath );

		// �������݂Ȃ���.
		FileManager::TextSave( FilePath, Data );
		return S_OK;
	}

	// ��������.
	o << Data;

	// �t�@�C�������.
	o.close();
	return S_OK;
}

//---------------------------.
// json�t�@�C�����J��.
//---------------------------.
json FileManager::JsonLoad( const std::string& FilePath )
{
	json Out;

	// �t�@�C�����J��.
	std::ifstream i( FilePath );
	if ( !i ) return Out;

	// json�^�ɕύX.
	i >> Out;

	// �t�@�C�������.
	i.close();
	return Out;
}

//---------------------------.
// json�t�@�C���ŏ�������.
//---------------------------.
HRESULT FileManager::JsonSave( const std::string& FilePath, const json& Data )
{
	// �t�@�C�����J��.
	std::ofstream o( FilePath );
	if ( !o ) {
		// �J���Ȃ����߃t�@�C���f�B���N�g�����쐬����.
		FileManager::CreateFileDirectory( FilePath );

		// �������݂Ȃ���.
		FileManager::JsonSave( FilePath, Data );
		return S_OK;
	}

	// ��������.
	o << std::setw( 2 ) << Data << std::endl;

	// �t�@�C�������.
	o.close();
	return S_OK;
}

//---------------------------.
// json �� std::unordered_map �ɕϊ�.
//---------------------------.
std::unordered_map<std::string, std::string> FileManager::JsonToMap( const json& Json )
{
	std::unordered_map<std::string, std::string> Out;
	for ( auto& [Key, Value] : Json.items() ) Out[Key] = Value;
	return Out;
}

//---------------------------.
// std::unordered_map �� json �ɕϊ�.
//---------------------------.
json FileManager::MapToJson( const std::unordered_map<std::string, std::string> Map )
{
	json Out;
	for ( auto&[Key, Value] : Map ) {
		// �����񂩂�^�𐄑����Ă��̌^�ɕϊ����ĕۑ�����.
		if (		Value == "nullptr"						) Out[Key] = nullptr;
		else if (	Value == "true"							) Out[Key] = true;
		else if (	Value == "false"						) Out[Key] = false;
		else if (	Value.find_first_not_of( "0123456789.f" ) == std::string::npos ) {
			if (	Value.find( "." ) != std::string::npos	) Out[Key] = std::stof( Value );
			else											  Out[Key] = std::stoi( Value );
		} else												  Out[Key] = Value;
	}
	return Out;
}
json FileManager::MapToJson( const std::unordered_map<std::string, std::vector<std::string>> Map )
{
	json Out;
	for ( auto&[Key, vValue] : Map ) {
		for ( auto& Value : vValue ) {
			// �����񂩂�^�𐄑����Ă��̌^�ɕϊ����ĕۑ�����.
			if (		Value == "nullptr"						) Out[Key].emplace_back( nullptr );
			else if (	Value == "true"							) Out[Key].emplace_back( true );
			else if (	Value == "false"						) Out[Key].emplace_back( false );
			else if (	Value.find_first_not_of( "0123456789.f" ) == std::string::npos ) {
				if (	Value.find( "." ) != std::string::npos	) Out[Key].emplace_back( std::stof( Value ) );
				else											  Out[Key].emplace_back( std::stoi( Value ) );
			} else												  Out[Key].emplace_back( Value );
		}
	}
	return Out;
}

//---------------------------.
// �t�@�C���f�B���N�g�����쐬.
//---------------------------.
HRESULT FileManager::CreateFileDirectory( const std::string& FilePath ) {
	size_t	FindPos = 0;
	bool	IsEnd	= false;

	// �t�@�C���f�B���N�g�����쐬���Ă���.
	while ( !IsEnd ) {
		FindPos = FilePath.find( "\\", FindPos + 1 );
		if ( FindPos == std::string::npos ) {
			// �t�@�C���f�B���N�g���̍쐬�I��.
			IsEnd = true;
			return S_OK;
		}
		// �t�@�C���f�B���N�g�����쐬.
		std::string CreatePath = FilePath.substr( 0, FindPos );
		std::filesystem::create_directories( CreatePath );
	}
	return S_OK;
}