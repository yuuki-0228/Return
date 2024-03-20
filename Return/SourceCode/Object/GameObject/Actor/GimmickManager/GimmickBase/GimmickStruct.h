#pragma once
#include "..\..\..\..\..\Global.h"
#include "..\..\..\..\..\Utility\FileManager\FileManager.h"

// ギミックの情報.
struct stGimmickState {
	std::string GimmickName;	// ギミック名.
	std::string ImageName;		// 画像名.
	//----------------------------------------------.
	//	壊れるギミック用変数( GimmickName == "Crack" ).
	//----------------------------------------------.
	int			Hp;				// 耐久値.
	std::string	DropBulletName;	// 落とす弾の名前.

	//--------------------------.
	// 情報の初期化.
	//--------------------------.
	inline void ActionInit( const json j ) {
		GimmickName = j["GimmickName"];
		ImageName	= j["ImageName"];
		if ( GimmickName == "Crack"	) CrackInit( j["Hp"], j["DropBulletName"] );
	}

private:
	//--------------------------.
	// 攻撃の初期化( ActionName == "Attack" ).
	//--------------------------.
	inline void CrackInit( const int hp, const std::string& Name ) {
		Hp				= hp;
		DropBulletName	= Name;
	}

} typedef SGimmickState;
