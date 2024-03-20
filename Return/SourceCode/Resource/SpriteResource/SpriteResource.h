#pragma once
#include "..\..\Global.h"
#include <unordered_map>
#include <string>
#include <memory>
#include <mutex>
#include <filesystem>

struct stSpriteRenderState;
class CSprite;

/************************************************
*	�X�v���C�g���\�[�X�N���X.
*		���c�F�P.
**/
class CSpriteResource final
{
public:
	using Sprite_map	= std::unordered_map<std::string, std::shared_ptr<CSprite>>;
	using Sprite_List	= std::vector<std::string>;

public:
	CSpriteResource();
	~CSpriteResource();

	// �C���X�^���X�̎擾.
	static CSpriteResource* GetInstance();

	// �X�v���C�g�̓ǂݍ���.
	static HRESULT SpriteLoad();

	// �X�v���C�g�擾�֐�.
	static CSprite* GetSprite( const std::string& FileName, stSpriteRenderState* pState = nullptr );

	// �ۑ����Ă���X�v���C�g�̖��O���X�g�̎擾.
	static Sprite_List GetSpriteNames() { return GetInstance()->m_SpriteNames; }

private:
	Sprite_map	m_SpriteList;	// �X�v���C�g���X�g.
	Sprite_List	m_SpriteNames;	// �X�v���C�g�̖��O���X�g.
	std::mutex	m_Mutex;
	bool		m_IsLoadEnd;	// �ǂݍ��݂��I�������.

private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CSpriteResource( const CSpriteResource& )				= delete;
	CSpriteResource& operator= ( const CSpriteResource& )	= delete;
	CSpriteResource( CSpriteResource&& )					= delete;
	CSpriteResource& operator = ( CSpriteResource&& )		= delete;
};