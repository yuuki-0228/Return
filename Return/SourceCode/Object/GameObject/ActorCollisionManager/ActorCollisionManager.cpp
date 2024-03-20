#include "ActorCollisionManager.h"
#include "..\Actor\Actor.h"

namespace{
	const int OBJECT_TAG_MIN = static_cast<int>( EObjectTag::None );	// オブジェクトタグの最小値.
	const int OBJECT_TAG_MAX = static_cast<int>( EObjectTag::Max );		// オブジェクトタグの最大値.
	const std::vector<bool> INIT_LIST( OBJECT_TAG_MAX, false );			// 初期化用リスト.
}

CActorCollisionManager::CActorCollisionManager()
	: m_HitActorList	()
	, m_BeAHitActorList	()
	, m_SkipTag			()
	, m_SkipTagToTag	()
{
	// オブジェクトタグ分配列を用意.
	m_SkipTag.resize( OBJECT_TAG_MAX );
	m_SkipTagToTag.resize( OBJECT_TAG_MAX );
	for ( auto& t : m_SkipTagToTag ) t.resize( OBJECT_TAG_MAX );
}

CActorCollisionManager::~CActorCollisionManager()
{
}

//---------------------------.
// インスタンスの取得.
//---------------------------.
CActorCollisionManager* CActorCollisionManager::GetInstance()
{
	static std::unique_ptr<CActorCollisionManager> pInstance = std::make_unique<CActorCollisionManager>();
	return pInstance.get();
}

//---------------------------.
// 当たり判定.
//---------------------------.
void CActorCollisionManager::Collision()
{
	CActorCollisionManager* pI = GetInstance();

	// 当たり判定を行うオブジェクトがない場合.
	//	行う必要がないため抜ける.
	if ( pI->m_HitActorList.empty() ||
		 pI->m_BeAHitActorList.empty() )
	{
		// リストの初期化.
		pI->m_HitActorList.clear();
		pI->m_BeAHitActorList.clear();
		return;
	}

	// 当てるオブジェクト.
	for ( auto& [BaAHit_a, SortType] : pI->m_BeAHitActorList ){
		if ( BaAHit_a == nullptr		) continue;

		// タグを保存.
		const EObjectTag BaAHit_Tag		= BaAHit_a->GetObjectTag();
		const int		 BaAHit_TagNo	= static_cast<int>( BaAHit_Tag );

		// タグが範囲外/スキップするタグか.
		if ( BaAHit_TagNo <= OBJECT_TAG_MIN	) continue;
		if ( BaAHit_TagNo >= OBJECT_TAG_MAX	) continue;
		if ( pI->m_SkipTag[BaAHit_TagNo]	) continue;

		// 当てられるオブジェクトを並び変える.
		std::vector<CActor*> HitList = pI->m_HitActorList;
		if ( SortType != ECollSort::None ) {
			// 距離リストを作成.
			std::vector<std::pair<float, CActor*>> m_DistanceList;
			for ( auto& h : HitList ) {
				const D3DXVECTOR3& Vector = BaAHit_a->GetPosition() - h->GetPosition();
				m_DistanceList.emplace_back( std::make_pair( D3DXVec3Length( &Vector ), h ) );
			}

			// 並び変える.
			if (		SortType != ECollSort::Near ) std::sort( m_DistanceList.begin(), m_DistanceList.end(), std::greater<std::pair<float, CActor*>>{} );
			else if (	SortType != ECollSort::Far  ) std::sort( m_DistanceList.begin(), m_DistanceList.end() );

			// 並び変えたリストにリストを更新する.
			HitList.clear();
			for ( auto& [Distance, Object] : m_DistanceList ) {
				HitList.emplace_back( Object );
			}
		}

		// 当てられるオブジェクト.
		for ( auto& Hit_a : HitList ){
			if ( Hit_a == nullptr		) continue;

			// 自分自身同士で当たり判定は行わなくていいため抜ける.
			if ( BaAHit_a == Hit_a ) continue;

			// タグを保存.
			const EObjectTag Hit_Tag		= Hit_a->GetObjectTag();
			const int		 Hit_TagNo	= static_cast<int>( Hit_Tag );

			// タグが範囲外/スキップするタグか.
			if ( Hit_TagNo <= OBJECT_TAG_MIN					) continue;
			if ( Hit_TagNo >= OBJECT_TAG_MAX					) continue;
			if ( pI->m_SkipTag[Hit_TagNo]						) continue;
			if ( pI->m_SkipTagToTag[BaAHit_TagNo][Hit_TagNo]	) continue;

			// 当たり判定を行う.
			BaAHit_a->Collision( Hit_a );
		}
	}
	// リストの初期化.
	pI->m_HitActorList.clear();
	pI->m_BeAHitActorList.clear();
}

//---------------------------.
// 当たり判定のリストを初期化.
//---------------------------.
void CActorCollisionManager::Reset()
{
	CActorCollisionManager* pI = GetInstance();

	// リストと当たり判定を行わないリストの初期化.
	pI->m_HitActorList.clear();
	pI->m_BeAHitActorList.clear();
	pI->m_SkipTag = INIT_LIST;
	for ( auto& vt : pI->m_SkipTagToTag )
		vt = INIT_LIST;
}
