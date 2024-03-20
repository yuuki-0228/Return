#pragma once
#include "..\..\GameObject\GameObject.h"

class CActor;

// 当たり判定をするオブジェクトの種類.
enum class enCollObjType : unsigned char {
	Both,	// 両方.
	Hit,	// 当てる.
	BeAHit	// 当てられる( Collision()が呼び出される ).

} typedef ECollObjType;

// 当たり判定を行う時のソートの仕方.
enum class enCollSort : unsigned char {
	None,	// ソート無し.
	Near,	// 近い順.
	Far		// 遠い順.
} typedef ECollSort;

/************************************************
*	オブジェクトの当たり判定マネージャークラス.
*		�ｱ田友輝.
**/
class CActorCollisionManager final
{
public:
	// 当たり判定を行わないタグリスト.
	using SkipTagList = std::vector<bool>;

public:
	CActorCollisionManager();
	~CActorCollisionManager();

	// インスタンスの取得.
	static CActorCollisionManager* GetInstance();

	// 当たり判定.
	static void Collision();

	// 当たり判定のリストを初期化.
	//	シーン切り替え時に使用する.
	static void Reset();

	// 当たり判定を行うオブジェクトを追加..
	inline static void PushObject( CActor* pActor, const ECollObjType Type = ECollObjType::Both, const ECollSort Sort = ECollSort::None ) {
		if ( Type == ECollObjType::Both || Type == ECollObjType::Hit ) {
			GetInstance()->m_HitActorList.emplace_back( pActor );
		}
		if ( Type == ECollObjType::Both || Type == ECollObjType::BeAHit ) {
			GetInstance()->m_BeAHitActorList.emplace_back( std::make_pair( pActor, Sort ) );
		}
	}

	// 特定のタグの当たり判定の有効/無効.
	inline static void SetSkipTag( EObjectTag tag, bool Flag ) {
		GetInstance()->m_SkipTag[static_cast<int>( tag )] = Flag;
	}
	// 特定のタグ同士の当たり判定の有効/無効.
	inline static void SetSkipTagToTag( EObjectTag mtag, EObjectTag ytag, bool Flag ) {
		GetInstance()->m_SkipTagToTag[static_cast<int>( mtag )][static_cast<int>( ytag )] = Flag;
		GetInstance()->m_SkipTagToTag[static_cast<int>( ytag )][static_cast<int>( mtag )] = Flag;
	}

private:
	std::vector<CActor*>						m_HitActorList;		// 当てるオブジェクトリスト.
	std::vector<std::pair<CActor*, ECollSort>>	m_BeAHitActorList;	// 当てられるオブジェクトリスト.
	SkipTagList									m_SkipTag;			// 当たり判定を行わないか.
	std::vector<SkipTagList>					m_SkipTagToTag;		// 特定のタグ同士の当たり判定をなくす.
};
