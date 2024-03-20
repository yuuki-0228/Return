#pragma once
#include "FlagManager/FlagManager.h"
#include <string>

/************************************************
*	�I���W�i��bool�N���X.
*	FlagManager �ňꊇ�Ǘ����\.
*		���c�F�P.
**/
class CBool final
{
public:
	void operator=(  const bool	 Flag );
	void operator=(  const char* sFlag );
	void operator=(  const int	 iFlag );
	bool operator==( const bool  Flag );
	bool operator!=( const bool  Flag );
	bool operator!();

public:
	CBool( bool Flag = false, std::string Name = u8"", std::string Group = u8"Object" );
	~CBool();

	// bool�̏�Ԃ𕶎���Ŏ擾.
	inline std::string ToString() {
		return CFlagManager::IsBitFlag( m_Handle ) ? "true" : "false";
	};

	// bool�^�Ŏ擾.
	inline bool get() {
		return CFlagManager::IsBitFlag( m_Handle );
	}

	// ���O���擾.
	inline std::string GetName() {
		return CFlagManager::GetName( m_Handle );
	};
	// ���O�̐ݒ�.
	inline void SetName( std::string Name ) {
		CFlagManager::SetName( m_Handle, Name );
	};

	// �t���O�𔽓]������.
	inline void Inversion() {
		CFlagManager::BitInversion( m_Handle );
	}

private:
	CFlagManager::Handle m_Handle; // �n���h��.
};
