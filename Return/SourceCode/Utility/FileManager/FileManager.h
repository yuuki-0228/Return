#pragma once
#include "..\..\Global.h"
#include <nlohmnn\json.hpp>
#include <memory>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

// json�^���쐬.
using json = nlohmann::json;

/************************************************
*	�t�@�C���֌W�ꗗ.
*		���c�F�P.
**/
namespace FileManager {
	// �e�L�X�g�t�@�C���̓ǂݍ���.
	//	"//" : �R�����g�A�E�g.
	//	"#"  : ��؂蕶��(',')�܂ŃR�����g�A�E�g.
	std::vector<std::string> TextLoad(
		const std::string&	FilePath,
		const bool			IsCommentOut = true,
		const char			Delim = ',' );

	// �e�L�X�g�t�@�C���ɏ�������.
	HRESULT TextSave( const std::string& FilePath, const std::string& Data );

	// �o�C�i���f�[�^�ł̓ǂݍ���.
	template<class T>
	HRESULT BinaryLoad( const char* FilePath, T& Out, const int& SeekPoint = 0 );
	template<class T>
	HRESULT BinaryLoad( const char* FilePath, std::vector<T>& OutList );

	// �o�C�i���f�[�^�ł̏�������.
	template<class T>
	HRESULT BinarySave( const char* FilePath, const T& Data );
	template<class T>
	HRESULT BinarySave( const char* FilePath, const std::vector<T>& DataList );

	// json�`����json�t�@�C�����J��.
	json JsonLoad( const std::string& FilePath );

	// json�`����json�t�@�C���ŏ�������.
	HRESULT JsonSave( const std::string& FilePath, const json& Data );

	// json �� std::unordered_map �ɕϊ�.
	std::unordered_map<std::string, std::string> JsonToMap( const json& Json );

	// std::unordered_map �� json �ɕϊ�.
	json MapToJson( const std::unordered_map<std::string, std::string> Map );
	json MapToJson( const std::unordered_map<std::string, std::vector<std::string>> Map );

	// �t�@�C���f�B���N�g�����쐬����.
	HRESULT CreateFileDirectory( const std::string& FilePath );
}

//---------------------------.
// �o�C�i���f�[�^�ł̓ǂݍ���.
//---------------------------.
template<class T>
HRESULT FileManager::BinaryLoad( const char* FilePath, T& Out, const int& SeekPoint )
{
	// �t�@�C�����J��.
	std::fstream o( FilePath, std::ios::in | std::ios::binary );
	if ( !o ) return E_FAIL;

	// �ǂݍ���.
	o.seekg( SeekPoint * sizeof( T ) );
	o.read(  reinterpret_cast<char*>( std::addressof( Out ) ), sizeof( T ) );

	//�t�@�C�������
	o.close();
	return S_OK;
}

//---------------------------.
// �o�C�i���f�[�^��vector�œǂݍ���.
//---------------------------.
template<class T>
HRESULT FileManager::BinaryLoad( const char* FilePath, std::vector<T>& OutList )
{
	// �t�@�C�����J��.
	std::ifstream o( FilePath, std::ios::in | std::ios::binary );
	if ( !o ) return E_FAIL;

	// ������.
	OutList.clear();
	int Size = 0;

	// �ǂݍ���.
	o.read( reinterpret_cast<char*>( &Size ), sizeof( Size ) );
	OutList.resize( Size );
	o.read( reinterpret_cast<char*>( &OutList[0] ), Size * sizeof( T ) );

	// �t�@�C�������.
	o.close();
	return S_OK;
}

//---------------------------.
// �o�C�i���f�[�^�ł̏�������.
//---------------------------.
template<class T>
HRESULT FileManager::BinarySave( const char* FilePath, const T& Data )
{
	// �t�@�C�����J��.
	std::fstream i( FilePath, std::ios::out | std::ios::binary | std::ios::trunc );
	if ( !i ) {
		// �J���Ȃ����߃t�@�C���f�B���N�g�����쐬����.
		FileManager::CreateFileDirectory( FilePath );

		// �������݂Ȃ���.
		FileManager::BinarySave<T>( FilePath, Data );
		return S_OK;
	};

	// ��������.
	i.write( reinterpret_cast<const char*>( std::addressof( Data ) ), sizeof( T ) );

	//�t�@�C�������
	i.close();
	return S_OK;
}

//---------------------------.
// �o�C�i���f�[�^��vector�ŏ�������.
//---------------------------.
template<class T>
HRESULT FileManager::BinarySave( const char* FilePath, const std::vector<T>& DataList )
{
	// �t�@�C�����J��.
	std::ofstream i( FilePath, std::ios::out | std::ios::binary );
	if ( !i ) {
		// �J���Ȃ����߃t�@�C���f�B���N�g�����쐬����.
		FileManager::CreateFileDirectory( FilePath );

		// �������݂Ȃ���.
		FileManager::BinarySave<T>( FilePath, DataList );
	};

	// ��������.
	const int& Size = DataList.size();
	i.write( reinterpret_cast<const char*>( &Size ), sizeof( Size ) );
	i.write( reinterpret_cast<const char*>( &DataList[0] ), Size * sizeof( T ) );

	// �t�@�C�������.
	i.close();
	return S_OK;
}