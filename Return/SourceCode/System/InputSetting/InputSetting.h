#pragma once
#include "..\SystemBase.h"
#include "..\..\Utility\Input\KeyBind.h"

/************************************************
*	���͐ݒ�N���X.
*		���c�F�P.
**/
class CInputSetting final
	: public CSystemBase
{
public:
	CInputSetting();
	~CInputSetting();

protected:
	// �������֐�.
	virtual void Init() override;

private:
	KeyBind_map	m_KeyBindList;	// �L�[�o�C���h�̃��X�g.
	std::string	m_ActionName;	// �A�N�V������.
};
