#pragma once
#include "..\..\..\..\..\Global.h"
#include "..\..\..\..\..\Utility\FileManager\FileManager.h"

// �M�~�b�N�̏��.
struct stGimmickState {
	std::string GimmickName;	// �M�~�b�N��.
	std::string ImageName;		// �摜��.
	//----------------------------------------------.
	//	����M�~�b�N�p�ϐ�( GimmickName == "Crack" ).
	//----------------------------------------------.
	int			Hp;				// �ϋv�l.
	std::string	DropBulletName;	// ���Ƃ��e�̖��O.

	//--------------------------.
	// ���̏�����.
	//--------------------------.
	inline void ActionInit( const json j ) {
		GimmickName = j["GimmickName"];
		ImageName	= j["ImageName"];
		if ( GimmickName == "Crack"	) CrackInit( j["Hp"], j["DropBulletName"] );
	}

private:
	//--------------------------.
	// �U���̏�����( ActionName == "Attack" ).
	//--------------------------.
	inline void CrackInit( const int hp, const std::string& Name ) {
		Hp				= hp;
		DropBulletName	= Name;
	}

} typedef SGimmickState;
